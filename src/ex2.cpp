#include <opencv2/opencv.hpp>
#include <iostream>
#include <stdio.h>


using namespace std; // std dont need

int ex2_a(string InputFileName, string OutputFileName){
    // Read image and put in black and white
    cv::Mat image = cv::imread(InputFileName, 0);
    // Check if image is loaded
    if(!image.data){
        cerr << "Image not loaded" << endl;
        return -1;
    }

    // save image to OutputFileName
    cv::imwrite(OutputFileName, image);

    return 1;
}

int main(int argc, char** argv){
    // arguments -a -b -c -d

    // You just can have one argument of each type (a, b, c, d)
    
    // Check if there is 4 arguments
    if(argc != 4){
        cerr << "a../bin/ex2 [ -a | -b | -c | -d ] <ImageIn> <ImageOut>" << endl;
        return 0;
    }
    
    // -a  EX2_A
    if (strcmp(argv[1], "-a") == 0){
        string InputFileName = argv[2];
        string OutputFileName = argv[3];
        ex2_a(InputFileName,OutputFileName); //"../Images/airplane.ppm"
    }
    else if (strcmp(argv[1], "-b") == 0){
        cout << "baboon" << endl;
    }
    else if (strcmp(argv[1], "-c") == 0){
        cout << "cameraman" << endl;
    }
    else if (strcmp(argv[1], "-d") == 0){
        cout << "lena" << endl;
    }
    else{
        cerr << "b../bin/ex2 [ -a | -b | -c | -d ] <ImageIn> <ImageOut>" << endl;
        return 0;
    }
    // if we have argument -b

}