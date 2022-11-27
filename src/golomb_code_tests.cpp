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
    char *encoded_c = &encoded[0];
    while(*encoded_c != '\0'){
        encoded_c = test.decode_string(encoded_c, decoded, 1);
        cout << *decoded << endl;
    }

    return 0;
}