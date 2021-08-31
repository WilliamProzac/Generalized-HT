/*
 * @Description: 
 * @Version: 
 * @Author: William
 * @Date: 2021-06-13 13:38:29
 * @LastEditors: William
 * @LastEditTime: 2021-08-31 11:33:07
 */
#ifndef RTABLE_HPP_
#define RTABLE_HPP_

#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <vector>
#include <iostream>
#include <stdio.h>
#include <string>
#include <thread>

using namespace cv;

typedef std::vector<std::vector<cv::Vec3f>> Rtable_typ;

class RTable
{
    int rowNum;
    int columnNum;
    int intervals = 360;
    Vec2i refPoint;
    Mat GrayImage;
    Mat EdgeImage;
    Mat PhaseImage;
    Mat dx;
    Mat dy;

    Rtable_typ table;

    void setRefPoint()
    {
        refPoint(0) = rowNum / 2;       // y
        refPoint(1) = columnNum / 2;    // x
    }

    void TableInit()
    {
        table.clear();
        table.resize(intervals);
    };

    // void GetGradient();
    void BuildTable();

    void CannyDetectEdge();

public:
    void inspectTable();

    void generate(Mat garyImg, int intervals);

    Rtable_typ getRTable();
};

// inline void RTable::GetGradient()
// {

// }

inline void RTable::BuildTable()
{
    for (int i = 0; i < EdgeImage.cols - 1; i++)
    {
        for (int j = 0; j < EdgeImage.rows - 1; j++)
        {
            if ( EdgeImage.at<unsigned char>(j, i) >= 200)
            {
                float gx = dx.at<float>(j, i);
                float gy = dy.at<float>(j, i);
                
                const float theta = fastAtan2(gy, gx + 0.00001);
                //float theta = PhaseImage.at<float>(j,i); 
                const float phi = fastAtan2( refPoint(0) - j,  refPoint(1) - i);     
                const float rho = sqrt((refPoint(0) - j) * (refPoint(0) - j) + ( refPoint(1) - i) * ( refPoint(1) - i));
                const float thetaScale = 360.0 / intervals;
                const int tableIndex = cvRound(theta / thetaScale) % intervals;

                table[tableIndex].push_back(Vec3f(rho, phi, theta));
            }
        }
    }
}

inline void RTable::CannyDetectEdge()
{

    Mat BlurredImg;
    cv::blur(GrayImage,BlurredImg,Size(3,3));
    //BlurredImg = GrayImage;
    imshow("Blurred", BlurredImg);
    waitKey(0);
    cv::Canny(BlurredImg, EdgeImage, 5, 150, 3, false);
    
    dx.create(BlurredImg.size(), CV_32F);
    dy.create(BlurredImg.size(), CV_32F);
    Mat dx_abs, dy_abs;
    PhaseImage.create(BlurredImg.size(), CV_32F);

    Sobel(BlurredImg, dx, CV_32F, 1, 0, 3);
    Sobel(BlurredImg, dy, CV_32F, 0, 1, 3);

/*     convertScaleAbs(dx,dx_abs);
    convertScaleAbs(dy,dy_abs);

    addWeighted(dx_abs,0.5,dy_abs,0.5,0,EdgeImage); */


/*     float t;
    for(int y = 0; y < BlurredImg.rows; y++)
    {
        for(int x=0; x < BlurredImg.cols; x++)
        {
            t = atan2(dy.at<float>(y,x), dx.at<float>(y,x));
            PhaseImage.at<float>(y,x) = (t == 180)? 0:t;
        }
    }
 */
    /*

    PhaseImage.create(BlurredImg.size(), CV_32F);
    phase(dx,dy,PhaseImage,true);




    convertScaleAbs(dx,dx);
    convertScaleAbs(dy,dy);
    
    imshow("dx", dx);
    imshow("dy", dy); 

    waitKey(0);

    convertScaleAbs(PhaseImage,PhaseImage);
    imshow("Phase", PhaseImage);

    waitKey(0);
    */
}


#endif /* RTable_HPP_ */