#ifndef SECGOLOMB_H
#define SECGOLOMB_H
#endif  SECGOLOMB_H

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <bitset>
#include <tgmath.h> 

using namespace std;

class secgolomb{
    private:
        int m;
        int i;
    public:
        secgolomb(int i,int m) : m{i}, i{m} {}
        
        string calculate_secgolomb2(int i, int m){
            int b = ceil(log2(m));        
            int q = abs(i) / m;
            int r = abs(i) -q*m;
            cout << "q: " << q << endl;
            cout << "r: " << r << endl;
            int aux = pow(2,b) - m;
            string array[r+1];
            for(int i = 0; i <= r; i++){
                if(i < aux){
                    array[i] = decimal_to_binary(i,b-1);       
                }else{
                    array[i] = decimal_to_binary(i+aux,b);
                }
            }
            if(i < 0){
                return calculate_unaryinv(q) + array[r] + "1";
            }else{
                return calculate_unaryinv(q) + array[r] + "0";
            }
        }

        string encode(int i,int m){
            if(isPowerOfTwo(m)){
                return calculate_secgolomb(i,m);
            }else{
                return calculate_secgolomb2(i,m);
            }
        }

        //method to transform a decimal number to binary with a specific number of bits
        string decimal_to_binary(int n, int bits){
            string binary = bitset<32>(n).to_string();
            return binary.substr(32-bits);
        }

        string calculate_secgolomb(int i, int m){
            string result = calculate_unary(calculate_q(i,m))+calculate_binary(calculate_r(i,m));
            return result;
        }
        
        //method to calculate if m is power of 2
        bool isPowerOfTwo(int m){
            if(m == 0){
                return false;
            }
            while(m != 1){
                if(m % 2 != 0){
                    return false;
                }
                m = m / 2;
            }
            return true;
        }
        //method to calculate q
        int calculate_q(int i, int m){
            return floor(i/m);
        }
        //method to calculate r
        int calculate_r(int i,int m){
            return i - (m*calculate_q(i,m));
        }
        //represent q as unary code
        string calculate_unary(int q){
            string unary = "";
            for(int i = 0; i < q; i++){
                unary += "0";
            }
            unary += "1";
            return unary;
        }
        string calculate_unaryinv(int q){
            string unary = "";
            for(int i = 0; i < q; i++){
                unary += "1";
            }
            unary += "0";
            return unary;
        }

        //represent r as binary code
        string calculate_binary(int r){
            //calculate the binary representation of r with 2 bits
            string binary = decimal_to_binary(r,2);
            return binary;
        }
        
};