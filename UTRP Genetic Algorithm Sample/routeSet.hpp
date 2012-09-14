#ifndef _ROUTESET_HPP
#define _ROUTESET_HPP

#include <iostream>
#include <vector>
#include <map>
#include <algorithm>

using namespace std;

#include "route.hpp"

#define S_WOTT 400
#define S_WOL 10
#define S_WOS 1

#define MAX_TRANSFERS 20

class RouteSet {
private:
	Graph const & g;
	vector<Route> r;
	
	Weight min_dist[MAX_NODES][MAX_NODES];
	bool min_dist_updated;
	unsigned transfers_[MAX_TRANSFERS];
	
	const int weight_of_travel_time;
	const int weight_of_length;
	const int weight_of_size;
public:
	RouteSet(Graph const & G, const int _wott = S_WOTT, const int _wol = S_WOL, const int _wos = S_WOS) : g(G), min_dist_updated(false), weight_of_travel_time(_wott), weight_of_length(_wol), weight_of_size(_wos) { }
	RouteSet(const RouteSet & rs, const int _wott = S_WOTT, const int _wol = S_WOL, const int _wos = S_WOS) : g(rs.g), r(rs.r), min_dist_updated(false), weight_of_travel_time(_wott), weight_of_length(_wol), weight_of_size(_wos) { }
	Route get(const unsigned int route) const { assert(route < r.size()); return r[route]; }
	Route & operator()(const unsigned int route) { assert(route < r.size()); min_dist_updated = false; return r[route]; }
	void add(bool bypass = false) { if(!bypass) assert(r.size() < MAX_NUM_ROUTES); min_dist_updated = false; r.push_back(Route(&g)); }
	void add(Route _r, bool bypass = false) { if(!bypass) assert(r.size() < MAX_NUM_ROUTES); min_dist_updated = false; r.push_back(_r); }
	void rem(const unsigned int route) { assert(route < r.size()); min_dist_updated = false; r.erase(r.begin()+route); }
	unsigned size() const { return r.size(); }
	Demand total_user_cost();
	double relative_user_cost();
	Weight total_operator_cost() const;
	void simplify(unsigned tries, bool calculate_first_one, bool dont_calculate_at_all);
	bool do_simplify();
	void hard_code() const;
	bool update_min_dist();
	unsigned transfers(unsigned i) { if(!min_dist_updated && !update_min_dist()) return 9650; return transfers_[i]; }
	bool valid() { return update_min_dist(); }
	RouteSet mix_up(RouteSet & rs) const;
	RouteSet extract(unsigned number_of_routes, unsigned min_size_for_route, unsigned max_size_for_route);
	RouteSet extract_routes(unsigned number) const;
	bool Dijkstra(Node s, pair<unsigned,unsigned> path_to_node[], map< pair<unsigned,unsigned>, pair<unsigned,unsigned> > &path_to_pair, Weight dist[]);
	Weight md(unsigned i, unsigned j) { return min_dist[i][j]; }
	RouteSet make_small_change() const;
	RouteSet& operator=(const RouteSet& rs) { r = rs.r; min_dist_updated = false; return *this; } 
	friend ostream& operator<<(ostream& os, const RouteSet & rs);
	void write(ostream&os) const {
		if(r.size())
			os << r.size() << endl;
		for(unsigned int j = 0 ; j < r.size() ; ++j) {
			r[j].write(os);
			os << endl;
		}
	}
	RouteSet read(istream& is) {
		RouteSet ret(g);
		Route dummy(&g);
		int num_of_routes;
		is >> num_of_routes;
		for(int i = 0 ; i < num_of_routes ; ++i)
			ret.add(dummy.read(is));
		return ret;
	}
	void graphviz_write(ostream & os,  bool print_graph) const;
};

Demand RouteSet::total_user_cost() {
	Demand ret = 0;
	//cout << r.size() << endl;
	assert(r.size() < MAX_NUM_ROUTES);
	if(!update_min_dist())
		return -1;
	unsigned n = g.size();
	/*for(unsigned i = 0 ; i < n ; ++i) {
		for(unsigned j = 0 ; j < n ; ++j)
			cout << min_dist[i][j] << " ";
		cout << endl;
	}*/
	for(unsigned i = 0 ; i < n ; ++i) {
		for(unsigned j = 0 ; j < n ; ++j)
			if(g.demand[i][j]) {
				ret += min_dist[i][j]*g.demand[i][j];
			}
		//cout << ret << endl;
		assert(ret < 1000000000);
	}
	return ret;
}

double RouteSet::relative_user_cost() {
	/*Demand total = 0;
	unsigned n = g.size();
	for(unsigned i = 0 ; i < n ; ++i)
		for(unsigned j = 0 ; j < n ; ++j)
			total += g.demand[i][j];*/
	Demand tuc = total_user_cost();
	if(tuc == (unsigned) -1)
		return -1;
	return ((double) tuc) / ((double)g.total_demand);
}

