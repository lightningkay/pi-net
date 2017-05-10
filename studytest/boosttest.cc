#include <iostream>

using namespace std;

#define BOOST_TEST_MAIN
#define BOOST_TEST_DYN_LINK
#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_CASE(t1)
{
    cout << "test 1" << endl;
}

BOOST_AUTO_TEST_CASE(t2)
{
    cout << "test 2" << endl;
}
