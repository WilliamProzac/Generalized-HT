/*
 * @Description: 
 * @Version: 
 * @Author: William
 * @Date: 2021-06-01 23:18:44
 * @LastEditors: William
 * @LastEditTime: 2021-09-01 16:13:21
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

int  getFilePaths(vector<string> &filepaths, cv::String filePath);

int main()
{
    int intervals = 360;
    Mat tempImg;
    Mat tempImg_conner;
    Mat TestImg_input;
    Mat TestImg;

    cv::String Pattern = "C://Users//yangw//Documents//Projects//cv_cpp//Dataset_generator/Results/*.png";
    vector<string> filepaths;
    vector<Vec4i> result_list;

    double t_begin = cv::getTickCount();
    getFilePaths(filepaths,Pattern);
    double t_end = cv::getTickCount();

    cout<<"Time elapsed "<< 1000 * (double)(t_end-t_begin) / cv::getTickFrequency() << " ms for reading imgs."<< endl;
/*
    size_t cnt = filepaths.size();
    for(size_t i = 0; i < cnt; i++)
    {
        imshow("img", imread(filepaths[i]));
        waitKey(100);
    }
*/

    
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
    //TestImg_input = imread("../Dataset_generator/Results/(115,200);degree10;scale110.png",1);

    //rotate(TestImg_input,TestImg,ROTATE_90_CLOCKWISE);
    

    
   //imshow("Test",TestImg_input); 
    //waitKey(0);

    GHT Simplified_GHT = GHT();
    Simplified_GHT.detect(tempImg, intervals, filepaths);     

    return 0;
}

int  getFilePaths(vector<string> &filepaths, cv::String filePath)
{
	filepaths.clear();
	cout << "Reading files from: " << filePath << endl;
	vector<cv::String> fn;
	cv::glob(filePath, fn, false);
 
	if (fn.size() == 0)
	{
		cout << "file in " << filePath << " not  exits" << endl;
		return -1;
	}
	//prepare pair for sort 
	vector<pair<int, string>> v1;
	pair<int, string> p1;
	vector<cv::String >::iterator it_;
	for (it_ = fn.begin(); it_ != fn.end();	++it_)
	{	
		//1.获取不带路径的文件名,1.txt
		string::size_type iPos = (*it_).find_last_of('\\') + 1;
		string filename = (*it_).substr(iPos, (*it_).length() - iPos);
		//2.获取不带后缀的文件名,1
		string name = filename.substr(0, filename.rfind("."));
		//3.构建键和值的pair
		try {
			//防止文件夹中出现非整数的文件名导致的错误
			p1 = make_pair(stoi(name), (*it_).c_str());
 
		}catch(exception e)
		{
			cout << "Crushed -> " << e.what() << endl;
			//continue; 直接continue一样 
			it_ = fn.erase(it_);
			
			it_--; //erase函数的返回的是指向被删除元素的下一个元素的迭代器，所以执行erase（）后要把迭代器减1，指向前面一个
		}
		v1.emplace_back(p1);
	}
	//cout << "v1.sie(): " << v1.size()<<endl;
	sort(v1.begin(), v1.end(), [](auto a, auto b) {return a.first < b.first; });
	vector<pair<int, string> >::iterator it;
	for (it = v1.begin(); it != v1.end(); ++it)
	{
		//cout << it->first << endl;
		//cout << it->second << endl;
 
		filepaths.emplace_back(it->second);
	}
	return 0;
}