#include <iostream>

#include "pillar/framework/formats/image_frame.h"

using namespace yuzu;
int main()
{
    yuzu::ImageFrame img1(ImageFormat::SRGB, 112, 112);
    std::cout << "image(1): " << img1 << std::endl;
    std::cout << "image(1) contiguous: " << img1.isContiguous() << std::endl;
    std::cout << "image(1) step: " << img1.step() << "|" << 112 % sizeof(void*) << std::endl;

    yuzu::ImageFrame img2(ImageFormat::Format::SRGB, 111, 111);
    std::cout << "image(2): " << img2 << std::endl;
    std::cout << "image(2) contiguous: " << img2.isContiguous() << std::endl;
    std::cout << "image(2) step: " << img2.step() << "|" << 111 % sizeof(void*) << std::endl;
    return 0;
}