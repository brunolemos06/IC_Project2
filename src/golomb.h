#ifndef GOLOMB_H
#define GOLOMB_H

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
        uint32_t m_encode, m_decode;
        //map<string, uint32_t> decode_bin;

        // void init_decode_map(){
        //     map<string, uint32_t> decode_table;
        //     int b = ceil(log2(m_decode));
        //     if(b==0) b=1;
        //     if(!(ceil(log2(m_decode)) == floor(log2(m_decode)))){
        //         //cout << "NOT power of 2" << endl;
        //         for(uint32_t i=0; i<(pow(2,b)-m_decode); i++){
        //             //create bin string with b-1 bits
        //             string bin;
        //             copy_n(bitset<32>(i).to_string().end()-b+1, b-1, back_inserter(bin));
        //             decode_table[bin] = i;
        //         }
        //         //code remaining values of r by coding the number r+(2^b - m) with b bits
        //         for(uint32_t i=(pow(2,b)-m_decode); i<m_decode; i++){
        //             uint32_t n = i + (pow(2,b) - m_decode);
        //             //create bin string with b bits
        //             string bin;
        //             copy_n(bitset<32>(n).to_string().end()-b, b, back_inserter(bin));
        //             decode_table[bin] = i;
        //         }
        //     }else{
        //         // 0 equals string with b 0's
        //         string bin(b, '0');
        //         decode_table[bin] = 0; 
        //         //create bin string with b bits
        //         for(uint32_t i=1; i<m_decode; i++){
        //             string bin;
        //             //create bin string and remove leading zeros
        //             bin = bitset<32>(i).to_string();
        //             //leave only b bits
        //             bin.erase(0, bin.size()-b);
        //             decode_table[bin] = i;
        //         }
        //     }
        //     //print tables
        //     this->decode_bin = decode_table;
        //     cout << "DECODE TABLE FOR "<< this->m_decode << endl;
        //     for (auto const& x : this->decode_bin)
        //         cout << x.first << " " << x.second << endl;
        // }

        //receives natural number and returns the binary string of unary part
        string encode_unary(uint32_t n){
            //calculate unary part
            //cout << "n: " << n << endl;
            //cout << "m: " << this->m_encode << endl;
            uint32_t f = n/m_encode;
            //create unary string
            //cout << "\t\tF: " << f <<"  ";
            string unary = "";
            //write 1 f times
            for(uint32_t i = 0; i < f; i++){
                unary += "1";
            }
            //add separator aka "0"
            unary += "0";
            //cout << "UNARY: " << unary << endl;
            return unary;
        }

        //receives natural number and returns the binary string of decimal part
        string encode_decimal(uint32_t n){
            //calculate decimal part
            uint32_t r = n%m_encode;
            uint32_t b = ceil(log2(m_encode));
            string bin;
            if(!(ceil(log2(m_encode)) == floor(log2(m_encode)))){
                if(r < (pow(2,b)-m_encode)){
                    //create bin string with b-1 bits
                    copy_n(bitset<32>(r).to_string().end()-b+1, b-1, back_inserter(bin));
                }else{
                    uint32_t tmp = r + (pow(2,b) - m_encode);
                    //create bin string with b bits
                    copy_n(bitset<32>(tmp).to_string().end()-b, b, back_inserter(bin));
                }
            }else{
                if(r == 0){
                    //bin is string with b bits of 0
                    bin = string(b, '0');
                }else{
                    bin = bitset<32>(r).to_string();
                    //leave only LSB B bits
                    bin.erase(0, bin.length()-b);
               }
            }
            //cout << "\t\tR: " << r << "  ";
            //cout << "\t\tBIN: " << bin << endl;
            return bin;
        }

    public:
        golomb(uint32_t m){
            this->m_encode = m;
            this->m_decode = m;
            //this->init_decode_map();
        }

        golomb(){
            this->m_encode = 1000;
            this->m_decode = 1000;
            //this->init_decode_map();
        }

        //decode binary string to decimal
        string decode_string(string bits, long *result_n, int mapping_on){
            //cout << "\nENCODED STRING: " << bits << endl;
            //create string to store decoded values
            string decoded;

            //check if string contains at least char 1
            if(bits.find('1') == string::npos){
                //cout << "NO 1's FOUND" << endl;
                return "";
            }

            //count numbers of 1s until first 0
            uint32_t count = 0;
            for(uint32_t i = 0; i < bits.length(); i++){
                if(bits[i] == '1'){
                    count++;
                }else{
                    break;
                }
            }
            //remove 1's and 0
            bits.erase(0, count+1);

            //check if string only as character 0
            if(bits.length() == 0){
                //cout << "ONLY 0's" << endl;
                *result_n = count*m_decode;
                return decoded;
            }

            //if m_decode is power of 2
            uint32_t b = floor(log2(m_decode));
            // if(ceil(log2(m_decode)) == floor(log2(m_decode))){
            //     //cout << "POWER OF 2" << endl;
            //     //read b bits
            //     key = bits.substr(0, b);
            //     //remove b bits from bits
            //     bits.erase(0, b);
            // }else{ //if m_decode is not power of 2 key can have b-1 or b bits
            //     //cout << "NOT POWER OF 2" << endl;
            //     //read b-1 bits
            //     if(b==0){ b=1; }
            //     key = bits.substr(0, b);
            //     //check if key not in map
            //     if(decode_bin.find(key) == decode_bin.end()){
            //         //cout << "KEY NOT IN MAP" << endl;
            //         //read b-1 bits
            //         key = bits.substr(0, b-1);
            //         //remove b-1 bits from bits
            //         bits.erase(0, b-1);
            //     }else{
            //         //cout << "KEY IN MAP" << endl;
            //         //remove b bits from bits
            //         bits.erase(0, b);
            //     }
            // }

            string key = bits.substr(0, b);                                //read b bits
            uint32_t decimal = (uint32_t) bitset<32>(key).to_ulong();   //transform key to uint32_t
            if(decimal< ( pow(2,(b+1)) - m_decode)){                //if decimal is less than 2^(b+1) - m then r' = r
                bits.erase(0, b);                                   //remove b bits from bits
            }else{
                key = bits.substr(0, b+1);                          //read b+1 bits
                decimal = (uint32_t) bitset<32>(key).to_ulong();    //r'
                decimal = decimal - pow(2,(b+1)) + m_decode;        //r = r' - 2^(b+1) + m
                bits.erase(0, b+1);
            }

            //uint32_t decimal = decode_bin[key];
            //sum unary and binary
            uint32_t result = count*m_decode + decimal;

            *result_n = (long) result;
            if(mapping_on){
                if(result % 2 == 0){
                    *result_n = (long) (*result_n/-2);
                }else{
                    *result_n = (long) ((*result_n-1)/2);
                }
            }
 
            //cout << "UNARY: " << count*m_decode << " + " << "BINARY: " << decimal << " = " << result << " -> UNMAPPED: "<< *result_n << endl;
            //cout << "REMAINING BITS: " << bits << endl;
            return bits;
        }

        //encode decimal to golomb code string
        string encode_number(int n, int mapping_on){
            //cout << "\tORIGINAL: " << n << endl;
            //map n<=0 to positive even numbers and n>0 to positive odd numbers
            uint32_t mapped;
            if(mapping_on){
                if(n < 0){
                    mapped = -2*n;
                }else{
                    mapped = (2*n) +1;
                }
            }else{
                mapped = n;
            }
            
            //cout << "\tMAPPED: " << mapped << endl;
            //calculate unary part
            string unary = encode_unary(mapped);
            //calculate binary part
            string binary = encode_decimal(mapped);

            //concatenate unary and binary
            return unary + binary;
        }

        //return m_decode
        uint32_t get_m_decode(){
            return this->m_decode;
        }

        void change_m_encode(uint32_t m){
            this->m_encode = m;
        }

        uint32_t get_m_encode(){
            return this->m_encode;
        }

        void change_m_decode(uint32_t m){
            this->m_decode = m;
            //this->init_decode_map();
        }
};

#endif