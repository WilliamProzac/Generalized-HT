#include <opencv2/opencv.hpp>
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

using namespace cv;
using namespace std;

int FillBlock(Mat src, Mat &mask, Point center)
{
    uchar back = src.at<uchar>(center.y, center.x);
    vector<Point> fill_point;
    int count = 0, count_mount = 1;
    fill_point.push_back(center);
    while (count < count_mount)
    {
        vector<uchar*> img;
        vector<uchar*> msk;
        for (int i = -1; i < 2; i++)
        {
            img.push_back(src.ptr<uchar>(fill_point[count].y + i));
            msk.push_back(mask.ptr<uchar>(fill_point[count].y + i));
        }
        for (size_t i = 0; i < 3; i++)
        {
            for (int j = -1; j < 2; j++)
            {
                if (img[i][fill_point[count].x + j] == back && !(j == 0 && i == 1) && msk[i][fill_point[count].x + j] == 255)
                {
                    fill_point.push_back(Point(fill_point[count].x + j, fill_point[count].y + i - 1));
                    msk[i][fill_point[count].x + j] = 1;
                }
            }
        }
        msk[1][fill_point[count].x] = 1;
        count_mount = fill_point.size() - 1;
        fill_point.erase(fill_point.begin());
    }
    return 0;
}

void MaskImage(InputArray _src, Mat &mask)
{
    Mat src = _src.getMat(),mask_tmp = Mat::zeros(src.size(), CV_8UC1);
    mask_tmp.setTo(255);
    Mat rows = Mat::zeros(Size(src.cols, 1), CV_8UC1), cols = Mat::zeros(Size(1, src.rows), CV_8UC1);
    Mat src_rows_beg = mask_tmp.row(0);
    Mat src_rows_end = mask_tmp.row(src.rows - 1);
    Mat src_cols_beg = mask_tmp.col(0);
    Mat src_cols_end = mask_tmp.col(src.cols - 1);
    rows.copyTo(src_rows_beg); rows.copyTo(src_rows_end);
    cols.copyTo(src_cols_beg); cols.copyTo(src_cols_end);

    for (size_t i = 3; i < src.rows-3; i++)
    {
        vector<uchar*> img;
        uchar* msk = mask_tmp.ptr(i);
        uchar* img1 = src.ptr(i);
        for (int k = -3; k < 4; k++)
        {
            img.push_back(src.ptr<uchar>(k + i));
        }
        for (size_t j = 3; j < src.cols-3; j++)
        {
            bool flag = false;
            if (msk[j] != 255) continue;
            float sum[4] = { 0 };
            sum[0] = img[3][j];
            Point x0 = Point(0, 0);
            Point x1 = Point(0, 0);
            uchar n_count = 0;
            for (int m = 2; m < 5; m++)
            {
                for (int n = -1; n < 2; n++)
                {
                    if (m == 3 && n == 0) continue;
                    sum[1] = sum[1] < img[m][j + n] ? img[m][j + n] : sum[1];
                    x0 = sum[0] == img[m][j + n] ? Point(m, n) : x0;
                    n_count = sum[0] == img[m][j + n] ? n_count+1 : n_count;
                    //flag = img[3][j + 0] < img[m][j + n] ? true : flag;//如果目标像素小于周围任何一个像素就说明这个一定不是最大值
                }
            }
            for (int m = 1; m < 6; m++)
            {
                for (int n = -2; n < 3; n++)
                {
                    if (2 <= m && m <= 4 && -1 <= n && n <= 1) continue;
                    sum[2] = sum[2] < img[m][j + n] ? img[m][j + n] : sum[2];
                    x1 = sum[0] == img[m][j + n] ? Point(m, n) : x1;
                    n_count = sum[0] == img[m][j + n] ? n_count+1 : n_count;
                    //flag = img[3][j + 0] < img[m][j + n] ? true : flag;//如果目标像素小于周围任何一个像素就说明这个一定不是最大值
                }
            }
            for (int m = 0; m < 7; m++)
            {
                for (int n = -3; n < 4; n++)
                {
                    sum[3] = sum[3] < img[m][j + n] && !(1 <= m && m <= 5 && -2 <=n && n <= 2) ? img[m][j + n] : sum[3];
                    //flag = img[3][j+0] < img[m][j + n] ? true : flag;
                }
            }
            x1 = (x1.x == 0 && x1.y == 0) || n_count >= 3 ? x0 : x1;
            int tmp = sum[0] >= sum[1] && sum[1] >= sum[2] && sum[2] >= sum[3] && (abs(x0.x - x1.x) <= 2 && abs(x0.y - x1.y) <= 2)
                ? 0 : FillBlock(src, mask_tmp, Point(j, i));
        }
    }
    mask = mask_tmp.clone();
}

