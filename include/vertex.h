#ifndef MESHTOOL_VERTEX_H_
#define MESHTOOL_VERTEX_H_

#include <Eigen/Core>

namespace meshtool {

class Vertex {
    private:
    Eigen::Vector3d coords_;
    Eigen::Vector3d normal_;
    std::array<uint8_t, 3> color_;
    double uncertainty_;

    public:
    Vertex() {};
    Vertex(const double& x, const double& y, const double& z);

    bool operator==(const Vertex& other);

    bool WithinRadius(const Vertex& other, const double& radius);
    bool IsNear(const Vertex& other);

    inline const Eigen::Vector3d Coords() const;
    inline Eigen::Vector3d& Coords();
    inline double Coords(const size_t idx) const;
    inline double& Coords(const size_t idx);
    inline void SetCoords(const Eigen::Vector3d& coords);

    inline const Eigen::Vector3d Normal() const;
    inline Eigen::Vector3d& Normal();
    inline double Normal(const size_t idx) const;
    inline void SetNormal(const Eigen::Vector3d& normal);

    inline const std::array<uint8_t, 3> Color() const;
    inline std::array<uint8_t, 3>& Color();
    inline uint8_t Color(const size_t idx) const;
    inline void SetColor(const std::array<uint8_t, 3>& color);

    inline double Uncertainty() const;
    inline void SetUncertainty(const double uncertainty);
};

const Eigen::Vector3d Vertex::Coords() const { return coords_; }

Eigen::Vector3d& Vertex::Coords() { return coords_; }

double Vertex::Coords(const size_t idx) const { return coords_(idx); }

double& Vertex::Coords(const size_t idx) { return coords_(idx); }

void Vertex::SetCoords(const Eigen::Vector3d& coords) { coords_ = coords; }

const Eigen::Vector3d Vertex::Normal() const { return normal_; }

Eigen::Vector3d& Vertex::Normal() { return normal_; }

double Vertex::Normal(const size_t idx) const { return normal_(idx); }

void Vertex::SetNormal(const Eigen::Vector3d& normal) { normal_ = normal; }

const std::array<uint8_t, 3> Vertex::Color() const { return color_; }

std::array<uint8_t, 3>& Vertex::Color() { return color_; }

uint8_t Vertex::Color(const size_t idx) const { return color_[idx]; }

void Vertex::SetColor(const std::array<uint8_t, 3>& color) { color_ = color; }

double Vertex::Uncertainty() const { return uncertainty_; }

void Vertex::SetUncertainty(const double uncertainty) { uncertainty_ = uncertainty; }

} // namespace meshtool

#endif // MESHTOOL_VERTEX_H_
