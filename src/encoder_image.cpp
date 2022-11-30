// Implement a lossless image codec, based on Golomb coding.

#include <stdio.h>
#include <opencv2/opencv.hpp>
#include <iostream>
#include "codec_image.h"

using namespace cv;
using namespace std;

//Using the OpenCV library, implement a program to copy an image, pixel by pixel, from one file
//to another file. Both file names should be passed as command line arguments to the program.

int main(int argc, char** argv ){

    if ( argc != 6 ){
        printf("usage: encoder_image <src.ppm> <out.txt> <order>[1..8]\n");
        return -1;
    }
    //  check the size of the image
    try{
        image_codec codec(atoi(argv[3]),atoi(argv[4]),atoi(argv[5]));
        codec.encode_image_file(argv[1], argv[2]);
    }
    catch (Exception e){
        printf("usage: encoder_image <src.ppm> <out.txt> <order>[1..8]\n");
    }
}