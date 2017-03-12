#include "deploy.h"
#include <stdio.h>
#include <string>
#include <iostream>
#include "array"
#include "queue"
//你要完成的功能总入口

struct Edge
{
	int s, t, flow, next;
};

int *level;
Edge *edge;
int *head;
int count; 
int network_node_number;
int network_edge_number;
int consume_node_number;

//函数
void initGraph(int **adj_table,int nodeCount);
void addEdge(int s, int t, int flow);
int dinic(int startnode, int endnode);
int dfs(int startnode, int maxflow, int endnode);


void deploy_server(char * topo[MAX_EDGE_NUM], int line_num,char * filename)
{
	char *delim = " ";
	// read firs line
	network_node_number = std::stoi(strtok(topo[0], delim));
	network_edge_number = std::stoi(strtok(NULL, delim));
	consume_node_number = std::stoi(strtok(NULL, delim));
	int server_cost = std::stoi(topo[2]);
	int **adjacent_table = new int*[network_node_number];
	int **value_table = new int*[network_node_number];
	int **consume_table = new int*[network_node_number];

	for (int i = 0; i != network_node_number; ++i) {
		adjacent_table[i] = new int[network_node_number];
		value_table[i] = new int[network_node_number];
		consume_table[i] = new int[2];
		memset(adjacent_table[i], 0, network_node_number * sizeof(int));
		memset(value_table[i], 0, network_node_number * sizeof(int));
	}
	// read network node
	for (int i = 0; i != network_edge_number; ++i) {
		int start_node = std::stoi(strtok(topo[i + 4], delim));
		int destination_node = std::stoi(strtok(NULL, delim));
		int capacity = std::stoi(strtok(NULL, delim));
		int  unit_price = std::stoi(strtok(NULL, delim));
		adjacent_table[start_node][destination_node] = capacity;
		value_table[start_node][destination_node] = unit_price;
	}

	std::cout << adjacent_table[0][26] << std::endl;
	std::cout << value_table[0][26] << std::endl;
	std::cout << adjacent_table[47][48] << std::endl;
	std::cout << value_table[47][48] << std::endl;
	std::cout << value_table[3][7] << std::endl;

	// read consume node
	for (int i = 0; i != consume_node_number; ++i) {
		int consume_node = std::stoi(strtok(topo[i + network_edge_number + 5], delim));
		int adjacent_node = std::stoi(strtok(NULL, delim));
		int need_number = std::stoi(strtok(NULL, delim));
		consume_table[consume_node][0] = adjacent_node;
		consume_table[consume_node][1] = need_number;
	}

	level = new int[network_node_number];
	edge = new Edge[2 * network_edge_number];
	head = new int[network_node_number];
	memset(head, -1, network_node_number * sizeof(int));
	count = 0;
	initGraph(adjacent_table, network_node_number);

	std::cout << dinic(0, 26) << std::endl;
	
	// 需要输出的内容
	char * topo_file = (char *)"17\n\n0 8 0 20\n21 8 0 20\n9 11 1 13\n21 22 2 20\n23 22 2 8\n1 3 3 11\n24 3 3 17\n27 3 3 26\n24 3 3 10\n18 17 4 11\n1 19 5 26\n1 16 6 15\n15 13 7 13\n4 5 8 18\n2 25 9 15\n0 7 10 10\n23 24 11 23";

	// 直接调用输出文件的方法输出到指定文件中(ps请注意格式的正确性，如果有解，第一行只有一个数据；第二行为空；第三行开始才是具体的数据，数据之间用一个空格分隔开)
	write_result(topo_file, filename);

}

void initGraph(int **adj_table,int nodeCount) 
{
	for (int i = 0; i<nodeCount; i++)
	{
		for (int j = 0; j<nodeCount; j++)
		{
			if (i != j && adj_table[i][j]!=0)
			{
				addEdge(i, j, adj_table[i][j]);
			}
		}
	}
}

void addEdge(int s,int t,int flow) 
{
	//正向边
	edge[count].s = s;
	edge[count].t = t;
	edge[count].flow = flow;
	edge[count].next = head[s];
	head[s] = count++;
	//反向边
	edge[count].s = t;
	edge[count].t = s;
	edge[count].flow = 0;
	edge[count].next = head[t];
	head[t] = count++;
}

bool bfs(int startnode,int endnode) 
{
	std::queue<int> mqueue;
	while (!mqueue.empty())
		mqueue.pop();
	memset(level, -1, network_node_number * sizeof(int));
	level[startnode] = 0;
	mqueue.push(startnode);
	while (!mqueue.empty())
	{
		int cur_s = mqueue.front(); mqueue.pop();
		if (cur_s == endnode) return true;
		for (int cur_edge = head[cur_s]; cur_edge != -1; cur_edge = edge[cur_edge].next) 
		{
			int cur_t = edge[cur_edge].t, cur_flow = edge[cur_edge].flow;
			if (level[cur_t]==-1 && cur_flow > 0)
			{
				level[cur_t] = level[cur_s] + 1;
				mqueue.push(cur_t);
			}
		}
	}
	return false;
}

int dfs(int startnode, int maxflow, int endnode) 
{
	if (startnode == endnode) return maxflow;
	int retflow = 0;
	for (int cur_edge = head[startnode]; cur_edge != -1; cur_edge = edge[cur_edge].next) 
	{
		int cur_t = edge[cur_edge].t, cur_flow = edge[cur_edge].flow;
		if (level[startnode] + 1 == level[cur_t] && cur_flow > 0) 
		{
			cur_flow = dfs(cur_t, std::min(maxflow - retflow, cur_flow), endnode);
			edge[cur_edge].flow -= cur_flow;
			edge[cur_edge + 1].flow += cur_flow;
			retflow += cur_flow;
			if (retflow == maxflow) return retflow;
		}
	}
	return retflow;
}

int dinic(int startnode, int endnode) 
{
	int ans = 0;
	while (bfs(startnode, endnode)) ans += dfs(startnode, INT_MAX, endnode);
	return ans;
}
