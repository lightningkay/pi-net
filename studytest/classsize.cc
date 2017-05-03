#include <iostream>

class Test
{
public:
    void print()
    {
        std::cout << "t = " << t << std::endl;
    }
    virtual void print(int i)
    {
        std::cout << "i = " << i << std::endl;
    }
private:
    int32_t t;
};

class TestSon : public Test
{
public:
    void print()
    {
        std::cout << "t = " << ts << std::endl;
    }
    void print(int s)
    {
        std::cout << "is = " << s << std::endl;
    }
private:
    int64_t ts;
};

int main()
{
    std::cout << " address size = " << sizeof(void*) * 8 << std::endl;
    std::cout << "Test size" << std::endl;
    std::cout << sizeof(Test) << std::endl;
    std::cout << "TestSon size" << std::endl;
    std::cout << sizeof(TestSon) << std::endl;
    return 0;
}
