#include <string>
#include <stdexcept>
#include <iostream>   

using namespace std;

class SubtractionError: public exception {
	public: 
	   unsigned int op1;
	   unsigned int op2;
	   SubtractionError(unsigned int operand1, unsigned int operand2){
	   	  op1 = operand1;
	   	  op2 = operand2;
	   }
	   const char* what(){
	   	return "Tried to subtract too much from unsigned int.";
	   }
};

unsigned int subtraction(unsigned int a, unsigned int b){
	if (b > a){
		throw SubtractionError(a, b);
	}
	return a-b;
};

int main ()
{
	unsigned int reduce_by = 0;
	unsigned int starts_at = 12345;
	cout << "Reduce " << starts_at << " by:" << endl;
	cin >> reduce_by;
	
	try {
	  starts_at = subtraction(starts_at, reduce_by);
	} catch (SubtractionError e){
	  	cout << e.what() << " Tried to subtract " << e.op2 << " from " << e.op1 << " without allowing negatives" << endl;
	  	return 0;
	}
  	cout << "Now: " << starts_at << endl;
	return 0;
} 