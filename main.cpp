#include "stereo_match.h"
#include "feature_match.h"
#include "epilines_match.h"

using namespace std;
using namespace cv;

int main(int argc, char **argv){
    //set path
    string L_path = "L4.jpg";
    string R_path = "R4.jpg";
    // string L_path = "art_L.png";
    // string R_path = "art_R.png";

    //read data
    Mat img_1 = imread("DataSets/" + L_path, CV_LOAD_IMAGE_COLOR);
    Mat img_2 = imread("DataSets/" + R_path, CV_LOAD_IMAGE_COLOR);
    // Mat img_1 = imread("DataSets/L3.jpg", CV_LOAD_IMAGE_COLOR);
    // Mat img_2 = imread("DataSets/R3.jpg", CV_LOAD_IMAGE_COLOR);

    // 1.feature match
    vector<Point2f> points_1, points_2;
    get_n_pair_points(img_1, img_2, points_1, points_2);

    // 2.epipolar rectify
    Mat F;
    draw_epilines(img_1, img_2, points_1, points_2, F);
    epipolar_rectify(img_1, img_2, points_1, points_2, F);
    
    //3.stereo match
    Mat depth;
    ssd_stereo_match(img_1, img_2, 80, 2, depth, L_path); // ssd range:80  kernel size:3 (Set the parameters appropriately.)

    waitKey();
    return 0;
}