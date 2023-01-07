#include <iostream>
#include <string>
#include <random>

using namespace std;

int main( int argc, char** argv ) {
   random_device rd;
   cout << rd() << endl;
   random_device rd2;
   cout << rd2() << endl;
   
  return 0;
}
