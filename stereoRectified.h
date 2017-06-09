#include "cv.h"
#include "highgui.h"
#include <iostream>

using namespace cv;
using namespace std;

class StereoRetifier
{
public:
	Mat camMatrixLeft;
	Mat camMatrixRight;

	Mat distCoeffLeft;
	Mat distCoeffRight;

	Mat map1Left;
	Mat map1Right;
	Mat map2Left;
	Mat map2Right;

	Mat R;
	Mat T;
	Mat Q;
	Mat RectRotLeft;
	Mat RectRotRight;

	Mat newCamMatrixLeft;
	Mat newCamMatrixRight;

	Rect ROILeft;
	Rect ROIRight;

	StereoRetifier(double* _camMatrixLeft,double* _camMatrixRight, double *_distCoeffLeft, double *_distCoeffRight, double *_R,double *_T);
	//StereoRetifier(float* _camMatrixLeft,float* _camMatrixRight, float *_distCoeffLeft, float *_distCoeffRight);
	~StereoRetifier();
	void init(int width ,int height);
	void process(Mat leftRaw, Mat rightRaw, Mat &left ,Mat &right);
};