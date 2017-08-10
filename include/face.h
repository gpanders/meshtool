#ifndef MESHTOOL_FACE_H_
#define MESHTOOL_FACE_H_

#include "vertex.h"

#include <Eigen/Geometry>

namespace meshtool {

class Face {
    private:
    std::vector< std::reference_wrapper<Vertex> > vertices_;
    Eigen::Vector3d centroid_;
    Eigen::Vector3d normal_;
    double area_;

    public:
    Face() {};
    Face(Vertex& a, Vertex& b, Vertex& c);

    inline const std::vector< std::reference_wrapper<Vertex> > Vertices() const;
    inline std::vector< std::reference_wrapper<Vertex> >& Vertices();
    inline void SetVertices(const std::vector< std::reference_wrapper<Vertex> >& vertices); 

    inline const Eigen::Vector3d Centroid() const;
    inline Eigen::Vector3d& Centroid();
    inline double Centroid(const size_t idx) const;
    inline void SetCentroid(const Eigen::Vector3d& centroid);

    inline const Eigen::Vector3d Normal() const;
    inline Eigen::Vector3d& Normal();
    inline double Normal(const size_t idx) const;
    inline void SetNormal(const Eigen::Vector3d& normal);

    inline double Area() const;
    inline void SetArea(const double area);
};

const std::vector< std::reference_wrapper<Vertex> > Face::Vertices() const { return vertices_; }
std::vector< std::reference_wrapper<Vertex> >& Face::Vertices() { return vertices_; }
void Face::SetVertices(const std::vector< std::reference_wrapper<Vertex> >& vertices) { vertices_ = vertices; }

const Eigen::Vector3d Face::Centroid() const { return centroid_; }
Eigen::Vector3d& Face::Centroid() { return centroid_; }
double Face::Centroid(const size_t idx) const { return centroid_(idx); }
void Face::SetCentroid(const Eigen::Vector3d& centroid) { centroid_ = centroid; }

const Eigen::Vector3d Face::Normal() const { return normal_; }
Eigen::Vector3d& Face::Normal() { return normal_; }
double Face::Normal(const size_t idx) const { return normal_(idx); }
void Face::SetNormal(const Eigen::Vector3d& normal) { normal_ = normal; }

double Face::Area() const { return area_; }
void Face::SetArea(const double area) { area_ = area; }

} // namespace meshtool

#endif // MESHTOOL_FACE_H_
