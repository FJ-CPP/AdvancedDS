#pragma once
#include <iostream>
#include <queue>
#include <vector>
#include <map>
#include <unordered_set>
#include <climits>
#include "UnionFindSet.hpp"

using std::cout;
using std::endl;

template <class V, class W, W W_MAX = 0x3f3f3f3f, bool Direction = false>
class Graph
{
private:
	std::vector<V> _vertexes; // ����
	std::map<V, int> _idx; // �����Ӧ�±�
	std::vector<std::vector<W>> _edges; // �ڽӾ���
public:
	Graph() = default;
	Graph(const V* v, size_t n)
	{
		// ��ʼ�����㼰��Ӧ�±�
		_vertexes.resize(n);
		for (size_t i = 0; i < n; ++i)
		{
			_vertexes[i] = v[i];
			_idx[v[i]] = i;
		}
		// ��ʼ���ڽӾ���
		_edges.resize(n);
		for (size_t i = 0; i < n; ++i)
		{
			_edges[i].resize(n, W_MAX);
		}
	}

	int GetIdx(const V& x)
	{
		auto it = _idx.find(x);
		if (it == _idx.end())
		{
			return -1;
		}
		else
		{
			return it->second;
		}
	}

	void _AddEdge(size_t srci, size_t dsti, const W& w)
	{
		_edges[srci][dsti] = w;
		// ����ͼ
		if (Direction == false)
		{
			_edges[dsti][srci] = w;
		}
	}

	bool AddEdge(const V& x, const V& y, const W& w)
	{
		int src = GetIdx(x);
		int dest = GetIdx(y);

		if (src == -1 || dest == -1)
		{
			return false;
		}

		_AddEdge(src, dest, w);

		return true;
	}

	void Print()
	{
		// ����
		for (size_t i = 0; i < _vertexes.size(); ++i)
		{
			cout << "[" << i << "]" << "->" << _vertexes[i] << endl;
		}
		cout << endl;

		// ����
		// ���±�
		cout << "  ";
		for (size_t i = 0; i < _vertexes.size(); ++i)
		{
			printf("%4d", i);
		}
		cout << endl;

		for (size_t i = 0; i < _edges.size(); ++i)
		{
			cout << i << " "; // ���±�
			for (size_t j = 0; j < _edges[i].size(); ++j)
			{
				//cout << _matrix[i][j] << " ";
				if (_edges[i][j] == W_MAX)
				{
					//cout << "* ";
					printf("%4c", 'x');
				}
				else
				{
					//cout << _matrix[i][j] << " ";
					printf("%4d", _edges[i][j]);
				}
			}
			cout << endl;
		}
		cout << endl;

		for (size_t i = 0; i < _edges.size(); ++i)
		{
			for (size_t j = 0; j < _edges[i].size(); ++j)
			{
				if (i < j && _edges[i][j] != W_MAX)
				{
					cout << _vertexes[i] << "->" << _vertexes[j] << ":" << _edges[i][j] << endl;
				}
			}
		}

	}

	// ��ȡ��С������ Min-Spanning-Tree(MST)

	struct Edge
	{
		int _src;
		int _dest;
		W _w; // Ȩ��
		Edge(int src, int dest, W w)
			: _src(src), _dest(dest), _w(w)
		{
		}
		bool operator>(const Edge& e) const
		{
			return _w > e._w;
		}
	};

	typedef Graph<V, W, W_MAX, Direction> MST;

