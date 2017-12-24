#include <opencv2/imgproc.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;
//global variables
Mat img,adp;
int max_value = 255;
int odd_ker = 97;//or 70-97 for gaussian//20-40 for mean
int ker_size = 1*odd_ker + 3;
int d = 80;
int c = d-80;
void adaptiveThres(int ,void*)
{
	ker_size = 2*odd_ker + 1;
	c = d-80;
	adaptiveThreshold(img,adp,max_value,ADAPTIVE_THRESH_MEAN_C, THRESH_BINARY,ker_size,c);
	imshow("adaptiveThreshold_mean",adp);
	adaptiveThreshold(img,adp,max_value,ADAPTIVE_THRESH_GAUSSIAN_C, THRESH_BINARY,ker_size,c);
	imshow("adaptiveThreshold_gaussian",adp);
	waitKey(0);
}
int main()
{
	img = imread("8_3.png",0);
	namedWindow("adaptiveThreshold",CV_WINDOW_FREERATIO);
	createTrackbar("max_value","adaptiveThreshold",&max_value,255,adaptiveThres);
	createTrackbar("odd_ker","adaptiveThreshold",&odd_ker,600,adaptiveThres);
	createTrackbar("d","adaptiveThreshold",&d,333,adaptiveThres);
	adaptiveThres(max_value,0);
	return 0;
}
