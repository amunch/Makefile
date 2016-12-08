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
    string to_open = "sample_make";
    int c;
    size_t start = 1;

    while((c = getopt(argc, argv, "f:")) != -1) {
        switch(c) {
            case 'f':
                to_open = optarg;
                start += 2;
        }
    }

    ifstream file_in(to_open);

    if(!file_in.good()) {
        cout << "File does not exist!" << endl;
        return 0;
    }

    Make_Graph mg;

    //get previous times
    mg.getprevtimes(to_open);
    //load the graphs
 //   cout << "TARGET -> DEPENDENCIES" << endl;
    mg.load_graph(file_in);   
   
    
    if(argc > start) { //if we have command line arguments
        for(int i = start; i < argc; i++) { //go through each item we want to do
            mg.clear_partial(); //clear previous graph
            
            string target = argv[i]; //target for this time
    
            mg.BFS(target); //find it
            
            mg.calculate_degrees(false); //get degrees for each file
            mg.topological_sort(false); //sort it
            
  	    //run the commands
            mg.compile(target, true); 
        }
    } else { //run all
 //       cout << endl << "DEGREES" << endl;
        mg.calculate_degrees(false); //get degrees for each item
   //     cout << endl << "TOPOLOGICAL SORT" << endl;
        mg.topological_sort(false); //sort it
	
        //run the commands
        mg.compile("", false);
    }
    mg.updateTimes(to_open); //update the times 
}
