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
	std::vector<V> _vertexes; // 顶点
	std::map<V, int> _idx; // 顶点对应下标
	std::vector<std::vector<W>> _edges; // 邻接矩阵
public:
	Graph() = default;
	Graph(const V* v, size_t n)
	{
		// 初始化顶点及对应下标
		_vertexes.resize(n);
		for (size_t i = 0; i < n; ++i)
		{
			_vertexes[i] = v[i];
			_idx[v[i]] = i;
		}
		// 初始化邻接矩阵
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
		// 无向图
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
		// 顶点
		for (size_t i = 0; i < _vertexes.size(); ++i)
		{
			cout << "[" << i << "]" << "->" << _vertexes[i] << endl;
		}
		cout << endl;

		// 矩阵
		// 横下标
		cout << "  ";
		for (size_t i = 0; i < _vertexes.size(); ++i)
		{
			printf("%4d", i);
		}
		cout << endl;

		for (size_t i = 0; i < _edges.size(); ++i)
		{
			cout << i << " "; // 竖下标
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

	// 获取最小生成树 Min-Spanning-Tree(MST)

	struct Edge
	{
		int _src;
		int _dest;
		W _w; // 权重
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
	 * Kruskal(克鲁斯卡尔)最小生成树算法：
	 * 贪心思想
	 * 将所有边按照权值从小到大排列
	 * 每轮选择剩余边中权值最小的边
	 * 如果该边的加入导致当前图中存在环(利用并查集判断，如果边的两个顶点在一个集合，则该边会导致成环)
	 * 则舍弃该边
	 * 否则，将该边作为最小生成树的一条边
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
		
		std::priority_queue<Edge, vector<Edge>, std::greater<Edge>> pq; // 小根堆
		for (int i = 0; i < n; ++i)
		{
			for (int j = 0; j < n; ++j)
			{
				// 将所有边加入小根堆
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
				// 该边的加入会导致成环
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
			cout << "最小生成树已完成，总权值：" << totalW << endl;
		}
		else
		{
			cout << "最小生成树生成失败" << endl;
		}
	}

	/*
	 * Prim(普里姆)最小生成树算法：
	 * 贪心思想
	 * 指定一个起始点start，将其所在集合设为X，其余点视为集合Y
	 * 每轮从X中挑选一个点x，Y中挑选y，使得x->y的距离是X集合中的点到Y集合中的点的最小值
	 * 然后，将y从集合Y中删除，加入到集合X
	 * 由于每次都是从两个集合中各选一个点，因此必然不会导致成环(成环是由于一个集合内挑选两个点造成的)
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

		// 集合内对应点为true表示存在
		int starti = GetIdx(start);
		vector<bool> X(n, false);
		vector<bool> Y(n, true);
		X[starti] = true;
		Y[starti] = false;

		std::priority_queue<Edge, vector<Edge>, std::greater<Edge>> pq; // 小根堆
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
				// 两个点在一个集合，成环
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
			cout << "最小生成树完成，权值为：" << totalW << endl;
		}
		else
		{
			cout << "最小生成树生成失败, num=" << num << endl;
		}

	}

	/*
	 * Dijkstra(迪杰斯特拉)单源最短路径算法 -> 仅适用于非负权值的贪心算法
	 * 贪心思想：
	 * 设distance[i]为 src->i 的最短路径，edge[u][v]为有向边 u->v 的的权值
	 * 一开始只确定了src至src的最短路径为0，即distance[src]=0
	 * 从确定了最短路径的节点中选出节点u使得distance[u]最小(贪心)
	 * 以该节点u为“中转站”，u能到达的所有节点v为“目标”，更新distance[v]
	 * 如果distance[v] > distance[u] + edge[u][v]，说明经过中转站到达v 比 从src直接到达v的 距离更短
	 * 此时更新distanc[v] = distance[u] + edge[u][v]（松弛操作）
	 * 注意：
	 * 当前轮选定的u在之后就不用再选了，原因在于：
	 * 当前已确定最短路径的点中，src->u的路径是最短的，因此distance[u]再也不会被更新成更小的值了
	 * 这也是为什么局部最优可以推至全局最优
	*/
	void Dijkstra(const V& src, vector<int>& distance, vector<int>& pPath)
	{
		int srci = GetIdx(src);
		size_t n = _edges.size();

		distance.resize(n, W_MAX);
		distance[srci] = 0;

		// pPath记录到达节点v的前驱结点
		pPath.resize(n, -1);
		pPath[srci] = srci; 
		
		std::vector<bool> visited(n, false);

		for (int i = 0; i < n; ++i)
		{
			// 找出最小值distance[u]
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
			visited[u] = true; // 标记，下次不需要再更新

			// 以u为中转站更新其它最短路径
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
	 * Dijkstra(迪杰斯特拉)单源最短路径算法 优化版
	 * 利用小根堆保存	当前已确定的最短路径节点，这样以O(logn)直接找到当前distance中的最小值
	 * 同时，已经选定的u弹出小根堆后便不会再被选中了，因此无需visited数组保存已选择的节点
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
	 * Bellman-Ford(贝尔曼福特)单源最短路径算法 ->适用于正负权值的暴力解法
	 * 补充定义：
	 * 负环，又叫负权回路，负权环，指的是一个图中存在一个环，里面包含的边的边权总和<0。
	 * 在存在负环的图中，是求不出最短路径的，因为只要在这个环上不停的兜圈子，最短路径就会无限小
	 * 
	 * 思路：
	 * distance[i]计算 src->i 的最短路径
	 * 已知最短路径：distance[src]=0
	 * 第一轮迭代：从distance[src]=0，松弛更新不超过一条边组成的最短路径
	 * 第二轮迭代：从当前已确定的一条边组成的最短路径中，松弛更新不超过两条边组成的最短路径
	 * ...
	 * 注：本轮更新的边不能在本轮内参与其它边的更新，否则就违反了第x轮迭代不超过x条边的规定
	 * 因此需要以上一轮的distance为基准进行松弛更新
	 * 由于有n个结点，因此最短路径最多由n-1条边构成
	 * 因此迭代的次数最多为n-1
	 * 但是，如果某一轮迭代没有更新路径，则说明上一轮迭代的结果就是最优解，此时算法停止(优化)
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
		// n-1轮迭代
		for (int i = 0; i < n - 1; ++i)
		{
			// 从现有的路径更新其余的路径
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
				// 上一轮已是最短路径，无需继续更新
				break;
			}
		}

		// 如果第n轮还能进行松弛操作，说明图内存在负权环路，返回false
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
	 * Floyd(弗洛伊德)多源最短路径 -> 适用于正负权值的动规算法
	 * distance[u][v]表示u—>v的最短路径
	 * 枚举“中转站”k，更新distance[u][v] = min(distance[u][v], distance[u][k] + distance[k][v])
	 * 
	 * pPath[u][v]表示 u->v 的最短路径中，v的前驱结点
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
	 * 根据前驱节点数组pPath打印源点src至dst的路径
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