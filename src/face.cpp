#include "face.h"

namespace meshtool {

Face::Face(Vertex& a, Vertex& b, Vertex& c)
{
    this->vertices_ = { 
        std::reference_wrapper<Vertex>(a), 
        std::reference_wrapper<Vertex>(b), 
        std::reference_wrapper<Vertex>(c)
    };

    Eigen::Vector3d x = b.Coords() - a.Coords();
    Eigen::Vector3d y = c.Coords() - a.Coords();

    // Compute the area of the face
    this->normal_ = x.cross(y);
    this->area_ = this->normal_.norm() / 2;
    this->normal_ = this->normal_ / this->normal_.norm();

    a.Normal() = a.Normal() + this->normal_;
    a.Normal() = a.Normal() / a.Normal().norm();

    b.Normal() = b.Normal() + this->normal_;
    b.Normal() = b.Normal() / b.Normal().norm();

    c.Normal() = c.Normal() + this->normal_;
    c.Normal() = c.Normal() / c.Normal().norm();

    // Calculate centroid
    this->centroid_ <<
        (a.Coords(0) + b.Coords(0) + c.Coords(0)) / 3,
        (a.Coords(1) + b.Coords(1) + c.Coords(1)) / 3,
        (a.Coords(2) + b.Coords(2) + c.Coords(2)) / 3;

}

} // namespace meshtool
