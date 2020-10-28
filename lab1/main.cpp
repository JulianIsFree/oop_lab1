#include <iostream>
#include "TritSetLab1.h"

using namespace std;
using namespace LabTritSetSpace;

int main()
{
	TritSet set2(32);
	for (size_t i = 0; i < 11; ++i)
		set2[i] = Trit::False;
	cout << "Last set index: " << set2.getLastSetTrit() << endl;
	cout << "True: " << set2.cardinality()[Trit::True] << endl;
	cout << "False: " << set2.cardinality()[Trit::False] << endl;
	cout << "Unknown: " << set2.cardinality()[Trit::Unknown] << endl;
	cout << "Unknown: " << set2.cardinality(Trit::Unknown) << endl;
	cout << set2 << endl;

	cout << endl << "Trim" << endl;
	set2.trim(5);
	
	cout << "Last set index: " << set2.getLastSetTrit() << endl;
	cout << "True: " << set2.cardinality()[Trit::True] << endl;
	cout << "False: " << set2.cardinality()[Trit::False] << endl;
	cout << "Unknown: " << set2.cardinality()[Trit::Unknown] << endl;
	cout << "Unknown: " << set2.cardinality(Trit::Unknown) << endl;
	cout << set2 << endl;

	cout << endl << "set2[64] = Trit::True" << endl;
	set2[64] = Trit::True;

	cout << "Last set index: " << set2.getLastSetTrit() << endl;
	cout << "True: " << set2.cardinality()[Trit::True] << endl;
	cout << "False: " << set2.cardinality()[Trit::False] << endl;
	cout << "Unknown: " << set2.cardinality()[Trit::Unknown] << endl;
	cout << "Unknown: " << set2.cardinality(Trit::Unknown) << endl;
	cout << set2 << endl;

	cout << endl << "set2[64] = Trit::Unknown" << endl;
	set2[64] = Trit::Unknown;

	cout << "Last set index: " << set2.getLastSetTrit() << endl;
	cout << "True: " << set2.cardinality()[Trit::True] << endl;
	cout << "False: " << set2.cardinality()[Trit::False] << endl;
	cout << "Unknown: " << set2.cardinality()[Trit::Unknown] << endl;
	cout << "Unknown: " << set2.cardinality(Trit::Unknown) << endl;
	cout << set2 << endl;
	return 0;
}