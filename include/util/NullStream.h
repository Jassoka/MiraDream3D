//
// Created by jassoka on 7/24/26.
//

#ifndef MIRADREAM3D_NULLSTREAM_H
#define MIRADREAM3D_NULLSTREAM_H
#include <sstream>

class NullStream : public std::ostringstream {
};

inline NullStream _cnull;
#endif //MIRADREAM3D_NULLSTREAM_H
