#include <fstream>

#include "colmap.h"

namespace meshtool {

namespace {

template<typename T>
T ReadBinary(std::istream* stream) {
    T data_little_endian;
    stream->read(reinterpret_cast<char*>(&data_little_endian), sizeof(T));
    return data_little_endian;
}

} // namespace

std::vector<Vertex> ReadCOLMAP(const std::string& fname)
{
    std::ifstream file(fname, std::ios::binary);
    const auto num_points = ReadBinary<uint64_t>(&file);
    std::vector<Vertex> points(num_points);

    for (size_t i = 0; i < num_points; i++)
    {
        Vertex vertex;
        ReadBinary<uint64_t>(&file); // point3d_id
        // The first three values are the X, Y, Z coordinates of the point
        vertex.Coords(0) = ReadBinary<double>(&file); // x
        vertex.Coords(1) = ReadBinary<double>(&file); // y
        vertex.Coords(2) = ReadBinary<double>(&file); // z

        // The next 3 values are the colors of the 3d point, which we are 
        // not concerned with
        ReadBinary<uint8_t>(&file); // color[0]
        ReadBinary<uint8_t>(&file); // color[1]
        ReadBinary<uint8_t>(&file); // color[2]

        // Next is the reprojection error
        ReadBinary<double>(&file); // error

        // And finally the uncertainty
        vertex.SetUncertainty(ReadBinary<double>(&file)); // uncertainty

        // Next are the tracks, which we don't care about
        const auto track_length = ReadBinary<uint64_t>(&file);
        for (size_t j = 0; j < track_length; j++)
        {
            ReadBinary<uint32_t>(&file); // image_id
            ReadBinary<uint32_t>(&file); // point2d_idx
        }
        
        points[i] = vertex;
    }

    return points;
}

} // namespace meshtool
