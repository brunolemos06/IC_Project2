#include <iostream>
#include <stdio.h>
#include "codec_golomb.h"

using namespace std;

int main(int argc, char** argv ){
    if(argc < 3){
        cout << "Usage: ./golomb_codec_tests <fileIn.wav> <fileOut.txt>\n";
        cout << "                            <fileIn.wav> <fileOut.txt> <order [1,3]> \n";
        return -1;
    }

    golomb_codec g();

    if(argc == 3){
        cout << "Encoding file " << argv[1] << " to file " << argv[2] << endl;
        cout << "Using default parameters: dual channel, order 3" << endl;
        golomb_codec codec(3);
        codec.encode_wav_file(argv[1], argv[2]);
        codec.decode_to_wav(argv[2], "decoded.wav");
    }else if(argc == 4){
        //check if order != NULL
        //check order is valid
        if(argv[4]!=NULL){
            if(atoi(argv[4])!=1 && atoi(argv[4])!=2 && atoi(argv[4])!=3){
                cout << argv[4] << endl;
                cout << "Invalid order. Must be 1, 2 or 3\n";
                return -1;
            }
        }
        cout << "Encoding file " << argv[1] << " to file " << argv[2] << endl;
        cout << "Using parameters: order " << argv[3] << endl;
        golomb_codec codec(atoi(argv[3]));
        codec.encode_wav_file(argv[1], argv[2]);
        codec.decode_to_wav(argv[2], "decoded.wav");
    }


    //cout << goal.decode_to_string("encoded_values.txt") << endl;
    //goal.decode_to_file("encoded_values.txt", "decoded_values.txt");

}