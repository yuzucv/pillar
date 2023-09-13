#include <iostream>

#include "pillar/framework/coretypes.h"

int main()
{
    // ==================================
    // Dim
    // ==================================
    yuzu::Dims dim{.d = {1, 112, 112, 3}, .nbDims = 4};
    std::cout << "dim: " << dim << " size of " << sizeof(yuzu::Dims) << std::endl;

    // ==================================
    // Point2
    // ==================================
    yuzu::Point2<int> point{1, 2};
    std::cout << "point: " << point << " size of " << sizeof(yuzu::Point2<int>) << std::endl;

    // ==================================
    // rectangle
    // ==================================
    yuzu::Rectangle<int> rect{1, 2, 3, 4};
    std::cout << "rect: " << rect << " size of " << sizeof(yuzu::Rectangle<int>) << std::endl;

    return 0;
}