
#include <opencv2/opencv.hpp>
#include <iostream>
#include <stdio.h>
using namespace std; // std dont need
int main(){
    // Read image and put in black and white
    cv::Mat image = cv::imread("../Images/airplane.ppm", 0);
    // Check if image is loaded
    if(!image.data){
        cerr << "Image not loaded" << endl;
        return -1;
    }
    // Create window
    cv::namedWindow("Display window", cv::WINDOW_AUTOSIZE);
    // Show image
    cv::imshow("Display window", image);
    // Wait for key press
    cv::waitKey(0);
    return 0;

}