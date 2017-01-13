//Fergal O'Shea

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <assert.h>

#define MAX_NUM_EDGES 10
#define MAX_NUM_NODES 26

struct Node {
	char data;
	//flag for search and dijkstra
	bool visited;
	Node* edge[MAX_NUM_EDGES];
	//Parrllel array to store cost 
	int edge_cost[MAX_NUM_EDGES];
	int num_edges;

	int distance;
	Node* parent;
};

struct Graph {
	Node* nodes[MAX_NUM_NODES];
	int num_nodes;
};


void make_graph(Graph* graph);
Node* add_node_to_graph(char data);
void add_edge(Node *left, Node *right, int cost);
void add_edges_as_far_away_from_main_as_possible_so_as_not_to_make_it_messy(Graph *graph);
bool dfs(Graph* graph, char start, char target);
bool dijkstra(Graph* graph, char start, char target);

void reset_graph(Graph* graph);



int main() {
	Graph *graph = new Graph;
	make_graph(graph);

	//creates edges of graph
	add_edges_as_far_away_from_main_as_possible_so_as_not_to_make_it_messy(graph);
	

	std::cout << "searching for 'X' starting at A:\n";
	if (!dfs(graph, 'A', 'X')) {
		std::cout << "\nPath not found\n";
	}
	reset_graph(graph);

	std::cout << "\nsearching for 'B' starting at K:\n";
	if (!dfs(graph, 'K', 'B')) {
		std::cout << "\nPath not found\n";
	}
	reset_graph(graph);

	std::cout << "\nLooking for shortest path to 'X' starting at 'A':\n";
	if (!dijkstra(graph, 'A', 'X')) {
		std::cout << "\nPath not found\n";
	}
	reset_graph(graph);

	std::cout << "\nLooking for shortest path to 'J' starting at 'A':\n";
	if (!dijkstra(graph, 'A', 'J')) {
		std::cout << "\nPath not found\n";
	}
	reset_graph(graph);


	return 0;
}



void make_graph(Graph * graph) {
	assert(graph);
	graph->num_nodes = 0;
	//adds edges 'A' to 'L'
	for (int i = 'A'; i <= 'L'; i++) {
					// i - 'A', sets 'A' to zero, 'B' to 1 etc.
		graph->nodes[i - 'A'] = add_node_to_graph((char)i);
		graph->num_nodes++;
	}
}

Node* add_node_to_graph(char data) {
	Node* new_node = new Node;
	new_node->data = data;
	new_node->visited = false;
	new_node->num_edges = 0;
	new_node->distance = 9999;
	new_node->parent = NULL;
	return new_node;
}



void add_edge(Node *left, Node *right, int cost) {
	assert(left);
	assert(right);
	//find first empty edge in left
	int i = 0;
	while (i<left->num_edges) {
		i++;
	}

	//store pointer to right in the first free spot in lefts edge array
	left->edge[i] = right;
	//store cost in the same index of a cost array
	left->edge_cost[i] = cost;
	left->num_edges++;

	//repeat for right
	int j = 0;
	while (j<right->num_edges){
		j++;
	}
	right->edge[j] = left;
	right->edge_cost[j] = cost;
	right->num_edges++;

}


bool dfs(Graph* graph, char start, char target) {
	assert(graph);
	bool search = false;
	//set current node as visited
	graph->nodes[start - 'A']->visited = true;

	std::cout << graph->nodes[start - 'A']->data << " ,";

	if (graph == NULL) {
		std::cout << "\nCannot search NULL graph\n";
		return false;
	}
	else {
		//check current node for target
		if (graph->nodes[start - 'A']->data == target) {

			std::cout << "\nTarget found - HALT\n";
			return true;
		}
		else {
			//loop through edges of current node
			for (int i = 0; i < graph->nodes[start-'A']->num_edges; i++) {
				//if the node is not visited
				if (graph->nodes[start-'A']->edge[i]->visited == false) {
					//search down edge - once edge is found, serach will = true in it's function, and this will get passed all the way up to the initial function.
					search = dfs(graph, graph->nodes[start-'A']->edge[i]->data, target);

					if (search == true) {

						return true;
					}//end if
				}//end if
			}//end for
		}//end else
	}//end else


	return false;
}

