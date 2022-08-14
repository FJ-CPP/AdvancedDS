#define _CRT_SECURE_NO_WARNINGS 1
#include <iostream>
using namespace std;
#include "BitSet.h"
int main()
{
	myBitSet::bitset<10> bs;
	bs.set(9);
	bs.set(5);
	bs.set(7);
	bs.set(0);
	for (size_t i = 0; i < 10; ++i)
	{
		cout << i << " : " << bs.test(i) << endl;
	}
	cout << "位图当前数据个数：" << bs.size() << endl;
	return 0;
}
