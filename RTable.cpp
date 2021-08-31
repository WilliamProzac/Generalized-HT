/*
 * @Description: 
 * @Version: 
 * @Author: William
 * @Date: 2021-06-13 14:39:09
 * @LastEditors: William
 * @LastEditTime: 2021-07-18 17:32:09
 */

#include "RTable.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <vector>
#include <iostream>
#include <stdio.h>
#include <string>
#include <thread>

using namespace std;


void RTable::inspectTable()
{
    int entryNum = 0;
    for (int i = 0; i < table.size(); i++) 
    {
        cout << "interval " << i << ":" << endl;
        std::vector<Vec3f> entries = table[i];
        for (int j = 0; j < entries.size(); j++) 
        {
            cout << "(" << entries[j](0) << " ";
            cout << entries[j](1) << " " << ", ";
            cout << entries[j](2) << ")" << "; ";
            entryNum++;
        }
        cout << endl;
    }
    cout << "end intervals" << endl;
    cout << "total edge points number is " << entryNum << "." <<endl;
    int imageNum = rowNum * columnNum;
    cout << "total points number is " << imageNum << "." <<endl;
}


void RTable::generate(Mat gray, int inter)
{
    GrayImage = gray;

    CannyDetectEdge();

    //for debug
    cv::imshow("Edge Image is detected", EdgeImage);
    waitKey(0);

    rowNum = GrayImage.rows;
    columnNum = GrayImage.cols;
    intervals = inter;
    setRefPoint();
    TableInit();
    BuildTable();

}

Rtable_typ RTable::getRTable()
{
    return table;
}