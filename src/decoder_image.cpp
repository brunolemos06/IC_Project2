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

    if ( argc != 3 ){
        printf("usage: wrong\n");
        return -1;
    }
    //  check the size of the image
    
    image_codec codec(8,2500,2500);
    codec.decode_image_file(argv[1], argv[2]);
    // codec.encode_image_file(argv[1], argv[2]);
}