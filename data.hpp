
#ifndef DATA_HEADER
#define DATA_HEADER

#include "totalpreprocess.hpp"

std::vector<Polygon> loadfile(std::istream &ifs);
void writefile(std::ostream& ofs, const std::vector<Polygon>& polygons);

#endif
