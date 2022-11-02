
#include <stdio.h>
#include <opencv2/opencv.hpp>
#include <iostream>

using namespace cv;
using namespace std;

//Using the OpenCV library, implement a program to copy an image, pixel by pixel, from one file
//to another file. Both file names should be passed as command line arguments to the program.

int main(int argc, char** argv )
{
    if ( argc != 4 )
    {
        printf("usage: DisplayImage.out <Image_Path> <Image_Path> <inc/decr>\n");
        return -1;
    }

    Mat image;
    image = imread( argv[1], 1 );

    if ( !image.data )
    {
        printf("No image data \n");
        return -1;
    }

    Mat image3(image.rows, image.cols, CV_8UC3, Scalar(0,0,0));

    //if the third argument is "inc", then increase the brightness of the image by 50
    if(strcmp(argv[3], "inc") == 0)
    {
        for(int i = 0; i < image.rows; i++)
        {
            for(int j = 0; j < image.cols; j++)
            {
                //increase the brightness of the image by 50
                image3.at<Vec3b>(i,j)[0] = image.at<Vec3b>(i,j)[0] + 50;
                image3.at<Vec3b>(i,j)[1] = image.at<Vec3b>(i,j)[1] + 50;
                image3.at<Vec3b>(i,j)[2] = image.at<Vec3b>(i,j)[2] + 50;
            }
        }
    }
    //if the third argument is "decr", then decrease the brightness of the image by 50
    else if(strcmp(argv[3], "decr") == 0)
    {
        for(int i = 0; i < image.rows; i++)
        {
            for(int j = 0; j < image.cols; j++)
            {
                //decrease the brightness of the image by 50
                image3.at<Vec3b>(i,j)[0] = image.at<Vec3b>(i,j)[0] - 50;
                image3.at<Vec3b>(i,j)[1] = image.at<Vec3b>(i,j)[1] - 50;
                image3.at<Vec3b>(i,j)[2] = image.at<Vec3b>(i,j)[2] - 50;
            }
        }
    }
    else
    {
        printf("usage: DisplayImage.out <Image_Path> <Image_Path> <inc/decr>\n");
        return -1;
    }

    //write the image to the second file
    imwrite(argv[2], image3);

    waitKey(0);

    return 0;
}