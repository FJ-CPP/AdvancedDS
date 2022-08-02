#define _CRT_SECURE_NO_WARNINGS 1
#include <iostream>
#include "LRU.hpp"

int main()
{
	LRUCache lru(2);
	cout << lru.get(2) << endl;
	lru.put(2, 6);
	cout << lru.get(1) << endl;
	lru.put(1, 5);
	lru.put(1, 2);
	cout << lru.get(1) << endl;
	cout << lru.get(2) << endl;

	return 0;
}