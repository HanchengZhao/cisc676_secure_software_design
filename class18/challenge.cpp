#include <iostream>
#include <cstdlib>
#include <cstring>

using namespace std;

int main(int argc, char* argv[]){
   long* ptr = new long;
   long value = 15;   
   char user[20];
   char dev[] = "Andy";
   strncpy(user, argv[1], strlen(argv[1]));
   *ptr = value;
   cout << "My name is " << dev << endl;
   cout << "Your name is " << user << endl;
   cout << "The pointer has value " << *ptr << endl;
   return 0;
}