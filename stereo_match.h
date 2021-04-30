#ifndef _SSD_H_
#define _SSD_H_

#include <iostream>
#include <opencv2/opencv.hpp>

cv::Mat bgr_to_grey(cv::Mat &bgr);
void ssd_stereo_match(cv::Mat img_1, cv::Mat img_2, int range, int kernel_size, cv::Mat &depth, std::string path);

#endif //_SSD_H_