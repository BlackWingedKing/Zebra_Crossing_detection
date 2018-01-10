#include <iostream>
#include <string>
#include "opencv2/core/core.hpp"
#include "opencv2/core/utility.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui/highgui.hpp"
using namespace std;
using namespace cv;
int main()
{
   /* std::string in;
    cv::CommandLineParser parser(argc, argv, "{@input|../data/building.jpg|input image}{help h||show help message}");
    if (parser.has("help"))
    {
        parser.printMessage();
        return 0;
    }
    in = parser.get<string>("@input");*/
    Mat image = imread("cos-lines.jpg", 1);
//#if 0
   Mat canny=image.clone();
   cvtColor(image ,image,CV_BGR2GRAY);
   imshow("image",image);
   Canny(image, image, 50, 200, 3); // Apply canny edge
   imshow("canny",image);
   
//#endif
    // Create and LSD detector with standard or no refinement.
//#if 1
    Ptr<LineSegmentDetector> ls = createLineSegmentDetector(LSD_REFINE_STD);
//#else
//  Ptr<LineSegmentDetector> ls = createLineSegmentDetector(LSD_REFINE_NONE);
//#endif
    double start = double(getTickCount());
    vector<Vec4f> lines_std;
    // Detect the lines
    ls->detect(image, lines_std);
    double duration_ms = (double(getTickCount()) - start) * 1000 / getTickFrequency();
    std::cout << "It took " << duration_ms << " ms." << std::endl;
    // Show found lines
    cout<<"The detected lines are "<<"\n";
    cout<<"no. of lines = "<<lines_std.size()<<"\n";
    Mat drawnLines1(canny);
    for(size_t i=0;i<lines_std.size();i++)
    {
        cout<<lines_std[i][0]<<"\t"<<lines_std[i][1]<<"\t"<<lines_std[i][2]<<"\t"<<lines_std[i][3]<<"\n";
        Vec4i l = lines_std[i];
        line( drawnLines1, Point(l[0], l[1]), Point(l[2], l[3]), Scalar(255,0,0), 1, LINE_AA);
    }
    imshow("Standard refinement123", drawnLines1);
    Mat drawnLines(image);
    ls->drawSegments(drawnLines, lines_std);
    imshow("Standard refinement", drawnLines);
    waitKey();
    return 0;
}
