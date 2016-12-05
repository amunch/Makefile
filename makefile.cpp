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

            prev_line = target;
        }
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

void compile(vector<string> sorted, Graph &g, Graph &commands) {
    cout << endl;
    for (auto s : sorted) {
        if(g.find(s) != g.end()) {
            vector<string> to_run = commands[s];
            for(auto r : to_run) {
                cout << r << endl;
                system(r.c_str());   
            }
        }
    }
}

int main() {
    ifstream file_in("sample_make");

    if(!file_in.good()) {
        cout << "File does not exist!" << endl;
        return 0;
    }

    Graph g;
    Graph to_sort;
    Graph commands;

    load_graph(g, to_sort, commands, file_in);   
    dump_graph(g);

    map<string, size_t> degrees = calculate_degrees(g, true);
    vector<string> sorted = topological_sort(degrees, to_sort, true);

/*    cout << endl;

    for(auto n : commands) {
        cout << n.first << endl;
        for(string s : n.second) {
            cout << s << endl;
        }
        cout << endl;
    } */

    compile(sorted, g, commands);
}
