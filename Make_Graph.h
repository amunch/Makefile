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
	void updateTimes(string); //update the times in the .times file
	void getprevtimes(string); //read in the previous times from the file
	void BFS(string); //find an item in the graph using BFS

	//running the actual commands
	void compile(string, bool);

	//checking if we need to actually run the commands based on time or file exist
	bool checkdeptime(string);	
	bool file_exist(string filename); //check if a file exists
	
        //sort the targets so we run everything in order
	void topological_sort(bool dump);
	
        //get the degrees of each targer
	void calculate_degrees(bool dump);
        void dump_graph(Graph&); //print out the graph
	
        //load the graphs from the makefile
	void load_graph(ifstream &);
		
        //check if we have a variable
	string check_var(string, string);

        void clear_partial() {partial.clear();}

    private:

        map<string,int> previous_times; // Map of the last times each file was changed.
        Graph g; //graph with target and its dependencies
        Graph to_sort; //graph of items we need to topologically sort
        Graph commands; //graph with target and the commands
        Graph partial;
        map<string,string> globals; //map of the global variables
        map<string, size_t> degrees; //map of how many degrees
        vector<string> sorted; //the sorted order we need to run things in
};

#endif
