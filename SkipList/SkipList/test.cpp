#define _CRT_SECURE_NO_WARNINGS 1
#include <iostream>
#include "SkipList.hpp"

int main()
{
	srand((unsigned int)time(nullptr));

	//SkipList sl;

	//sl.Add(1);
	//sl.Add(2);
	//sl.Add(3);
	//sl.Add(4);
	//sl.Add(5);

	//cout << sl.Search(1) << endl;
	//cout << sl.Search(2) << endl;
	//cout << sl.Search(3) << endl;
	//cout << sl.Search(4) << endl;
	//cout << sl.Search(5) << endl;

	SkipList sl;
	int time = rand();

	for (int i = 0; i < 10; ++i)
	{
		sl.Add(i);
		sl.Erase(i);
	}


	//sl.Add(1);
	//cout << sl.Search(1) << endl;
	//sl.Erase(1);
	//cout << sl.Search(1) << endl;

	return 0;
}