#ifndef _BESTDEMAND_HPP
#define _BESTDEMAND_HPP

#include <vector>
#include <iostream>
#include <cassert>

using namespace std;

#include "graph.hpp"
#include "floydWarshall.hpp"

bool covered[MAX_NODES][MAX_NODES];

Demand calculate_gained_demand(Graph & g, vector<Node> & already_visited, Node to_visit) {
	Demand ret = 0;

	for(unsigned i = 0 ; i < already_visited.size() ; ++i) {
		if(already_visited[i] == to_visit)
			return 0;
		ret += g.demand[to_visit][already_visited[i]]*(!covered[to_visit][already_visited[i]]);
	}

	return ret;
}

bool depth_first_search(Graph & g, Node to_visit, Node goal, bool dfs_visited[MAX_NODES], vector<Node> & visiting) {
	dfs_visited[to_visit] = true;

	Neighbor nb = g(to_visit);
	random_shuffle(nb.begin(),nb.end());
	for(Node i = 0 ; i < nb.size() ; ++i) {
		if(dfs_visited[nb[i].index])
			continue;
		if(nb[i].index == goal) {
			visiting.push_back(goal);
			return true;
		}
		if(depth_first_search(g,nb[i].index,goal,dfs_visited,visiting)) {
			visiting.push_back(nb[i].index);
			return true;
		}
	}

	return false;
}

void update_covered(vector<Node> & to_visit) {
	for(unsigned i = 0 ; i < to_visit.size() ; ++i)
		for(unsigned j = 0 ; j < to_visit.size() ; ++j)
			covered[to_visit[i]][to_visit[j]] = true;
}

bool greedy_find_high_demand(Graph & g, RouteSet & rs, unsigned route_limit) {
	vector<Node> to_visit;

	Node start, goal;

	unsigned max_demand = 0;
	for(unsigned i = 0 ; i < g.size() ; ++i)
		for(unsigned j = 0 ; j < g.size() ; ++j)
			if(max_demand < g.demand[i][j] && !covered[i][j]) {
				max_demand = g.demand[i][j];
				start = i;
				goal = j;
			}
	if(!max_demand)
		return true;

	bool dfs_visited[MAX_NODES];
	for(int i = 0 ; i < MAX_NODES ; ++i)
		dfs_visited[i] = 0;

	depth_first_search(g,start,goal,dfs_visited,to_visit);

	do {
		to_visit.push_back(start);
		update_covered(to_visit);

		Neighbor nb = g(start);

		Node next = 0;

		max_demand = 0;
		for(unsigned i = 0 ; i < nb.size() ; ++i) {
			if(max_demand < calculate_gained_demand(g,to_visit,nb[i].index)) {
				max_demand = calculate_gained_demand(g,to_visit,nb[i].index);
				next = nb[i].index;
			}
		}
		start = next;
	} while(max_demand);

	do {
		update_covered(to_visit);

		Neighbor nb = g(to_visit[0]);

		Node next;

		max_demand = 0;
		for(unsigned i = 0 ; i < nb.size() ; ++i) {
			if(max_demand < calculate_gained_demand(g,to_visit,nb[i].index)) {
				max_demand = calculate_gained_demand(g,to_visit,nb[i].index);
				next = nb[i].index;
			}
		}
		if(max_demand)
			to_visit.insert(to_visit.begin(),next);
	} while(max_demand);

	unsigned index = rs.size();
	rs.add(true);
	for(unsigned t = 0 ; t < to_visit.size() ; ++t) {
		rs(index).add(to_visit[t],false,true);
	}

	return rs.size() >= route_limit;
}

RouteSet best_demand_route_set(Graph & g, unsigned route_limit) {
	assert(routes_go_both_ways);

	RouteSet rs(g);

	for(unsigned i = 0 ; i < MAX_NODES ; ++i) {
		if(i<g.size())
			assert(!g.demand[i][i]);
		for(int j = 0 ; j < MAX_NODES ; ++j)
			covered[i][j] = false;
	}

	while(!greedy_find_high_demand(g,rs,route_limit));

	return rs;
}


#endif
