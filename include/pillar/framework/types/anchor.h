#pragma once

namespace yuzu
{
namespace deps
{
class Anchor
{
public:
    float xCenter() const { return mXCenter; }
    float yCenter() const { return mYCenter; }
    float width() const { return mWidth; }
    float height() const { return mHeight; }

    void setXCenter(float value) { mXCenter = value; }
    void setYCenter(float value) { mYCenter = value; }
    void setWidth(float value) { mWidth = value; }
    void setHeight(float value) { mHeight = value; }

private:
    float mXCenter;
    float mYCenter;
    float mWidth;
    float mHeight;
};
} // namespace deps
} // namespace yuzu
