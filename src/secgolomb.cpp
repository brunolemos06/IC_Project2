#include <iostream>
#include <stdio.h>
#include "secgolomb.h"

using namespace std;

int main(int argc, char** argv ){
    if(argc != 3){
        cout << "Usage: ./secgolomb <i> <m>" << endl;
        return -1;
    }
    int i = atoi(argv[2]);
    int m = atoi(argv[1]);
    secgolomb secgolomb(i,m);
    if(secgolomb.isPowerOfTwo(m)){
        cout << "m is power of 2" << endl;
        cout << secgolomb.calculate_secgolomb(i,m) << endl;
    }else{
        cout << "m is not power of 2" << endl;
        cout << secgolomb.calculate_secgolomb2(i,m) << endl;
    }
    // cout << secgolomb.decimal_to_binary(5,6)<< endl;
    return 0;
}