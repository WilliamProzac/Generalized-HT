#ifndef VOTER_HPP_
#define VOTER_HPP_

#include <opencv2/opencv.hpp>
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "RTable.hpp"
#include <algorithm>

using namespace std;
using namespace cv;

typedef vector<double> Vote_space;

typedef vector<vector<int> > Vote_space_2D;

typedef vector<vector<vector<vector<float> > > > Vote_space_4D;

class Voter
{
    
    
    Vote_space_2D Vote_Space;

    Vote_space C_Vote_Space;

    void high_pass();

public:
    Voter(long size);
    Mat vote(Mat EdgeImg, Mat gradImg, Rtable_typ rtable, int intervals);
    Mat vote_SGHT(Mat EdgeImg, Mat gradImg, Rtable_typ rtable, int intervals);
    vector<Vec4i> vote_CGHT(Mat EdgeImg, Mat gradImg, Rtable_typ rtable, int intervals);
};


#endif /* VOTER_HPP_ */