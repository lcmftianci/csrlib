#ifndef _CSR_FACE_H_
#define _CSR_FACE_H_


#include <iostream>
#include <stdio.h>
#include <opencv/cv.h>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/opencv.hpp>

cv::String face_cascade_filename = "C:\\Opencv3.2.0\\opencv\\sources\\data\\haarcascades\\haarcascade_frontalface_default.xml";
cv::String eyes_cascade_filename = "C:\\Opencv3.2.0\\opencv\\sources\\data\\haarcascades\\haarcascade_eye_tree_eyeglasses.xml";

//人脸分类器
cv::CascadeClassifier face_cascade;
//人眼分类器
cv::CascadeClassifier eyes_cascade;

cv::String winName = "recogFace";


//检测人脸
void detectedFace(cv::Mat frame);

//人脸采集器
void ColectFace(cv::String strPath);




#endif
