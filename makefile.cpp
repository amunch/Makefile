#include <iostream>
#include <fstream>

using namespace std;

int main() {
    ifstream file_in;
    file_in.open("sample_make");

    if(!file_in.good()) {
        cout << "File does not exist!" << endl;
    }

    
}
