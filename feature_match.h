#ifndef _FM_H_
#define _FM_H_

#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/features2d.hpp>
#include <opencv2/highgui.hpp>

void orb_feature_match(cv::Mat img_1, cv::Mat img_2, std::vector<cv::KeyPoint> &keypoints_1, \
                        std::vector<cv::KeyPoint> &keypoints_2, std::vector<cv::DMatch> &matches);
void choose_matches(std::vector<cv::KeyPoint> keypoints_1, std::vector<cv::KeyPoint> keypoints_2, \
                        std::vector<cv::DMatch> matches, std::vector<cv::DMatch> &matches_choose);
void get_n_pair_points(cv::Mat img_1, cv::Mat img_2, \
                        std::vector<cv::Point2f> &points_1, std::vector<cv::Point2f> &points_2);

#endif //_FM_H_