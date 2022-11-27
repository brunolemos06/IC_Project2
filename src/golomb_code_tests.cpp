#include <iostream>
#include <stdio.h>
#include "golomb.h"

using namespace std;

int main(int argc, char** argv ){

    golomb test(3);
    //open file
    FILE *fp;
    fp = fopen("try.txt", "r");
    if(fp == NULL){
        cout << "Error opening file" << endl;
        return -1;
    }
    //read file and encode value and add to string
    int value;
    string encoded;
    while(fscanf(fp, "%d", &value) != EOF){
        cout << value << endl;
        encoded += test.encode_number(value, 1);
    } 

    //print encoded string
    cout << encoded << endl;

    //decode string to int and print
    long *decoded = (long*) malloc(sizeof(long));
    while(encoded.length()>0){
        encoded = test.decode_string(encoded, decoded, 1);
    }

    return 0;
}