vector<Point> Find_Max(InputArray _src, Mat&mask,int gap,int radius)
{
    Mat src = _src.getMat();

    typedef struct MyStruct
    {
        Point position;
        float data;
    }MyStruct;

    MaskImage(src, mask);
    vector<MyStruct> max_point;
    for (size_t i = 0; i < src.rows; i++)
    {
        uchar *img = src.ptr<uchar>(i);
        uchar *msk = mask.ptr<uchar>(i);
        for (size_t j = 0; j < src.cols; j++)
        {
            if (msk[j] != 255) continue;
            MyStruct my_struct;
            my_struct.data = img[j];
            my_struct.position = Point(j, i);
            max_point.push_back(my_struct);
        }
    }
    for (size_t i = 0; i < max_point.size(); i++)
    {
        for (size_t j = i; j < max_point.size(); j++)
        {
            MyStruct temp;
            if (max_point[i].data <= max_point[j].data)
            {
                if (max_point[j].data == 0) continue;
                temp = max_point[j];
                max_point[j] = max_point[i];
                max_point[i] = temp;
            }
        }
    }

    for (size_t k = 0; k < max_point.size(); k++)//---
    {
        uchar back = src.at<uchar>(max_point[k].position.y, max_point[k].position.x);
        vector<Point> fill_point;
        int count = 0, count_mount = 1;
        fill_point.push_back(max_point[k].position);

        while (count < count_mount &&  max_point[k].data != 1)
        {
            vector<uchar*> img;
            vector<uchar*> msk;
            for (int i = -1; i < 2; i++)
            {
                img.push_back(src.ptr<uchar>(fill_point[count].y + i));
                msk.push_back(mask.ptr<uchar>(fill_point[count].y + i));
            }
            for (int i = 0; i < 3; i++)
            {
                for (int j = -1; j < 2; j++)
                {
                    //---
                    double x = pow((max_point[k].position.x - fill_point[count].x + j), 2); //(max_point[k].position.x - img[i][fill_point[count].x + j])*(max_point[k].position.x - img[i][fill_point[count].x + j]);
                    double y = pow((max_point[k].position.y - (fill_point[count].y + i - 1)), 2); // (max_point[k].position.y - img[i][fill_point[count].y + j])*(max_point[k].position.y - img[i][fill_point[count].x + j]);
                    int distance = sqrt(x + y);
                    if (img[i][fill_point[count].x + j] <= img[0][fill_point[count].x] - gap
                        || msk[i][fill_point[count].x + j] == 3
                        //|| msk[i][fill_point[count].x + j] == 0
                        || (j == 0 && i == 1)
                        || distance >= radius) continue;
                    if (img[i][fill_point[count].x + j] == 2 || msk[i][fill_point[count].x + j] == 0) max_point[k].data = 1;
                    msk[i][fill_point[count].x + j] = 3;
                    fill_point.push_back(Point(fill_point[count].x + j, fill_point[count].y + i - 1));
                    count_mount++;
                }
            }
            count++;
        }
        if (max_point[k].data == 1)
        {
            for (size_t i = 0; i < fill_point.size(); i++)
            {
                mask.at<uchar>(fill_point[i]) = 1;
            }
        }
        else
        {
            for (size_t i = 0; i < fill_point.size(); i++)
            {
                mask.at<uchar>(fill_point[i]) = 2;
            }
            max_point[k].data = 255;
            mask.at<uchar>(max_point[k].position) = 255;
        }
    }
    vector<Point> print_wjy;
    for (size_t i = 0; i < mask.rows; i++)
    {
        uchar *msk = mask.ptr<uchar>(i);
        for (size_t j = 0; j < mask.cols; j++)
        {
            if (msk[j] == 255)
                print_wjy.push_back(Point(j, i));
        }

    }
    return print_wjy;
}