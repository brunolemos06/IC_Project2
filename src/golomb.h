#ifndef COLOMB_H
#define COLOMB_H

#include <iostream>
#include <map>
#include <bitset>
#include <stdio.h>
#include <stdlib.h>
#include <bits/stdc++.h>

using namespace std;

class golomb{
    private:
        int m;
        int power_of_2;
        map<string, int> decode_bin;

    
    public:
        golomb(int m){
            this->m = m;
            this->power_of_2 = (ceil(log2(m)) == floor(log2(m)));
            map<string, int> table;
            if(!this->power_of_2){
                int b = ceil(log2(m));
                for(int i=0; i<(pow(2,b)-m); i++){
                    //create bin string with b-1 bits
                    string bin = bitset<32>(i).to_string();
                    bin = bin.substr(8-b+1, b-1);
                    table[bin] = i;
                }
                //code remaining values of r by coding the number r+(2^b - m) with b bits
                for(int i=(pow(2,b)-m); i<m; i++){
                    int n = i + (pow(2,b) - m);
                    //create bin string with b bits
                    string bin = bitset<32>(n).to_string();
                    bin = bin.substr(8-b, b);
                    table[bin] = i;
                }
            }else{
                map<string, int> table;
                //create bin string with b bits
            }
            this->decode_bin = table;
        }

        //decode binary string to decimal
        int decode_string(string bits){
            //count numbers of 1s until first 0
            int count = 0;
            for(int i = 0; i < bits.length(); i++){
                if(bits[i] == '1'){
                    count++;
                }else{
                    break;
                }
            }
            //convert the rest of the bits to decimal
            string rest = bits.substr(count+1);
            int decimal = stoi(rest, 0, 2);
            //if the number is even, make it negative
            if(decimal % 2 == 0){
                return -(((count*m) + decimal)/2);
            }
            
            return (((count*m) + decimal)-1)/2; 
        }

        //encode decimal to golomb code string
        string encode_number(int n){
            //map n<=0 to positive even numbers and n>0 to positive odd numbers
            if(n <= 0){
                n = -2*n;
            }else{
                n = 2*n + 1;
            }
            //calculate unary part
            string unary = calculate_unary(n);
            //calculate binary part
            string binary = calculate_binary(n);
            //concatenate unary and binary
            return unary + binary;
        }

        //receives natural number and returns the binary string of unary part
        string calculate_unary(int n){
            //calculate unary part
            int f = n/m;
            //create unary string
            string unary = "";
            //write 1 f times
            for(int i = 0; i < f; i++){
                unary += "1";
            }
            //add separator aka "0"
            unary += "0";
            return unary;
        }

        //receives natural number and returns the binary string of binary part
        string calculate_binary(int n){
            //calculate decimal part
            int r = n%m;
            //create binary string
            string binary;
            if(!this->power_of_2){
                //calculate b
                int b = ceil(log2(m));
                //if r is less than 2^b - m, code r with b-1 bits
                if(r < (pow(2,b) - m)){
                    //create bin string with b-1 bits
                    binary = bitset<32>(r).to_string();
                    binary = binary.substr(8-b+1, b-1);
                //else code r+(2^b - m) with b bits
                }else{
                    //create bin string with b bits
                    binary = bitset<32>(r+(pow(2,b) - m)).to_string();
                    binary = binary.substr(8-b, b);
                }
            }else{
                //calculate b
                int b = log2(m);
                binary = bitset<32>(r).to_string();
                binary = binary.substr(8-b, b);
            }
            return binary;
        }
};

#endif