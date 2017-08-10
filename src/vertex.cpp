#include "vertex.h"

namespace meshtool {

Vertex::Vertex(const double& x, const double& y, const double& z)
{
    this->coords_ << x, y, z;
    this->normal_ << 0.0, 0.0, 0.0;
    this->color_ = { 128, 128, 128 };
    this->uncertainty_ = -1.0;
}

bool Vertex::operator==(const Vertex& other)
{
    return this->coords_.isApprox(other.Coords());
}

bool Vertex::WithinRadius(const Vertex& other, const double& radius)
{
    return this->coords_.isApprox(other.Coords(), radius);
}

bool Vertex::IsNear(const Vertex& other)
{
    return WithinRadius(other, 0.01);
}

} // namespace meshtool
