/*
 * @Description: 
 * @Version: 
 * @Author: William
 * @Date: 2021-06-13 23:20:16
 * @LastEditors: William
 * @LastEditTime: 2021-07-18 19:07:42
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
    C_Vote_Space.resize(size);
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

vector<Vec4i> Voter::vote_CGHT(Mat EdgeImg, Mat gradImg, Rtable_typ rtable, int intervals)
{
    C_Vote_Space = vector<double>(EdgeImg.cols*EdgeImg.rows*360*16,0.0);

    for(int i = 0; i < EdgeImg.cols; i++)
    {
        for(int j = 0; j< EdgeImg.rows; j++)
        {
            if(EdgeImg.at<unsigned char>(j,i) == 255)
            {            
                for(int k = 0; k < rtable.size(); k++)
                 {
                    
                    
                    for(int s = 0; s < rtable[k].size(); s++)
                    {
                        float phi = rtable[k][s](1);
                        float rho = rtable[k][s](0);

                         for(int angle = 0; angle < 181; angle++)
                         {
                            for(int S = 0; S<=10; S+=1)
                            {
                                int voteX = cvRound(i + rho * (S + 5) / 10.0 * cos((phi + angle - 90) * 3.1415926 / 180.0));
                                int voteY = cvRound(j + rho * (S + 5) / 10.0 * sin((phi + angle - 90) * 3.1415926 / 180.0));
 
                                 if((voteX >= 0) && (voteX < EdgeImg.cols) && (voteY >= 0) && (voteY < EdgeImg.rows))
                                {
                                    C_Vote_Space[voteX * EdgeImg.rows * 360 * 16 + voteY * 360 * 16 + angle * 16 + S] ++;
                                }
                            }
                         }
                    }
                }
            }
        }
    }

    cout<<"Finish voting"<<endl;

    Vote_space::iterator maxPosition = max_element(C_Vote_Space.begin(), C_Vote_Space.end());
    
    long distan = distance(C_Vote_Space.begin(),maxPosition);

    int maxScale = ((distan % (EdgeImg.rows * 360 * 16)) % (360*16)) % 16;
    int maxRotation = ((distan % (EdgeImg.rows * 360 * 16)) % (360*16) - maxScale) / 16;
    int maxRows = (distan % (EdgeImg.rows * 360 * 16) - maxRotation * 16 - maxScale) / (360 * 16);
    int maxCols = (distan - maxRows * 360 * 16 - maxRotation * 16 - maxScale) / (EdgeImg.rows * 360 * 16);
    
    
    

    vector<Vec4i> result;

    result.push_back(Vec4i(maxCols,maxRows,maxRotation,maxScale));

    return result;

}

