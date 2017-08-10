#ifndef MESHTOOL_COLMAP_H_
#define MESHTOOL_COLMAP_H_

#include "vertex.h"

namespace meshtool {

std::vector<Vertex> ReadCOLMAP(const std::string& fname); 

} // namespace meshtool

#endif // MESHTOOL_COLMAP_H_
