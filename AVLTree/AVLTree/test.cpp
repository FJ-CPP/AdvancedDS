#define _CRT_SECURE_NO_WARNINGS 1
#include <iostream>
#include <algorithm>
#include <map>
#include <vector>
using namespace std;
#include "AVLTree.hpp"

int main()
{
	//int a[] = { 11,24,73,45,23,59,86,61 };
	vector<int> v;
	AVLTree<int, int> t;
	srand(time(0));
	for (size_t i = 0; i < 100; ++i)
	{
		v.push_back(rand());
	}
	for (auto e : v)
	{
		t.insert(make_pair(e, e));
	}
	//t.inOrderTraversal();
	cout << t.balanced() << endl;
	return 0;
}