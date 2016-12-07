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

    mg.previous_times= mg.getprevtimes();
  
    mg.load_graph(mg.g, mg.to_sort, mg.commands, mg.globals, file_in);   
   
    if(argc > 1) {
        for(int i = 1; i < argc; i++) {
            mg.partial.clear();
            string target = argv[i];
            mg.BFS(mg.g, mg.partial, target);

            mg.dump_graph(mg.partial);

            mg.degrees = mg.calculate_degrees(mg.partial, true);
            mg.sorted = mg.topological_sort(mg.degrees, mg.to_sort, true);

            mg.dump_graph(mg.commands);           

            mg.compile(mg.sorted, mg.partial, mg.commands, target,mg.previous_times,mg.globals);
        }
    } else {
        mg.dump_graph(mg.g);

        mg.degrees = mg.calculate_degrees(mg.g, true);
        mg.sorted = mg.topological_sort(mg.degrees, mg.to_sort, true);

        mg.compile(mg.sorted, mg.g, mg.commands, "placeholder",mg.previous_times,mg.globals);
    }
    mg.updateTimes(mg.previous_times);
}
