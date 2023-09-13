#include "pillar/utility/timeit.h"

class TestObject
{
public:
    TestObject(float value) : mValue(value) {}
    float value() const { return mValue; }
    void setValue(float value) { mValue = value; }

private:
    float mValue;
};

int main()
{
    TestObject obj(1.f);
    yuzu::timeit(&obj, &TestObject::setValue, 2.f);
    auto res = yuzu::timeit(&obj, &TestObject::value);
    std::cout << "value: " << res << std::endl;
    return 0;
}