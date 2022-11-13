#include <iostream>
#include <stdio.h>
#include "secgolomb.h"

using namespace std;

int main(int argc, char** argv ){
    if(argc != 3){
        cout << "Usage: ./secgolomb <toencode file> <output file>" << endl;
        return -1;
    }
    int i = 0;
    int m = 0;

    secgolomb secgolomb(i,m);
    
    cout << secgolomb.encode(37000,4)<< endl;
    
    return 0;
}