#include <string>
#include <stdexcept>
#include <iostream>   

using namespace std;

int main ()
{
    // First we'll incite and catch an exception from the C++ Standard
    // library class std::string by attempting to replace a substring
    // starting at a position beyond the end of the string object.
    try {
        string().replace(100, 1, 1, 'c');
    }
    catch (out_of_range &e) {
        // Print out the exception string, which in this implementation
        // includes the location and the name of the function that threw
        // the exception along with the reason for the exception.
        cout << "Caught an out_of_range exception: "
                  << e.what () << '\n';
    }
    catch (exception &e) {
        cout << "Caught an exception of an unexpected type: "
                  << e.what () << '\n';
    }
    catch (...) {
        cout << "Caught an unknown exception\n";
    }
    
    // Throw our own exception.
    try {
        throw runtime_error("a runtime error");
    }
    catch (runtime_error &e) {
        cout << "Caught a runtime_error exception: "
                  << e.what () << '\n';
    }
    catch (exception &e) {
        cout << "Caught an exception of an unexpected type: "
                  << e.what () << '\n';
    } 
    catch (...) {
        cout << "Caught an unknown exception\n";
    }
   
    return 0;
}