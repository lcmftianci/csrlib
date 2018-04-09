#include "header.h"
#include <caffe/caffe.hpp>
#include <caffe\blob.hpp>
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

/*根据指定数据，前向传播网络*/
//! Note: data_ptr指向已经处理好（去均值的，符合网络输入图像的长宽和Batch Size）的数据
void caffe_forward(boost::shared_ptr< Net<float> > & net, float *data_ptr)
{
	Blob<float>* input_blobs = net->input_blobs()[0];
	switch (Caffe::mode())
	{
	case Caffe::CPU:
		memcpy(input_blobs->mutable_cpu_data(), data_ptr,
			sizeof(float) * input_blobs->count());
		break;
	case Caffe::GPU:
		cudaMemcpy(input_blobs->mutable_gpu_data(), data_ptr,
			sizeof(float) * input_blobs->count(), cudaMemcpyHostToDevice);
		break;
	default:
		LOG(FATAL) << "Unknown Caffe mode.";
	}
	net->ForwardPrefilled();
}

/*根据Feature层的名字获取其在网络中的Index*/
//! Note: Net的Blob是指，每个层的输出数据，即Feature Maps
// char *query_blob_name = "conv1";
unsigned int get_blob_index(boost::shared_ptr< Net<float> > & net, char *query_blob_name)
{
	std::string str_query(query_blob_name);
	vector< string > const & blob_names = net->blob_names();
	for (unsigned int i = 0; i != blob_names.size(); ++i)
	{
		if (str_query == blob_names[i])
		{
			return i;
		}
	}
	LOG(FATAL) << "Unknown blob name: " << str_query;
}

/*根据Layer的名字获取其在网络中的Index*/
//! Note: Layer包括神经网络所有层，比如，CaffeNet共有23层
// char *query_layer_name = "conv1";
unsigned int get_layer_index(boost::shared_ptr< Net<float> > & net, char *query_layer_name)
{
	std::string str_query(query_layer_name);
	vector< string > const & layer_names = net->layer_names();
	for (unsigned int i = 0; i != layer_names.size(); ++i)
	{
		if (str_query == layer_names[i])
		{
			return i;
		}
	}
	LOG(FATAL) << "Unknown layer name: " << str_query;
}

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

#if 0
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

