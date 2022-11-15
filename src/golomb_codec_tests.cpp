#include <iostream>
#include <stdio.h>
#include "codec_golomb.h"

using namespace std;

int main(int argc, char** argv ){
    if(argc != 3){
        cout << "Usage: ./golomb_codec_tests <m> <file>" << endl;
        return -1;
    }
    int m = atoi(argv[1]);

    golomb_codec goal(m);
    goal.encode_to_file(argv[2], "encoded_values.txt");
    cout << goal.decode_to_string("encoded_values.txt") << endl;

}