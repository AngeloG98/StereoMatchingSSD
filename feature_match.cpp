#include "feature_match.h"

using namespace std;
using namespace cv;

void orb_feature_match(Mat img_1, Mat img_2, vector<KeyPoint> &keypoints_1, vector<KeyPoint> &keypoints_2, vector<DMatch> &matches){
    // check
    assert(img_1.data != nullptr && img_2.data != nullptr);

    // init
    Mat descriptors_1, descriptors_2;
    Ptr<FeatureDetector> detector = ORB::create();
    Ptr<DescriptorExtractor> descriptor = ORB::create();
    Ptr<DescriptorMatcher> matcher = DescriptorMatcher::create("BruteForce-Hamming");

    // Oriented FAST
    chrono::steady_clock::time_point t1 = chrono::steady_clock::now();
    detector->detect(img_1, keypoints_1);
    detector->detect(img_2, keypoints_2);

    // BRIEF
    descriptor->compute(img_1, keypoints_1, descriptors_1);
    descriptor->compute(img_2, keypoints_2, descriptors_2);
    chrono::steady_clock::time_point t2 = chrono::steady_clock::now();
    chrono::duration<double> time_used = chrono::duration_cast<chrono::duration<double>>(t2 - t1);
    cout << "extract ORB cost = " << time_used.count() << " seconds. " << endl;

    // match
    vector<DMatch> matches_raw;
    t1 = chrono::steady_clock::now();
    matcher->match(descriptors_1, descriptors_2, matches_raw);
    t2 = chrono::steady_clock::now();
    time_used = chrono::duration_cast<chrono::duration<double>>(t2 - t1);
    cout << "match ORB cost = " << time_used.count() << " seconds. " << endl;

    //max min Hamming
    auto min_max = minmax_element(matches_raw.begin(), matches_raw.end(),
                                [](const DMatch &m1, const DMatch &m2) { return m1.distance < m2.distance; });
    double min_dist = min_max.first->distance;
    double max_dist = min_max.second->distance;

    printf("-- Max dist : %f \n", max_dist);
    printf("-- Min dist : %f \n", min_dist);

    // filtrate
    for (int i = 0; i < descriptors_1.rows; i++) {
        if (matches_raw[i].distance <= max(2 * min_dist, 20.0)) {
            matches.push_back(matches_raw[i]);
        }
    }
}

void choose_matches(vector<KeyPoint> keypoints_1, vector<KeyPoint> keypoints_2, vector<DMatch> matches, vector<DMatch> &matches_choose){
    int range = 0.5*matches.size();
    // int range = 100;
    int dis_x = 0;
    int dis_y = 0;
    for (int i = 0; i < matches.size(); i++){
        if (i == 0){
            matches_choose.push_back(matches[i]);
        }
        else{
            for (int j = 0; j < matches_choose.size(); j++){
                dis_x = abs(keypoints_1[matches_choose[j].queryIdx].pt.x - keypoints_1[matches[i].queryIdx].pt.x);
                dis_y = abs(keypoints_1[matches_choose[j].queryIdx].pt.y - keypoints_1[matches[i].queryIdx].pt.y);
                if (dis_x < range || dis_y < range)
                    continue;
                if (j == matches_choose.size() - 1)
                    matches_choose.push_back(matches[i]);
            }
        }
    }
}

void get_n_pair_points(Mat img_1, Mat img_2, vector<Point2f> &points_1, vector<Point2f> &points_2){
    vector<DMatch> matches;
    vector<DMatch> matches_choose;
    vector<KeyPoint> keypoints_1, keypoints_2;

    orb_feature_match(img_1, img_2, keypoints_1, keypoints_2, matches);
    choose_matches(keypoints_1, keypoints_2, matches, matches_choose);

    // Mat outimg1;
    // drawKeypoints(img_1, keypoints_1, outimg1, Scalar::all(-1), DrawMatchesFlags::DEFAULT);
    // namedWindow("matches", 0);
    // imshow("ORB features", outimg1);

    // Mat img_match;
    // drawMatches(img_1, keypoints_1, img_2, keypoints_2, matches, img_match);
    // namedWindow("matches", 0);
    // imshow("matches", img_match);

    Mat img_match_choose;
    drawMatches(img_1, keypoints_1, img_2, keypoints_2, matches_choose, img_match_choose, Scalar::all(-1), Scalar::all(-1), vector<char>(), 2);
    namedWindow("matches_choose", 0);
    imshow("matches_choose", img_match_choose);

    if (matches_choose.size() >= 10){
    // if (0){
        for (int i = 0; i < matches_choose.size(); i++){
            points_1.push_back(Point(keypoints_1[matches_choose[i].queryIdx].pt.x, keypoints_1[matches_choose[i].queryIdx].pt.y));
            points_2.push_back(Point(keypoints_2[matches_choose[i].trainIdx].pt.x, keypoints_2[matches_choose[i].trainIdx].pt.y));
        }
        printf("true");
    }
    else{
        for (int i = 0; i < matches.size(); i++){
            points_1.push_back(Point(keypoints_1[matches[i].queryIdx].pt.x, keypoints_1[matches[i].queryIdx].pt.y));
            points_2.push_back(Point(keypoints_2[matches[i].trainIdx].pt.x, keypoints_2[matches[i].trainIdx].pt.y));
        }
        printf("false");
    }
}
