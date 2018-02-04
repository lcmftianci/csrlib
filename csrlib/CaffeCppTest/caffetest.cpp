#include "header.h"
#include <caffe/caffe.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iosfwd>
#include <memory>
#include <string>
#include <utility>
#include <vector>

using namespace caffe;
using namespace cv;
using namespace std;

//使用opencv进行人脸识别
/** Global variables */
String face_cascade_name = "D:\\Opencv\\compile\\install\\etc\\haarcascades\\haarcascade_frontalface_alt2.xml";
String eyes_cascade_name = "D:\\Opencv\\compile\\install\\etc\\haarcascades\\haarcascade_eye.xml";
CascadeClassifier face_cascade;   //定义人脸分类器  
CascadeClassifier eyes_cascade;   //定义人眼分类器  
String window_name = "Capture - Face detection";

void DetectFace(Mat frame)
{
	std::vector<Rect> faces;
	Mat frame_gray;

	cvtColor(frame, frame_gray, COLOR_BGR2GRAY);
	equalizeHist(frame_gray, frame_gray);

	//-- Detect faces  
	face_cascade.detectMultiScale(frame_gray, faces, 1.1, 3, 3);
	cout << faces.size() << endl;

	for (size_t i = 0; i < faces.size(); i++)
	{
		rectangle(frame, faces[i], Scalar(255, 0, 0), 2, 8, 0);

		Mat faceROI = frame_gray(faces[i]);
		std::vector<Rect> eyes;
#if 1
		//-- In each face, detect eyes  
		cout << "检测人眼开始" << endl;
		eyes_cascade.detectMultiScale(faceROI, eyes, 1.1, 3, 0 | CV_HAAR_SCALE_IMAGE, Size(30, 30));
		cout << eyes.size() << endl;
		for (size_t j = 0; j < eyes.size(); j++)
		{
			Rect rect(faces[i].x + eyes[j].x, faces[i].y + eyes[j].y, eyes[j].width, eyes[j].height);
			rectangle(frame, rect, Scalar(0, 255, 0), 2, 8, 0);
		}
#endif
	}
	//-- Show what you got  
	//namedWindow(window_name, 2);
	imshow(window_name, frame);
}


int main(int argc, char** argv)
{
#if 0
	char *proto = "E:\\Library\\caffe\\wincaffe\\ninjacaffe\\caffe\\models\\bvlc_reference_caffenet\\deploy.prototxt";
	Phase phase = TEST;
	Caffe::set_mode(Caffe::CPU);
	//Caffe::set_mode(Caffe::GPU);
	//Caffe.SetDevice(0);

	//加载caffenet配置
	boost::shared_ptr<caffe::Net<float>> net(new caffe::Net<float>(proto, phase));
	//boost::shared_ptr<int> net(new int);

	//加载训练好的模型
	char *model = "E:\\Library\\caffe\\wincaffe\\ninjacaffe\\caffe\\models\\bvlc_reference_caffenet\\bvlc_reference_caffenet.caffemodel";
	net->CopyTrainedLayersFrom(model);

	//读物模型中每层的结构配置参数
	NetParameter params;
	ReadNetParamsFromBinaryFileOrDie(model, &params);

	int numlayer = params.layer_size();
	for (int i = 0; i < numlayer; ++i)
	{
		//结构配置参数:name,type,kernel,size, pad,stride等
		LOG(ERROR) << "Layer " << i << ":" << params.layer(i).name() << "\t" << params.layer(i).type();
		if (params.layer(i).type() == "Convolution")
		{
			ConvolutionParameter convparam = params.layer(i).convolution_param();
			//LOG(ERROR) << "\t\tkernel size"
			//LOG(ERROR) << const_cast<int*>(convparam.kernel_size())
				//<< ",pad" << convparam.pad() 
				//<< ",stride:" << convparam.stride();
		}
	}

	//读取像素均值
	char *mean_file = "";

	printf("hello caffe\n");
#endif

#if 0
	Blob<float> bf;
	cout << "size: " << bf.shape_string() << endl;
	bf.Reshape(1, 2, 3, 4);
	cout << "size: " << bf.shape_string() << endl;
#endif

#if 0
	Mat src = imread("D:\\Databases\\picData\\girls.jpeg");
#elif 0
	Mat src = imread("D:\\Databases\\myData\\lulu_1.jpg");
#endif

#if 1
	VideoCapture cap(0);
	if (!face_cascade.load(face_cascade_name))
		return -1;
	if (!eyes_cascade.load(eyes_cascade_name))
		return -1;
	Mat frame;
	while (1)
	{
		cap >> frame;
		DetectFace(frame);
		waitKey(10);
	}
#endif

	//imshow("显示图片",src);
	//waitKey(10000);
	//system("pause");
	return 0;
}