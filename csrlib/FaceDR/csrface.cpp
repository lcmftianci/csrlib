#define _CRT_SECURE_NO_WARNINGS

#include "csrface.h"

#if 0
#pragma comment(lib, "opencv_world320d.lib")
#endif

using namespace std;
using namespace cv;


int main(void)
{
#if 0
	Mat img = imread("E:\\AllPicture\\照片\\sonrun\\yuzhu1.jpg");

	imshow("人脸识别", img);
	//waitKey(6000);

	VideoCapture cap(0);

	Mat frame;
	while (1)
	{
		cap >> frame;
		imshow("camera", frame);
		waitKey(30);
	}
#endif

#if 1

	VideoCapture cap(0);
	Mat frame;
	//Mat img = imread("D:\\Databases\\picdata\\girls.jpeg");
	//Mat img = imread("E:\\AllPicture\\照片\\sonrun\\yuzhu1.jpg");
	if (!face_cascade.load(face_cascade_filename))
		return -1;
	if (!eyes_cascade.load(eyes_cascade_filename))
		return -1;

	while (1)
	{
		cap >> frame;
		detectedFace(frame);

		int c = waitKey(10);
		if ((char)c == 27)
			return 0;
	}

#endif

#if 0
	VideoCapture cap(0);
	int inx = 0;
	//拍照程序
	while (1)
	{
		char key = waitKey(100);
		Mat frame;
		string strFileName = format("D:\\Databases\\myData\\pic%d.jpg", inx);
		cap >> frame;
		imshow(strFileName, frame);
		switch (key)
		{
		case 'v':
			inx++;
			imwrite(strFileName, frame);
			imshow("photo", frame);
			waitKey(500);
			destroyWindow("photo");
			break;
		default:
			break;
		}
	}
#endif

#if 0

	Mat img = imread("D:\\Databases\\myData\\pic1.jpg");
	std::vector<Rect> faces;
	Mat img_gray;

	cvtColor(img, img_gray, COLOR_BGR2GRAY);
	equalizeHist(img_gray, img_gray);

	//加载模型文件
	face_cascade.load(face_cascade_filename);

	//-- Detect faces
	face_cascade.detectMultiScale(img_gray, faces, 1.1, 3, CV_HAAR_DO_ROUGH_SEARCH, Size(50, 50));

	for (size_t j = 0; j < faces.size(); j++)
	{
		Mat faceROI = img(faces[j]);
		Mat MyFace;
		if (faceROI.cols > 100)
		{
			resize(faceROI, MyFace, Size(92, 112));
			string  str = format("D:\\Databases\\myData\\MyFcae%d.jpg", j);
			imwrite(str, MyFace);
			imshow("ii", MyFace);
		}
		waitKey(10);
	}
#endif

#if 0
	//图片处理
	IplImage* img = cvLoadImage("D:\\Databases\\myData\\lulu_1.jpg");

	CvRect rect;
	rect.x = 107;
	rect.y = 13;
	rect.width = 160;
	rect.height = 140;
	cvSetImageROI(img, rect);
	cvResetImageROI(img);
#endif

#if 0
	//img->origin = 1;		//旋转中心点，这样就会出现倒着
	//IplImage* src = cvCreateImage(cvSize(30, 30), IPL_DEPTH_32S, 1);	//大小，颜色深度、通道数
	IplImage* b_img, *g_img, *r_img;
	b_img = cvCreateImage(cvGetSize(img), IPL_DEPTH_8U, 1);
	g_img = cvCreateImage(cvGetSize(img), IPL_DEPTH_8U, 1);
	r_img = cvCreateImage(cvGetSize(img), IPL_DEPTH_8U, 1);

	printf("bChannels is %d\n", img->nChannels);		//打印通道数
	//BGR
	for (int inx = 0; inx < img->height; inx++)
	{
		unsigned char* prow = (unsigned char*)(img->imageData + inx*img->widthStep);

		unsigned char* brow = (unsigned char*)(b_img->imageData + inx*b_img->widthStep);
		unsigned char* grow = (unsigned char*)(g_img->imageData + inx*g_img->widthStep);
		unsigned char* rrow = (unsigned char*)(r_img->imageData + inx*r_img->widthStep);
	
		for (int jnx = 0; jnx < img->width; jnx++)
		{
			//prow[jnx * 3 + 0] = 0;	//B通道
			//prow[jnx * 3 + 1] = 0;	//G通道
			//prow[jnx * 3 + 2] = 0;	//R通道

			brow[jnx] = prow[jnx * 3 + 0];
			grow[jnx] = prow[jnx * 3 + 1];
			rrow[jnx] = prow[jnx * 3 + 2];
		}
	}
#endif

#if 0

	cvNamedWindow("图片处理");
	cvShowImage("图片处理", img);
	cvWaitKey(0);
#endif

	return 0;
}

void detectedFace(cv::Mat frame)
{
	std::vector<Rect> faces;
	Mat frame_fray;
	cvtColor(frame, frame_fray, COLOR_BGR2GRAY);
	equalizeHist(frame_fray, frame_fray);

	//加载xml文件后再检测

	//检测人脸
	face_cascade.detectMultiScale(frame_fray, faces, 1.1, 3);
	for (size_t inx = 0; inx < faces.size(); ++inx)
	{
		rectangle(frame, faces[inx], Scalar(255, 0, 0), 2, 8, 0);
		Mat faceROI = frame_fray(faces[inx]);

		//检测人眼
		std::vector<Rect> eyes;
		eyes_cascade.detectMultiScale(faceROI, eyes, 1.1, 3, 0 | CV_HAAR_SCALE_IMAGE, Size(30, 30));
		for (size_t jnx = 0; jnx < eyes.size(); ++jnx)
		{
			rectangle(frame, eyes[jnx], Scalar(0, 255, 0), 2, 8, 0);
		}
	}
	namedWindow(winName, 1);
	imshow(winName, frame);
}

void ColectFace(cv::String strPath)
{

}
