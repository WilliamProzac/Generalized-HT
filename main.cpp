/*
 * @Description: 
 * @Version: 
 * @Author: William
 * @Date: 2021-06-01 23:18:44
 * @LastEditors: William
 * @LastEditTime: 2021-08-31 11:33:12
 */


#include "RTable.hpp"
#include "Voter.hpp"
#include "GHT.hpp"
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/core/core.hpp>
//#include <vector>
//#include <iostream>
//#include <stdio.h>
//#include <string>
//#include <thread>

using namespace cv;
using namespace std;

int main()
{
    int intervals = 360;
    Mat tempImg;
    Mat tempImg_conner;
    Mat TestImg_input;
    Mat TestImg;

    tempImg = imread("../Dataset_generator/templates/apple-4.png",1);
    //rotate(tempImg,tempImg,ROTATE_90_CLOCKWISE);
    imshow("Template",tempImg); 
    waitKey(0);
    /*
    Mat tempImg_gary = tempImg.clone();
    cvtColor(tempImg,tempImg_gary,COLOR_BGR2GRAY);
    imshow("Template_gary",tempImg_gary); 
    waitKey(0);
    tempImg_conner = tempImg.clone();
    cornerHarris(tempImg_gary,tempImg_conner,2,3,0.04,BORDER_DEFAULT);

    Mat normalized_corner = tempImg_conner.clone();
    normalize(tempImg_conner, normalized_corner, 0, 255, NORM_MINMAX, CV_8UC1); 
    imshow("Template_corner", normalized_corner);
   waitKey(0); 


*/ 
    TestImg_input = imread("../Dataset_generator/Results/(115,200);degree10;scale110.png",1);

    //rotate(TestImg_input,TestImg,ROTATE_90_CLOCKWISE);
    

    
    imshow("Test",TestImg_input); 
    waitKey(0);

    GHT Simplified_GHT = GHT();
    Simplified_GHT.detect(tempImg, TestImg_input, intervals);     

    return 0;
}