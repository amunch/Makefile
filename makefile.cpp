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

void load_graph(Graph &g, Graph &to_sort, Graph &commands, ifstream &file) {
    string line;
    string prev_line;    

    g.clear();
        
    while (getline(file, line)) {
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
        boost::trim(sources);
    
        stringstream ss(sources);
        string source;
        while (ss >> source) {
            g[target].push_back(source);
            to_sort[source].push_back(target);
        }
        prev_line = target;
    }
}

void dump_graph(Graph &g) {
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

map<string, size_t> calculate_degrees(Graph &g, bool dump) {
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

vector<string> topological_sort(map<string, size_t> degrees, Graph &g, bool dump) {
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

bool checkdeptime(Graph &g, map<string,int> &times,string s) {	
	bool noupdate = true;
	for(auto dep : g[s]) {
		struct stat st;
		int ierr = stat(dep.c_str(),&st);
		int newtime = st.st_mtime;
		if(times[dep]<newtime) {
			times[dep] = newtime;
			noupdate= false;
		}
	}
	return noupdate;
} 

void compile(vector<string> sorted, Graph &g, Graph &commands, string target,map<string,int> &times) {
    if(!sorted.empty()) {
        for (auto s : sorted) {
            if(g.find(s) != g.end()) {
		if(!checkdeptime(g,times,s)) {
                	vector<string> to_run = commands[s];
                	for(auto r : to_run) {
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

void BFS(Graph &g, Graph &partial, string target) {
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

map<string,int> getprevtimes() {
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

void updateTimes(map<string,int> times) {
	system("rm .time.txt");
	ofstream file;
	file.open(".time.txt");
	for(auto s : times) {
		file<<s.first<<" "<<s.second<<endl;
	}
}

int main(int argc, char* argv[]) {
    ifstream file_in("sample_make");

    if(!file_in.good()) {
        cout << "File does not exist!" << endl;
        return 0;
    }

    Graph g;
    Graph to_sort;
    Graph commands;
    Graph partial;

    map<string,int> previous_times= getprevtimes();
  
    load_graph(g, to_sort, commands, file_in);   

    if(argc > 1) {
        for(int i = 1; i < argc; i++) {
            partial.clear();
            string target = argv[i];
            BFS(g, partial, target);

            dump_graph(partial);

            map<string, size_t> degrees = calculate_degrees(partial, true);
            vector<string> sorted = topological_sort(degrees, to_sort, true);

            dump_graph(commands);           

            compile(sorted, partial, commands, target,previous_times);
        }
    } else {
        dump_graph(g);

        map<string, size_t> degrees = calculate_degrees(g, true);
        vector<string> sorted = topological_sort(degrees, to_sort, true);

        compile(sorted, g, commands, "placeholder",previous_times);
    }
    updateTimes(previous_times);
}
