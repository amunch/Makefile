#include <iostream>
#include <fstream>
#include <map>
#include <set>
#include <string>
#include <sstream>
#include <vector>
#include <queue>

#include <boost/algorithm/string/trim.hpp>

using namespace std;

struct Command {
    string name;
    vector<string> commands;

    bool operator<(const Command &c) const {
        return this->name < c.name;
    }
};

typedef map<Command, vector<Command> > Graph;

void load_graph(Graph &g, Graph &to_sort, ifstream &file) {
    string line;

    g.clear();
        
    while (getline(file, line)) {
        size_t split = line.find(':');
        if (split == string::npos) {
            if(line.empty()) {
                continue;
            }
            else {
                
        }
        string target  = line.substr(0, split);
        string sources = line.substr(split + 1, line.size() - split);
 
        boost::trim(target);
        boost::trim(sources);
    
        stringstream ss(sources);
        string source;
        while (ss >> source) {
            Command key;
            key.name = target;

            Command value;
            value.name = source;    

            g[key].push_back(value);
            to_sort[value].push_back(key);
        }
    }
}

void dump_graph(Graph &g) {
    cout << "target -> dependencies {" << endl;
    for (auto &pair : g) {
        auto target  = pair.first;
        auto sources = pair.second;
        for (auto &source : sources) {
            cout << "\t\"" << target.name << "\" -> \"" << source.name << "\";" << endl;
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
            if(!degrees.count(source.name)) {
                degrees[source.name] = 0;
            }
            degrees[target.name] += 1;
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
    
        Command search;
        search.name = node;

        for(auto &v : g[search]) {
            degrees[v.name] -= 1;
    
            if(degrees[v.name] == 0) {
                frontier.push(v.name);
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

int main() {
    ifstream file_in("sample_make");

    if(!file_in.good()) {
        cout << "File does not exist!" << endl;
        return 0;
    }

    Graph g;
    Graph to_sort;

    load_graph(g, to_sort, file_in);   
    dump_graph(g);

    map<string, size_t> degrees = calculate_degrees(g, true);
    vector<string> sorted = topological_sort(degrees, to_sort, true);
}
