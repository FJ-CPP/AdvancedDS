#define _CRT_SECURE_NO_WARNINGS 1
#include "BTree.hpp"


int main()
{
	int a[] = { 53, 139, 75, 49, 145, 36, 101, 321, 13, 45, 543, 233, 334, 49 };
	//int a[] = { 3, 4, 2, 1, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14 };

	auto t = new BTree<int, 3>;

	for (auto e : a)
	{
		t->Insert(e);
	}

	t->Inorder();
	return 0;
}