	/*
	 * Kruskal(��³˹����)��С�������㷨��
	 * ̰��˼��
	 * �����б߰���Ȩֵ��С��������
	 * ÿ��ѡ��ʣ�����Ȩֵ��С�ı�
	 * ����ñߵļ��뵼�µ�ǰͼ�д��ڻ�(���ò��鼯�жϣ�����ߵ�����������һ�����ϣ���ñ߻ᵼ�³ɻ�)
	 * �������ñ�
	 * ���򣬽��ñ���Ϊ��С��������һ����
	*/ 
	void MinSpanningTreeKruskal(MST& mst)
	{
		size_t n = _vertexes.size();
		mst._vertexes = _vertexes;
		mst._idx = _idx;
		mst._edges.resize(n);
		for (size_t i = 0; i < n; ++i)
		{
			mst._edges[i].resize(n, W_MAX);
		}
		
		std::priority_queue<Edge, vector<Edge>, std::greater<Edge>> pq; // С����
		for (int i = 0; i < n; ++i)
		{
			for (int j = 0; j < n; ++j)
			{
				// �����б߼���С����
				if (_edges[i][j] != W_MAX)
				{
					pq.push(Edge(i, j, _edges[i][j]));
				}
			}
		}
		
		UnionFindSet ufs(n);
		
		size_t i = 0;
		W totalW = W();
		while (!pq.empty())
		{
			Edge e = pq.top();
			pq.pop();

			int x = e._src;
			int y = e._dest;

			if (ufs.InSet(x, y))
			{
				// �ñߵļ���ᵼ�³ɻ�
				continue;
			}

			ufs.Union(x, y);
			++i;
			totalW += e._w;
			mst._edges[x][y] = e._w;

			if (i == n - 1)
			{
				break;
			}
		}
		
		if (i == n - 1)
		{
			cout << "��С����������ɣ���Ȩֵ��" << totalW << endl;
		}
		else
		{
			cout << "��С����������ʧ��" << endl;
		}
	}

	/*
	 * Prim(����ķ)��С�������㷨��
	 * ̰��˼��
	 * ָ��һ����ʼ��start���������ڼ�����ΪX���������Ϊ����Y
	 * ÿ�ִ�X����ѡһ����x��Y����ѡy��ʹ��x->y�ľ�����X�����еĵ㵽Y�����еĵ����Сֵ
	 * Ȼ�󣬽�y�Ӽ���Y��ɾ�������뵽����X
	 * ����ÿ�ζ��Ǵ����������и�ѡһ���㣬��˱�Ȼ���ᵼ�³ɻ�(�ɻ�������һ����������ѡ��������ɵ�)
	*/
	void MinSpanningTreePrim(MST& mst, const V& start)
	{
		size_t n = _vertexes.size();
		mst._vertexes = _vertexes;
		mst._idx = _idx;
		mst._edges.resize(n);
		for (size_t i = 0; i < n; ++i)
		{
			mst._edges[i].resize(n, W_MAX);
		}

		// �����ڶ�Ӧ��Ϊtrue��ʾ����
		int starti = GetIdx(start);
		vector<bool> X(n, false);
		vector<bool> Y(n, true);
		X[starti] = true;
		Y[starti] = false;

		std::priority_queue<Edge, vector<Edge>, std::greater<Edge>> pq; // С����
		for (int i = 0; i < n; ++i)
		{
			if (_edges[starti][i] != W_MAX)
			{
				pq.push(Edge(starti, i, _edges[starti][i]));
			}
		}

		size_t num = 0;
		W totalW = W();
		while (num < n - 1 && !pq.empty())
		{
			Edge e = pq.top();
			pq.pop();

			int x = e._src;
			int y = e._dest;
			if (X[x] && X[y])
			{
				// ��������һ�����ϣ��ɻ�
				continue;
			}

			X[y] = true;
			Y[y] = false;
			totalW += e._w;
			mst._AddEdge(x, y, e._w);
			++num;

			for (int i = 0; i < n; ++i)
			{
				if (Y[i] && _edges[y][i] != W_MAX)
				{
					pq.push(Edge(y, i, _edges[y][i]));
				}
			}

		}

		if (num == n - 1)
		{
			cout << "��С��������ɣ�ȨֵΪ��" << totalW << endl;
		}
		else
		{
			cout << "��С����������ʧ��, num=" << num << endl;
		}

	}

