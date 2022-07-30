#define _CRT_SECURE_NO_WARNINGS 1
#include "UnionFindSet.hpp"

using namespace std;

int main()
{
	auto ufs = new UnionFindSet(10);
	ufs->Union(0, 6);
	ufs->Union(0, 7);
	ufs->Union(0, 8);
	ufs->Union(0, 1);

	ufs->Union(1, 4);
	ufs->Union(1, 9);

	ufs->Union(2, 3);
	ufs->Union(2, 5);

	/*ufs->Show();
	cout << endl;*/

	cout << ufs->Count() << endl;
	for (int i = 0; i < 10; ++i)
	{
		cout << ufs->Find(i) << " ";
	}

	return 0;
}