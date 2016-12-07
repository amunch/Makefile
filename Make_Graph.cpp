//Make_Graph.cpp

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

//checks if there is a variabe and replaces it
string Make_Graph::check_var(string cmd,map<string,string> &globals,string prev_target,Graph &g) {
	stringstream ss(cmd);
	string word;
	string line;
	bool first = true; //so we don't add a space at the start
	while(ss >> word) { //for each word
		bool nonglobal=false;
		if(!first) { //add space if not 1st
			line+=" ";	
		}
		first=false;
		if(word == "$@" || word == "$^" || word == "$<") { //macros
			if(word=="$@") { //the target on previous line
				line += prev_target;
			} else if (word== "$^") { //all items on dependency list
				bool first_one=true; //dont add space for first one
				for(auto s : g[prev_target]) {
					if(!first_one) { //not the first one
						line+=" ";
					}
					first_one=false;
					line+=s; //add the line
				}
			} else { //word = $< which is 1st item in dependency list
				line+=g[prev_target][0];
			}
			nonglobal=true;
		} else if(word[0] == '$' && word[1] == '(' && word[word.size()-1]==')') { //we have a global variable
			string vname = word.substr(2,word.size()-3);
			if(globals.find(vname)!=globals.end()) { //we have the value
				word = globals[vname]; //replace it
			}			
		}
		if(!nonglobal) { //not a global variable
			line+=word; //add it to the new line
		}
	} 	
	return line;
}

void Make_Graph::load_graph(Graph &g, Graph &to_sort, Graph &commands, map<string,string> &globals, ifstream &file) {
    string line;
    string prev_line;    

    g.clear();
   
    bool poss_globals = true;
    
    while (getline(file, line)) {
	if(line[0] == '#' || line == "\n") { //the line is a comment
		continue;
	}
	if(poss_globals) {
		size_t eq_split = line.find('=');
		if(eq_split==string::npos) {
			poss_globals = false;
		} else {
			string var_tar = line.substr(0,eq_split);
			string var_val = line.substr(eq_split+1, line.size() - eq_split);
			boost::trim(var_tar); boost::trim(var_val);
			globals[var_tar]=var_val;
			continue;					
		}
	}
        size_t split = line.find(':');
        if (split == string::npos) {
            if(!line.empty()) {
                boost::trim(line);
                commands[prev_line].push_back(line);
            }
            continue;
        }
        string target  = line.substr(0, split);
        string sources = line.substr(split + 1, line.size() - split);
 
        boost::trim(target);
        target = check_var(target,globals,"",g);
	boost::trim(sources);
	sources = check_var(sources,globals,prev_line,g);
    
        stringstream ss(sources);
        string source;
        while (ss >> source) {
            g[target].push_back(source);
            to_sort[source].push_back(target);
        }
        prev_line = target;
    }
}

void Make_Graph::dump_graph(Graph &g) {
    cout << "target -> dependencies {" << endl;
    for (auto &pair : g) {
        auto target  = pair.first;
        auto sources = pair.second;
        for (auto &source : sources) {
            cout << "\t\"" << target << "\" -> \"" << source << "\";" << endl;
        }
    }

    cout << "}" << endl;
}

map<string, size_t> Make_Graph::calculate_degrees(Graph &g, bool dump) {
    map<string, size_t> degrees;
    
    for(auto &pair : g) {
        auto target = pair.first;
        auto sources = pair.second;
        for(auto &source : sources) {
            if(!degrees.count(source)) {
                degrees[source] = 0;
            }
            degrees[target] += 1;
        }
    }
    if(dump) {
        for(auto i : degrees) {
            cout << i.first << '\t' << i.second << endl;
        }
    }
    return degrees;
}   

vector<string> Make_Graph::topological_sort(map<string, size_t> degrees, Graph &g, bool dump) {
    vector<string> sorted;
    
    queue<string> frontier;

    for(auto &d : degrees) {
        if(d.second == 0) {
            frontier.push(d.first);
        }
    }    
        
    while(!frontier.empty()) {
        auto node = frontier.front();
        frontier.pop();
    
        sorted.push_back(node);
    
        for(auto &v : g[node]) {
            degrees[v] -= 1;
    
            if(degrees[v] == 0) {
                frontier.push(v);
            }
        }
    }
    if(dump) {
        for(string n : sorted) {
            cout << n << endl;
        }
    }

    return sorted;
}

//returns true if the file exists
bool Make_Graph::file_exist(string filename) {
	ifstream f(filename.c_str());
	return f.good(); //true if it exists
}

//returns if we need to recomple based on last changed and if files are missing
bool Make_Graph::checkdeptime(Graph &g, map<string,int> &times,string s) {	
	bool noupdate = true;
	for(auto dep : g[s]) { //for each dependent
		struct stat st;
		int ierr = stat(dep.c_str(),&st);
		int newtime = st.st_mtime; //get last changed time
		if(times[dep]<newtime || !file_exist(s)) { //if we need to recompile
			times[dep] = newtime; //update time
			noupdate= false;
		}
	}
	return noupdate; //no need to recompie
} 


void Make_Graph::compile(vector<string> sorted,Graph &g,Graph &commands,string target,map<string,int> &times,map<string,string> &globals) {
    if(!sorted.empty()) {
        for (auto s : sorted) {
            if(g.find(s) != g.end()) {
		if(!checkdeptime(g,times,s)) {
                	vector<string> to_run = commands[s];
                	for(auto r : to_run) {
				r = check_var(r,globals,s,g);
                    		cout << r << endl;
                    		system(r.c_str());   
                	}
		} else {
			cout<<"Nothing changed for "<<s<<" so nothing done for it."<<endl;
		}
            }
        }
    } else {
        vector<string> to_run = commands[target];
        for(auto r : to_run) {
            cout << r << endl;
            system(r.c_str());
        }
    }
}

void Make_Graph::BFS(Graph &g, Graph &partial, string target) {
    set<string> marked;   
    queue<string> frontier;
    frontier.push(target);

    while(!frontier.empty()) {
        string v = frontier.front();
        frontier.pop();

        if(marked.find(v) != marked.end()) {
            continue;
        }
        partial[v] = g[v];
        marked.insert(v);
        for(auto i : g[v]) {
            frontier.push(i);
        }
    }
}

map<string,int> Make_Graph::getprevtimes() {
	map<string,int> prev;
	ifstream file;
	file.open(".time.txt");
	string name;
	while(file>>name) {
		int time;
		file >> time;
		prev[name]=time;
	}
	return prev;
}

void Make_Graph::updateTimes(map<string,int> times) {
	system("rm .time.txt");
	ofstream file;
	file.open(".time.txt");
	for(auto s : times) {
		file<<s.first<<" "<<s.second<<endl;
	}
}