	/*
	 * Dijkstra(�Ͻ�˹����)��Դ���·���㷨 -> �������ڷǸ�Ȩֵ��̰���㷨
	 * ̰��˼�룺
	 * ��distance[i]Ϊ src->i �����·����edge[u][v]Ϊ����� u->v �ĵ�Ȩֵ
	 * һ��ʼֻȷ����src��src�����·��Ϊ0����distance[src]=0
	 * ��ȷ�������·���Ľڵ���ѡ���ڵ�uʹ��distance[u]��С(̰��)
	 * �Ըýڵ�uΪ����תվ����u�ܵ�������нڵ�vΪ��Ŀ�ꡱ������distance[v]
	 * ���distance[v] > distance[u] + edge[u][v]��˵��������תվ����v �� ��srcֱ�ӵ���v�� �������
	 * ��ʱ����distanc[v] = distance[u] + edge[u][v]���ɳڲ�����
	 * ע�⣺
	 * ��ǰ��ѡ����u��֮��Ͳ�����ѡ�ˣ�ԭ�����ڣ�
	 * ��ǰ��ȷ�����·���ĵ��У�src->u��·������̵ģ����distance[u]��Ҳ���ᱻ���³ɸ�С��ֵ��
	 * ��Ҳ��Ϊʲô�ֲ����ſ�������ȫ������
	*/
	void Dijkstra(const V& src, vector<int>& distance, vector<int>& pPath)
	{
		int srci = GetIdx(src);
		size_t n = _edges.size();

		distance.resize(n, W_MAX);
		distance[srci] = 0;

		// pPath��¼����ڵ�v��ǰ�����
		pPath.resize(n, -1);
		pPath[srci] = srci; 
		
		std::vector<bool> visited(n, false);

		for (int i = 0; i < n; ++i)
		{
			// �ҳ���Сֵdistance[u]
			W minL = W_MAX;
			int u = 0;
			for (int j = 0; j < n; ++j)
			{
				if (!visited[j] && distance[j] < minL)
				{
					minL = distance[j];
					u = j;
				}
			}
			visited[u] = true; // ��ǣ��´β���Ҫ�ٸ���

			// ��uΪ��תվ�����������·��
			for (int v = 0; v < n; ++v)
			{
				if (_edges[u][v] != W_MAX && distance[u] + _edges[u][v] < distance[v])
				{
					distance[v] = distance[u] + _edges[u][v];
					pPath[v] = u;
				}
			}

		}
	}

	/*
	 * Dijkstra(�Ͻ�˹����)��Դ���·���㷨 �Ż���
	 * ����С���ѱ���	��ǰ��ȷ�������·���ڵ㣬������O(logn)ֱ���ҵ���ǰdistance�е���Сֵ
	 * ͬʱ���Ѿ�ѡ����u����С���Ѻ�㲻���ٱ�ѡ���ˣ��������visited���鱣����ѡ��Ľڵ�
	*/
	void DijkstraPlus(const V& src, vector<int>& distance, vector<int>& pPath)
	{
		int n = _edges.size();
		int srci = GetIdx(src);

		distance.resize(n, W_MAX);
		distance[srci] = 0;

		pPath.resize(n, -1);
		pPath[srci] = srci;

		typedef std::pair<W, int> pwi;
		std::priority_queue<pwi, vector<pwi>, std::greater<pwi>> pq;
		pq.push(std::make_pair(0, srci));

		while (!pq.empty())
		{
			pwi p = pq.top();
			pq.pop();

			int u = p.second;

			for (int v = 0; v < n; ++v)
			{
				if (_edges[u][v] != W_MAX && distance[u] + _edges[u][v] < distance[v])
				{
					distance[v] = distance[u] + _edges[u][v];
					pPath[v] = u;
					pq.push(std::make_pair(distance[v], v));
				}
			}
		}

	}

