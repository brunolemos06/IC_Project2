
#include <stdio.h>
#include <opencv2/opencv.hpp>
#include <iostream>

using namespace cv;
using namespace std;

//Using the OpenCV library, implement a program to copy an image, pixel by pixel, from one file
//to another file but rotated by a multiple of 90 degrees. Both file names should be passed as command line arguments to the program.

int main(int argc, char** argv )
{
    if ( argc != 4 )
    {
        printf("usage: DisplayImage.out <Image_Path> <Image_Path> <degrees>\n");
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

    //rotate by the number of degrees passed in
    int degrees = atoi(argv[3]);
    if(degrees == 90)
    {
        for(int i = 0; i < image.rows; i++)
        {
            for(int j = 0; j < image.cols; j++)
            {
                //rotate the image 90 degrees
                image3.at<Vec3b>(i,j)[0] = image.at<Vec3b>(j,image.cols - i - 1)[0];
                image3.at<Vec3b>(i,j)[1] = image.at<Vec3b>(j,image.cols - i - 1)[1];
                image3.at<Vec3b>(i,j)[2] = image.at<Vec3b>(j,image.cols - i - 1)[2];            
            }
        }
    }
    else if(degrees == 180)
    {
        for(int i = 0; i < image.rows; i++)
        {
            for(int j = 0; j < image.cols; j++)
            {
                //rotate the image 180 degrees
                image3.at<Vec3b>(i,j)[0] = image.at<Vec3b>(image.rows - i - 1,image.cols - j - 1)[0];
                image3.at<Vec3b>(i,j)[1] = image.at<Vec3b>(image.rows - i - 1,image.cols - j - 1)[1];
                image3.at<Vec3b>(i,j)[2] = image.at<Vec3b>(image.rows - i - 1,image.cols - j - 1)[2];            
            }
        }
    }
    else if(degrees == 270)
    {
        for(int i = 0; i < image.rows; i++)
        {
            for(int j = 0; j < image.cols; j++)
            {
                //rotate the image 270 degrees
                image3.at<Vec3b>(i,j)[0] = image.at<Vec3b>(image.rows - j - 1,i)[0];
                image3.at<Vec3b>(i,j)[1] = image.at<Vec3b>(image.rows - j - 1,i)[1];
                image3.at<Vec3b>(i,j)[2] = image.at<Vec3b>(image.rows - j - 1,i)[2];            
            }
        }
    }
    else
    {
        printf("Invalid number of degrees. Must be 90, 180, or 270.\n");
        return -1;
    }

    //write image into a new file with the name of the second file passed in
    imwrite(argv[2], image3);

    waitKey(0);

    return 0;
}

