#include <iostream>
#include "TritSetLab1.h"

using namespace std;
using namespace LabTritSetSpace;

int main()
{
	TritSet set1(10);
	cout << set1 << endl;
	TritSet set2(11);
	for (size_t i = 0; i < 11; ++i)
		set2[i] = Trit::False;
	cout << set2 << endl;
	TritSet set3 = set1 & set2;
	cout << set3;


	return 0;
}