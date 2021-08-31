/*
 * @Description: 
 * @Version: 
 * @Author: William
 * @Date: 2021-06-12 14:44:46
 * @LastEditors: William
 * @LastEditTime: 2021-08-31 11:05:29
 */

#include "GHT.hpp"
#include <opencv2/opencv.hpp>
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
#include <fstream>
#include "RTable.hpp"
#include "Voter.hpp"

using namespace std;
using namespace cv;

void GHT::detect(Mat Template_Image, Mat Testing_Image, int interval)
{
    TestImg = Testing_Image;
    TempImg = Template_Image;
    intervals = interval;

    blur(Testing_Image,Testing_Image,Size(3,3));

    // offline stage
    RTable C_RTable = RTable();
    C_RTable.generate(Template_Image, intervals);
    C_RTable.inspectTable();
    r_table = C_RTable.getRTable();

    CannyDetectEdge();

    cout<<"Finished model building"<<endl;
    waitKey(0);


    // online stage

    Voter C_voter = Voter(Testing_Image.cols * Testing_Image.rows * 360 * 16);
 /*     Mat mask;
    mask = C_voter.vote(EdgeImage,gradientMat,r_table,intervals);

    Mat copy = TestImg.clone();
    for(int i = 0; i < TestImg.cols; i++)
    {
        for(int j = 0; j < TestImg.rows; j++)
        {
            if(mask.at<unsigned char>(j,i) == 255)
            {
                circle (copy, Point(i,j),  1, Scalar(0,0,255),2,8,0);
            }
        }
    }
    imshow("result", copy);
    waitKey(0); */

 // Conventional GHT
    vector<Vec4i> maxCells = C_voter.vote_CGHT(EdgeImage,gradientMat,r_table,intervals);

    Mat copy = TestImg.clone();
    for(vector<Vec4i>::iterator iter = maxCells.begin(); iter!= maxCells.end(); iter++)
    {
        circle (copy, Point((*iter)(0),(*iter)(1)), 1, Scalar(0,0,255),2,8,0);
        putText(copy, format("( %d, %d ) ", (*iter)(0), (*iter)(1)), Point((*iter)(0) + 2, (*iter)(1) + 5),
            cv::FONT_HERSHEY_PLAIN, 0.8, cv::Scalar(0, 0, 255));
        putText(copy, format("scale: %0.2f ",((*iter)(3) + 5) / 10.0), Point((*iter)(0) + 2, (*iter)(1) + 15),
            cv::FONT_HERSHEY_PLAIN, 0.8, cv::Scalar(0, 0, 255), true);
        putText(copy, format("angle: %d ", (*iter)(2)), Point((*iter)(0) + 2, (*iter)(1) + 25),
            cv::FONT_HERSHEY_PLAIN, 0.8, cv::Scalar(0, 0, 255), true);
        
        cout<<"("<<(*iter)(0)<<","<<(*iter)(1)<<"); Angle: "<<(*iter)(2)<<"degree; Scale:"<<((*iter)(3) + 5) / 10.0<<endl;
    }
    imshow("result", copy);
    waitKey(0); 
    
}