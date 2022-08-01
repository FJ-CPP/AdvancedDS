#pragma once
#include <vector>
#include <iostream>

using std::vector;
using std::cout;

/*
 * ���鼯�У�
 * ���Ŵ���ýڵ�Ϊ���ϵĸ��ڵ㣬�ø����ľ���ֵ��ʾ������Ԫ�صĸ���
 * �Ǹ�����ʾ�ýڵ����ڼ��ϵĸ��ڵ��±�
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

		// �������ϲ�ͬ������кϲ�
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