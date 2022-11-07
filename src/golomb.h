#ifndef COLOMB_H
#define COLOMB_H

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <bitset>

using namespace std;

class golomb{
    private:
        int m;
    
    public:
        golomb(int m){
            this->m = m;
        }

        //decode binary string to decimal
        int decode_string(string bits){
            //count numbers of 0s until first 1
            int count = 0;
            for(int i = 0; i < bits.length(); i++){
                if(bits[i] == '0'){
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
            //write 0 f times
            for(int i = 0; i < f; i++){
                unary += "0";
            }
            //add separator aka "1"
            unary += "1";
            return unary;
        }

        //receives natural number and returns the binary string of binary part
        string calculate_binary(int n){
            //calculate decimal part
            int r = n%m;
            //create to bin string
            string bin = "";
            //write 0 r times
            for(int i = 0; i < r; i++){
                bin += "0";
            }
            //add separator aka "1"
            bin += "1";
        }

        string findTwoscomplement(string str){
            int n = str.length();
            // Traverse the string to get first '1' from
            // the last of string
            int i;
            for (i = n-1 ; i >= 0 ; i--)
                if (str[i] == '1')
                    break;
        
            // If there exists no '1' concatenate 1 at the
            // starting of string
            if (i == -1)
                return '1' + str;
        
            // Continue traversal after the position of
            // first '1'
            for (int k = i-1 ; k >= 0; k--)
            {
                //Just flip the values
                if (str[k] == '1')
                    str[k] = '0';
                else
                    str[k] = '1';
            }
        
            // return the modified string
            return str;
        }

        
};
