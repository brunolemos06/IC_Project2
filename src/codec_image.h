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
                //(a+b)/2
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
        golomb_codec(int n_order) : codec_alg(){ buffer=0; count=0; order=n_order; num_channels=0; cb_encode = cb_init(50); cb_decode = cb_init(50);};
        golomb_codec() : codec_alg() {buffer=0; count=0; order=3; num_channels=2; cb_encode = cb_init(50); cb_decode = cb_init(50);};

        int encode_image_file(const char* fileIn, const char* fileOut){
            
            clock_t time_req;                           //for time measurement
            time_req = clock();                         //start time measurement

            //  Open the input file
            if ( argc != 3 ){
                printf("usage: ../bin/ex6 <Image_Path> <Image_Path>");
                return -1;
            }
            Mat image;
            image = imread( argv[1], 1 );
            if (!image.data){
                printf("No image data");
                return -1;
            }

            //  Get the image size
            int width = image.cols;
            int height = image.rows;
            int channels = image.channels();

            for(int i = 0; i < image.rows; i++){
                for(int j = 0; j < image.cols; j++){
                    Vec3b bgrPixel = image.at<Vec3b>(i,j);
                    printf("image3.at<Vec3b>(%d,%d) = (%d,%d,%d)\n", i, j,bgrPixel[0], bgrPixel[1], bgrPixel[2]);
                }
            }       

            time_req = clock() - time_req;
            cout << "Execution time: " << (float)time_req/CLOCKS_PER_SEC << " seconds\n" << endl;
            return 0;
        }

        int decode_to_image(const char* fileIn, const char* fileOut){
            //start clock
            clock_t time_req;
            time_req = clock();

            //read encoded file
            string encoded = read_bin_from_file(fileIn);
            
            uint32_t *decoded_value;                        //variable to store decoded value
            decoded_value = (uint32_t*)malloc(sizeof(uint32_t));    //initialize decoded_value
            
            uint32_t *unmapped_value;                       //variable to store mapped value
            unmapped_value = (uint32_t*)malloc(sizeof(uint32_t));   //initialize unmapped_value

            vector<short> decoded_error_samples;            //vector to store decoded error samples

            //decode encoded file
            //decode first order*num_channels samples
            for(uint32_t i=0; i<order*num_channels; i++){
                encoded = codec_alg.decode_string(encoded, decoded_value, 0);

                //unmap decoded error value
                if(*decoded_value % 2 == 0){
                    //-(result/2)
                    *unmapped_value = -((*decoded_value)/2);
                }else{
                    //(result+1)/2
                    *unmapped_value = (*decoded_value-1)/2;
                }
                //push unmapped error value to vector
                decoded_error_samples.push_back(*unmapped_value);

            }

            //decode rest of samples
            uint32_t i = 0;
            while(encoded.size() > 0){
                //string decode_string(string bits, uint32_t *result_n, int mapping_on)
                encoded = codec_alg.decode_string(encoded, decoded_value, 0);
                //if encoded is empty, break
                if(encoded.size() == 0){
                    break;
                }

                //push decoded value to cb
                cb_push(cb_decode, *decoded_value);
                if(cb_is_full(cb_decode)){
                    uint32_t cb_avg=1;
                    //call average function
                    cb_average(cb_decode, &cb_avg);
                    //cout << "\tAVERAGE: " << cb_avg << endl;
                    codec_alg.change_m_decode(calc_m(cb_avg));
                    //cout << "\tNEW M: " << codec_alg.get_m_decode()<< endl;
                    //cb_print(cb_decode);
                    cb_clear(cb_decode);
                }

               //unmap decoded error value
                if(*decoded_value % 2 == 0){
                    //-(result/2)
                    *unmapped_value = -((*decoded_value)/2);
                }else{
                    //(result+1)/2
                    *unmapped_value = (*decoded_value-1)/2;
                }
                //push unmapped error value to vector
                decoded_error_samples.push_back(*unmapped_value);
                //print iteration
                if(i%1000000 == 0){
                    cout << "Iteration: " << ++i << endl;
                }
            }

            vector<short> decoded_samples;                          //vector to store original samples
            decoded_samples.resize(samples_size);   //resize samples vector to fit all samples

            //after unmapping all values, calculate original samples 
            //reconstruct original value of first order*num_channels samples
            for(uint32_t i=0; i<order*num_channels; i++){
                decoded_samples[i] = decoded_error_samples[i];
            }
            //reconstruct original value of rest of samples
            for(uint32_t i=order*num_channels; i<decoded_samples.size(); i++){
                decoded_samples[i] = decoded_error_samples[i] + calculate_prediction(order, decoded_samples[i-num_channels], decoded_samples[i-(2*num_channels)], decoded_samples[i-(3*num_channels)]);
            }

            //write all samples to file
            ofstream file;
            file.open("RECONSTRUCTED_SAMPLES.txt");
            for (uint32_t i = 0; i < decoded_samples.size(); i++){
                file << decoded_samples[i] << endl;
            }
            file.close();

            //write decoded samples to wav file
            cout << "Writing to file: " << fileOut << endl;
            write_wav_file(fileOut, decoded_samples);

            //print execution time
            time_req = clock() - time_req;
            cout << "Execution time: " << (float)time_req/CLOCKS_PER_SEC << " seconds" << endl;
            return 0;
        }

};

#endif