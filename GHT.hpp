/*
 * @Description: 
 * @Version: 
 * @Author: William
 * @Date: 2021-06-13 13:34:48
 * @LastEditors: William
 * @LastEditTime: 2021-08-31 11:33:08
 */
#ifndef GHT_HPP_
#define GHT_HPP_


#include <opencv2/opencv.hpp>
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
#include <fstream>
#include "RTable.hpp"
#include "Voter.hpp"

using namespace cv;

class GHT
{
    
    Mat TestImg;    // Image for detecting
    Mat TempImg;    // Image with template
    Mat EdgeImage;
    Mat gradientMat;
    Mat IndexMat;

    int intervals;

    Rtable_typ r_table;
    
    Vec2i Result; 

    void CannyDetectEdge();

   // void overlay();
public:
    
    void detect(Mat Template_Image, Mat Testing_Image, int intervals);

    

};

inline void GHT::CannyDetectEdge()
{
    Mat BlurredImg;
    blur(TestImg,BlurredImg,Size(3,3)); 
    //
    Canny(BlurredImg, EdgeImage, 5, 150, 3, false);  
    imshow("Edge_Test", EdgeImage); 

    
    Mat dx;
    Mat dy;
    Mat dx_abs, dy_abs;
    dx.create(BlurredImg.size(), CV_32F);
    dy.create(BlurredImg.size(), CV_32F);

    Sobel(BlurredImg, dx, CV_32F, 1, 0,3);
    Sobel(BlurredImg, dy, CV_32F, 0, 1,3);

/*     convertScaleAbs(dx,dx_abs);
    convertScaleAbs(dy,dy_abs);

    addWeighted(dx_abs,0.5,dy_abs,0.5,0,EdgeImage); */
    imshow("Edge_Test", EdgeImage); 
    gradientMat.create(EdgeImage.size(), CV_32F);
    IndexMat.create(EdgeImage.size(), CV_16S);
    gradientMat = Scalar(0.0);
    IndexMat = Scalar(intervals);       // cannot bigger than intervals

    for(int i = 0; i < EdgeImage.cols; i++)
    {
        for(int j = 0; j< EdgeImage.rows; j++)
        {
            if(EdgeImage.at<unsigned char>(j,i) >= 200)
            {
                float gx = dx.at<float>(j, i);
                float gy = dy.at<float>(j, i);

                const float theta = fmod(fastAtan2(gy, gx+0.000001), 360);
                gradientMat.at<float>(j,i) = theta;
                const float thetaScale = 360.0 / intervals;
                IndexMat.at<short>(j,i) = cvRound(theta / thetaScale) % intervals;
            }
        }
    }

    //phase(dx,dy,gradientMat,true);
}


#endif /* GHT_HPP_ */