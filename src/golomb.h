#ifndef COLOMB_H
#define COLOMB_H

#include <iostream>
#include <map>
#include <bitset>
#include <stdio.h>
#include <stdlib.h>
#include <bits/stdc++.h>

#include "circularBuffer.h"

using namespace std;

class golomb{
    private:
        int m_encode, m_decode;
        circular_buffer *cb_encode;
        circular_buffer *cb_decode;
        map<string, int> decode_bin;

        void init_decode_map(){
            map<string, int> decode_table;
            int b = ceil(log2(m_decode));
            if(!(ceil(log2(m_decode)) == floor(log2(m_decode)))){
                cout << "NOT power of 2" << endl;
                for(int i=0; i<(pow(2,b)-m_decode); i++){
                    //create bin string with b-1 bits
                    string bin;
                    copy_n(bitset<32>(i).to_string().end()-b+1, b-1, back_inserter(bin));
                    decode_table[bin] = i;
                }
                //code remaining values of r by coding the number r+(2^b - m) with b bits
                for(int i=(pow(2,b)-m_decode); i<m_decode; i++){
                    int n = i + (pow(2,b) - m_decode);
                    //create bin string with b bits
                    string bin;
                    copy_n(bitset<32>(n).to_string().end()-b, b, back_inserter(bin));
                    decode_table[bin] = i;
                }
            }else{
                decode_table["0"] = 0;
                //create bin string with b bits
                for(int i=1; i<m_decode; i++){
                    string bin;
                    //create bin string and remove leading zeros
                    bin = bitset<32>(i).to_string();
                    //remove leading zeros
                    bin.erase(0, min(bin.find_first_not_of('0'), bin.size()-1));
                    decode_table[bin] = i;
                }
            }
            //print tables
            this->decode_bin = decode_table;
            cout << "DECODE TABLE FOR "<< this->m_decode << endl;
            for (auto const& x : this->decode_bin)
                cout << x.first << " " << x.second << endl;
        }

        int calc_m(double avg){
            double p = 1 / (avg +1);
            // cout << "P: " << p << endl;
            // cout << "LOG(P): " << log2(p) << endl;
            // cout << "m': " << 1/log2(p) << endl;
            // cout << "M: " << ceil(-(1/(log(p)))) << endl;
            return ceil(-1/(log(p)));
        }

    public:
        golomb(int m){
            this->m_encode = m;
            this->m_decode = m;
            this->init_decode_map();
            this->cb_encode = cb_init();
            this->cb_decode = cb_init();
        }

        //decode binary string to decimal
        string decode_string(string bits){
            cout << "\nENCODED STRING: " << bits << endl;
            //create string to store decoded values
            string decoded;
            //don't stop until string is empty
            while(bits.length() > 0){
                //count numbers of 1s until first 0
                int count = 0;
                for(uint32_t i = 0; i < bits.length(); i++){
                    if(bits[i] == '1'){
                        count++;
                    }else{
                        break;
                    }
                }

                string rest = bits.substr(count+1);
                //read bits from rest until find key in map
                string key;
                for(uint32_t i = 0; i < rest.length(); i++){
                    key += rest[i];
                    if(decode_bin.find(key) != decode_bin.end()){
                        break;
                    }
                }
                //remove key from rest
                rest.erase(0, key.length());
                //add bits to key until find key+1bit not in map
                for(uint32_t i = 0; i < rest.length(); i++){
                    key += rest[i];
                    //if can't find key+1bit in map, break
                    if(decode_bin.find(key) == decode_bin.end()){
                        key.erase(key.length()-1);
                        break;
                    }
                }
                //remove key from rest
                rest.erase(0, key.length());
                //remove key from bits
                bits.erase(0, count+1+key.length());
                cout << "\tKEY: " << key << endl;
                int decimal = decode_bin[key];
                cout << "\tDECIMAL: " << decimal << endl;
                //sum unary and binary
                int result = count*m_decode + decimal;
                cout << "\tRESULT: " << result << endl; 

        
                //if the number is even, make it negative
                if(result % 2 == 0){
                    result=  -(result/2);
                }else{
                    result=  (result-1)/2;
                }

                cb_push(cb_decode, result);
                if(cb_is_full(cb_decode)){
                    double cb_avg=1;
                    cb_average(cb_decode, &cb_avg); 
                    double tmp_m = calc_m(cb_avg);
                    if(tmp_m != this->m_decode){
                        this->m_decode = tmp_m;
                        this->init_decode_map();
                        cout << "\tNEW M: " << this->m_encode << endl;
                    }
                }
                cout << "DECODED: " << result << endl;
                cout << "LEFT: " << bits << endl;
                //append result to string
                decoded += to_string(result) + ", ";
            }
            return decoded;
        }

        //encode decimal to golomb code string
        string encode_number(int n){
            //cout << "\tORIGINAL: " << n << endl;
            //map n<=0 to positive even numbers and n>0 to positive odd numbers
            int mapped;
            if(n <= 0){
                mapped = -2*n;
            }else{
                mapped = 2*n + 1;
            }
            
            //calculate unary part
            cout << "\tMAPPED: " << mapped << endl;
            string unary = encode_unary(mapped);
            //calculate binary part
            string binary = encode_decimal(mapped);
            
            cb_push(cb_encode, n);
            if(cb_is_full(cb_encode)){
                double cb_avg=1;
                //call average function
                cb_average(cb_encode, &cb_avg);
                cout << "\tAVERAGE: " << cb_avg << endl;
                this->m_encode = calc_m(cb_avg);
                cout << "\tNEW M: " << this->m_encode << endl;
            }

            //concatenate unary and binary
            return unary + binary;
        }

        //receives natural number and returns the binary string of unary part
        string encode_unary(int n){
            //calculate unary part
            int f = n/m_encode;
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
            int r = n%m_encode;
            int b = ceil(log2(m_encode));
            string bin;
            if(!(ceil(log2(m_encode)) == floor(log2(m_encode)))){
                if(r < (pow(2,b)-m_encode)){
                    //create bin string with b-1 bits
                    copy_n(bitset<32>(r).to_string().end()-b+1, b-1, back_inserter(bin));
                }else{
                    int tmp = r + (pow(2,b) - m_encode);
                    //create bin string with b bits
                    copy_n(bitset<32>(tmp).to_string().end()-b, b, back_inserter(bin));
                }
            }else{
                if(r == 0){
                    bin = "0";
                }else{
                    bin = bitset<32>(r).to_string();
                    //remove leading zeros
                    bin.erase(0, min(bin.find_first_not_of('0'), bin.size()-1));
               }
            }
            cout << "\t\tR: " << r << "  ";
            cout << "BIN: " << bin << endl;
            return bin;
        }
};

#endif