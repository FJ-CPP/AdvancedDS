#define _CRT_SECURE_NO_WARNINGS 1
#include <iostream>
using namespace std;
//#include "BSTree(K-Mode).hpp"
#include "BSTree(K-V-Mode).hpp"
//void testKMode()
//{
//	BSTree<int> bst;
//	int a[] = { 5,3,4,1,7,8,2,6,0,9 };
//	for (auto e : a)
//	{
//		bst.insert(e);
//	}
//	bst.inorder();
//
//	bst.erase(7);
//	bst.inorder();
//
//	bst.erase(2);
//	bst.inorder();
//
//	bst.erase(6);
//	bst.inorder();
//
//	bst.erase(9);
//	bst.inorder();
//
//	for (int i = 0; i < 8; ++i)
//	{
//		bst.erase(a[i]);
//	}
//	bst.inorder();
//
//	bst.erase(0);
//	bst.inorder();
//}
void testKVMode1()
{
	BSTree<string, int> count;
	string str[] = { "苹果","香蕉", "橘子", "香蕉", "橘子", "苹果", "苹果",
		"苹果", "橘子", "香蕉", "苹果", "橘子", "苹果", "苹果", "香蕉" };
	for (auto e : str)
	{
		BSTreeNode<string, int>* ret = count.find(e);
		if (ret == nullptr)
		{
			count.insert(e, 1);
		}
		else
		{
			ret->_val++;
		}
	}
	count.inorder();
}
void testKVMode2()
{
	BSTree<string, string> dict;
	dict.insert("hello", "你好");
	dict.insert("world", "世界");
	string input;
	while (cin >> input)
	{
		BSTreeNode<string, string>* ret = dict.find(input);
		if (ret != nullptr)
		{
			cout << ret->_val << endl;
		}
		else
		{
			cout << "没有相关单词翻译" << endl;
		}
	}
}
int main()
{
	//testKMode();
	//testKVMode1();
	testKVMode2();
	return 0;
}