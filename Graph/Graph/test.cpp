#define _CRT_SECURE_NO_WARNINGS 1
#include "Graph.hpp"

void TestDijkstra()
{
	const char* str = "syztx";
	Graph<char, int, INT_MAX, true> g(str, strlen(str));
	g.AddEdge('s', 't', 10);
	g.AddEdge('s', 'y', 5);
	g.AddEdge('y', 't', 3);
	g.AddEdge('y', 'x', 9);
	g.AddEdge('y', 'z', 2);
	g.AddEdge('z', 's', 7);
	g.AddEdge('z', 'x', 6);
	g.AddEdge('t', 'y', 2);
	g.AddEdge('t', 'x', 1);
	g.AddEdge('x', 'z', 4);

	vector<int> distance;
	vector<int> pPath;

	// g.Dijkstra('s', distance, pPath);
	g.DijkstraPlus('s', distance, pPath);

	for (int i = 0; i < strlen(str); ++i)
	{
		g.PrintPath(str, pPath, 0, i);
		cout << ": " << distance[i] << endl;
	}
	
}

void TestBellmanFord()
{
	const char* str = "syztx";
	Graph<char, int, INT_MAX, true> g(str, strlen(str));
	g.AddEdge('s', 't', 6);
	g.AddEdge('s', 'y', 7);
	g.AddEdge('y', 'z', 9);
	g.AddEdge('y', 'x', -3);
	g.AddEdge('z', 's', 2);
	g.AddEdge('z', 'x', 7);
	g.AddEdge('t', 'x', 5);
	g.AddEdge('t', 'y', 8);
	g.AddEdge('t', 'z', -4);
	g.AddEdge('x', 't', -2);

	vector<int> dist;
	vector<int> pPath;
	bool ret = g.BellmanFord('s', dist, pPath);

	if (ret)
	{
		cout << "�����ڸ�Ȩ��·�����·����" << endl;
		for (int i = 0; i < strlen(str); ++i)
		{
			g.PrintPath(str, pPath, 0, i);
			cout << ": " << dist[i] << endl;
		}
	}
	else
	{
		cout << "���ڸ�Ȩ��·" << endl;
	}

}

void TestFloyd()
{
	const char* str = "12345";
	Graph<char, int, INT_MAX, true> g(str, strlen(str));
	g.AddEdge('1', '2', 3);
	g.AddEdge('1', '3', 8);
	g.AddEdge('1', '5', -4);
	g.AddEdge('2', '4', 1);
	g.AddEdge('2', '5', 7);
	g.AddEdge('3', '2', 4);
	g.AddEdge('4', '1', 2);
	g.AddEdge('4', '3', -5);
	g.AddEdge('5', '4', 6);
	vector<vector<int>> dist;
	vector<vector<int>> pPath;
	g.Floyd(dist, pPath);

	// ��ӡ��������֮������·��
	for (size_t i = 0; i < strlen(str); ++i)
	{
		for (size_t j = 0; j < strlen(str); ++j)
		{
			g.PrintPath(str, pPath[i], i, j);
			cout << ": " << dist[i][j] << endl;
		}
		cout << endl;
	}
}

int main()
{
	// ͼ����
	/*Graph<char, int, true> g("0123", 4);
	g.AddEdge('0', '1', 1);
	g.AddEdge('0', '3', 4);
	g.AddEdge('1', '3', 2);
	g.AddEdge('1', '2', 9);
	g.AddEdge('2', '3', 8);
	g.AddEdge('2', '1', 5);
	g.AddEdge('2', '0', 3);
	g.AddEdge('3', '2', 6);

	g.Print();*/

	// ��С������min spanning tree
	//Graph<char, int> g("abcdefghi", 9);
	//g.AddEdge('a', 'b', 4);
	//g.AddEdge('a', 'h', 8);
	//g.AddEdge('b', 'c', 8);
	//g.AddEdge('b', 'h', 11);
	//g.AddEdge('c', 'i', 2);
	//g.AddEdge('c', 'f', 4);
	//g.AddEdge('c', 'd', 7);
	//g.AddEdge('d', 'f', 14);
	//g.AddEdge('d', 'e', 9);
	//g.AddEdge('e', 'f', 10);
	//g.AddEdge('f', 'g', 2);
	//g.AddEdge('g', 'h', 1);
	//g.AddEdge('g', 'i', 6);
	//g.AddEdge('h', 'i', 7);
	//g.Print();

	//Graph<char, int> mst;
	//// g.MinSpanningTreeKruskal(mst);
	//g.MinSpanningTreePrim(mst, 'i');

	// mst.Print();

	// TestDijkstra();

	// TestBellmanFord();

	TestFloyd();

	return 0;
}