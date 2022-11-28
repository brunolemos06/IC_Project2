#ifndef CODEC_GOLOMB_H
#define CODEC_GOLOMB_H

#include <bitset>
#include <stdio.h>
#include <stdlib.h>
#include <bits/stdc++.h>
#include <fstream>
#include <sndfile.hh>

#include "golomb.h"

using namespace std;

constexpr size_t FRAMES_BUFFER_SIZE = 65536; // Buffer for reading/writing frames

uint32_t samples_size = 0;
size_t frames=0;
int format=0, samplerate=0;

class golomb_codec{
    private:
        golomb codec_alg;
        char buffer = 0;
        int count=0, num_channels=0;
        int x, y, order;

        int write_wav_file(const char* fileOut, vector<short> &samples){
            //create output file
            SndfileHandle sfhOut(fileOut, SFM_WRITE, format, num_channels, samplerate);
            
            //print samples
            uint32_t tmp = sfhOut.write(samples.data(), samples.size());
            cout << "Samples written: " << tmp << endl;
            return 0;
        }
        
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

        uint32_t calc_m(double avg){
            double alfa = avg / (avg + 1);
            return ceil(-1/log2(alfa));
        }

    public:
        golomb_codec(int n_order, int x_in, int y_in): codec_alg() { order = n_order; x = x_in; y = y_in; };

        int encode_wav_file(const char* fileIn, const char* fileOut){
            clock_t time_req;                           //for time measurement
            time_req = clock();                         //start time measurement

            vector<short> all_samples;  //vector to store all samples

            SndfileHandle sfhIn { fileIn };
            if(sfhIn.error()) {
		        cerr << "Error: invalid input file\n";
		        return {};
            }

            if((sfhIn.format() & SF_FORMAT_TYPEMASK) != SF_FORMAT_WAV) {
		        cerr << "Error: file is not in WAV format\n";
		        return {};
            }

	        if((sfhIn.format() & SF_FORMAT_SUBMASK) != SF_FORMAT_PCM_16) {
		        cerr << "Error: file is not in PCM_16 format\n";
		        return {};
	        }

            size_t nFrames;
            vector<short> tmp_samples(FRAMES_BUFFER_SIZE * sfhIn.channels());

            //details to be used in write_wav_file
            frames = { static_cast<size_t>(sfhIn.frames()) };
            samplerate = { static_cast<int>(sfhIn.samplerate()) };
            format = { static_cast<int>(sfhIn.format()) };
            num_channels = sfhIn.channels();

            while((nFrames = sfhIn.readf(tmp_samples.data(), FRAMES_BUFFER_SIZE))) {
                tmp_samples.resize(nFrames * sfhIn.channels());
                samples_size += tmp_samples.size();

                //add samples to all_samples
                for( auto &sample : tmp_samples){
                    all_samples.push_back(sample);
                }
                //all_samples.insert(all_samples.end(), samples.begin(), samples.end());
                
                //clear samples
                //samples.clear();
                //samples.shrink_to_fit();
                
                vector<short>().swap(tmp_samples);
                //resize samples
                tmp_samples.resize(FRAMES_BUFFER_SIZE * sfhIn.channels());

            }

            //cout << "SIZE: " << samples_size << endl;

            vector<uint32_t> mapped_samples;            //mapp predicted values
            mapped_samples.resize(samples_size);        //resize vector to fit all mapped error values

            string encoded = "";

            //FRIST ORDER*NUM_CHANNELS SAMPLES
            uint32_t med = 0;
            for(uint32_t i=0; i<order*num_channels; i++){
                //calculate errors  aka error_values[i] = all_samples[i] - 0              
                //map errors
                if(all_samples[i] < 0){
                    mapped_samples[i] = (all_samples[i] * -2);
                } else {
                    mapped_samples[i] = (all_samples[i] * 2) + 1;
                }
                med += mapped_samples[i];
            }
            //calculate initial m
            codec_alg.change_m_encode(med/(order*num_channels));
            codec_alg.change_m_decode(med/(order*num_channels));
            //cout << "INITIAL M: " << codec_alg.get_m_encode() << endl;
            //encode first order*channels samples
            for(uint32_t i=0; i<order*num_channels; i++){
                encoded += codec_alg.encode_number(mapped_samples[i], 0);
            }

            //FOR THE REST OF THE SAMPLES       
            long tmp_error = 0;  
            for(uint32_t i=order*num_channels; i<all_samples.size(); i++){
                //0 2 4 6 ... left channel and 1 3 5 7 ... right channel in case DUAL CHANNEL
                if(order == 3){
                    //xˆ(3)n = 3xn−1 − 3xn−2 + xn−3
                    tmp_error = all_samples[i] - ( (3*all_samples[i-num_channels]) - ( 3*all_samples[i-(2*num_channels)]) + (all_samples[i-(3*num_channels)]) );
                }else if(order == 2){
                    //xˆ(2)n = 2xn−1 − xn−2
                    tmp_error = all_samples[i] - ( (2*all_samples[i-num_channels]) - all_samples[i-(2*num_channels)] ); 
                }

                //map tmp_error
                if(tmp_error < 0){
                    mapped_samples[i] = (tmp_error * -2);
                } else {
                    mapped_samples[i] = (tmp_error * 2) + 1;
                }
                //encode mapped error
                encoded += codec_alg.encode_number(mapped_samples[i], 0);

                //update m to encode
                if(i % x == 0){ //every x samples  
                    med = 0;
                    for(uint32_t j=i - y; j<i; j++){ //calculate average of last y samples
                        med += mapped_samples[j];
                    }
                    codec_alg.change_m_encode(calc_m(med/x));
                }

            }

            //ERRORS CHECK
            //int error = 0;
            //print samples , error values and mapped error values
            // for(uint32_t i=0; i<mapped_samples.size(); i++){
            //     if(mapped_samples[i] == 1){
            //         //cout << "\033[1;31m( "<< i <<" ) SAMPLE: " << samples[i] << "\tERROR: " << calculate_prediction(order, samples[i-num_channels], samples[i-(2*num_channels)], samples[i-(3*num_channels)])<< "\tPREDICTED: " << predicted_values[i] << "\tMAPPED: \033[0m" << mapped_samples[i] << endl;
            //     }else if(mapped_samples[i] < 0){
            //         cout << "\033[1;31m( "<< i <<" ) SAMPLE: " << samples[i] << "\tPRED: " << calculate_prediction(order, samples[i-num_channels], samples[i-(2*num_channels)], samples[i-(3*num_channels)])<< "\tERROR: " << error_values[i] << "\tMAPPED: " << mapped_samples[i]<< "\033[0m" << endl;
            //         //print calculated error
            //         cout << "MATH.: "<< samples[i] << " - ((3*"<<samples[i-num_channels]<<") - (3*"<< samples[i-(2*num_channels)]<< ") + "<< samples[i-(3*num_channels)] << ")" << endl;
            //         error++;
            //     }else{
            //         //cout << "( "<< i <<" ) SAMPLE: " << samples[i] << "      \tPREDICTED: " << predicted_values[i] << "      \tMAPPED: " << mapped_samples[i] << endl;
            //     }
            // }
            // cout << "\033[1;31mERRORS: " << error << "\033[0m"<<endl;

            cout << "Encoded size in bytes: " << (double)encoded.size()/8 << endl;
            cout << "Encoded size in Mbytes: " << (double)encoded.size()/8/1024/1024 << endl;


            //write encoded string to file
            cout << "Writing to file: " << fileOut << endl;
            write_bin_to_file(fileOut, encoded);
            //print execution time
            time_req = clock() - time_req;
            cout << "Execution time: " << (float)time_req/CLOCKS_PER_SEC << " seconds\n" << endl;
            return 0;
        }

