#include <iostream>
#include <stdio.h>
#include "golomb.h"

using namespace std;

int main(int argc, char** argv ){
    if(argc != 3){
        cout << "Usage: ./golomb_code_tests <n> <m>" << endl;
        return -1;
    }
    int n = atoi(argv[1]);
    int m = atoi(argv[2]);

    golomb goal(m);
    cout << "ENCODE:" <<endl;
    string test = goal.encode_number(n);
    cout << "DECODE: "<<goal.decode_string(test)<<endl;
    return 0;
}