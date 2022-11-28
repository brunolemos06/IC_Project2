#include <iostream>
#include <stdio.h>
#include "codec_golomb.h"

using namespace std;

int main(int argc, char** argv ){
    if(argc < 4){
        cout << "Usage: ./golomb_codec_tests <fileIn.wav> <encoded.txt> <decoded.wav>\n";
        cout << "                            <fileIn.wav> <fileOut.txt> <decoded.wav> <order [2,3]>\n";
        cout << "                            <fileIn.wav> <fileOut.txt> <decoded.wav> <order [2,3]> <x(update m every x samples)> <y(calculate medium of y samples)>\n";
        return -1;
    }

    if(argc == 4){
        cout << "Encoding file " << argv[1] << " to file " << argv[2] << endl;
        cout << "Using default parameters: order 3, update m every 2000 samples, calculate medium value for 1250 samples" << endl;
        golomb_codec codec(3, 2000, 1250);
        codec.encode_wav_file(argv[1], argv[2]);
        cout << "Decoding file " << argv[2] << " to file " << argv[3] << endl;
        codec.decode_to_wav(argv[2], argv[3]);
    }else if(argc == 5){
        int order = atoi(argv[4]);
        if(order < 2 || order > 3){
            cout << "Invalid order. Order must be 2 or 3" << endl;
            return -1;
        }
        cout << "Encoding file " << argv[1] << " to file " << argv[2] << endl;
        cout << "Using parameters: order " << argv[4] << ", update m every 2000 samples, calculate medium value for 1250 samples" << endl;
        golomb_codec codec(order, 2000, 1250);
        codec.encode_wav_file(argv[1], argv[2]);
        cout << "Decoding file " << argv[2] << " to file " << argv[3] << endl;
        codec.decode_to_wav(argv[2], argv[3]);
    }else if(argc == 7){
        int order = atoi(argv[4]);
        if(order < 2 || order > 3){
            cout << "Invalid order. Order must be between 1 and 3" << endl;
            return -1;
        }
        int x = atoi(argv[5]);
        if(x < 1){
            cout << "Invalid x. x must be greater than 0" << endl;
            return -1;
        }
        int y = atoi(argv[6]);
        if(y < 1){
            cout << "Invalid y. y must be greater than 0" << endl;
            return -1;
        }
        cout << "Encoding file " << argv[1] << " to file " << argv[2] << endl;
        cout << "Using parameters: order " << argv[4] << ", update m every " << argv[5] << " samples, calculate medium value for " << argv[6] << " samples" << endl;
        golomb_codec codec(order, x, y);
        codec.encode_wav_file(argv[1], argv[2]);
        cout << "Decoding file " << argv[2] << " to file " << argv[3] << endl;
        codec.decode_to_wav(argv[2], argv[3]);
    }

}