#ifndef CODEC_GOLOMB_H
#define CODEC_GOLOMB_H

#include <bitset>
#include <stdio.h>
#include <stdlib.h>
#include <bits/stdc++.h>
#include <fstream>
#include <sndfile.hh>

#include "golomb.h"
#include "circularBuffer.h"

using namespace std;

constexpr size_t FRAMES_BUFFER_SIZE = 65536; // Buffer for reading/writing frames

uint32_t samples_size = 0;
size_t frames=0;
int format=0, samplerate=0;

class golomb_codec{
    private:
        golomb codec_alg;
        circular_buffer *cb_encode;
        circular_buffer *cb_decode;
        char buffer;
        int count, num_channels, order;

        int write_wav_file(const char* fileOut, vector<short> &samples){
            //create output file
            SndfileHandle sfhOut(fileOut, SFM_WRITE, format, num_channels, samplerate);
            
            //print samples
            cout << "!Samples: "<< samples.size() << endl;
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

        int calculate_prediction(int order, int prev_1, int prev_2, int prev_3){
            if(order == 3){
                //xˆ(3)n = 3xn−1 − 3xn−2 + xn−3
                return ((3*prev_1) - (3*prev_2) + prev_3);
            }else if(order==2){
                //xˆ(2)n = 2xn−1 − xn−2
                return ((2*prev_1) - prev_2);
            }else if(order==1){
                //xˆ(1)n = xn−1
                return  prev_1;
            }
            return -1;
        }

        uint32_t calc_m(double avg){
            double alfa = avg / (avg + 1);
            return ceil(-1/log2(alfa));
        }

    public:
        golomb_codec(int n_order) : codec_alg(){ buffer=0; count=0; order=n_order; num_channels=0; cb_encode = cb_init(50); cb_decode = cb_init(50);};
        golomb_codec() : codec_alg() {buffer=0; count=0; order=3; num_channels=2; cb_encode = cb_init(50); cb_decode = cb_init(50);};

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

            cout << "SIZE: " << samples_size << endl;

            ofstream file;
            file.open("ORIGINAL_SAMPLES.txt");
            for (uint32_t i = 0; i < all_samples.size(); i++){
                file << all_samples[i] << endl;
            }
            file.close();

            
            vector<short> error_values;                 //error values
            error_values.resize(samples_size);          //resize vector to fit all error values


            vector<uint32_t> mapped_samples;            //mapp predicted values
            mapped_samples.resize(samples_size);        //resize vector to fit all mapped error values

            string encoded = "";

            //for initial order*num_channels samples
            //calculate error values, map them, calculate initial m and encode them
            uint32_t med = 0;
            uint32_t tmp_val = 0;
            for(uint32_t i=0; i<order*num_channels; i++){
                error_values[i] = all_samples[i];
                if(all_samples[i] < 0){
                    tmp_val = (all_samples[i] * -2);
                }else{
                    tmp_val = (all_samples[i] * 2) + 1;
                }
                med += tmp_val;
                encoded += codec_alg.encode_number(tmp_val, 0);

            }
            codec_alg.change_m_encode(med/(order*num_channels));
            codec_alg.change_m_decode(med/(order*num_channels));
            //cout << "INITIAL M: " << codec_alg.get_m_encode() << endl;

            //for the rest of the samples
            //calculate error values, map them and encode them        
            for(uint32_t i=order*num_channels; i<all_samples.size(); i++){
                //0 2 4 6 ... left channel and 1 3 5 7 ... right channel in case DUAL CHANNEL
                error_values[i] = (all_samples[i] - calculate_prediction(order, all_samples[i-num_channels], all_samples[i-(2*num_channels)], all_samples[i-(3*num_channels)]));
                if(error_values[i] < 0){
                    tmp_val = (error_values[i] * -2);
                } else {
                    tmp_val = (error_values[i] * 2) + 1;
                }

                encoded += codec_alg.encode_number(tmp_val, 0);
                
                cb_push(cb_encode, tmp_val);
                if(cb_is_full(cb_encode)){
                    uint32_t cb_avg=1;
                    cb_average(cb_encode, &cb_avg);
                    codec_alg.change_m_encode(calc_m(cb_avg));
                    //cout << "\tNEW M: " << codec_alg.get_m_encode()<< endl;
                    cb_clear(cb_encode);
                }
                //print original and mapped values
                //cout << i <<"   ERROR: " << error_values[i] << " MAPPED: " << mapped_samples[i] << endl;
            }

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
            
            uint32_t *decoded_value;                        //variable to store decoded value
            decoded_value = (uint32_t*)malloc(sizeof(uint32_t));    //initialize decoded_value
            
            uint32_t *unmapped_value;                       //variable to store mapped value
            unmapped_value = (uint32_t*)malloc(sizeof(uint32_t));   //initialize unmapped_value

            //vector<short> decoded_error_samples;            //vector to store decoded error samples

            vector<short> decoded_samples;                          //vector to store original samples
            decoded_samples.resize(samples_size);   //resize samples vector to fit all samples

            //decode encoded file
            //decode first order*num_channels samples
            for(uint32_t i=0; i<order*num_channels; i++){
                encoded = codec_alg.decode_string(encoded, decoded_value, 0);

                //unmap decoded error value
                if(*decoded_value % 2 == 0){
                    *unmapped_value = -((*decoded_value)/2);
                }else{
                    *unmapped_value = (*decoded_value-1)/2;
                }
                //push unmapped error value to vector
                //decoded_error_samples.push_back(*unmapped_value);
                decoded_samples[i] = *unmapped_value;
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

                decoded_samples[i] = *unmapped_value + calculate_prediction(order, decoded_samples[i-num_channels], decoded_samples[i-(2*num_channels)], decoded_samples[i-(3*num_channels)]);
                //print iteration
                if(i%1000 == 0){
                    cout << "Iteration: " << ++i << endl;
                }
            }

            //reconstruct original value of rest of samples
            // for(uint32_t i=order*num_channels; i<decoded_samples.size(); i++){
            //     decoded_samples[i] = decoded_error_samples[i] + calculate_prediction(order, decoded_samples[i-num_channels], decoded_samples[i-(2*num_channels)], decoded_samples[i-(3*num_channels)]);
            // }

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