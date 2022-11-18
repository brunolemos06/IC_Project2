#include <iostream>
#include <stdio.h>
#include "secgolomb.h"

using namespace std;

int main(int argc, char** argv ){
    if(argc != 4){
        cout << "Usage: ./secgolomb <encode/decode> <toencode/decoded file> <todecode/encoded file>" << endl;
        return -1;
    }
    int i = 0;
    int m = 5;
    string towrite;

    secgolomb secgolomb(i,m);
    string arg1 = argv[1];
    if(arg1 == "encode"){
        cout << "Encoding..." << endl;
        cout<<secgolomb.encode(8,5)<<endl;
    }else if(arg1=="decode"){
        FILE *fp;
        fp = fopen(argv[2],"r");
        map<string,int> map;
        if(fp == NULL){
            cout << "Error opening file" << endl;
            return -1;
        }
        string result = "";
        char c;

        while((c = fgetc(fp)) != EOF){
            result += c;
        }
        fclose(fp);
        cout<<secgolomb.separator(result,m)<<endl;
        towrite = secgolomb.separator(result,m);
        FILE *fp2;
        fp2 = fopen(argv[3],"w");
        if(fp2 == NULL){
            cout << "Error opening file" << endl;
            return -1;
        }
        fprintf(fp2,"%s",towrite.c_str());
        fclose(fp2);      
    }
    
    return 0;
}