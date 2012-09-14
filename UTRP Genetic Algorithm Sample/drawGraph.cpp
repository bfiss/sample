/*
 * drawGraph.cpp
 *
 *  Created on: Jun 15, 2012
 *      Author: bruno
 */

#include <cstdio>
#include <fstream>
#include <ctime>
#include <cassert>
#include <string>
#include "geneticAlgorithm.hpp"

int main(int argc, char * argv[]) {
	if(argc != 3) {
		printf("Usage: %s file.routes prefix_for_graphviz\n",argv[0]);
		exit(1);
	}
	ifstream route_file;
	route_file.open(argv[1]);

	Graph g;
	unsigned n;
	cin >> n;
	g.init(n);
	//g.read_graph_glpsol_sim();
	g.read_graph_normal();

	TransitGeneticAlgorithm this_test(g, 100, 30, 2, MAX_NUM_NODES, true);

	this_test.clear_u();
	//this_test.read_u(route_file);
	this_test.read_u_unchecked(route_file);
	route_file.close();

	//this_test.sort_u();

	cout << this_test << endl;

	string current_graph_name;

	for(unsigned i = 0 ; i <= this_test.size_u() ; ++i) {
		current_graph_name = argv[2];
		char number[5] = "1";
		sprintf(number,"%d",i);
		current_graph_name.append(number);
		current_graph_name.append(".viz");

		ofstream gv_file;
		gv_file.open(current_graph_name.c_str());
		this_test.graphviz_write(gv_file,i);
		gv_file.close();
	}

	return 0;
}


