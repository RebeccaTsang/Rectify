#include "highgui.h"
#include "cv.h"
#include "stereoRectified.h"

using namespace cv;

StereoRetifier::StereoRetifier(double* _camMatrixLeft,double* _camMatrixRight, double *_distCoeffLeft, double *_distCoeffRight, double *_R,double *_T)
{
	camMatrixLeft .create(3,3,CV_64FC1);
	camMatrixRight.create(3,3,CV_64FC1);

	distCoeffLeft.create(4,1,CV_64FC1);
	distCoeffRight.create(4,1,CV_64FC1);

	R.create(3,3,CV_64FC1);
	T.create(3,1,CV_64FC1);

	RectRotLeft .create(3,3,CV_64FC1);
	RectRotRight.create(3,3,CV_64FC1);

	newCamMatrixLeft .create(3,4,CV_64FC1);
	newCamMatrixRight.create(3,4,CV_64FC1);

	Q.create(4,4,CV_64FC1);

	memcpy(camMatrixLeft.data,_camMatrixLeft,sizeof(double)*3*3);
	memcpy(camMatrixRight.data,_camMatrixRight,sizeof(double)*3*3);

	memcpy(distCoeffLeft.data,_distCoeffLeft,sizeof(double)*4);
	memcpy(distCoeffRight.data,_distCoeffRight,sizeof(double)*4);

	memcpy(R.data,_R,3*3*sizeof(double));
	memcpy(T.data,_T,3*sizeof(double));

	cout<<"R"<<R<<endl;
	//cout<<"T"<<T<<endl;
}

void StereoRetifier::process(Mat leftRaw, Mat rightRaw, Mat &left ,Mat &right)
{
	remap(leftRaw ,left ,map1Left,map2Left,INTER_CUBIC);
	remap(rightRaw,right,map1Right,map2Right,INTER_CUBIC);

	int x1 = max(ROILeft.x,ROIRight.x);
	int y1 = max(ROILeft.y,ROIRight.y);
	int x2 = min(ROILeft.x+ROILeft.width,  ROIRight.x+ROIRight.width);
	int y2 = min(ROILeft.y+ROILeft.height, ROIRight.y+ROIRight.height);

	int width  = x2-x1;
	int height = y2-y1;

	left = left(Rect(x1,y1,width,height));
	right = right(Rect(x1,y1,width,height));
	//rectangle(left,Point(ROILeft.x,ROILeft.y),Point(ROILeft.x+ROILeft.width,ROILeft.y+ROILeft.height),Scalar(255));
	//rectangle(right,Point(ROIRight.x,ROIRight.y),Point(ROIRight.x+ROIRight.width,ROIRight.y+ROIRight.height),Scalar(255));
	//imshow("crop",left_);
	//imshow("cropR",right_); waitKey();
}

void StereoRetifier::init(int width ,int height)
{
	stereoRectify(camMatrixLeft, distCoeffLeft, camMatrixRight, distCoeffRight,
		Size(width,height), R,T,RectRotLeft,RectRotRight,
		newCamMatrixLeft,newCamMatrixRight,Q,CALIB_ZERO_DISPARITY,
		-1,Size(0,0),&ROILeft,&ROIRight);

	initUndistortRectifyMap(camMatrixLeft,distCoeffLeft,RectRotLeft,newCamMatrixLeft,Size(width,height),CV_32FC1,map1Left,map2Left);
	cout<<"rect rot left:"<<RectRotLeft<<endl;
	cout<<"new cam left:" <<newCamMatrixLeft<<endl;
	initUndistortRectifyMap(camMatrixRight,distCoeffRight,RectRotRight,newCamMatrixRight,Size(width,height),CV_32FC1,map1Right,map2Right);
	cout<<"rect rot right:"<<RectRotRight<<endl;
	cout<<"new cam right:" <<newCamMatrixRight<<endl;
}

StereoRetifier::~StereoRetifier()
{

}