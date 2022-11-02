#include <opencv2/opencv.hpp>
#include <iostream>
#include <stdio.h>


using namespace std; // std dont need
using namespace cv; // cv dont need

int ex2_ba(string InputFileName, string OutputFileName){
    Mat image;
    image = imread( InputFileName, 1 );

    if ( !image.data )
    {
        printf("No image data \n");
        return -1;
    }

    Mat image2;
    image2 = imread( OutputFileName, 1 );

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
            //mirror the image horizontally
            image3.at<Vec3b>(i,j)[0] = image.at<Vec3b>(i,image.cols-j-1)[0];
            image3.at<Vec3b>(i,j)[1] = image.at<Vec3b>(i,image.cols-j-1)[1];
            image3.at<Vec3b>(i,j)[2] = image.at<Vec3b>(i,image.cols-j-1)[2];
        }
    }

    //write the image to the second file
    imwrite(OutputFileName, image3);
    namedWindow("Display Image", WINDOW_AUTOSIZE );
    imshow("Display Image", image3);

    waitKey(0);

    return 0;
}

int ex2_a(string InputFileName, string OutputFileName){
    Mat image;
    image = imread( InputFileName, 1 );
    if ( !image.data )
    {
        printf("No image data \n");
        return -1;
    }

    Mat image2;
    image2 = imread( OutputFileName, 1 );

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
            //  black and white
            image3.at<Vec3b>(i,j)[0] = (image.at<Vec3b>(i,j)[0] + image.at<Vec3b>(i,j)[1] + image.at<Vec3b>(i,j)[2])/3;
            image3.at<Vec3b>(i,j)[1] = (image.at<Vec3b>(i,j)[0] + image.at<Vec3b>(i,j)[1] + image.at<Vec3b>(i,j)[2])/3;
            image3.at<Vec3b>(i,j)[2] = (image.at<Vec3b>(i,j)[0] + image.at<Vec3b>(i,j)[1] + image.at<Vec3b>(i,j)[2])/3;
        }
    }
    //write the image to the second file
    imwrite(OutputFileName, image3);
    namedWindow("Display Image", WINDOW_AUTOSIZE );
    imshow("Display Image", image3);

    waitKey(0);

    return 0;

}

int ex2_bb(string InputFileName, string OutputFileName){
    Mat image;
    image = imread( InputFileName, 1 );

    if ( !image.data )
    {
        printf("No image data \n");
        return -1;
    }

    Mat image2;
    image2 = imread( OutputFileName, 1 );

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
    imwrite(OutputFileName, image3);
    namedWindow("Display Image", WINDOW_AUTOSIZE );
    imshow("Display Image", image3);

    waitKey(0);

    return 0;
}

int main(int argc, char** argv){
    // arguments -a -b -c -d

    // You just can have one argument of each type (a, b, c, d)
    
    // Check if there is 4 arguments
    if(argc != 4){
        cerr << "a../bin/ex2 [ -a | -ba | -bb | -c | -d ] <ImageIn> <ImageOut>" << endl;
        return 0;
    }
    // input file name and output file name
    string InputFileName = argv[2];
    string OutputFileName = argv[3];

    // -a  EX2_A
    if (strcmp(argv[1], "-a") == 0){
        cout << "Processing ex2 a)\nInputFile: " + InputFileName + "\nOutputFile: " + OutputFileName << endl;
        ex2_a(InputFileName,OutputFileName); //"../Images/airplane.ppm"
        cout << "Done !! "<< endl;
    } // -b EX2_BA
    else if (strcmp(argv[1], "-ba") == 0){
        cout << "Processing ex2 b)\nInputFile: " + InputFileName + "\nOutputFile: " + OutputFileName << endl;
        ex2_ba(InputFileName,OutputFileName); //"../Images/airplane.ppm"
        cout << "Done !! "<< endl;
    } // -b EX2_BB
    else if (strcmp(argv[1], "-bb") == 0){
        cout << "Processing ex2 b)\nInputFile: " + InputFileName + "\nOutputFile: " + OutputFileName << endl;
        ex2_bb(InputFileName,OutputFileName); //"../Images/airplane.ppm"
        cout << "Done !! "<< endl;
    } // -c EX2_C
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