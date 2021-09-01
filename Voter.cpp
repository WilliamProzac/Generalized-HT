/*
 * @Description: 
 * @Version: 
 * @Author: William
 * @Date: 2021-06-13 23:20:16
 * @LastEditors: William
 * @LastEditTime: 2021-09-01 17:14:16
 */


#include "Voter.hpp"
#include <opencv2/opencv.hpp>
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "RTable.hpp"
#include <algorithm>

void Voter::high_pass()
{
    
}

Voter::Voter(long size)
{
   // C_Vote_Space.resize(size);    
    C_Vote_Space = vector<double>(size,0.0);
}

Mat Voter::vote(Mat EdgeImg, Mat gradImg, Rtable_typ rtable, int intervals)
{
    Mat inspection = Mat(EdgeImg.rows, EdgeImg.cols, CV_32F, cvScalar(0));
        cout<<"Clare the voter class"<<endl;
        waitKey(0);

    int TestImg_Edgenum = 0;
    Mat origin = inspection.clone();
    for(int i = 0; i < EdgeImg.cols; i++)
    {
        for(int j = 0; j< EdgeImg.rows; j++)
        {
            if(EdgeImg.at<unsigned char>(j,i) == 255)
            {                
                TestImg_Edgenum ++;
                 for(int k = 0; k < 360; k++)
                {
                    //int bin = cvRound(gradImg.at<float>(j,i) + angle) % intervals;
                    for(int s = 0; s < rtable[k].size(); s++)
                    {
                        //float DeltaY = rtable[k][s](1);
                        float rho = rtable[k][s](0);
                        float DeltaA =rtable[k][s](1) - rtable[k][s](2);

                        //float rho = sqrt(DeltaX * DeltaX + DeltaY * DeltaY);

                        float voteA = fmod(gradImg.at<float>(j,i) + DeltaA, 360) ;  // ??
                        // line(inspection,)                 // using the line function to do the vote>

                        for(float S = 0.5; S<=2.0; S+=0.1)
                        {
                            int voteX = cvRound(i + rho * S * cos(voteA * 3.1415926 / 180.0));
                            int voteY = cvRound(j + rho * S * sin(voteA * 3.1415926 / 180.0));

                            if((voteX >= 0) && (voteX < EdgeImg.cols) && (voteY >= 0) && (voteY < EdgeImg.rows))
                            {
                                inspection.at<float>(voteY, voteX) += 0.1;
                                
                            }  
                            else continue;     
                        }
                        

                        //destroyWindow("vote");
/*                      int voteX_min = cvRound(i + rho * 0.5 * cos(voteA * 3.141592 / 180.0));
                        int voteY_min = cvRound(j + rho * 0.5 * sin(voteA * 3.141592 / 180.0));

                        int voteX_max = cvRound(i + rho * 2.0 * cos(voteA * 3.141592 / 180.0));
                        int voteY_max = cvRound(j + rho * 2.0 * sin(voteA * 3.141592 / 180.0));
    */
                                                                
                    }
                }
/*                     normalize(inspection, origin, 0, 255, NORM_MINMAX, CV_8UC1); 
                    imshow("vote", origin);
                    waitKey(1); */
                
            }
        }
    }
    
    normalize(inspection, origin, 0, 255, NORM_MINMAX, CV_8UC1); 
    imshow("vote", origin);
    cout<<"After voting"<<endl;
    cout<<"Test Image contains "<< TestImg_Edgenum <<"edge points"<<endl;
    waitKey(0);

    
    return origin;
}

Mat Voter::vote_SGHT(Mat EdgeImg, Mat gradImg, Rtable_typ rtable, int intervals)
{
     Mat inspection = Mat(EdgeImg.rows, EdgeImg.cols, CV_32F, cvScalar(0));
        cout<<"Clare the voter class"<<endl;
        waitKey(0);

    Mat origin = inspection.clone();
    for(int i = 0; i < EdgeImg.cols; i++)
    {
        for(int j = 0; j< EdgeImg.rows; j++)
        {
            if(EdgeImg.at<unsigned char>(j,i) == 255)
            {            
                 for(int angle = 0; angle < 360; angle++)
                {
                    int bin = cvRound(gradImg.at<float>(j,i) + angle) % intervals;
                    for(int s = 0; s < rtable[bin].size(); s++)
                    {
                        float phi = rtable[bin][s](1);
                        float rho = rtable[bin][s](0);
                    
                            for(float S = 0.5; S<=2.0; S+=0.1)
                            {
                                int voteX = cvRound(i + rho * S  * cos((phi) * 3.1415926 / 180.0));
                                int voteY = cvRound(j + rho * S  * sin((phi) * 3.1415926 / 180.0));

                                if((voteX >= 0) && (voteX < EdgeImg.cols) && (voteY >= 0) && (voteY < EdgeImg.rows))
                                {
                                    inspection.at<float>(voteY, voteX) += 1;
                                    
                                }  
                                else continue;     
                            } 
                                            
                    }
                }
                
            }
        }
    }
    
    normalize(inspection, origin, 0, 255, NORM_MINMAX, CV_8UC1); 
    imshow("vote", origin);
    cout<<"After voting"<<endl;
    waitKey(0);
    
    return origin;
}

