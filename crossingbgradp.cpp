#include <opencv2/imgproc.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/opencv.hpp>
#include <iostream>
#include <algorithm>
#include <vector>
#include <math.h>
using namespace cv;
using namespace std;
//global images and variables
Mat img,adp,blurr,canny,hou,final,img1;
int max_value = 255;
int odd_ker = 82;//or 70-97 for gaussian//20-40 for mean
int ker_size = 2*odd_ker + 3;
int d = 89;
int c = d-100;
int i = 13;
int thres_inter = 50;
int lmin = 50;
int dmax = 10;
int mincluster_size = 8;
int maxcluster_diff = 10;
//2D vectors cluster contains a the slope clusters
//cluster_point acts as a pointer for the cluster
vector<vector<int> > cluster;
vector<vector<int> > cluster_point;

//functions
//clusterises the given data and feeds into cluster
void clusterise(std::vector<float> a, std::vector<int> b,float diff)
{
	std::vector<int> v;
	std::vector<int> u;
	int r = 0;
	for (size_t i = 0; i < a.size()-1; i++)
	{	
		if (r==0)
		{
			v.push_back(a[i]);
			u.push_back(b[i]);
		}
		
		if (abs(a[i]-a[i+1])<=diff)
		{
			v.push_back(a[i+1]);
			u.push_back(b[i+1]);
			r=1;
		}
		else
		{
			r=0;
			cluster.push_back(v);
			cluster_point.push_back(u);
			v.clear();
			u.clear();	
		}
	
	}
	cout<<"no. of clusters = "<<cluster.size()<<"\n";
/**	cout<<"the values in the clusters are :"<<endl;
	for(i=0;i<u.size();i++)
	{

	}*/
}
//removes the clusters with less than a particular size
void cluster_filter(vector<vector<int> > &a,vector<vector<int> > &b,int min_size)
{
	vector<vector<int> > d;
	vector<vector<int> > e;
	cout<<"each cluster sizes :"<<endl;
	for(int i=0;i<a.size();i++)
	{
		cout<<a[i].size()<<endl;
	}
	for(int i=0;i<a.size();i++)
	{
		if(a[i].size()>min_size)
		{
			d.push_back(a[i]);
			e.push_back(b[i]);
			//a.erase(a.begin()+i);
			//b.erase(b.begin()+i);
		}
	
	}
	a=d;
	b=e;
	cout<<"no. of clusters remained after cluster_filter :"<<cluster.size()<<endl;
	cout<<"Sizes of the each cluster after filtering are :"<<endl;
	for(int i=0;i<cluster.size();i++)
	{
		cout<<cluster[i].size()<<endl;
	}
	
}
//normal sorting function with pointer array sorted
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
	cout<<"sorted values :"<<endl;
	for(int i=0;i<a.size();i++)
	{
		cout<<a[i]<<"\n";
	}
}
//function in which everything happens
/*void clear_outlier(std::vector<char> u;,std::vector<char> v;)
{

}
*/
void hough(int ,void*)
{
	//initialisations
	vector <float> slope;
	vector <int> pointer;
	//parameters for trackbars
	ker_size = 2*odd_ker + 1;
	c = d-80;
	//adaptive thresholding on grayscale
	/*adaptiveThreshold(img,adp,max_value,ADAPTIVE_THRESH_MEAN_C, THRESH_BINARY,ker_size,c);
	imshow("adaptiveThreshold_mean",adp);*/
	//adaptive on blue split
	adaptiveThreshold(img,adp,max_value,ADAPTIVE_THRESH_GAUSSIAN_C, THRESH_BINARY,ker_size,c);
	imshow("adaptiveThreshold_gaussian",adp);
	//filter,blur to remove noise dil,erode +blurs
	/*to be added*/
	//blur
	medianBlur(adp,blurr,(2*i+1));
	//GaussianBlur(adp,blurr,Size(2*i+1,2*i+1),0,0);
	imshow("blur",blurr);
	//canny edge detection
	Canny(blurr,canny,100,300,3);
	imshow("canny",canny);
	//hough line transfom
	vector<Vec4i> linesP;
	HoughLinesP(canny,linesP,1,CV_PI/180,thres_inter,lmin,dmax );
	//drawing the lines
	hou = img1.clone();
	//drawing the lines and filling the slopes
	for( size_t i = 0; i < linesP.size(); i++ )
    {
        Vec4i l = linesP[i];
        line( hou, Point(l[0], l[1]), Point(l[2], l[3]), Scalar(255,0,0), 3, LINE_AA);
    	float g = atan((float)(l[1]-l[3])/(l[0]-l[2]))*57.3;
    	//convert the angle from (-90,90) to (0,180) 
    	if(g>=0)
    	{
    		slope.push_back(g);
    	}
    	else
    	{
    		slope.push_back(g+180);
    	}
    	pointer.push_back(i);
    }
	//cout
	cout<<"no. of houghlines = "<<linesP.size()<<endl;
	imshow("hough",hou);
	//sort out the slopes and keep a pointer array
	selectionsort(slope,pointer);
	//clusterise into 2 2d vectors 
	clusterise(slope,pointer,maxcluster_diff);
	//filter the cluster
	cluster_filter(cluster,cluster_point,mincluster_size);
	
	final = img1.clone();
	if (cluster.size()==1)
	{
		//zebra crossing is that one only
		for(int i=0;i<cluster_point[0].size();i++)
		{
			Vec4i l = linesP[cluster_point[0][i]];
        	line( final, Point(l[0], l[1]), Point(l[2], l[3]), Scalar(255,0,0), 3, LINE_AA);
		}
	}
	else if(cluster.size()==0)
	{
		//screwed up
		cout<<"code screwed up"<<endl;
	}
	else if(cluster.size()>1)
	{
		//linefit on each cluster
		//define each mid point for each clusters initialise a 2d vector
		vector<Vec2i> mid;
		//value vector for the SD of each line and value_point as a pointer for value
		std::vector<float> value;
		std::vector<int> value_point;
		
		//filling the mid
		Vec4i q;
		for(int i=0;i<cluster.size();i++)
		{	//i is the number or the index of the cluster
			for(int j=0;j<cluster[i].size();j++)
			{
				q = linesP[cluster_point[i][j]];
				Vec2f extra_temp;// = ((q[0]+q[2])/2,(q[1]+q[3])/2);
				extra_temp[0]=(q[0]+q[2])/2;
				extra_temp[1]=(q[1]+q[3])/2;
				cout<<extra_temp[0]<<"\t"<<extra_temp[1]<<endl;
				mid.push_back(extra_temp);
			}
				
			//bestfit by method of least squares SD from that
			//define the parameters
			float m=0;
			float b=0;
			float x1=0;
			float y1=0;
			float temp1=0;
			float temp2=0;
			//xbar and ybar
			for(int k =0;k<mid.size();k++)
			{
				x1 = x1+mid[k][0];
				y1 = y1+mid[k][1];
			
			}
			cout<<"x1 = "<<x1<<"y1 = "<<y1<<endl;
			x1=x1/mid.size();
			y1=y1/mid.size();
			
			for(int k =0;k<mid.size();k++)
			{
				temp1 = temp1+((mid[k][0]-x1)*(mid[k][1]-y1));
				temp2 = temp2+((mid[k][0]-x1)*(mid[k][0]-x1));
			}
			m = temp1/temp2;
			b = y1-m*x1;
			cout<<"parameters:"<<endl;
			cout<<"b = "<<b<<"\t"<<"m = "<<m<<endl;
			//got all the values for best fit
			//find the value of SD and minimise
			for(int k =0;k<mid.size();k++)
			{
				temp1 = temp1+(abs((m*mid[k][0]-mid[k][1]+b)/sqrt((1+m*m))))*(abs((m*mid[k][0]-mid[k][1]+b)/sqrt((1+m*m))));
			}
			cout<<"temp1 = "<<temp1<<"\t"<<"n = "<<mid.size()<<endl;
			temp1 = abs(temp1/(mid.size()-1));
			temp1 = sqrt(temp1);
			//temp1 is the SD
			value.push_back(temp1);
			value_point.push_back(i);
			mid.clear();
		}
		
		selectionsort(value,value_point);
		
		cout<<"SD values are :";
		for(int i=0;i<value.size();i++)
		{
			cout<<value[i]<<endl;
		}
		//value_point[0] is the min SD cluster number
		//draw the lines in the
		for(int i=0;i<cluster_point[value_point[0]].size();i++)
		{
			Vec4i l = linesP[cluster_point[value_point[0]][i]];
        	line( final, Point(l[0], l[1]), Point(l[2], l[3]), Scalar(255,0,0), 3, LINE_AA);
		}	
	}
	
	imshow("final",final);
	waitKey(0);
}

int main()
{
	img = imread("ideal_zebra6.jpg",1);
	img1 = img.clone();
	imshow("input_image",img);
	//split the image and send the blue part of the image for adaptive
	Mat spilt[3];
	split(img,spilt);
	img =spilt[0];
	//windows and trackbars
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
	//initialise the function
	hough(max_value,0);
	return 0;
}