#include <iostream>
#include <gmp.h>
using namespace std;

int main(){
  mpz_t x,y,z;
  mpz_init(x);
  mpz_init(y); 
  mpz_init(z);
  mpz_set_str(x, "1234567890123456789012345678901234567890", 10);
  mpz_set_str(y, "9876543210987654321098765432109876543210", 10);
  mpz_add(z, x, y);
  cout << z << endl;
}