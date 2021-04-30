#include "stereo_match.h"

using namespace std;
using namespace cv;

Mat bgr_to_grey(Mat &bgr){
    
    int width = bgr.size().width;
    int height = bgr.size().height;
    Mat grey(height, width, 0);

    for (int y = 0; y < height; y++)
    {
        for (int x = 0; x < width; x++)
        {
            uchar r = 0.333 * bgr.at<Vec3b>(y, x)[2];
            uchar g = 0.333 * bgr.at<Vec3b>(y, x)[1];
            uchar b = 0.333 * bgr.at<Vec3b>(y, x)[0];
            grey.at<uchar>(y, x) = uchar(r + g + b);
        }
    }

    return grey;
}

void ssd_stereo_match(Mat img_1, Mat img_2, int range, int kernel_size, Mat &depth, string path){
    int width = img_1.size().width;
    int height = img_1.size().height;

    int max_d = range;
    int kernel = kernel_size;

    depth = Mat(height, width, 0);
    vector<vector<int>> min_ssd;

    Mat left = bgr_to_grey(img_1);
    Mat right = bgr_to_grey(img_2);

    // Mat left_raw = bgr_to_grey(img_1);
    // Mat right_raw = bgr_to_grey(img_2);
    // Mat left;
    // Mat right;
    // equalizeHist(left_raw, left);
    // equalizeHist(right_raw, right);

    for (int i = 0; i < height; ++i)
    {
        vector<int> largevalue(width, numeric_limits<int>::max());
        min_ssd.push_back(largevalue);
    }

    for (int y = 0; y < height; y++){
        #if defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)
            system("cls");
        #else
            system("clear");
        #endif

        cout << "====================Calculating...====================" << endl;
        int k = 50 * y / height;
        string s(k, '#');
        cout << s << 100 * y / height << '%';

        for (int x = 0; x < width; x++){
            for (int d = 0; d < max_d; d++){
                int sum_sd = 0;
                for (int i = max(0, y - kernel); i <= min(height - 1, y + kernel); i++){
                    for (int j = max(0, x - kernel); j <= min(width - 1, x + kernel); j++){
                        int delta = abs(left.at<uchar>(i, j) - right.at<uchar>(i, max(0, j - d)));
                        sum_sd += delta * delta;
                    }
                }
                if (sum_sd < min_ssd[y][x]){
                    min_ssd[y][x] = sum_sd;
                    depth.at<uchar>(y, x) = (uchar)(d);
                }
            }
        }
    }

    double minv = 0.0, maxv = 0.0;
	double* minp = &minv;
	double* maxp = &maxv;
    minMaxIdx(depth, minp, maxp);
    int kk = 255 / maxv - minv;
    int bb = -minv * kk;
    for (int y = 0; y < height; y++){
        for (int x = 0; x < width; x++){
            depth.at<uchar>(y, x) = depth.at<uchar>(y, x) * kk + bb;
        }
    }
    
    namedWindow("SSD", WINDOW_NORMAL | WINDOW_KEEPRATIO);
    string str = "_";
    imwrite("SSDResults/" + to_string(range) + str + to_string(kernel) + str + path, depth);
    imshow("SSD", depth);
    resizeWindow("SSD", 800, 600);
    printf("\n====================Done!====================\n");
}
