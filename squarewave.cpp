#include <opencv2/imgproc.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/opencv.hpp>

#include <vector>
#include <iostream>
#include <algorithm>
#include <math.h>

using namespace cv;
using namespace std;
//global variables
Mat img,adp,blue,blurr,img1,canny;
Mat spilt[3];
int odd_ker = 48;//
int bv = 60;
int c = 89;
int blur_value = 2;
int min_fluc = 10;
int max_sd = 100;
int area = 100;

vector<vector<int > >pixel_len;
std::vector<int> row_len;
std::vector<int> temp;
std::vector<float> sd;
std::vector<int> sd_point;
vector<Vec4i> hierarchy;
vector<vector<Point> > contours;
void row_wise_search(int x)
{
	Mat tar = canny.clone();
	int r,c;
	int b=0;
	int w=0;
	int fluc=0;
	int count=1;
	int fluc_check=0;
	r=tar.rows;
	c=tar.cols;
	//b for black and w for white 
	//fluc for fluctuations
	//fluc_check for previous value of fluc
	for(int i=0;i<r;i++)
	{
		for(int j=0;j<c;j++)
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
//		row_len.erase(row_len.begin(),row_len.begin()+0);
		pixel_len.push_back(row_len);
		row_len.clear();	
	}
}
float SD(std::vector<int> v)
{
	float value=0;
	for(size_t i =0;i<v.size();i++)
	{
		value+= v[i]*v[i];
	}
	value = value/(v.size()-1);
	value = sqrt(value);
	return value;
}
void fluctuation_filter(int x)
{
	//checks the min value of fluctuation and the 
	cout<<pixel_len.size()<<endl;
	for(size_t i=0;i<pixel_len.size();i++)
	{
		//check for min fluctuation
		if(pixel_len[i].size()>min_fluc)
		{
			//copy the vector components to a temp vector
			temp = pixel_len[i];
			//erase the length of the first fluctuation
			temp.erase(temp.begin(),temp.begin()+0);
			//Now calculate the SD of the given data
			sd.push_back(SD(temp));
			sd_point.push_back(i);
			//sd vector got filled with the values of the SD and the sd_point with the indexes 
		}
	}
	temp.clear();
	
}
void selectionsort(std::vector<float> &a, std::vector<int> &b)
{
	int min_id;
	for(size_t i = 0;i<a.size()-1;i++)
	{
		min_id = i;
		for(size_t j = i+1;j<a.size();j++)
		{
			if(a[j]<a[min_id])
				min_id = j;
			//swap the elements
		}
			
		swap(a[min_id],a[i]);
		swap(b[min_id],b[i]);
		
		
	}
	//cout<<"sorted values :"<<endl;
	for(int i=0;i<a.size();i++)
	{
		//cout<<a[i]<<"\n";
	}
}
void angle_scan(int x)
{
	//x is the id of the min sd row
	//create point (x,y) which should be the start point of the scan
	Vec2i p;
	float m = -1*0.58; //nearly the value of the tan30
	p[0]= pixel_len[x][0];
	p[1]= sd_point[x];
	//initially the line is y=p[1] now we should change it set a slope  
	while(m > 0.58)
	{
		
		m+=0.1;
	}
}
void squarewave(int ,void*)
{	//adaptive thresholding of the blue split image
	adaptiveThreshold(blue,adp,255,ADAPTIVE_THRESH_GAUSSIAN_C, THRESH_BINARY,2*bv+3,c-100);
	imshow("adaptiveThreshold",adp);
	//mean blur the image  
	medianBlur(adp,blurr,2*blur_value+1);
	findContours(blurr, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_NONE);
	canny = Mat::zeros(blurr.rows, blurr.cols, CV_8UC1);
	for(int i = 0;i < contours.size(); i++)
	{
	if (contourArea(contours[i]) > area )
		{
			drawContours(canny, contours, i, Scalar(255),-1);
			//cout<<"index:"<<i<<"\t"<<hierarchy[i]<<endl;
		}
	}
	//row_wise_search function
	imshow("after area thresholding ",canny);
	row_wise_search(0);
	fluctuation_filter(0);
	//selection sort the sd vector
	selectionsort(sd,sd_point);
	//sd_point[0] is the row with min SD
	//max SD filter
	if(sd[0]<max_sd)
	{
		cout<<"min SD line found"<<"\n";
		//angle_scan(sd_point[0]);
		cout<<"min sd ="<<sd[0]<<" min sd index = "<<sd_point[0]<<"\n";
		//print the line on img1
		line(img1,Point(0,sd_point[0]), Point(img1.cols-1,sd_point[0]), Scalar(255,0,0), 3, LINE_AA);
		//angle_scan(sd_point[i]);
	}
	else
	{
		cout<<"min sd = "<<sd[0]<<endl;
		cout<<"NO Zebra!!"<<"\n";
	}
	imshow("min sd line",img1);
	sd.clear();
	sd_point.clear();
	pixel_len.clear();
	cout<<pixel_len.size()<<endl;
	waitKey(0);
}
int main()
{
	//cout<<"123";
	
	img = imread("8_2.png",1);
	split(img,spilt);
	blue = spilt[0];
	img1 = img.clone();
	//imshow("y",blue);
	namedWindow("trackbars",CV_WINDOW_FREERATIO);
	createTrackbar("bv","trackbars",&bv,100,squarewave);
	createTrackbar("c","trackbars",&c,100,squarewave);
	createTrackbar("blur_value","trackbars",&blur_value,20,squarewave);
	createTrackbar("area","trackbars",&area,1000,squarewave);
	squarewave(bv,0);
	return 0;
}