        int decode_to_wav(const char* fileIn, const char* fileOut){
            //start clock
            clock_t time_req;
            time_req = clock();

            //read encoded file
            string encoded = read_bin_from_file(fileIn);
            //pointer to encoded string
            char *p = &encoded[0];

            //print encoded size in bytes
            cout << "Encoded size in bytes: " << (double)encoded.size()/8 << endl;
            cout << "Encoded size in Mbytes: " << (double)encoded.size()/8/1024/1024 << endl;

            long * tmp_val = (long*)malloc(sizeof(long));    //pointer to store decoded number

            vector<uint32_t> mapped_samples;                //vector to store mapped samples
            vector<short> decoded_samples;                  //vector to store decoded samples

            uint32_t i=0;
            //FIRST ORDER*CHANNELS SAMPLES
            for(; i<order*num_channels; i++){
                //decode first order*channels samples
                p = codec_alg.decode_string(p, tmp_val, 0);
                mapped_samples.push_back(*tmp_val);
                //unmap decoded value
                if(*tmp_val % 2 == 0){
                    *tmp_val = ( (*tmp_val) / (-2));
                } else {
                    *tmp_val = ((*tmp_val - 1) / 2);
                }
                //add unmapped value to decoded_samples
                decoded_samples.push_back((*tmp_val));
            }

            //decode rest of samples
            uint32_t med=0;
            //check if pointer is not at the end of encoded string aka char '\0'
            while( *p != '\0'){
                //string decode_string(string bits, uint32_t *result_n, int mapping_on)
                p = codec_alg.decode_string(p, tmp_val, 0);
                mapped_samples.push_back(*tmp_val);
                //update m to decode
                if(i % x == 0){     //if(1){    
                    med = 0;
                    for(uint32_t j=i - y; j<i; j++){
                        med += mapped_samples[j];
                    }
                    codec_alg.change_m_decode(calc_m(med/x));
                }

                //unmap decoded value
                if(*tmp_val % 2 == 0){
                    *tmp_val = ( (*tmp_val) / (-2));
                } else {
                    *tmp_val = ((*tmp_val - 1) / 2);
                }
                //add unmapped value to decoded_samples
                if(order == 3){
                    //xˆ(3)n = 3xn−1 − 3xn−2 + xn−3
                    decoded_samples.push_back( ((short)(*tmp_val)) + ( (3*decoded_samples[i-num_channels]) - ( 3*decoded_samples[i-(2*num_channels)]) + (decoded_samples[i-(3*num_channels)]) ) );
                }else if(order == 2){
                    //xˆ(2)n = 2xn−1 − xn−2
                    decoded_samples.push_back( ((short)(*tmp_val)) + ( (2*decoded_samples[i-num_channels]) - (decoded_samples[i-(2*num_channels)]) ) );
                }
                i++;
            }

            //remove last sample
            decoded_samples.pop_back();

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