Weight RouteSet::total_operator_cost() const {
	Weight ret = 0;
	for(unsigned i = 0 ; i < r.size() ; ++i) {
		for(unsigned j = 0 ; j < r[i].size()-1 ; ++j)
			ret += g(r[i](j),r[i](j+1));
		assert(ret < 100000000);
	}
	return ret;
}

#define VERY_HIGH_USER_COST 10000000

void RouteSet::simplify(unsigned tries = 1, bool calculate_for_first_one = true, bool dont_calculate_at_all = false) {
	min_dist_updated = false;
	
	assert(tries>=1);
	
	if(!use_shuffle)
		tries = 1;
	
	vector<Route> best_r = r;
	unsigned best_size = size();
	double best_user_cost;
	Weight best_operator_cost;
	if(calculate_for_first_one) {
		best_user_cost = relative_user_cost();
		best_operator_cost = total_operator_cost();
	} else {
		best_user_cost = VERY_HIGH_USER_COST;
		best_operator_cost = best_size*g.total_demand;
	}

	double old_route_result = best_user_cost*weight_of_travel_time + best_operator_cost*weight_of_length + best_size*weight_of_size;
	
	while(tries-- > 0) {
		RouteSet n_rs(*this);
		if(!n_rs.do_simplify()) {
			//cout << "just gained " << tries << endl;
			break;
		}
		
		if((!calculate_for_first_one && n_rs.size() >= MAX_NUM_ROUTES) || dont_calculate_at_all) {
			best_r = n_rs.r;
			if(dont_calculate_at_all)
				return;
			break;
		}
		
		double new_route_result = n_rs.relative_user_cost()*weight_of_travel_time + n_rs.total_operator_cost()*weight_of_length + n_rs.size()*weight_of_size;
		
		if( new_route_result < old_route_result ) {
			best_r = n_rs.r;
			best_user_cost = n_rs.relative_user_cost();
			best_operator_cost = n_rs.total_operator_cost();
			best_size = n_rs.size();
			old_route_result = best_user_cost*weight_of_travel_time + best_operator_cost*weight_of_length + best_size*weight_of_size;
		}
	}
	
	r = best_r;
}

bool RouteSet::do_simplify() {
	bool made_a_change = false;
	min_dist_updated = false;

	if(use_shuffle)
		random_shuffle(r.begin(),r.end());
	
	for(int i = r.size() - 1 ; i >= 0 ; --i) {
		int r_size = r.size();
		for(int j = 0 ; j < r_size ; ++j) {
			if(i == j)
				continue;
			if(r[i].merge_this_into(r[j])) {
				rem(i);
				made_a_change = true;
				/*if(use_shuffle)
					random_shuffle(r.begin(),r.begin()+i);*/
				break;
			}
		}
	}
	return made_a_change;
}

void RouteSet::hard_code() const {
	int unique = 1;
	int n = g.size();
	for(unsigned i = 0 ; i < r.size() ; ++i) {
		bool used_up[MAX_NODES][MAX_NODES];
		for(int a = 0 ; a < n ; ++a)
			for(int b = 0 ; b < n ; ++b)
				used_up[a][b] = false;
		for(unsigned j = 0 ; j < r[i].size()-1 ; ++j) {
			used_up[r[i](j)][r[i](j+1)] = true;
		}
		/*if(routes_go_both_ways)
			for(int a = 0 ; a < n ; ++a)
				for(int b = 0 ; b < n ; ++b)
					used_up[a][b] = used_up[a][b] || used_up[b][a];*/
		for(int a = 0 ; a < n ; ++a)
			for(int b = 0 ; b < n ; ++b)
				if(g(a,b) != UNDEF_NODE)
					cout << "s.t. hard_code_" << unique++ << ": edge_R[" << i+1 << "," << a+1 << "," << b+1 << "] = " << used_up[a][b] << ";" << endl;
	}
}

RouteSet RouteSet::mix_up(RouteSet & rs) const {
	RouteSet ret(g);
	unsigned new_size = (size() + rs.size() + 1)/2;
	unsigned unwanted_size = size() + rs.size() - new_size;
	assert((int)size() + (int)rs.size() - (int)new_size >= 0);
	vector<bool> ones(new_size,true);
	ones.insert(ones.begin(),unwanted_size,false);
	
	random_shuffle(ones.begin(),ones.end());
	
	vector<Route> new_routes;
	for(unsigned i = 0 ; i < size() ; ++i)
		if(ones[i])
			new_routes.push_back(r[i]);
	for(unsigned i = 0, j = size() ; i < rs.size() ; ++i, ++j)
		if(ones[j])
			new_routes.push_back(rs.r[i]);
			
	ret.r = new_routes;
	ret.min_dist_updated = false;
	
	return ret;
}