	/*
	 * Bellman-Ford(����������)��Դ���·���㷨 ->����������Ȩֵ�ı����ⷨ
	 * ���䶨�壺
	 * �������ֽи�Ȩ��·����Ȩ����ָ����һ��ͼ�д���һ��������������ıߵı�Ȩ�ܺ�<0��
	 * �ڴ��ڸ�����ͼ�У����󲻳����·���ģ���ΪֻҪ��������ϲ�ͣ�Ķ�Ȧ�ӣ����·���ͻ�����С
	 * 
	 * ˼·��
	 * distance[i]���� src->i �����·��
	 * ��֪���·����distance[src]=0
	 * ��һ�ֵ�������distance[src]=0���ɳڸ��²�����һ������ɵ����·��
	 * �ڶ��ֵ������ӵ�ǰ��ȷ����һ������ɵ����·���У��ɳڸ��²�������������ɵ����·��
	 * ...
	 * ע�����ָ��µı߲����ڱ����ڲ��������ߵĸ��£������Υ���˵�x�ֵ���������x���ߵĹ涨
	 * �����Ҫ����һ�ֵ�distanceΪ��׼�����ɳڸ���
	 * ������n����㣬������·�������n-1���߹���
	 * ��˵����Ĵ������Ϊn-1
	 * ���ǣ����ĳһ�ֵ���û�и���·������˵����һ�ֵ����Ľ���������Ž⣬��ʱ�㷨ֹͣ(�Ż�)
	*/
	bool BellmanFord(const V& src, vector<int>& distance, vector<int>& pPath)
	{
		int n = _edges.size();
		int srci = GetIdx(src);

		distance.resize(n, W_MAX);
		distance[srci] = 0;

		pPath.resize(n, -1);
		pPath[srci] = srci;

		bool update = false;
		// n-1�ֵ���
		for (int i = 0; i < n - 1; ++i)
		{
			// �����е�·�����������·��
			vector<int> copyDist = distance;
			for (int u = 0; u < n; ++u)
			{
				for (int v = 0; v < n; ++v)
				{
					if (_edges[u][v] != W_MAX && (long long)copyDist[u] + _edges[u][v] < distance[v])
					{
						update = true;
						distance[v] = copyDist[u] + _edges[u][v];
						pPath[v] = u;
					}
				}
			}
			if (!update)
			{
				// ��һ���������·���������������
				break;
			}
		}

		// �����n�ֻ��ܽ����ɳڲ�����˵��ͼ�ڴ��ڸ�Ȩ��·������false
		for (int u = 0; u < n; ++u)
		{
			for (int v = 0; v < n; ++v)
			{
				if (_edges[u][v] != W_MAX && distance[u] + _edges[u][v] < distance[v])
				{
					return false;
				}
			}
		}

		return true;
	}

	/*
	 * Floyd(��������)��Դ���·�� -> ����������Ȩֵ�Ķ����㷨
	 * distance[u][v]��ʾu��>v�����·��
	 * ö�١���תվ��k������distance[u][v] = min(distance[u][v], distance[u][k] + distance[k][v])
	 * 
	 * pPath[u][v]��ʾ u->v �����·���У�v��ǰ�����
	 * 
	*/
	void Floyd(vector<vector<int>>& distance, vector<vector<int>>& pPath)
	{
		int n = _edges.size();
		distance.resize(n);
		pPath.resize(n);

		for (int i = 0; i < n; ++i)
		{
			distance[i].resize(n, W_MAX);
			distance[i][i] = 0;
			pPath[i].resize(n, -1);
		}

		for (int u = 0; u < n; ++u)
		{
			for (int v = 0; v < n; ++v)
			{
				if (_edges[u][v] != W_MAX)
				{
					distance[u][v] = _edges[u][v];
					pPath[u][v] = u;
				}
			}
		}

		for (int k = 0; k < n; ++k)
		{
			for (int u = 0; u < n; ++u)
			{
				for (int v = 0; v < n; ++v)
				{
					if (distance[u][k] != W_MAX && distance[k][v] != W_MAX && 
						(long long)distance[u][k] + distance[k][v] < distance[u][v])
					{
						distance[u][v] = distance[u][k] + distance[k][v];
						pPath[u][v] = pPath[k][v];
					}
				}
			}
		}

		for (int u = 0; u < n; ++u)
		{
			for (int v = 0; v < n; ++v)
			{
				printf("%2d ", distance[u][v]);
			}
			cout << endl;
		}

		cout << endl;

		for (int u = 0; u < n; ++u)
		{
			for (int v = 0; v < n; ++v)
			{
				printf("%2d ", pPath[u][v]);
			}
			cout << endl;
		}
	}

	/*
	 * ����ǰ���ڵ�����pPath��ӡԴ��src��dst��·��
	*/
	void PrintPath(const char* str, vector<int>& pPath, int src, int dst)
	{
		if (dst == src)
		{
			cout << str[src];
			return;
		}
		PrintPath(str, pPath, src, pPath[dst]);
		cout << "->" << str[dst];
	}
};