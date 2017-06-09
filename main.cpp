#include "cv.h"
#include <iostream>
#include "highgui.h"
#include "stereoRectified.h"

using namespace cv;
using namespace std;

void rotVectorToMatrix(Mat omMat,double *_R)
{
	cout << "[[omMat]]--- "<< omMat << endl;
	Mat R;
	Rodrigues(omMat,R);
	memcpy(_R,R.data,sizeof(double)*9);
	cout<<"*******"<<R<<endl;
}

int main()
{
	// 2017-04-16
	double om[3] = { 0.01936  ,  0.01652  ,  0.00884};
	double _R[9];
	Mat omMat(3,1,CV_64FC1,om);

	rotVectorToMatrix(omMat,_R);
	double _T[3] = { 359.70529 ,  -4.33367 , -9.04791 };  

	/*double _R[9] = {0.999108 ,0.005448 ,-0.041868
					,-0.006208 ,0.999818 ,-0.018052
					,0.041762 ,0.018296 ,0.998960}; */

	double _camMatrixLeft[9] = {1793.85498,0, 1018.19109,
		0,1798.95019,472.22228,
		0,0,1};

	double _camMatrixRight[9] = { 1798.13584 ,0	,964.42974,
		0	,1808.75515,	440.41376,
		0,	0,	1};

	double _distCoeffLeft[4] =  { -0.16264 ,  0.07629  ,   0.00456 , 0.00000};
	double _distCoeffRight[4] = { -0.19966 ,  0.22224  ,   -0.00050 , 0.00000};
	StereoRetifier stereoRectifier(_camMatrixLeft,_camMatrixRight,_distCoeffLeft,_distCoeffRight,_R,_T);


	int beginFrame =1;
	int endFrame = 15;

	char *leftDir = "/home/rebecca/Rebecca/dataCalib/calib/left_%d.jpg";
	char *rightDir = "/home/rebecca/Rebecca/dataCalib/calib/right_%d.jpg";
	char *saveLeftDir = "/home/rebecca/Rebecca/dataCalib/calib/rectified/left_%06d.jpg";
	char *saveRightDir = "/home/rebecca/Rebecca/dataCalib/calib/rectified/right_%06d.jpg";
	char *saveMergeDir = "/home/rebecca/Rebecca/dataCalib/calib/rectified/merge/merge_%06d.jpg";

	char mergeFile[256];
	char leftFile[256];
	char rightFile[256];
	char saveLeftFile[256];
	char saveRightFile[256];
	Mat leftRightMerge;


	for (int frameCtr = beginFrame; frameCtr <= endFrame; frameCtr++)
	{
		cout<<"processing frame:"<<frameCtr<<endl;

		sprintf(leftFile,leftDir,frameCtr);
		sprintf(rightFile,rightDir,frameCtr);
		sprintf(saveLeftFile,saveLeftDir,frameCtr);
		sprintf(saveRightFile,saveRightDir,frameCtr);
		sprintf(mergeFile,saveMergeDir,frameCtr);

		Mat leftRaw = imread(leftFile,CV_LOAD_IMAGE_GRAYSCALE);
		Mat rightRaw = imread(rightFile,CV_LOAD_IMAGE_GRAYSCALE);

		if(leftRaw.empty()) continue;

		if(frameCtr == beginFrame)
			stereoRectifier.init(leftRaw.cols,leftRaw.rows);

		Mat leftRectified(leftRaw.rows,leftRaw.cols,CV_8UC1,Scalar(255));
		Mat rightRectified(leftRaw.rows,leftRaw.cols,CV_8UC1,Scalar(255));

		stereoRectifier.process(leftRaw,rightRaw,leftRectified,rightRectified);

		if(frameCtr == beginFrame)
			leftRightMerge.create(leftRectified.rows,leftRectified.cols+rightRectified.cols,CV_8UC1);

		leftRectified.copyTo(leftRightMerge(Rect(0,0,leftRectified.cols,leftRectified.rows)));
		rightRectified.copyTo(leftRightMerge(Rect(leftRectified.cols,0,rightRectified.cols,rightRectified.rows)) );

		for (int i = 0; i < leftRightMerge.rows; i+=30)
		{
			line(leftRightMerge,Point(0,i),Point(leftRightMerge.cols-1,i),Scalar(128),3);
		}
		//namedWindow("merge",0);
		//imshow("merge",leftRightMerge);

		//resize(leftRectified,leftRectified,Size(leftRectified.cols/2,leftRectified.rows/2));
		//resize(rightRectified,rightRectified,Size(rightRectified.cols/2,rightRectified.rows/2));

		imwrite(saveLeftFile,leftRectified);
		imwrite(saveRightFile,rightRectified);
		imwrite(mergeFile,leftRightMerge);
		waitKey(10);
	}

	return 0;
}
