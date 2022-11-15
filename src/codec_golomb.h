#ifndef CODEC_GOLOMB_H
#define CODEC_GOLOMB_H

#include <bitset>
#include <stdio.h>
#include <stdlib.h>
#include <bits/stdc++.h>
#include <fstream>

#include "golomb.h"

using namespace std;

class golomb_codec{
    private:
        golomb codec_alg;
        char buffer;
        int count;

    public:
        //golomb_codec(int m) : codec_alg(m) {};
        golomb_codec(int m) : codec_alg(m) {buffer=0; count=0;};

        void encode_to_file(const char* fileIn, const char* fileOut){
            //read ints from fileIn
            ifstream file(fileIn);
            //for each int, encode it and write to fileOut
            int n;
            string encoded;
            while (file >> n){
                //add encoded string to encoded
                encoded += codec_alg.encode_number(n);
            }
            file.close();

            //print encoded to terminal
            cout << encoded << endl;

            ofstream filew(fileOut,ios::out | ios::binary);
            for (uint32_t i = 0; i < encoded.length(); i++){
                buffer <<= 1;
                if (encoded[i] == '1') {
                    buffer |= 1;
                }
                count++;
                if(count == 8) {
                    //print the buffer
                    cout << "BUFFER VAL: "<< buffer << endl;
                    filew.write(&buffer, 1);
                    buffer = 0;
                    count = 0;
                }
            }
            //write last buffer
            if(count != 0){
                filew.write(&buffer, 1);
                count = 0;
                buffer = 0;
            }
            filew.close();
        }

        void decode_to_file(const char* fileIn, const char* fileOut){
            //read bits from fileIn
            ifstream file(fileIn);
            //read all file
            string encoded;
            char c;
            while (file.read(&c, 1)){
                //convert the byte to binary
                string binary = bitset<8>(c).to_string();
                //write only the first bit of the byte
                encoded += binary;
            }
            file.close();

            string decoded = codec_alg.decode_string(encoded);
            //write to file decoded, numbers separated by ',' and write each int in a new line
            ofstream filew(fileOut,ios::in | ios::binary);
            //separete numbers by ','
            for (uint32_t i = 0; i < decoded.length(); i++){
                if (decoded[i] == ','){
                    filew << endl;
                }else{
                    filew << decoded[i];
                }
            }
            filew.close();
        }

        string decode_to_string(const char* fileIn){
            //read bits from fileIn
            ifstream file(fileIn);
            //read all file
            string encoded;
            char c;
            while (file.read(&c, 1)){
                //convert the byte to binary
                string binary = bitset<8>(c).to_string();
                //write only the first bit of the byte
                encoded += binary;
            }
            file.close();

            string decoded = codec_alg.decode_string(encoded);
            return decoded;
        }
};

#endif