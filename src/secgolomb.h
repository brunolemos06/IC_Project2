#ifndef SECGOLOMB_H
#define SECGOLOMB_H
#endif  SECGOLOMB_H

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <bitset>
#include <tgmath.h> 
#include <map>
#include <bits/stdc++.h>

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
        map<string, int> calcdecode(int m){
            map<string, int> decode_table;
            int b = ceil(log2(m));
            if(!(ceil(log2(m)) == floor(log2(m)))){
                // cout << "NOT power of 2" << endl;
                for(int i=0; i<(pow(2,b)-m); i++){
                    //create bin string with b-1 bits
                    string bin;
                    copy_n(bitset<32>(i).to_string().end()-b+1, b-1, back_inserter(bin));
                    decode_table[bin] = i;
                    // cout << "decode_table[" << bin << "] = " << i << endl;
                    //store the value in the map
                }
                //code remaining values of r by coding the number r+(2^b - m) with b bits
                for(int i=(pow(2,b)-m); i<m; i++){
                    int n = i + (pow(2,b) - m);
                    //create bin string with b bits
                    string bin;
                    copy_n(bitset<32>(n).to_string().end()-b, b, back_inserter(bin));
                    decode_table[bin] = i;
                    // cout << "decode_table[" << bin << "] = " << i << endl;
                    // cout << "encode_table[" << i << "] = " << bin << endl;
                    //store the value in the map
                }
            }else{
                //create bin string with b bits
                for(int i=0; i<m; i++){
                    string bin;
                    copy_n(bitset<32>(i).to_string().end()-b, b, back_inserter(bin));
                    decode_table[bin] = i;
                    // cout << "decode_table[" << bin << "] = " << i << endl;
                }
            }
            return decode_table;
        }
        

        void decoder(string code, int m){
            //read the code until the first 0
            int q = 0;
            int r = 0;
            int b = ceil(log2(m));
            int aux = pow(2,b) - m;
            int i = 0;
            int decoded = 0;
            string r_string;
            map<string, int> map;
            for (int i = 0; i < code.length(); i++){
                //count the firts ones before the first 0
                if(code[i] == '1'){
                    q++;
                }else{
                    break;
                }                         
            }
            for (int j = q+1; j < code.length()-1; j++)
            {
                //store the value of r in a string
                r_string += code[j];
            }
            // cout << "q: " << q << endl;
            // cout << r_string <<endl;
            map = calcdecode(m);
            for(auto it = map.begin(); it != map.end(); ++it){
            // cout << it->first << " => " << it->second << '\n';
            //find the value of r_string in the map
                if(it->first == r_string){
                    r = it->second;
                }
            }
            // cout << "r: " << r << endl;
            decoded = (q*m)+r;
            if(code[code.length()-1]=='1'){
                decoded = decoded * -1;
            }
            cout << "decoder: " << decoded << endl;
        }

        string separator(string code,int m){
            map<string, int> map;
            map = calcdecode(5);  
            string aux;
            // for(auto it = map.begin(); it != map.end(); ++it){
            //     cout << it->first << " => " << it->second << '\n';
            // }
            int max = map.size();
            int findzero = 0;
            string aux2;
            string aux3;
            int aux4;
            string r;
            int binfound=0;
            string signal;
            int signalfound=0;
            string words;
            string word;
            int inc=0;
            for (int i=0; i<code.length(); i++){
                while (findzero==0){
                    aux2 += code[i];
                    if(code[i]=='1'){
                        inc++;
                    }
                    if(code[i]=='0'){
                        findzero = 1;
                        r+=1;   
                        // cout<<"unario: "<<aux2<<endl;   
                    }   
                    i++;
                }
                if(findzero==1){
                    aux3 += code[i];
                    for(auto it = map.begin(); it != map.end(); ++it){
                        if(it->first == aux3){
                            aux4 = it->second;
                            r=it->first;
                            // cout<<"binario: "<< r <<endl;
                            binfound=1;
                            i++;
                        }
                    }
                }
                if(binfound==1){
                    signal=code[i];
                    if(signal=="1"){
                        // cout<<"sinal: "<<signal<<endl;
                        signalfound=1;
                    }else{
                        // cout<<"sinal: "<<signal<<endl;
                        signalfound=1;
                    }
                }
                if(binfound==1&&findzero==1&&signalfound==1){
                    word=aux2+r+signal;
                    decoder(word,m);
                    aux2="";
                    aux3="";
                    aux4=0;
                    inc=0;
                    findzero=0;
                    binfound=0;
                    signalfound=0;
                }
                
            }
            
            return "moca";
        }
            
        
};