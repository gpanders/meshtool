#include "face.h"
#include "vertex.h"
#include "colmap.h"
#include "tinyply.h"

#include <Eigen/Core>

#include <fstream>
#include <string>

#define UNCERTAINTY_UB 0.0001
#define UNCERTAINTY_LB 0.00005

int main(int argc, const char * argv[])
{
    if (argc != 3)
    {
        std::cerr << "Usage: " << argv[0] << " input.ply points3d.bin" << std::endl;
        return 1;
    }

    const std::string finply = argv[1];
    const std::string rawname = finply.substr(0, finply.find_last_of('.'));

    const std::string foutply = rawname + "_out.ply";

    const std::string finpoints = argv[2]; 

    // Vector of Eigen::Vector3f storing vertex coordinates
    std::vector<meshtool::Vertex> vertices;
    std::vector<meshtool::Face> faces;

    std::vector<float> verts;
    std::vector<uint32_t> indices;
    std::vector<float> norms;
    std::vector<uint8_t> colors;
    uint32_t nverts, nfaces;

    // Read points from COLMAP
    std::vector<meshtool::Vertex> points3d = meshtool::ReadCOLMAP(finpoints);

    try
    {
        std::ifstream ss(finply, std::ios::binary);

        std::filebuf fb;
        fb.open(foutply, std::ios::out | std::ios::binary);
        std::ostream os(&fb);

        tinyply::PlyFile infile(ss);
        tinyply::PlyFile outfile;

        nverts = infile.request_properties_from_element("vertex", { "x", "y", "z" }, verts);
        nfaces = infile.request_properties_from_element("face", { "vertex_indices" }, indices, 3);

        // Allocate appropriate memory
        vertices.resize(nverts);
        faces.resize(nfaces);

        norms.resize(verts.size());
        colors.resize(verts.size());

        std::cout << "COLMAP file has " << points3d.size() << " points." << std::endl;
        std::cout << "Input PLY file has " << nverts << " points." << std::endl;

        infile.read(ss);

        // Convert vertices into Eigen vectors
        for (size_t i = 0; i < nverts; i++)
        {
            // There are 3 values in `verts` per vertex
            const size_t n = 3*i;
            meshtool::Vertex vertex(verts[n], verts[n+1], verts[n+2]);
            vertices[i] = vertex;

            // Find vertices that match to the COLMAP 3d points. Inflate a sphere
            // until we find at least one point within the sphere
            double radius = 0.005; // initial radius of 5 mm
            size_t num_points = 0;
            double uncertainty = 0;
            while (num_points == 0 && radius <= 1)
            {
                auto it = std::find_if(points3d.begin(), points3d.end(), 
                    [&vertex, &radius](const meshtool::Vertex& p) {
                        return vertex.WithinRadius(p, radius);
                    }
                );

                while (it != points3d.end())
                {
                    uncertainty += it->Uncertainty();
                    num_points++;
                    it = std::find_if(++it, points3d.end(), 
                        [&vertex, &radius](const meshtool::Vertex& p) {
                            return vertex.WithinRadius(p, radius);
                        }
                    );
                }

                // If no points were found, increase radius
                if (num_points == 0)
                {
                    radius += 0.005;
                }
            }

            if (num_points > 0)
            {
                vertex.SetUncertainty(uncertainty / num_points);
                if (vertex.Uncertainty() > UNCERTAINTY_UB)
                {
                    vertex.SetColor({ 255, 0, 0 });
                }
                else if (vertex.Uncertainty() < UNCERTAINTY_LB)
                {
                    vertex.SetColor({ 0, 255, 0 });
                }
                else
                {
                    const uint8_t r = static_cast<uint8_t>(nearbyint((255 / (UNCERTAINTY_UB - UNCERTAINTY_LB)) * (vertex.Uncertainty() - UNCERTAINTY_LB)));
                    const uint8_t g = 255 - r;
                    vertex.SetColor({ r, g, 0 });
                }
            }

            colors[n] = vertex.Color(0);
            colors[n+1] = vertex.Color(1);
            colors[n+2] = vertex.Color(2);
        }

        // Get vertex indices for each face. The `faces` vector contains the
        // ordered indices of the vertices of each face, three per face.
        for (size_t i = 0; i < nfaces; i++)
        {
            // There are 3 indices in `faces` per face
            const size_t n = 3*i;

            meshtool::Face face(
                    vertices[indices[n]], 
                    vertices[indices[n+1]], 
                    vertices[indices[n+2]]
            );

            faces[i] = face;
        }

        // Iterate through the vertices one more time to populate the `norms'
        // vector. We MUST do this AFTER calculating the `faces' vector,
        // because the Face constructor calculates the normals for each Vertex
        // that makes up a Face
        for (size_t i = 0; i < nverts; i++)
        {
            const size_t n = 3*i;
            meshtool::Vertex vertex = vertices[i];
            norms[n] = static_cast<float>(vertex.Normal(0));
            norms[n+1] = static_cast<float>(vertex.Normal(1));
            norms[n+2] = static_cast<float>(vertex.Normal(2));
        }

        outfile.add_properties_to_element("vertex", { "x", "y", "z" }, verts);
        outfile.add_properties_to_element("vertex", { "nx", "ny", "nz" }, norms);
        outfile.add_properties_to_element("vertex", { "red", "green", "blue" }, colors);
        outfile.add_properties_to_element("face", { "vertex_indices" }, indices, 3, tinyply::PlyProperty::Type::UINT8);
        outfile.comments.emplace_back("Generated by meshtool");
        outfile.write(os, true);

        fb.close();

    }
    catch (const std::exception & e)
    {
        std::cerr << "Caught exception: " << e.what() << std::endl;
    }

    return 0;
}
