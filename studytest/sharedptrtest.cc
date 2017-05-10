#include <iostream>
#include <boost/shared_ptr.hpp>

using namespace std;

class A
{
public:
    A(): a_(0)
    {
    }

    void setAa(int a)
    {
        a_ = a;
    }

    void print()
    {
        std::cout << " class A" << std::endl;
        std::cout << "a_ = " << a_ << std::endl;
    }
private:
    int a_;
};

int main()
{
    boost::shared_ptr<A> a1(new A());
    std::cout << " a1 reference count: " << a1.use_count() << std::endl;
    boost::shared_ptr<A> a2 = a1;
    std::cout << " a1 reference count: " << a1.use_count() << std::endl;
    a2.reset();
    std::cout << "a2 == " << a2 << std::endl;
    assert(a2 == nullptr);
    // std::cout << " a1 reference count: " << a1.use_count() << std::endl;
    std::cout << " a1 reference count: " << a1.use_count() << std::endl;
    cout << "Hello C++ !" << endl;
    return 0;
}
