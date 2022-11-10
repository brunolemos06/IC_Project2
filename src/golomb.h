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
        map<string, int> decode_bin;
        map<int, string> encode_bin;

    
    public:
        golomb(int m){
            this->m = m;
            map<string, int> decode_table;
            map<int, string> encode_table;
            int b = ceil(log2(m));
            if(!(ceil(log2(m)) == floor(log2(m)))){
                cout << "NOT power of 2" << endl;
                for(int i=0; i<(pow(2,b)-m); i++){
                    //create bin string with b-1 bits
                    string bin;
                    copy_n(bitset<32>(i).to_string().end()-b+1, b-1, back_inserter(bin));
                    decode_table[bin] = i;
                    encode_table[i] = bin;
                }
                //code remaining values of r by coding the number r+(2^b - m) with b bits
                for(int i=(pow(2,b)-m); i<m; i++){
                    int n = i + (pow(2,b) - m);
                    //create bin string with b bits
                    string bin;
                    copy_n(bitset<32>(n).to_string().end()-b, b, back_inserter(bin));
                    decode_table[bin] = i;
                    encode_table[i] = bin;
                }
            }else{
                //create bin string with b bits
                for(int i=0; i<m; i++){
                    string bin;
                    copy_n(bitset<32>(i).to_string().end()-b, b, back_inserter(bin));
                    decode_table[bin] = i;
                    encode_table[i] = bin;
                }
            }
            this->decode_bin = decode_table;
            this->encode_bin = encode_table;
            //print tables
            // cout << "DECODE TABLE" << endl;
            // for (auto const& x : this->decode_bin)
            //     cout << x.first << " " << x.second << endl;
            // cout << "ENCODE TABLE" << endl;
            // for (auto const& x : this->encode_bin)
            //     cout << x.first << " " << x.second << endl;

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
            int decimal = decode_bin[rest];
            //sum unary and binary
            int result = count*m + decimal; 
            //if the number is even, make it negative
            if(result % 2 == 0){
                return -(result/2);
            }else{
                return (result-1)/2;
            } 
        }

        //encode decimal to golomb code string
        string encode_number(int n){
            //map n<=0 to positive even numbers and n>0 to positive odd numbers
            //cout << "\tORIGINAL: " << n << endl;
            if(n <= 0){
                n = -2*n;
            }else{
                n = 2*n + 1;
            }
            //calculate unary part
            cout << "\tMAPPED: " << n << endl;
            string unary = encode_unary(n);
            //calculate binary part
            string binary = encode_decimal(n);
            //concatenate unary and binary
            return unary + binary;
        }

        //receives natural number and returns the binary string of unary part
        string encode_unary(int n){
            //calculate unary part
            int f = n/m;
            //create unary string
            cout << "\t\tF: " << f <<"  ";
            string unary = "";
            //write 1 f times
            for(int i = 0; i < f; i++){
                unary += "1";
            }
            //add separator aka "0"
            unary += "0";
            cout << "UNARY: " << unary << endl;
            return unary;
        }

        //receives natural number and returns the binary string of decimal part
        string encode_decimal(int n){
            //calculate decimal part
            int r = n%m;
            cout << "\t\tR: " << r << "  ";
            cout << "BIN: " << this->encode_bin[r] << endl;
            return this->encode_bin[r];
        }
};

#endif