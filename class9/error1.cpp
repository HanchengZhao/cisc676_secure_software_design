#include <iostream>
#include <cstdlib>
#include <ctime>

using namespace std;

int main(){
  srand(time(0));
  int even = 0;
  int odd = 0;
  int i=0;
  int x, xm2 = 0;
  for(; i < 3; i++){
    x = rand();
    xm2 = x % 2;
    if (xm2 = 0){
      even++;
    } else {
      odd++;
    }    
  }
  
  cout << "After " << i << " trials of rand() found " << even << " evens and " << odd << " odds" << endl;
  
  return 0;
}