#include <opencv2/imgproc.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/opencv.hpp>
#include <iostream>
#include <algorithm>
#include <vector>

using namespace cv;
using namespace std;
//global
Mat img,adp,blurr,canny,hou;
int max_value = 255;
int odd_ker = 60;//or 70-97 for gaussian//20-40 for mean
int ker_size = 2*odd_ker + 3;
int d = 80;
int c = d-80;
int i = 10;
int thres_inter = 50;
int lmin = 50;
int dmax = 10;

void selectionsort(std::vector<float> &a,std::vector<int> &b;)
{
	int min_id;
	for(size_t i = 0;i<a.size()-1;i++)
	{
		min_id = i;
		for(size_t j = i+1;j<a.size();j++)
		{
			if(a[j]<a[min_id])
				min_id = j;
			swap(a[min_id],a[i]);
			swap(b[min_id],b[i]);
		}
	}
}

void hough(int ,void*)
{
	//initialisations
	vector <float> slope;
	vector <int> pointer;
	//parameters for trackbars
	ker_size = 2*odd_ker + 1;
	c = d-80;
	//adaptive thresholding
	/*adaptiveThreshold(img,adp,max_value,ADAPTIVE_THRESH_MEAN_C, THRESH_BINARY,ker_size,c);
	imshow("adaptiveThreshold_mean",adp);*/
	adaptiveThreshold(img,adp,max_value,ADAPTIVE_THRESH_GAUSSIAN_C, THRESH_BINARY,ker_size,c);
	imshow("adaptiveThreshold_gaussian",adp);
	//filter of blur to remove noise dil,erode +blurs
	
	//blur
	medianBlur(adp,blurr,(2*i+1));
	//GaussianBlur(adp,blurr,Size(2*i+1,2*i+1),0,0);
	imshow("blur",blurr);
	//edge detection
	Canny(blurr,canny,100,300,3);
	imshow("canny",canny);
	//hough line transfom
	vector<Vec4i> linesP;
	HoughLinesP(canny,linesP,1,CV_PI/180,thres_inter,lmin,dmax );
	//drawing the lines
	hou = img.clone();
	for( size_t i = 0; i < linesP.size(); i++ )
    {
        Vec4i l = linesP[i];
        line( hou, Point(l[0], l[1]), Point(l[2], l[3]), Scalar(255,0,0), 3, LINE_AA);
    	slope.push_back(atan((float)(l[1]-l[3])/(l[0]-l[2])));
    	pointer.push_back(i);
    }
	selectionsort(slope,pointer);
	imshow("hough",hou);
	waitKey(0);

}

int main()
{
	img = imread("8_3.png",0);
	namedWindow("adaptiveThreshold",CV_WINDOW_FREERATIO);
	namedWindow("blur",CV_WINDOW_FREERATIO);
	namedWindow("canny",CV_WINDOW_FREERATIO);
	namedWindow("hough",CV_WINDOW_FREERATIO);
	createTrackbar("max_value","adaptiveThreshold",&max_value,255,hough);
	createTrackbar("odd_ker","adaptiveThreshold",&odd_ker,600,hough);
	createTrackbar("d","adaptiveThreshold",&d,333,hough);
	createTrackbar("i","blur",&i,50,hough);
	createTrackbar("thres_inter","hough",&thres_inter,200,hough);
	createTrackbar("lmin","hough",&lmin,100,hough);
	createTrackbar("dmax","hough",&dmax,50,hough);
	hough(max_value,0);
	return 0;
}
