#define _CRT_SECURE_NO_WARNINGS 1
#include <iostream>
using namespace std;
#include "BloomFilter.h"
int main()
{
	BloomFliter<100> b;
	b.set("abc");
	b.set("abd");
	b.set("abe");
	b.set("abf");
	cout << b.test("abd") << endl;
	return 0;
}