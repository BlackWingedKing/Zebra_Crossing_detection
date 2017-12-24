#include <opencv2/imgproc.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;
//global variables
Mat src,dst;
int i = 31;


int main()
{
	src = imread("8_3.png",1);
	namedWindow("blur",CV_WINDOW_FREERATIO);
	imshow("src",src);
    //for ( int i = 1; i < MAX_KERNEL_LENGTH; i = i + 2 )
        { blur( src, dst, Size( 31, 31 ), Point(-1,-1) );}
    imshow("blur",dst);
 
   // for ( int i = 1; i < MAX_KERNEL_LENGTH; i = i + 2 )
        { GaussianBlur( src, dst, Size( i, i ), 0, 0 );}
    imshow("GaussianBlur",dst);
    //for ( int i = 1; i < MAX_KERNEL_LENGTH; i = i + 2 )
        { medianBlur ( src, dst, i );}
	imshow("medianBlur",dst);       
    //for ( int i = 1; i < MAX_KERNEL_LENGTH; i = i + 2 )
        { bilateralFilter ( src, dst, i, i*2, i/2 );}
    imshow("bilateralFilter",dst);
    waitKey(0);
    return 0;
}
