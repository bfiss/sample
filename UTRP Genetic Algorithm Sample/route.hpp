#ifndef _ROUTE_HPP
#define _ROUTE_HPP

#include <iostream>
#include <vector>

using namespace std;

#include "graph.hpp"

#define MIN_NUM_NODES 2
#define MAX_NUM_NODES 150
#define MAX_NUM_ROUTES 220

#define FIXED_ROUTE_NUMBER false

const bool routes_go_both_ways = true;
const bool there_can_be_cycles = false;
const bool use_shuffle = true;
const unsigned transfer_penalty = 5;

class Route {
private:
	vector<Node> n;
	vector<int> pos[MAX_NODES];
	int deleted_positions;
	
	Graph const * g;
public:
	Route(Graph const * _g) : n(), deleted_positions(0), g(_g) {}
	bool add(Node i, bool to_beginning = false, bool bypass = false) {
		if(((n.size() == MAX_NUM_NODES) && !bypass) || (!there_can_be_cycles && !pos[i].empty()) )
			return false;
		if(to_beginning) {
			if(n.size()>1 && ((*g)(i,n[0]) == UNDEF_NODE) )
				return false;
			--deleted_positions;
			pos[i].push_back(deleted_positions);
			n.insert(n.begin(),i);
		} else {
			if(n.size()>1 && ((*g)(n[n.size()-1],i) == UNDEF_NODE) ) 
				return false;
			pos[i].push_back(n.size()+deleted_positions);
			n.push_back(i);
		}
		return true;
	}
	bool rem(bool from_end = false,  bool bypass = false) {
		if((n.size() == MIN_NUM_NODES) && !bypass)
			return false;
		if(from_end) {
			Node i = n.back();
			n.pop_back();
			if(there_can_be_cycles) {
				for(unsigned j = 0 ; j < pos[i].size() ; ++j)
					if(pos[i][j] == ((signed)n.size()) + deleted_positions)
						pos[i].erase(pos[i].begin()+j);
			} else {
				assert(pos[i].back() == ((signed)n.size()) + deleted_positions);
				pos[i].pop_back();
			}
		} else {
			Node i = n[0];
			n.erase(n.begin());
			++deleted_positions;
			if(there_can_be_cycles) {
				for(unsigned j = 0 ; j < pos[i].size() ; ++j)
					if(pos[i][j] == deleted_positions - 1)
						pos[i].erase(pos[i].begin()+j);
			} else {
				assert(pos[i].back() == deleted_positions - 1);
				pos[i].pop_back();
			}
		}
		return true;
	}
	bool contains(Node node) const { return pos[node].size(); }
	unsigned size() const { return n.size(); }
	Node node_position(const Node node, unsigned int how_many_have_gone = 0) {
		if( pos[node].size() > how_many_have_gone )
			return pos[node][how_many_have_gone] - deleted_positions;
		return UNDEF_NODE;
	}
	Node operator()(const int node) const { return n[node]; }
	bool merge_this_into(Route&);
	friend ostream& operator<<(ostream& os, const Route & r);
	void write(ostream& os) const {
		if(size())
			os << size();
		for(unsigned int j = 0 ; j < size() ; ++j)
			os << " " << n[j];
	}
	Route read(istream& is) {
		Route ret(g);
		int num_of_nodes;
		is >> num_of_nodes;
		for(int i = 0 ; i < num_of_nodes ; ++i) {
			Node to_read;
			is >> to_read;
			ret.add(to_read);
		}
		return ret;
	}
	void graphviz_write(ostream& os) const {
		for(unsigned i = 1 ; i < n.size() ; ++i) {
			os << "n" << n[i-1] << " -- n" << n[i] << endl;
		}
	}
};

bool Route::merge_this_into(Route & a_route) {
	for(unsigned i = 0 ; i < a_route.pos[n[0]].size() ; ++i) {
		//cout << a_route << endl << endl;
		//cout << *this << endl << endl;
		for(int diff = 1 ; diff > 0 - 2*routes_go_both_ways ; diff -= 2) {
			bool continue_loop = false;
			unsigned j = 0;
			unsigned index = a_route.pos[n[0]][i] - a_route.deleted_positions + 1;
			assert(a_route.n[index-1] == n[0]);
			for( ; index <= a_route.size() && index > 0 && j < n.size() ; index += diff, ++j)
				if(a_route.n[index - 1] != n[j]) {
					continue_loop = true;
					break;
				}
			if(continue_loop)
				continue;
			if(n.size()-j + a_route.n.size() > MAX_NUM_NODES)
				continue;
			unsigned old_j = j;
			if(!there_can_be_cycles) {
				for( ; j < n.size() ; ++j)
					if(a_route.pos[n[j]].size()) {
						continue_loop = true;
						break;
					}
				if(continue_loop)
					break;
			}
			for(j = old_j ; j < n.size() ; ++j)
				a_route.add(n[j], diff == -1);
			//cout << a_route << endl << endl;
			return true;
		}
	}
	return false;
}

ostream& operator<<(ostream& os, const Route & r) {
	if(r.size())
		os << r(0);
	for(unsigned int j = 1 ; j < r.size() ; ++j)
		os << " " << (routes_go_both_ways?"<":"") << "==> " << r(j);
	return os;
}

#endif // _ROUTE_HPP
