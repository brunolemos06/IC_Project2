
#include <stdio.h>
#include <opencv2/opencv.hpp>
#include <iostream>

using namespace cv;
using namespace std;

//Using the OpenCV library, implement a program to copy an image, pixel by pixel, from one file
//to another file. Both file names should be passed as command line arguments to the program.

int main(int argc, char** argv )
{
    if ( argc != 3 )
    {
        printf("usage: DisplayImage.out <Image_Path> <Image_Path>\n");
        return -1;
    }

    Mat image;
    image = imread( argv[1], 1 );

    if ( !image.data )
    {
        printf("No image data \n");
        return -1;
    }

    Mat image2;
    image2 = imread( argv[2], 1 );

    if ( !image2.data )
    {
        printf("No image data2 \n");
        return -1;
    }

    Mat image3(image.rows, image.cols, CV_8UC3, Scalar(0,0,0));

    for(int i = 0; i < image.rows; i++)
    {
        for(int j = 0; j < image.cols; j++)
        {
            //mirror the image vertically
            image3.at<Vec3b>(i,j)[0] = image.at<Vec3b>(image.rows - i - 1,j)[0];
            image3.at<Vec3b>(i,j)[1] = image.at<Vec3b>(image.rows - i - 1,j)[1];
            image3.at<Vec3b>(i,j)[2] = image.at<Vec3b>(image.rows - i - 1,j)[2];            
        }
    }

    //write the image to the second file
    imwrite(argv[2], image3);
    namedWindow("Display Image", WINDOW_AUTOSIZE );
    imshow("Display Image", image3);

    waitKey(0);

    return 0;
}