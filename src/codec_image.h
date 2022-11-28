#ifndef CODEC_GOLOMB_H
#define CODEC_GOLOMB_H

#include <bitset>
#include <stdio.h>
#include <stdlib.h>
#include <bits/stdc++.h>
#include <fstream>
#include <sndfile.hh>


#include <opencv2/opencv.hpp>
#include <iostream>

#include "golomb.h"
#include "circularBuffer.h"

using namespace std;
using namespace cv;

constexpr size_t FRAMES_BUFFER_SIZE = 65536; // Buffer for reading/writing frames

uint32_t samples_size = 0;
size_t frames=0;
int format=0, samplerate=0;

class image_codec{
    private:
        golomb codec_alg;
        circular_buffer *cb_encode;
        circular_buffer *cb_decode;
        char buffer;
        int count, num_channels, order;
        string encoded;

        //  Function to write the encoded data to a file
        int write_bin_to_file(const char* fileOut, string encoded){
            ofstream filew(fileOut,ios::out | ios::binary);
            for (uint32_t i = 0; i < encoded.length(); i++){
                buffer <<= 1;
                if (encoded[i] == '1') {
                    buffer |= 1;
                }
                count++;
                if(count == 8) {
                    //print the buffer
                    //cout << "BUFFER VAL: "<< buffer << endl;
                    filew.write(&buffer, 1);
                    buffer = 0;
                    count = 0;
                }
            }
            //write last buffer
            //if buffer is not empty
            if(count != 0) {
                //write buffer + padding
                buffer <<= (8-count);
                filew.write(&buffer, 1);
                count = 0;
                buffer = 0;
            }
            filew.close();
            return 0;
        }

        //  Function to read the encoded data from a file
        string read_bin_from_file(const char* fileIn){
            //read file
            ifstream filer(fileIn, ios::in | ios::binary);

            //read file
            char c;
            string encoded;
            while(filer.get(c)){
                //cout << "BUFFER VAL: "<< (int)c << endl;
                //convert char to binary
                bitset<8> x(c);
                //cout << "BUFFER VAL: "<< x << endl;
                encoded += x.to_string();
            }
            filer.close();
            return encoded;
        }

        int calculate_prediction(int mode, int a, int b, int c){
            if(mode == 1){
                // a
                return a;
            }else if(mode==2){
                //b
                return b;
            }else if(mode==3){
                //c
                return c;
            }else if(mode==4){
                //a+b-c
                return a+b-c;
            }else if(mode==5){
                //a+(b-c)/2
                return a+(b-c)/2;
            }else if(mode==6){
                //b+(a-c)/2
                return b+(a-c)/2;
            }else if(mode==7){
                return (a+b)/2;
            }else{
                //error
                return 0;
            }
        }

        uint32_t calc_m(double avg){
            double alfa = avg / (avg + 1);
            return ceil(-1/log2(alfa));
        }

    public:
        image_codec(int n_order) : codec_alg(){ buffer=0; count=0; num_channels=3;order=n_order; cb_encode = cb_init(50); cb_decode = cb_init(50);};
        image_codec() : codec_alg() {buffer=0; count=0; order=3; num_channels=3;cb_encode = cb_init(50); cb_decode = cb_init(50);};
        
        

        int encode_image_file(const char* fileIn, const char* fileOut){
            
            clock_t time_req;                           //for time measurement
            time_req = clock();                         //start time measurement
            encoded = "";
            
            //  declaare char array to store fileIn
            Mat image;
            image = imread( fileIn , 1 );
            if (!image.data){
                printf("No image data");
                return -1;
            }

            //  Get the image size
            int num_channels = 3;

            int real0;
            int real1;
            int real2;
            int a1, b1, c1, a2, b2, c2, a3, b3, c3;
            int pred1, pred2, pred3;
            int value1, value2, value3;
            codec_alg.change_m_decode(4);
            // for first line and first column
            for(int i=0; i<image.cols; i++){
                int value = image.at<Vec3b>(0,i)[0];
                encoded += codec_alg.encode_number(value, 0) + '\n';
                value = image.at<Vec3b>(0,i)[1];
                encoded += codec_alg.encode_number(value, 0) + '\n';
                value = image.at<Vec3b>(0,i)[2];
                encoded += codec_alg.encode_number(value, 0) + '\n';
            }
            
            int soma =0;
            for(int i = 1; i < 2; i++){
                for(int j = 0; j < image.cols; j++){
                    soma += image.at<Vec3b>(i,j)[0] + image.at<Vec3b>(i,j)[1] + image.at<Vec3b>(i,j)[2];
                    if (j == 0){
                        for(int k=0; k<num_channels; k++){
                            encoded += codec_alg.encode_number(image.at<Vec3b>(i,j)[k], 0);
                        }
                    }else{
                        real0 = image.at<Vec3b>(i,j)[0];
                        real1 = image.at<Vec3b>(i,j)[1];
                        real2 = image.at<Vec3b>(i,j)[2];

                        a1 = image.at<Vec3b>(i,j-1)[0];
                        b1 = image.at<Vec3b>(i-1,j)[0];
                        c1 = image.at<Vec3b>(i-1,j-1)[0];
                        a2 = image.at<Vec3b>(i,j-1)[1];
                        b2 = image.at<Vec3b>(i-1,j)[1];
                        c2 = image.at<Vec3b>(i-1,j-1)[1];
                        a3 = image.at<Vec3b>(i,j-1)[2];
                        b3 = image.at<Vec3b>(i-1,j)[2];
                        c3 = image.at<Vec3b>(i-1,j-1)[2];


                        pred1 = (a1+b1)/2;
                        value1 = real0 - pred1;
                        pred2 = (a2+b2)/2;
                        value2 = real1 - pred2;
                        pred3 = (a3+b3)/2;
                        value3 = real2 - pred3;

                        encoded += codec_alg.encode_number(value1,0);
                        encoded += codec_alg.encode_number(value2,0);
                        encoded += codec_alg.encode_number(value3,0);
                    }
                }
                // codec_alg.change_m_decode(calc_m(soma/(image.cols*3)));
            }

            write_bin_to_file(fileOut, encoded);
            time_req = clock() - time_req;
            cout << "Execution time: " << (float)time_req/CLOCKS_PER_SEC << " seconds\n" << endl;
            return 0;
        }

};

#endif