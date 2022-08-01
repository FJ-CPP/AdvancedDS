#pragma once
#include <vector>
#include <iostream>

using std::vector;
using std::cout;

/*
 * 并查集中：
 * 负号代表该节点为集合的根节点，该负数的绝对值表示集合中元素的个数
 * 非负数表示该节点所在集合的根节点下标
*/
class UnionFindSet
{
private:
	vector<int> _ufs;
public:
	UnionFindSet(size_t size)
		:_ufs(size, -1)
	{
	}

	int Find(int x)
	{
		if (_ufs[x] < 0)
		{
			return x;
		}
		else
		{
			return _ufs[x] = Find(_ufs[x]);
		}
	}

	void Union(int x, int y)
	{
		int root1 = Find(x);
		int root2 = Find(y);

		// 两个集合不同，则进行合并
		if (root1 != root2)
		{
			_ufs[root1] += _ufs[root2];
			_ufs[root2] = root1;
		}

	}

	int Count()
	{
		int res = 0;
		for (auto& e : _ufs)
		{
			if (e < 0)
			{
				++res;
			}
		}
		return res;
	}

	bool InSet(int x, int y)
	{
		return Find(x) == Find(y);
	}

};