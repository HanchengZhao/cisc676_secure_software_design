#include <iostream>
#include <cstring>
#include <stdexcept>
using namespace std;

void go(char *data) {
    char name[10];
    if(strlen(data) > 9){
        throw runtime_error("overflow!");
    }
    strcpy(name, data);
}

int main(int argc, char **argv) {
    // cout << "I need your input: "<<endl;
    // cin >> argv[1];
    try{
        go(argv[1]);
    }catch(exception &e){
        // throw();
        cout<<e.what()<<endl;
    }
}