#include "epilines_match.h"

using namespace std;
using namespace cv;

void draw_epilines(Mat img_1, Mat img_2, vector<Point2f> points_1, vector<Point2f> points_2, Mat &F){
	F = findFundamentalMat(points_1, points_2, CV_FM_8POINT); //CV_FM_8POINT
    vector<Vec<float, 3>> epilines1, epilines2;
	cv::computeCorrespondEpilines(points_1, 1, F, epilines1); // ax+by+c=0;
    computeCorrespondEpilines(points_2, 2, F, epilines2);
	
	cv::Mat img1, img2;
	if (img_1.type() == CV_8UC3)
    {
		img_1.copyTo(img1);
		img_2.copyTo(img2);
	}
	else if (img_1.type() == CV_8UC1){
		cvtColor(img_1, img1, COLOR_GRAY2BGR);
		cvtColor(img_2, img2, COLOR_GRAY2BGR);
	}
	else{
		cout << "unknow img type\n" << endl;
		exit(0);
	}
 
	cv::RNG& rng = theRNG();
	for (uint i = 0; i < points_1.size(); i++)
    {
        Scalar color = Scalar(rng(256), rng(256), rng(256));
 
		circle(img2, points_2[i], 5, color);
		line(img2, Point(0, -epilines1[i][2] / epilines1[i][1]), Point(img2.cols, -(epilines1[i][2] + epilines1[i][0] * img2.cols) / epilines1[i][1]), color);

		circle(img1, points_1[i], 4, color);
		line(img1, cv::Point(0, -epilines2[i][2] / epilines2[i][1]), cv::Point(img1.cols, -(epilines2[i][2] + epilines2[i][0] * img1.cols) / epilines2[i][1]), color);	
 	}

    namedWindow("img2 epiline1", 0);
 	imshow("img2 epiline1", img2);
    namedWindow("img1 epiline2", 0);
	imshow("img1 epiline2", img1);
}

void epipolar_rectify(Mat img_1, Mat img_2, vector<Point2f> points_1, vector<Point2f> points_2, Mat &F){
    Mat H1, H2;
    Mat img_1_rec, img_2_rec;
    Size imgSize = img_1.size();
    stereoRectifyUncalibrated(points_1, points_2, F, imgSize, H1, H2);
    // perspectiveTransform(img_1, img_1_rec, H1);
    // perspectiveTransform(img_2, img_2_rec, H1);
    warpPerspective(img_1, img_1_rec, H1, imgSize);
    warpPerspective(img_2, img_2_rec, H2, imgSize);
    namedWindow("img2 rectify", 0);
 	imshow("img2 rectify", img_2_rec);
    namedWindow("img1 rectify", 0);
	imshow("img1 rectify", img_1_rec);
}