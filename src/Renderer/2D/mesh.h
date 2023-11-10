#pragma once

#include "./primitives.h"
#include <vector>


struct Mesh{
    std::vector<Point> points;
    std::vector<uint32_t> indices;
};
