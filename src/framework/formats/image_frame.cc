#include <algorithm>
#include <cstring>
#include <memory>
#include <sstream>

#include "pillar/framework/deps/aligned_malloc_and_free.h"
#include "pillar/framework/formats/image_frame.h"

namespace yuzu
{
const ImageFrame::Deleter ImageFrame::PixelDataDeleter::kArrayDelete = std::default_delete<uint8_t[]>();
const ImageFrame::Deleter ImageFrame::PixelDataDeleter::kFree = free;
const ImageFrame::Deleter ImageFrame::PixelDataDeleter::kAlignedFree = alignedFree;
const ImageFrame::Deleter ImageFrame::PixelDataDeleter::kNone = [](uint8_t* x) {};

ImageFrame::ImageFrame() : mFormat(ImageFormat::Format::UNKNOWN), mWidth(0), mHeight(0), mWidthStep(0) {}
ImageFrame::ImageFrame(ImageFormat::Format format, int width, int height)
{ //
    reset(format, width, height, kDefaultAlignmentBoundary);
}
ImageFrame::ImageFrame(ImageFormat::Format format, int width, int height, int widthStep, uint8_t* pixelData,
                       Deleter deleter)
{ //
    adoptPixelData(format, width, height, widthStep, pixelData, deleter);
}

ImageFrame::ImageFrame(ImageFrame&& move_from) { *this = std::move(move_from); }

ImageFrame& ImageFrame::operator=(ImageFrame&& move_from)
{
    mPixelData = std::move(move_from.mPixelData);
    mFormat = move_from.mFormat;
    mWidth = move_from.mWidth;
    mHeight = move_from.mHeight;
    mWidthStep = move_from.mWidthStep;

    move_from.mFormat = ImageFormat::Format::UNKNOWN;
    move_from.mWidth = 0;
    move_from.mHeight = 0;
    move_from.mWidthStep = 0;
    return *this;
}

std::unique_ptr<uint8_t[], ImageFrame::Deleter> ImageFrame::release() { return std::move(mPixelData); }

void ImageFrame::reset(ImageFormat::Format format, int width, int height, uint32_t alignmentBoundary)
{
    mFormat = format;
    mWidth = width;
    mHeight = height;
    mWidthStep = width * channels() * byteDepth();
    if (alignmentBoundary == 1)
    {
        mPixelData = {new uint8_t[height * mWidthStep], PixelDataDeleter::kArrayDelete};
    }
    else
    {
        mWidthStep = ((mWidthStep - 1) | (alignmentBoundary - 1)) + 1;
        mPixelData = {reinterpret_cast<uint8_t*>(alignedMalloc(height * mWidthStep, alignmentBoundary)),
                      PixelDataDeleter::kAlignedFree};
    }
}

// Be sure the `format` is not equal to `ImageFormat::UNKNOWN`, and the `step` is
// equal to `channels() * byteDepth()`
void ImageFrame::adoptPixelData(ImageFormat::Format format, int width, int height, int step, uint8_t* data,
                                ImageFrame::Deleter deleter)
{
    mFormat = format;
    mWidth = width;
    mHeight = height;
    mWidthStep = step;
    mPixelData = {data, deleter};
}

void ImageFrame::setToZero()
{
    if (mPixelData)
    {
        std::fill_n(mPixelData.get(), mWidthStep * mHeight, 0);
    }
}

bool ImageFrame::isContiguous() const
{
    if (!mPixelData)
    {
        return false;
    }

    return mWidthStep == mWidth * channels() * byteDepth();
}

bool ImageFrame::isAligned(uint32_t alignmentBoundary) const
{
    if (!mPixelData)
    {
        return false;
    }

    if ((reinterpret_cast<uintptr_t>(mPixelData.get()) % alignmentBoundary) != 0)
    {
        return false;
    }

    if ((mWidthStep % alignmentBoundary) != 0)
    {
        return false;
    }

    return true;
}

int ImageFrame::channels() const { return numberOfChannelsForFormat(format()); }
int ImageFrame::channelSize() const { return channelSizeForFormat(format()); }
int ImageFrame::byteDepth() const { return byteDepthForFormat(format()); }

int ImageFrame::numberOfChannelsForFormat(ImageFormat::Format format)
{
    switch (format)
    {
        case ImageFormat::Format::SRGB:
            return 3;
        case ImageFormat::Format::SRGBA:
            return 4;
        case ImageFormat::Format::GRAY8:
            return 1;
        case ImageFormat::Format::GRAY16:
            return 1;
        case ImageFormat::Format::SRGB48:
            return 3;
        case ImageFormat::Format::SRGBA64:
            return 4;
        case ImageFormat::Format::VEC32F1:
            return 1;
        case ImageFormat::Format::VEC32F2:
            return 2;
        case ImageFormat::Format::LAB8:
            return 3;
        case ImageFormat::Format::SBGRA:
            return 4;
        default:
            return 0;
    }
}

int ImageFrame::channelSizeForFormat(ImageFormat::Format format)
{
    switch (format)
    {
        case ImageFormat::Format::SRGB:
            return sizeof(uint8_t);
        case ImageFormat::Format::SRGBA:
            return sizeof(uint8_t);
        case ImageFormat::Format::GRAY8:
            return sizeof(uint8_t);
        case ImageFormat::Format::GRAY16:
            return sizeof(uint16_t);
        case ImageFormat::Format::SRGB48:
            return sizeof(uint16_t);
        case ImageFormat::Format::SRGBA64:
            return sizeof(uint16_t);
        case ImageFormat::Format::VEC32F1:
            return sizeof(float);
        case ImageFormat::Format::VEC32F2:
            return sizeof(float);
        case ImageFormat::Format::LAB8:
            return sizeof(uint8_t);
        case ImageFormat::Format::SBGRA:
            return sizeof(uint8_t);
        default:
            return 0;
    }
};

int ImageFrame::byteDepthForFormat(ImageFormat::Format format)
{
    switch (format)
    {
        case ImageFormat::Format::SRGB:
            return 1;
        case ImageFormat::Format::SRGBA:
            return 1;
        case ImageFormat::Format::GRAY8:
            return 1;
        case ImageFormat::Format::GRAY16:
            return 2;
        case ImageFormat::Format::SRGB48:
            return 2;
        case ImageFormat::Format::SRGBA64:
            return 2;
        case ImageFormat::Format::VEC32F1:
            return 4;
        case ImageFormat::Format::VEC32F2:
            return 4;
        case ImageFormat::Format::LAB8:
            return 1;
        case ImageFormat::Format::SBGRA:
            return 1;
        default:
            return 0;
    }
};

void ImageFrame::internalCopyFrom(int width, int height, int widthStep, int channelSize, const uint8_t* pixelData)
{ //
    const int rowBytes = channelSize * channels() * width;
    if (widthStep == 0)
    {
        widthStep = channelSize * channels() * width;
    }

    const char* src = reinterpret_cast<const char*>(pixelData);
    char* dst = reinterpret_cast<char*>(mPixelData.get());

    if (widthStep == rowBytes && mWidthStep == rowBytes)
    {
        std::memcpy(dst, src, mHeight * rowBytes);
    }
    else
    {
        for (int i = mHeight; i > 0; --i)
        {
            memcpy(dst, src, rowBytes);
            src += widthStep;
            dst += mWidthStep;
        }
    }
}

void ImageFrame::internalCopyToBuffer(int widthStep, char* buffer) const
{
    const int rowBytes = channelSize() * channels() * mWidth;
    if (widthStep == 0)
    {
        widthStep = channelSize() * channels() * mWidth;
    }

    const char* src = reinterpret_cast<const char*>(mPixelData.get());
    char* dst = buffer;
    if (widthStep == rowBytes && mWidthStep == rowBytes)
    {
        std::memcpy(dst, src, mHeight * rowBytes);
    }
    else
    {
        for (int i = mHeight; i > 0; --i)
        {
            memcpy(dst, src, rowBytes);
            src += mWidthStep;
            dst += widthStep;
        }
    }
}

void ImageFrame::copyFrom(const ImageFrame& imageFrame, uint32_t alignmentBoundary)
{
    reset(imageFrame.format(), imageFrame.width(), imageFrame.height(), alignmentBoundary);
    internalCopyFrom(imageFrame.width(), imageFrame.height(), imageFrame.step(), imageFrame.channelSize(),
                     imageFrame.pixelData());
}
void ImageFrame::copyPixelData(ImageFormat::Format format, int width, int height, const uint8_t* pixelData,
                               uint32_t alignmentBoundary)
{
    copyPixelData(format, width, height, 0, pixelData, alignmentBoundary);
}
void ImageFrame::copyPixelData(ImageFormat::Format format, int width, int height, int withStep,
                               const uint8_t* pixelData, uint32_t alignmentBoundary)
{
    reset(format, width, height, alignmentBoundary);
    internalCopyFrom(width, height, withStep, channelSizeForFormat(format), pixelData);
}

void ImageFrame::copyToBuffer(uint8_t* buffer, int bufferSize) const
{
    // dataSize <= bufferSize
    const int dataSize = mWidth * mHeight * channels();
    if (isContiguous())
    {
        const uint8_t* src = reinterpret_cast<const uint8_t*>(mPixelData.get());
        std::copy_n(src, dataSize, buffer);
    }
    else
    {
        internalCopyToBuffer(0, reinterpret_cast<char*>(buffer));
    }
}

void ImageFrame::copyToBuffer(uint16_t* buffer, int bufferSize) const
{
    // dataSize <= bufferSize
    // byteDepth == 2
    const int dataSize = mWidth * mHeight * channels();
    if (isContiguous())
    {
        const uint16_t* src = reinterpret_cast<const uint16_t*>(mPixelData.get());
        std::copy_n(src, dataSize, buffer);
    }
    else
    {
        internalCopyToBuffer(0, reinterpret_cast<char*>(buffer));
    }
}

void ImageFrame::copyToBuffer(float* buffer, int bufferSize) const
{
    // dataSize <= bufferSize
    // byteDepth == 4
    const int dataSize = mWidth * mHeight * channels();
    if (isContiguous())
    {
        const float* src = reinterpret_cast<const float*>(mPixelData.get());
        std::copy_n(src, dataSize, buffer);
    }
    else
    {
        internalCopyToBuffer(0, reinterpret_cast<char*>(buffer));
    }
}

std::ostream& operator<<(std::ostream& os, const ImageFrame& obj)
{
    return os << "[" << obj.width() << ", " << obj.height() << ", " << obj.channels() << "]";
}
} // namespace yuzu
