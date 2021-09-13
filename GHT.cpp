/*
 * @Description: 
 * @Version: 
 * @Author: William
 * @Date: 2021-06-12 14:44:46
 * @LastEditors: William
 * @LastEditTime: 2021-09-01 17:32:46
 */
/*
 * @Description: 
 * @Version: 
 * @Author: William
 * @Date: 2021-06-12 14:44:46
 * @LastEditors: William
 * @LastEditTime: 2021-09-01 16:15:33
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

void GHT::detect(Mat Template_Image, int interval, vector<string> &filepaths)
{

    TempImg = Template_Image;
    intervals = interval; 

    // offline stage
    RTable C_RTable = RTable();
    C_RTable.generate(Template_Image, intervals);
    C_RTable.inspectTable();
    r_table = C_RTable.getRTable();

    cout<<"Finished model building"<<endl;
    waitKey(0);

    int old_top = 0;
    int old_left = 0;
    int old_bottom = 0;
    int old_right = 0;
    int old_angle_min = 0;
    int old_angle_max = 0;
    int old_scale_min = 0;
    int old_scale_max = 0;

    size_t cnt = filepaths.size();
    for(size_t i = 0; i < cnt; i++)
    {

        double t1 = cv::getTickCount();
        Mat Testing_Image = imread(filepaths[i]);
        imshow("Testing",Testing_Image);
        //waitKey(0);



        TestImg = Testing_Image;
        blur(Testing_Image,Testing_Image,Size(3,3));
        CannyDetectEdge();      // 对测试图像进行预处理


        // online stage
        Voter C_voter = Voter(1);
        
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
         vector<Vec4i> maxCells;
        if(i==0)  maxCells = C_voter.vote_CGHT(EdgeImage,r_table,0,Testing_Image.cols-1,Testing_Image.rows-1,0,0,181,5,20);        
        else maxCells = C_voter.vote_CGHT(EdgeImage,r_table,old_top,old_right,old_bottom,old_left,old_angle_min,old_angle_max,old_scale_min,old_scale_max);

        C_voter.~Voter();

        Mat copy = TestImg.clone();
        for(vector<Vec4i>::iterator iter = maxCells.begin(); iter!= maxCells.end(); iter++)
        {
            circle (copy, Point((*iter)(0),(*iter)(1)), 1, Scalar(0,0,255),2,8,0);
            putText(copy, format("( %d, %d ) ", (*iter)(0), (*iter)(1)), Point((*iter)(0) + 2, (*iter)(1) + 5),
                cv::FONT_HERSHEY_PLAIN, 0.8, cv::Scalar(0, 0, 255));
            putText(copy, format("scale: %0.2f ",((*iter)(3)) / 10.0), Point((*iter)(0) + 2, (*iter)(1) + 15),
                cv::FONT_HERSHEY_PLAIN, 0.8, cv::Scalar(0, 0, 255), true);
            putText(copy, format("angle: %d ", (*iter)(2)), Point((*iter)(0) + 2, (*iter)(1) + 25),
                cv::FONT_HERSHEY_PLAIN, 0.8, cv::Scalar(0, 0, 255), true);
            
            cout<<"("<<(*iter)(0)<<","<<(*iter)(1)<<"); Angle: "<<(*iter)(2)<<"degree; Scale:"<<((*iter)(3)) / 10.0<<endl;
            double t2 = cv::getTickCount();
            cout<<"Time elapsed "<< 1000 * (double)(t2-t1) / cv::getTickFrequency() << " ms for detecting" << i <<"th frame."<< endl;


            old_angle_min = (*iter)(2) - 10;
            if (  old_angle_min< 0)   old_angle_min = 0;
            old_angle_max = (*iter)(2) + 10;
            if (  old_angle_max > 360)   old_angle_max = 360;
            old_scale_min = (*iter)(3) - 2;
            if (old_scale_min < 5)   old_scale_min = 5;
            old_scale_max = (*iter)(3) + 2;
             if (old_scale_max > 20)   old_scale_max = 20;

            old_top = (*iter)(1) - this->TempImg.rows;
            if (old_top < 0) old_top = 0;
            old_bottom = (*iter)(1) + this->TempImg.rows;
            if(old_bottom >= this->TestImg.rows) old_bottom = this->TestImg.rows - 1;
            old_left = (*iter)(0) - this->TempImg.cols;
            if (old_left < 0) old_left = 0;
            old_right = (*iter)(0) + this->TempImg.cols;
            if(old_right >= this->TestImg.cols) old_right = this->TestImg.cols - 1;


        }
        imshow("result", copy);         
        waitKey(100);
    }

    cout<<"Finished "<<endl;
    waitKey(0);
}