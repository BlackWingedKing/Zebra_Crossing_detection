#include <opencv2/imgproc.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;
//global variables
Mat img,adp,blue,blurr;
int odd_ker = 48;//
int bv = 60;
int c = 89;
int blur_value = 2;
vector<vector<int > >pixel_len;
std::vector<int> row_len;
void row_wise_search(int x)
{
	Mat tar = blurr.clone();
	int r,c;
	int b=0;
	int w=0;
	int fluc=0;
	int count=1;
	int fluc_check=0;
	r=tar.rows();
	c=tar.cols();
	//b for black and w for white 
	//fluc for fluctuations
	//fluc_check for previous value of fluc
	for(int i=0;i<r;i++)
	{
		for(int j=0;j<c;k++)
		{
			//initially b=w=0;
			//scanning row wise and counting the fluctuations,push_back the lengths into the vector
			fluc_check = fluc;
			if(w==0 && b==0)
			{
				if(tar.at<uchar>(i,j)==255)
				{
					w=1;
					b=0;
				}
				if(tar.at<uchar>(i,j)==0)
				{
					b=1;
					w=0;
				}
			}
			//w=1
			if(w==1)
			{
				if(tar.at<uchar>(i,j)==255)
				{
					w=1;
					b=0;
				}
				if(tar.at<uchar>(i,j)==0)
				{
					b=1;
					w=0;
					fluc++;
				}
			}
			//b=1
			if(b==1)
			{
				if(tar.at<uchar>(i,j)==255)
				{
					w=1;
					b=0;
					fluc++;
				}
				if(tar.at<uchar>(i,j)==0)
				{
					b=1;
					w=0;
				}
			}
			
			if(fluc_check == fluc)
			{
				count++;
			}
			else
			{
				row_len.push_back(count);
				count=1;
			}
		}
		row_len.erase(row_len.begin(),row_len.begin()+0);
		pixel_len.push_back(row_len);
		row_len.clear();	
	}
}
void fluctuation_filter(int x)
{
	//checks the min value of fluctuation and the 
	for(size_t i=0;i<pixel_len.size();i++)
	{
		if(pixel_len)
	}
}
void squarewave(int ,void*)
{	//adaptive thresholding of the blue split image
	adaptiveThreshold(blue,adp,255,ADAPTIVE_THRESH_GAUSSIAN_C, THRESH_BINARY,2*bv+3,c-100);
	imshow("adaptiveThreshold",adp);
	//mean blur the image  
	medianBlur(adp,blurr,2*blur_value+1);
	waitKey(0);
	//row_wise_search function
	row_wise_search(0);
	fluctuation_filter(0);

}
int main()
{
	Mat spilt[3];
	img = imread("8_3.png",0);
	spilt[3] = split(img);
	blue = spilt[0];
	namedWindow("trackbars",CV_WINDOW_FREERATIO);
	createTrackbar("bv","trackbars",&bv,100,squarewave);
	createTrackbar("c","trackbars",&c,100,squarewave);
	createTrackbar("blur_value","trackbars",&blur_value,20,squarewave);
	squarewave(bv,0);
	return 0;
}