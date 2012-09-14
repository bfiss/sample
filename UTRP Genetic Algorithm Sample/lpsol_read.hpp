#ifndef _LPSOL_READ_HPP
#define _LPSOL_READ_HPP

#include <cstdio>
#include <cstdlib>
#include <vector>
#include <map>

#include "routeSet.hpp"

using namespace std;

RouteSet read_lp_solution(char * const filename, Graph & g, unsigned num_of_routes) {
	RouteSet ret(g);
	
	FILE * fout = fopen(filename,"r");
	
	//assert(there_can_be_cycles == true);
	
	vector<int> start(num_of_routes,0);
	vector<int> end(num_of_routes,0);
	
	vector< map<int,int> > edges(num_of_routes);
	
	/* Reading */
	
	char sol_type;
	assert(fscanf(fout,"%c\n",&sol_type)==1);
	
	for(unsigned j = 1 ; j <= g.size()*num_of_routes ; ++j) {
		int route;
		int node;
		int dum_index;
		int valid; 
		if( sol_type == 'c' )
			assert(fscanf(fout,"start_R(%d,%d)\" index=\"%d\" value=\"%d\"\n",&route,&node,&dum_index,&valid)==4);
		else
			assert(fscanf(fout,"start_R[%d,%d] * %d\n",&route,&node,&valid)==3);
		if(valid) {
			start[route-1] = node-1;
			cout << "Route " << route-1 << " starts on node " << node-1 << endl;
		}
	}

	for(unsigned j = 1 ; j <= g.size()*num_of_routes ; ++j) {
		int route;
		int node;
		int dum_index;
		int valid; 
		if( sol_type == 'c' )
			assert(fscanf(fout,"end_R(%d,%d)\" index=\"%d\" value=\"%d\"\n",&route,&node,&dum_index,&valid)==4);
		else
			assert(fscanf(fout,"end_R[%d,%d] * %d\n",&route,&node,&valid)==3);
		if(valid) {
			end[route-1] = node-1;
			cout << "Route " << route-1 << " ends on node " << node-1 << endl;
		}
	}

	for(unsigned j = 1 ; j <= g.size()*num_of_routes*g.size() ; ++j) {
		int route;
		int node;
		int node2;
		int dum_index;
		int valid; 
		if( sol_type == 'c' ) {
			if( fscanf(fout,"edge_R(%d,%d,%d)\" index=\"%d\" value=\"%d\"\n",&route,&node,&node2,&dum_index,&valid) != 5 )
				break;
		} else {
			if( fscanf(fout,"edge_R[%d,%d,%d] * %d\n",&route,&node,&node2,&valid) != 4 )
				break;
		}
		if(valid) {
			edges[route-1][node-1] = node2-1;
			cout << "In route " << route - 1 << " there is an edge from " << node-1 << " to " << node2-1 << endl;
		}
	}
	
	fclose(fout);
	
	/* Processing */
	
	for(unsigned i = 0 ; i < num_of_routes ; ++i) {
		Route new_route(&g);
		
		int size = 0;
		int node = end[i];
		
		new_route.add(start[i]);
		
		while(!edges[i].empty()) {
		
			while((signed)new_route(size) != node) {
				cout << "from " << new_route(size) << " to " << edges[i][new_route(size)] << endl;
				assert(new_route.add(edges[i][new_route(size)]));
				edges[i].erase(new_route(size));
				++size;
			}
			
			ret.add();
			ret(ret.size()-1) = new_route;
			
			if( !edges[i].empty() ) {
				new_route = Route(&g);
				size = 0;
				node = edges[i].begin()->first;
				assert(new_route.add(node));
				assert(new_route.add(edges[i][new_route(size)]));
				edges[i].erase(new_route(size));
				++size;
			}
		}
	}
	
	return ret;
}

#endif /* _LPSOL_READ_HPP */