vector<Vec4i> Voter::vote_CGHT(Mat EdgeImg, Rtable_typ rtable, int top, int right, int bottom, int left, int angle_min, int angle_max, int S_min, int S_max)
{


    //C_Vote_Space = vector<double>((right - left + 1)*(bottom - top + 1)*(angle_max - angle_min + 1)*(S_max - S_min + 1),0.0);

    C_Vote_Space.resize((right - left + 1)*(bottom - top + 1)*(angle_max - angle_min + 1)*(S_max - S_min + 1));

    for(int i = left; i <= right; i++)
    {
        for(int j = top; j <= bottom; j++)
        {
            if(EdgeImg.at<unsigned char>(j,i) == 255)
            {            
                for(int k = 0; k < rtable.size(); k++)
                 {                    
                    for(int s = 0; s < rtable[k].size(); s++)
                    {
                        float phi = rtable[k][s](1);
                        float rho = rtable[k][s](0);

                         for(int angle = angle_min; angle <= angle_max; angle++)
                         {
                            for(int S = S_min; S <= S_max; S+=1)
                            {
                                int voteX = cvRound(i + rho * S  / 10.0 * cos((phi + angle - 90) * 3.1415926 / 180.0));
                                int voteY = cvRound(j + rho * S  / 10.0 * sin((phi + angle - 90) * 3.1415926 / 180.0));
 
                                 if((voteX >= left) && (voteX < right+1) && (voteY >= top) && (voteY < bottom+1))
                                {
                                    C_Vote_Space[ (voteX - left)  * (bottom - top + 1) * (angle_max - angle_min + 1) * (S_max - S_min + 1) 
                                                    + (voteY - top) *  (angle_max - angle_min + 1) * (S_max - S_min + 1)  
                                                    + (angle-angle_min) * (S_max - S_min + 1)  + S - S_min] ++;
                                }
                            }
                         }
                    }
                }
            }
        }
    }

    cout<<"Finish voting"<<endl;

    Vote_space::iterator maxPosition = max_element(C_Vote_Space.begin(), C_Vote_Space.end());       //仅实现了单个最高点的检测
    
    long distan = distance(C_Vote_Space.begin(),maxPosition);

    C_Vote_Space.clear();
    Vote_space().swap(C_Vote_Space);

    int maxScale = ((distan % ((bottom - top + 1)*(angle_max - angle_min + 1)*(S_max - S_min + 1))) % ((angle_max - angle_min + 1)*(S_max - S_min + 1))) % (S_max - S_min + 1);
    int maxRotation = ((distan % ((bottom - top + 1)*(angle_max - angle_min + 1)*(S_max - S_min + 1))) % ((angle_max - angle_min + 1)*(S_max - S_min + 1)) - maxScale) / (S_max - S_min + 1);
    int maxRows = (distan % ((bottom - top + 1)*(angle_max - angle_min + 1)*(S_max - S_min + 1)) - maxRotation * (S_max - S_min + 1) - maxScale) / ((angle_max - angle_min + 1)*(S_max - S_min + 1));
    int maxCols = (distan - maxRows * (angle_max - angle_min + 1)*(S_max - S_min + 1) - maxRotation * (S_max - S_min + 1) - maxScale) / ((bottom - top + 1)*(angle_max - angle_min + 1)*(S_max - S_min + 1));
    
    vector<Vec4i> result;

    result.push_back(Vec4i(maxCols + left,maxRows + top,maxRotation + angle_min,maxScale + S_min));

    return result;

}

Voter::~Voter()
{
    //delete &C_Vote_Space;    

}

