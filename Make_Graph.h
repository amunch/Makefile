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
		//Variables
		Graph g; //graph with target and its dependencies
    		Graph to_sort; //graph of items we need to topologically sort
    		Graph commands; //graph with target and the commands
    		Graph partial; 
    		map<string,string> globals; //map of the global variables
    		map<string,int> previous_times; //map of the times of last change for each file
		map<string, size_t> degrees; //map of how many degrees
		vector<string> sorted; //the sorted order we need to run things in
		//functions
		void updateTimes(map<string,int>); //update the times in the .times file
		map<string,int> getprevtimes(); //read in the previous times from the file
		void BFS(Graph &, Graph &, string); //find an item in the graph using BFS
		//running the actual commands
		void compile(vector<string>,Graph &,Graph &,string,map<string,int> &,map<string,string> &);
		//checking if we need to actually run the commands based on time or file exist
		bool checkdeptime(Graph &, map<string,int> &,string);	
		bool file_exist(string filename); //check if a file exists
		//sort the targets so we run everything in order
		vector<string> topological_sort(map<string, size_t>, Graph &, bool);
		//get the degrees of each targer
		map<string, size_t> calculate_degrees(Graph &, bool);
		void dump_graph(Graph &); //print out the graph
		//load the graphs from the makefile
		void load_graph(Graph &, Graph &, Graph &, map<string,string> &, ifstream &);
		//check if we have a variable
		string check_var(string,map<string,string> &,string,Graph &);
};

#endif
