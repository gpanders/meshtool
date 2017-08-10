#include "tinyply.h"

#include <fstream>
#include <string>

int main(int argc, const char * argv[])
{
    if (argc != 2)
    {
        std::cerr << "Usage: " << argv[0] << " input.ply" << std::endl;
        return 1;
    }

    const std::string finply = argv[1];

    std::vector<float> verts;
    std::vector<uint32_t> indices;
    std::vector<float> norms;
    std::vector<uint8_t> colors;

    try
    {
        std::ifstream ss(finply, std::ios::binary);

        //std::filebuf fb;
        //fb.open(foutply, std::ios::out | std::ios::binary);
        //std::ostream os(&fb);
        //std::ostream os(std::cout);

        tinyply::PlyFile infile(ss);
        tinyply::PlyFile outfile;

        infile.request_properties_from_element("vertex", { "x", "y", "z" }, verts);
        infile.request_properties_from_element("face", { "vertex_indices" }, indices, 3);
        infile.request_properties_from_element("vertex", { "nx", "ny", "nz" }, norms);
        infile.request_properties_from_element("vertex", { "red", "green", "blue" }, colors);

        infile.read(ss);

        if (!verts.empty()) 
            outfile.add_properties_to_element("vertex", { "x", "y", "z" }, verts);

        if (!norms.empty())
            outfile.add_properties_to_element("vertex", { "nx", "ny", "nz" }, norms);

        if (!colors.empty())
            outfile.add_properties_to_element("vertex", { "red", "green", "blue" }, colors);

        if (!indices.empty())
            outfile.add_properties_to_element("face", { "vertex_indices" }, indices, 3, tinyply::PlyProperty::Type::UINT8);

        outfile.write(std::cout, false);

        //fb.close();

    }
    catch (const std::exception & e)
    {
        std::cerr << "Caught exception: " << e.what() << std::endl;
    }

    return 0;
}
