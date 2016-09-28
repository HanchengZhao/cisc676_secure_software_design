#include <iostream>
#include <string>

using namespace std;

int main ( int argc, char *argv[] ){
    string name;
    string title;
  cout << argc << " arguments" << endl;
  cout << "Hello ";
  for(int i=0; i < argc; i++){
    if (string(argv[i]) == "-t"){
        title = argv[i+1];
        cout << title;
    }else if(string(argv[i]) == "-n"){
        name = argv[i+1];
        cout << name << endl;
    }
    // cout << "Arg " << i << ": " << argv[i] << endl;
  }
  return 0;
}