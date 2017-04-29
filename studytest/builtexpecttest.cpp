#include <iostream>

using namespace std;

int main()
{
    cout << "Hello C++ !" << endl;
    int i = 1;
    if (__builtin_expect(i == 1, 0))
    {
        cout << "i != 1" << endl;

    }
    i = 2;
    if (__builtin_expect(i != 1, 1))
    {
        cout << "i == 1" << endl;
    }
    return 0;
}
