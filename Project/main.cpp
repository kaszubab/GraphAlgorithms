#include <iostream>
#include <vector>
#include <string>
#include <utility>
#include <queue>
#include <climits>
#include <unordered_set>
#include <algorithm>

using namespace std;

int nullVertex = 0;

struct graph_edge {
	int to = -1;
	int min_weight = -1;
	int max_weight = -1;
};


struct pair_hash {
	inline std::size_t operator()(const std::pair<int, int> & v) const {
		return v.first * 31 + v.second;
	}
};


struct myPair {
	graph_edge first_neighbour;
	graph_edge second_neighbour;

};

struct bipartiteGraph
{
	int left, right;
	vector<myPair> adj;
	int *pair, *dist;
};

bipartiteGraph prim_graph;

void constructBipartiteGraph(vector<vector <graph_edge>> & graph, int min_weight)
{
	myPair p;
	prim_graph.adj.push_back(p);

	for (int i = 0; i < prim_graph.left; i++)
	{
		myPair p;

		for (graph_edge y : graph[i])
		{
			

			if (p.first_neighbour.to != -1)
			{
				p.second_neighbour = y;
				p.second_neighbour.to++;
			}

			else 
			{
				p.first_neighbour = y;
				p.first_neighbour.to++;
			}
		}
		

		prim_graph.adj.push_back(p);

	}

}

bool hopcroftBFS(int curr_weight)
{
	queue<int> Q;

	for (int i = 1; i <= prim_graph.left; i++)
	{

		if (prim_graph.pair[i] == nullVertex)
		{
			prim_graph.dist[i] = 0;
			Q.push(i);
		}

		else prim_graph.dist[i] = INT_MAX;
	}

	prim_graph.dist[nullVertex] = INT_MAX;

	while (!Q.empty())
	{
		int i = Q.front();
		Q.pop();

		if (prim_graph.dist[i] < prim_graph.dist[nullVertex])
		{
			if (prim_graph.adj[i].first_neighbour.min_weight <= curr_weight && prim_graph.adj[i].first_neighbour.max_weight >= curr_weight)
			{
				int v = prim_graph.adj[i].first_neighbour.to + prim_graph.left;

				if (prim_graph.dist[prim_graph.pair[v]] == INT_MAX)
				{
					prim_graph.dist[prim_graph.pair[v]] = prim_graph.dist[i] + 1;
					Q.push(prim_graph.pair[v]);
				}
			}

			if (prim_graph.adj[i].second_neighbour.to != -1 && prim_graph.adj[i].second_neighbour.min_weight <= curr_weight && prim_graph.adj[i].second_neighbour.max_weight >= curr_weight) {


				int v = prim_graph.adj[i].second_neighbour.to + prim_graph.left;

				if (prim_graph.dist[prim_graph.pair[v]] == INT_MAX)
				{
					prim_graph.dist[prim_graph.pair[v]] = prim_graph.dist[i] + 1;
					Q.push(prim_graph.pair[v]);
				}

			}

		}
	}

	return prim_graph.dist[nullVertex] != INT_MAX;
}


bool hopcroftDFS(int vertex, int curr_weight)
{
	if (vertex != nullVertex)
	{
		if (prim_graph.adj[vertex].first_neighbour.min_weight <= curr_weight && prim_graph.adj[vertex].first_neighbour.max_weight >= curr_weight)
		{
			int v = prim_graph.adj[vertex].first_neighbour.to + prim_graph.left;

			if (prim_graph.dist[prim_graph.pair[v]] == prim_graph.dist[vertex] + 1)
			{
				if (hopcroftDFS(prim_graph.pair[v], curr_weight))
				{
					prim_graph.pair[v] = vertex;
					prim_graph.pair[vertex] = v;
					return true;
				}
			}
		}

		if (prim_graph.adj[vertex].second_neighbour.to != -1 && prim_graph.adj[vertex].second_neighbour.min_weight <= curr_weight && prim_graph.adj[vertex].second_neighbour.max_weight >= curr_weight)
		{

			int v = prim_graph.adj[vertex].second_neighbour.to + prim_graph.left;

			if (prim_graph.dist[prim_graph.pair[v]] == prim_graph.dist[vertex] + 1)
			{
				if (hopcroftDFS(prim_graph.pair[v], curr_weight))
				{
					prim_graph.pair[v] = vertex;
					prim_graph.pair[vertex] = v;
					return true;
				}
			}

		}

		prim_graph.dist[vertex] = INT_MAX;
		return false;
	}
	return true;
}



