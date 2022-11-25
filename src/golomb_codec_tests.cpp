#include <iostream>
#include <stdio.h>
#include "codec_golomb.h"

using namespace std;

int main(int argc, char** argv ){
    if(argc < 3){
        cout << "Usage: ./golomb_codec_tests <fileIn.wav> <fileOut.txt>\n";
        cout << "                            <fileIn.wav> <fileOut.txt> <num_channel [1,2]>\n";
        cout << "                            <fileIn.wav> <fileOut.txt> <num_channel [1,2]> <order [1,3]> \n";
        return -1;
    }

    golomb_codec g();

    if(argc == 3){
        cout << "Encoding file " << argv[1] << " to file " << argv[2] << endl;
        cout << "Using default parameters: dual channel, order 3" << endl;
        golomb_codec codec(2,3);
        codec.encode_wav_file(argv[1], argv[2]);
        codec.decode_to_wav(argv[2], "decoded.wav");
    }else if(argc == 4){
        //check if argv[3] != NULL
        //check num_channel is valid
        if(argv[3]!=NULL){
            if(atoi(argv[3])!=1 && atoi(argv[3])!=2){
                cout << argv[3] << endl;
                cout << "Invalid num_channel. Must be 1 or 2\n";
                return -1;
            }
        }
        cout << "Encoding file " << argv[1] << " to file " << argv[2] << endl;
        cout << "Using parameters: channel " << argv[3] << ", order 3" << endl;
        golomb_codec codec(atoi(argv[3]),3);
        codec.encode_wav_file(argv[1], argv[2]);
        codec.decode_to_wav(argv[2], "decoded.wav");
    }else if(argc == 5){
        //check if argv[3] != NULL
        //check num_channel is valid
        if(argv[3]!=NULL){
            if(atoi(argv[3])!=1 && atoi(argv[3])!=2){
                cout << argv[3] << endl;
                cout << "Invalid num_channel. Must be 1 or 2\n";
                return -1;
            }
        }
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
        cout << "Using parameters: channel " << argv[3] << ", order " << argv[4] << endl;
        golomb_codec codec(atoi(argv[3]),atoi(argv[4]));
        codec.encode_wav_file(argv[1], argv[2]);
    }


    //cout << goal.decode_to_string("encoded_values.txt") << endl;
    //goal.decode_to_file("encoded_values.txt", "decoded_values.txt");

}