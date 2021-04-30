#ifndef _EP_H_
#define _EP_H_

#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/features2d.hpp>
#include <opencv2/highgui.hpp>

void draw_epilines(cv::Mat img_1, cv::Mat img_2, std::vector<cv::Point2f> points_1, std::vector<cv::Point2f> points_2, cv::Mat &F);
void epipolar_rectify(cv::Mat img_1, cv::Mat img_2, std::vector<cv::Point2f> points_1, std::vector<cv::Point2f> points_2, cv::Mat &F);

#endif //_EP_H_