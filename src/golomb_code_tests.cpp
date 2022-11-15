#include <iostream>
#include <stdio.h>
#include "golomb.h"

using namespace std;

int main(int argc, char** argv ){
    if(argc != 3){
        cout << "Usage: ./golomb_code_tests <m> <file_to_encode>" << endl;
        return -1;
    }
    int m = atoi(argv[1]);

    golomb goal(m);
    //read ints from file
    FILE *fp;
    fp = fopen(argv[2], "r");
    int n;
    //write encoded ints to file
    FILE *fp2;
    fp2 = fopen("encoded_values.txt", "w");
    while(fscanf(fp, "%d", &n) != EOF){
        //write encoded value to file all on same line
        fprintf(fp2, "%s", goal.encode_number(n).c_str());
        //retiro .c_str() e mete bytes no ficheiro?
    }
    fclose(fp);
    fclose(fp2);

    //read encoded ints from file
    FILE *fp3;
    fp3 = fopen("encoded_values.txt", "r");
    //read all file into string
    string encoded;
    char c;
    while(fscanf(fp3, "%c", &c) != EOF){
        encoded += c;
    }

    //decode encoded string
    cout << "DECODING VALUES: " << goal.decode_string(encoded) << endl;
    return 0;
}