#include <string>
#include <stdexcept>
#include <iostream>   

using namespace std;
void sub_error(int result){
    if(result < 0){
	    throw underflow("underflow!");
	}
}
int main ()
{
	unsigned int reduce_by = 0;
	unsigned int starts_at = 12345;
	cout << "Reduce " << starts_at << " by:" << endl;
	cin >> reduce_by;
	starts_at = starts_at - reduce_by;
	if(starts_at < 0){
	    throw
	}
	cout << "Now: " << starts_at << endl;
	return 0;
} 