#include <iostream>

using namespace std;

int i = 92;

int main()
{
	cout << "Hello C++ !" << endl;
	int seti = 0;
	// lock set
	seti = __sync_lock_test_and_set(&i, 1);
	cout << "sync lock test set i = " << seti << endl;
	cout << "new i = " << i << endl;

	// add
	seti = __sync_fetch_and_add(&i, 5);
	cout << "sync lock test set i = " << seti << endl;
	cout << "new i = " << i << endl;
	// get
	seti = __sync_val_compare_and_swap(&i, 6, 0);
	cout << "sync lock test set i = " << seti << endl;
	cout << "new i = " << i << endl;
	return 0;
}
