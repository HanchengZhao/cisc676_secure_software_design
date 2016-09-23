#include <iostream>
#include <cstdlib>

using namespace std;

int main(){
  int* x = (int*) malloc(32);
  x[0] = 23;
  cout << x[0] << endl;
  
  return 0;
}