RouteSet RouteSet::extract(unsigned number_of_routes, unsigned min_size_for_route, unsigned max_size_for_route) {
	RouteSet ret(RouteSet(*this).extract_routes(number_of_routes));
	
	//cout << "Beginning route prolongation/diminishing\n";
	
	for(unsigned i = 0 ; i < ret.size() ; ++i) {
		while(ret.r[i].size() < min_size_for_route) {
			/* add nodes */
			vector<unsigned> n_order;
			for(Node j = 0 ; j < g.size() ; ++j)
				n_order.push_back(j);
			random_shuffle(n_order.begin(), n_order.end());
			
			for(unsigned nod_ = 0 ; nod_ < g.size() ; ++nod_)
				if(ret.r[i].add(n_order[nod_]) || ret.r[i].add(n_order[nod_],true) ) { // clearly depends on chort-circuiting
					if(ret.r[i].size() >= min_size_for_route)
						break;
				}
		}
		while(ret.r[i].size() > max_size_for_route) {
			/* remove nodes */
			bool where = rand()%2;
			ret.r[i].rem(where);
		}
	}
	
	return ret;
}

RouteSet RouteSet::extract_routes(unsigned number) const {
	RouteSet ret(g);
	
	assert(size()>0);
	
	int unwanted_size = size() - number;
	if(unwanted_size < 0 && !FIXED_ROUTE_NUMBER) {
		return *this;
	}
		
	vector<bool> ones(number,true);
	ones.insert(ones.begin(),unwanted_size<0?0:unwanted_size,false);
	
	random_shuffle(ones.begin(),ones.end());
	
	vector<Route> new_routes;
	for(unsigned i = 0 ; i < size() || i < number ; ++i) {
		if(ones[i]) {
			new_routes.push_back(r[i%size()]);
		}
	}
			
	ret.r = new_routes;
	ret.min_dist_updated = false;
	
	assert(number == ret.size());
	
	return ret;
}

RouteSet RouteSet::make_small_change() const {
	RouteSet new_rs(*this);
	
	vector<unsigned> r_order;
	for(unsigned i = 0 ; i < new_rs.size() ; ++i)
		r_order.push_back(i);
	random_shuffle(r_order.begin(), r_order.end());
	
	for(unsigned rou_ = 0 ; rou_ < new_rs.size() ; ++rou_) {
		//cout << "Trying to make a small change to route " << r_order[rou_] << endl;
		unsigned route = r_order[rou_];
		
		// 50% chance of adding a node, 50% of deleting
	
		bool add_a_node = rand() % 2;
		
		bool worked = false;
	
		if(add_a_node) {
			vector<unsigned> n_order;
			for(Node i = 0 ; i < g.size() ; ++i)
				n_order.push_back(i);
			random_shuffle(n_order.begin(), n_order.end());
			
			for(unsigned nod_ = 0 ; nod_ < g.size() ; ++nod_)
				if(new_rs.r[route].add(n_order[nod_]) || new_rs.r[route].add(n_order[nod_],true) ) { // clearly depends on chort-circuiting
					worked = true;
					break;
				}
		} else
			if(new_rs.r[route].rem())
				worked = true;
		
		if(worked)
			break;
	}
	/*cout << "made a change from this:" << endl;
	cout << *this << endl;
	cout << "to that:" << endl;
	cout << new_rs << endl;*/
	
	return new_rs;
}

ostream& operator<<(ostream& os, const RouteSet & rs) {
	for(unsigned int route = 0 ; route < rs.size() ; ++route)
		os << rs.get(route) << endl;
	return os;
}

#include <iomanip>

void RouteSet::graphviz_write(ostream& os, bool print_graph = false) const {
	os << "graph {\n";

	os << "graph [overlap = scale]\n";

	os << "node [shape = circle]\n";

	/* add invisibly every edge that exists */
	if(!print_graph)
		os << "edge [color = transparent]\n";
	//os << "graph [splines = true]\n";
	//os << "graph [sep = 12.1]\n";

	for (unsigned i = 0; i < g.size(); ++i) {
		Neighbor nb = g.get_neighbor(i);
		for (unsigned j = 0; j < nb.size(); ++j) {
			os << "n" << i << " -- n" << nb[j].index << " [len = "
					<< nb[j].weight << ".0]" << endl;
		}
	}

	if(print_graph) {
		os << "}" << endl;
		return;
	}

	int per_color = r.size() / 7;
	int mod_color = r.size() % 7;

	int red[] =   { 1, 0, 0, 1, 0, 1, 1 };
	int green[] = { 0, 0, 1, 1, 1, 0, 1 };
	int blue[] =  { 0, 1, 0, 0, 1, 1, 1 };

	int i = 0;

	for (int mode = 0; mode < 7; ++mode) {
		for (int j = 0; j < per_color + (mode < mod_color); ++i, ++j) {
			int delta =
					per_color + (mode < mod_color) > 1 ?
							180 / (per_color + (mode < mod_color) - 1) : 0;
			int base = per_color + (mode < mod_color) > 1 ? 40 : 127;

			os << hex << uppercase << "edge [color = \"#" << setw(2)
					<< setfill('0') << red[mode] * (base + delta * j) << " "
					<< setw(2) << setfill('0')
					<< green[mode] * (base + delta * j) << " " << setw(2)
					<< setfill('0') << blue[mode] * (base + delta * j)
					<< "\"]\n";
			os << dec << nouppercase;
			r[i].graphviz_write(os);
		}
	}

	os << "}" << endl;
}

#endif // _ROUTESET_HPP
