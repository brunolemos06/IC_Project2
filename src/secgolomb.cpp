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
    
    //read the file encode.txt
    FILE *filer;
    filer = fopen(argv[1],"r");
    FILE *filew;
    filew = fopen(argv[2],"w");
    //print the size of the file
    fseek(filer,0,SEEK_END);
    int size = ftell(filer);
    fseek(filer,0,SEEK_SET);
    //create an array with the size of the file
    int *array = new int[size];
    //read each int of the file 
    while(fscanf(filer,"%d",&i) != EOF){
        //set the array with the int read
        for(int j = 0; j < size; j++){
            array[j] = i;
        }
    }
    
    return 0;
}