#if 1
	/*初始化网络*/
	char *proto = "E:\\Library\\caffe\\wincaffe\\ninjacaffe\\caffe\\models\\bvlc_reference_caffenet\\deploy.prototxt";
	Phase phase = TEST;
	Caffe::set_mode(Caffe::CPU);
	//Caffe::set_mode(Caffe::GPU);
	//Caffe.SetDevice(0);

	/*加载caffenet配置*/
	boost::shared_ptr<caffe::Net<float>> net(new caffe::Net<float>(proto, phase));
	//boost::shared_ptr<int> net(new int);

	/*加载训练好的模型*/
	char *model = "E:\\Library\\caffe\\wincaffe\\ninjacaffe\\caffe\\models\\bvlc_reference_caffenet\\bvlc_reference_caffenet.caffemodel";
	net->CopyTrainedLayersFrom(model);

	/*读物模型中每层的结构配置参数*/
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
			char kernelSize[100];
			char pad[100];
			char stride[100];
			sprintf(kernelSize, "%d", convparam.kernel_size());
			sprintf(pad, "%d", convparam.pad());
			sprintf(stride, "%d", convparam.stride());
			LOG(ERROR) << "\t\tkernel size：：" << kernelSize << ",pad：：" << pad << ",stride:：：" << stride;
		}
	}

	/*读取像素均值*/
	char *mean_file = "H:\\Models\\Caffe\\imagenet_mean.binaryproto";
	Blob<float> image_mean;
	BlobProto blob_proto;
	const float *mean_ptr;
	unsigned int num_pixel;

	bool succeed = ReadProtoFromBinaryFile(mean_file, &blob_proto);
	if (succeed)
	{
		image_mean.FromProto(blob_proto);
		num_pixel = image_mean.count(); /* NCHW=1x3x256x256=196608 */
		mean_ptr = (const float *)image_mean.cpu_data();
	}

	/*根据指定数据，前向传播网络*/

	/*根据Feature层的名字获取其在网络中的Index*/

	/*读取网络指定Feature层数据*/
	//! Note: 根据CaffeNet的deploy.prototxt文件，该Net共有15个Blob，从data一直到prob    
	char *query_blob_name = "conv1"; /* data, conv1, pool1, norm1, fc6, prob, etc */
	unsigned int blob_id = get_blob_index(net, query_blob_name);

	boost::shared_ptr<Blob<float> > blob = net->blobs()[blob_id];
	unsigned int num_data = blob->count(); /* NCHW=10x96x55x55 */
	const float *blob_ptr = (const float *)blob->cpu_data();

	/*
	根据文件列表，获取特征，并存为二进制文件
	详见get_features.cpp文件：
    主要包括三个步骤
    生成文件列表，格式与训练用的类似，每行一个图像 包括文件全路径、空格、标签（没有的话，可以置0）
    根据train_val或者deploy的prototxt，改写生成feat.prototxt 主要是将输入层改为image_data层，最后加上prob和argmax（为了输出概率和Top1/5预测标签）
    根据指定参数，运行程序后会生成若干个二进制文件，可以用MATLAB读取数据，进行分析
	*/

	/*根据Layer的名字获取其在网络中的Index*/

	/*读取指定Layer的权重数据*/
	//! Note: 不同于Net的Blob是Feature Maps，Layer的Blob是指Conv和FC等层的Weight和Bias
	char *query_layer_name = "conv1";
	const float *weight_ptr, *bias_ptr;
	unsigned int layer_id = get_layer_index(net, query_layer_name);
	boost::shared_ptr<Layer<float> > layer = net->layers()[layer_id];
	std::vector<boost::shared_ptr<Blob<float>  >> blobs = layer->blobs();
	if (blobs.size() > 0)
	{
		weight_ptr = (const float *)blobs[0]->cpu_data();
		bias_ptr = (const float *)blobs[1]->cpu_data();
	}
	//! Note: 训练模式下，读取指定Layer的梯度数据，与此相似，唯一的区别是将cpu_data改为cpu_diff


	/*修改某层的Weight数据*/
	const float* data_ptr;          /* 指向待写入数据的指针， 源数据指针*/
	//float* weight_ptr = NULL;       /* 指向网络中某层权重的指针，目标数据指针*/
	unsigned int data_size = 0;         /* 待写入的数据量 */
	char *layer_name = "conv1";     /* 需要修改的Layer名字 */

	//unsigned int layer_id = get_layer_index(net, query_layer_name);
	//boost::shared_ptr<Blob<float>> blob = net->layers()[layer_id]->blobs()[0];

	CHECK(data_size == blob->count());
	switch (Caffe::mode())
	{
	case Caffe::CPU:
		weight_ptr = blob->mutable_cpu_data();
		break;
	case Caffe::GPU:
		weight_ptr = blob->mutable_gpu_data();
		break;
	default:
		LOG(FATAL) << "Unknown Caffe mode";
	}
	//caffe_copy(blob->count(), (void*)data_ptr, (void*)weight_ptr);

	//! Note: 训练模式下，手动修改指定Layer的梯度数据，与此相似
	// mutable_cpu_data改为mutable_cpu_diff，mutable_gpu_data改为mutable_gpu_diff


	/*保存新的模型*/
	char* weights_file = "bvlc_reference_caffenet_new.caffemodel";
	NetParameter net_param;
	net->ToProto(&net_param, false);
	WriteProtoToBinaryFile(net_param, weights_file);

	printf("hello caffe\n");
	system("pause");
#endif
	return 0;
}