bool dijkstra(Graph* graph, char start, char target) {
	assert(graph);
	//make sure the graph exists
	if (graph == NULL) {
		std::cout << "\nCannot search NULL graph\n";
		return false;
	}

	//check if data is in correct range
	if (start < 'A' || start > 'L') {
		return false;
	}

	Node* curr = graph->nodes[start - 'A'];
	//add curent node to permanent list (actually just set a flag, because no need for a whole list)
	curr->visited = true;
	//this sets the first node we visit to 0 distance, as I know all nodes will be 9999 initially.
	if (curr->distance == 9999) {
		curr->distance = 0;
		std::cout << "Starting search at Node: " << curr->data << std::endl;
	}
	else {
		std::cout << "Node: " << curr->data << " - Parent: " << curr->parent->data << " - Distance: " << curr->distance <<std::endl;
	}
	//if found
	if (curr->data == target) {
		std::cout << "\nTarget found - HALT\n";
		return true;
	}
	else {
		//update edge costs
		for (int i = 0; i < curr->num_edges; i++) {
			//if new distance is less than old, update
			if (curr->distance + curr->edge_cost[i] < curr->edge[i]->distance) {
				curr->edge[i]->distance = curr->distance + curr->edge_cost[i];
				curr->edge[i]->parent = curr;
			}
		}
		//find shortest distance
		//This is my way of implementing the unvisited list. I figured for a small size, like this, the creation of 2 arrays, 
		//and then sorting them every time we do anything, would be overly complicated. Also, to sort an array, woul normally 
		//require more than O(n) each time, wheras this is max O(n)
		int shortest=10000;
		int short_pos=-1;
		for (int i = 0; i < graph->num_nodes; i++) {
			if (graph->nodes[i]->distance < shortest && graph->nodes[i]->visited != true) {
				shortest = graph->nodes[i]->distance;
				short_pos = i;
			}
		}
		if (short_pos == -1) {
			//this checks the event that there are no unvisited nodes
			return false;
		}
		//this is where we recurse into function.
		if (dijkstra(graph, graph->nodes[short_pos]->data, target)){
			return true;
		}

	}
	return false;
}

//reset graphs variables back to original
void reset_graph(Graph* graph) {
	assert(graph);
	for (int i = 0; i < graph->num_nodes; i++) {
		graph->nodes[i]->visited = false;
		graph->nodes[i]->distance = 9999;
		graph->nodes[i]->parent = NULL;
	}
}

//add edges
void add_edges_as_far_away_from_main_as_possible_so_as_not_to_make_it_messy(Graph* graph) {
	assert(graph);
	add_edge(graph->nodes['A' - 'A'], graph->nodes['B' - 'A'], 1);  //a---b  1
	add_edge(graph->nodes['B' - 'A'], graph->nodes['C' - 'A'], 2);  //b---c  2
	add_edge(graph->nodes['B' - 'A'], graph->nodes['D' - 'A'], 2);  //b---d  2
	add_edge(graph->nodes['C' - 'A'], graph->nodes['E' - 'A'], 3);  //c---e  3
	add_edge(graph->nodes['C' - 'A'], graph->nodes['F' - 'A'], 2);  //c---f  2
	add_edge(graph->nodes['F' - 'A'], graph->nodes['H' - 'A'], 1);  //f---h  1
	add_edge(graph->nodes['F' - 'A'], graph->nodes['I' - 'A'], 2);  //f---i  2
	add_edge(graph->nodes['F' - 'A'], graph->nodes['J' - 'A'], 4);  //f---j  4
	add_edge(graph->nodes['I' - 'A'], graph->nodes['K' - 'A'], 11);//i---k  11
	add_edge(graph->nodes['J' - 'A'], graph->nodes['G' - 'A'], 1);  //j---g  1
	add_edge(graph->nodes['J' - 'A'], graph->nodes['C' - 'A'], 5); //j---c  5
	add_edge(graph->nodes['G' - 'A'], graph->nodes['D' - 'A'], 2);  //g---d  2
	add_edge(graph->nodes['G' - 'A'], graph->nodes['K' - 'A'], 2); //g---k  2
	add_edge(graph->nodes['G' - 'A'], graph->nodes['L' - 'A'], 3); //g---c  3

}
