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
#include "Make_Graph.h"

#include <boost/algorithm/string/trim.hpp>

using namespace std;

typedef map<string, vector<string> > Graph;


int main(int argc, char* argv[]) {
    ifstream file_in("sample_make");

    if(!file_in.good()) {
        cout << "File does not exist!" << endl;
        return 0;
    }

    Make_Graph mg;
    /*Graph g;
    Graph to_sort;
    Graph commands;
    Graph partial;
    map<string,string> globals;*/
    //get previous times
    mg.previous_times= mg.getprevtimes();
    //load the graphs
    mg.load_graph(mg.g, mg.to_sort, mg.commands, mg.globals, file_in);   
   
    if(argc > 1) { //if we have command line arguments
        for(int i = 1; i < argc; i++) { //go through each item we want to do
            mg.partial.clear(); //clear previous graph
            string target = argv[i]; //target for this time
            mg.BFS(mg.g, mg.partial, target); //find it

            //mg.dump_graph(mg.partial);

            mg.degrees = mg.calculate_degrees(mg.partial, false); //get degrees for each file
            mg.sorted = mg.topological_sort(mg.degrees, mg.to_sort, false); //sort it

            //mg.dump_graph(mg.commands);           
  	    //run the commands
            mg.compile(mg.sorted, mg.partial, mg.commands, target,mg.previous_times,mg.globals);
        }
    } else { //run all
        //mg.dump_graph(mg.g);

        mg.degrees = mg.calculate_degrees(mg.g, false); //get degrees for each item
        mg.sorted = mg.topological_sort(mg.degrees, mg.to_sort, false); //sort it
	//run the commands
        mg.compile(mg.sorted, mg.g, mg.commands, "placeholder",mg.previous_times,mg.globals);
    }
    mg.updateTimes(mg.previous_times); //update the times
}
