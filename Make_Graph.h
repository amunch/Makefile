//Make_Graph.h
//header file for the underlying graphs

#ifndef MAKE_GRAPH_H
#define MAKE_GRAPH_H

#include <iostream>
#include <fstream>
#include <map>
#include <set>
#include <string>
#include <sstream>
#include <vector>
#include <queue>
#include <stdio.h>
#include <cstring>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#include <boost/algorithm/string/trim.hpp>

using namespace std;

typedef map<string, vector<string> > Graph;

class Make_Graph {
	public:
		Graph g;
    		Graph to_sort;
    		Graph commands;
    		Graph partial;
    		map<string,string> globals;
    		map<string,int> previous_times;
		map<string, size_t> degrees;
		vector<string> sorted;
		//functions
		void updateTimes(map<string,int>);
		map<string,int> getprevtimes();
		void BFS(Graph &, Graph &, string);
		void compile(vector<string>,Graph &,Graph &,string,map<string,int> &,map<string,string> &);
		bool checkdeptime(Graph &, map<string,int> &,string);	
		bool file_exist(string filename);
		vector<string> topological_sort(map<string, size_t>, Graph &, bool);
		map<string, size_t> calculate_degrees(Graph &, bool);
		void dump_graph(Graph &);
		void load_graph(Graph &, Graph &, Graph &, map<string,string> &, ifstream &);
		string check_var(string,map<string,string> &,string,Graph &);
};

#endif