int hopcroftKarp(int curr_weight)
{
	for (int i = 0; i <= prim_graph.left + prim_graph.right; i++)
	{
		prim_graph.pair[i] = nullVertex;
	}

	int result = 0;

	for (int i = 1; i < prim_graph.adj.size(); i++) 
	{
		myPair p = prim_graph.adj[i];

		bool is_isolated = true;

		if (p.first_neighbour.to != -1 && p.first_neighbour.min_weight <= curr_weight && p.first_neighbour.max_weight >= curr_weight)
		{
			is_isolated = false;
		}

		if (is_isolated && p.second_neighbour.to != -1 && p.second_neighbour.min_weight <= curr_weight && p.second_neighbour.max_weight >= curr_weight)
		{
			is_isolated = false;
		}

		if (is_isolated) return -1;

	}


	while (hopcroftBFS(curr_weight))
	{
		for (int i = 1; i <= prim_graph.left; i++)
		{


			if (prim_graph.pair[i] == nullVertex && hopcroftDFS(i, curr_weight))
			{
				result++;
			}
		}
	}


	if (result != prim_graph.left)
		return -1;

	cout << curr_weight << endl;

	for (int i = 1; i <= prim_graph.left; i++) 
	{
		cout << i << " " << prim_graph.pair[i] - prim_graph.left << endl;
	}


	/*
	unordered_set< pair<int, int>, pair_hash > res_list;

	for (int i = 1; i <= graph.left; i++)
	{
		if (graph.pair[graph.leftMap[i - 1] + 1] != nullVertex)
		{
			pair<int, int> resPair;
			resPair.second = graph.leftMap[i - 1];
			resPair.first = graph.pair[graph.leftMap[i - 1] + 1] - 1;
			if (res_list.find(resPair) == res_list.end())
			{
				resPair.first = graph.leftMap[i - 1];
				resPair.second = graph.pair[graph.leftMap[i - 1] + 1] - 1;
				res_list.insert(resPair);
				cout << endl;
				cout << graph.leftMap[i - 1] << " " << graph.pair[graph.leftMap[i - 1] + 1] - 1;
			}

		}
	}
	delete[] graph.leftMap;

	*/
	return result;
}



int main()
{
	int data_sets;
	cin >> data_sets;
	
	while (data_sets > 0) {


		vector<vector <graph_edge>> graph1;

		int vertices;
		cin >> vertices;

		for (int i = 0; i < vertices; i++)
		{
			vector<graph_edge> v;
			graph1.push_back(v);
		}


		int edges;
		cin >> edges;

		vector<int> weights(edges);

		for (int i = 0; i < edges; i++)
		{
			int start, end, min_weight, max_weight;
			cin >> start;
			cin >> end;
			cin >> min_weight;
			cin >> max_weight;

			weights[i] = min_weight;

			graph_edge e;
			e.to = end - 1;
			e.min_weight = min_weight;
			e.max_weight = max_weight;

			graph1[start-1].push_back(e);
		}

		sort(weights.begin(), weights.end());

		int l = unique(weights.begin(), weights.end()) - weights.begin();


		prim_graph.left = graph1.size();
		prim_graph.right = graph1.size();

		prim_graph.pair = new int[prim_graph.left + prim_graph.right + 1];
		prim_graph.dist = new int[prim_graph.left + 1];

		
		constructBipartiteGraph(graph1, 0);

		bool good = false;

		for (int i = 0; i < l; i++) 
		{
			int result = hopcroftKarp(weights[i]);

			if (result != -1) 
			{
				good = true;
				break;
			}
			
		}

		if (!good)
		{
			cout << -1 << endl;
		}

		vector<myPair> a;
		prim_graph.adj = a;

		data_sets--;

	}

	return 0;
}

