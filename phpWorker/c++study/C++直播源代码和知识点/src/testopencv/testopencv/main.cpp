//
//  main.cpp
//  testopencv
//
//  Created by tangwei on 17/10/17.
//  Copyright © 2017年 tangwei. All rights reserved.
//
//  Point类 代表2D点 数值：x、y
//  Rect类 代表一个矩形 数值：x、y、width、height  可以拿到面积大小、左上角的坐标、右下角的坐标
//  Size类 代表区域大小 如：Rect中的Size()就是返回的这个
//  Scalar类 用于传递像素值 如：Scalar(double B, double G, double R, double Alpha) eg:Scalar(0, 0, 255) 为红色
//  Mat类:
//          有两部分组成，矩阵头（大小、通道、数据类型等）和数据块（像素值）
//          Mat类的初始化：（什么叫单通道，单通道又叫灰度图，每个像素点只能有有一个值表示颜色，它的像素值在0到255之间，0是黑色，255是白色，中间值是一些不同等级的灰色。什么叫三通道，每个像素点都有3个值表示，每个像素都是、通过对红(R)、绿(G)、蓝(B)三个颜色通道的变化以及它们相互之间的叠加来得到各式各样的颜色的）
//                  1、创建无初始化矩阵：
//                          Mat img;
//                  2、创建大小200行 X 100列的8位单通道矩阵：
//                          Mat img1(200, 100, CV_8UC1);
//                  3、创建大小为200 X 100的8位3通道矩阵：
//                          Mat img2(200, 100, CV_8UC3);
//                  4、创建大小为200 X 100的8位3通道矩阵：
//                          Mat img3(200, 100, CV_8UC3);

//  Mat图像在内存中是如何存储的呢？ RGB每一个正好占用一个字节，即R占一个字节
//      以 Mat img(3, 3, CV_8UC3);为例，这是一张3行3列的8位3通道的矩阵，在内存中，存放如下：
//              clo1                  clo2                    clo3
//      row1    0:B 1:G 2:R(0x00)     3:B 4:G 5:R(0x01)       6:B 7:G 8:R(0x02)
//
//      row2    9:B 10:G 11:R         12:B 13:G 14:R          15:B 16:G 17:R
//
//      row3    18:B 19:G 20:R        21:B 22:G 23:R          24:B 25:G 26:R

//  针对向上的连续空间，怎么取访问？
//      int size = mat.rows  * mat.cols * mat.elemSize();
//      for(int i=0; i<size; i=i+mat.elemSize()){
//          mat.data[i] = 0;//B
//          mat.data[i+1] = 0;//G
//          mat.data[i+2] = 0;//R
//      }

//  VideoCapture是opencv中用来读取视频的类，视频源可以是摄像机也可以是文件或者是rtsp流。
//
//  bool open(int index, int apiPreference); 或者 VideoCapture cap(index); //这两个方法都是打开摄像头，一般使用open打开。参数是index，是一个索引，是对应摄像头列表中的索引, apiPreference指定摄像头类型，默认值为0 ，是系统自动检测。

//  打开摄像头之后，读取一帧视频：
//       bool res = read(OutputArray image);//不管是摄像头的视频还是打开文件的视频，都是通过它获取数据。参数 image是一个Mat类型，打开这一帧数据之后会存到这个Mat类型中去。这其中，read其实会对这帧数据进行解码(bool grab())、图像色彩转换(virtual bool retrieve(OutputArray image, int flag=0))操作。
//
//
//

#include <iostream>

#include "opencv2/core.hpp"//Mat

#include "opencv2/imgcodecs.hpp"//imread

#include "opencv2/highgui.hpp"//namedWindow imshow waitKey

int main(int argc, const char * argv[]) {
    
    cv::Mat img = cv::imread("/Users/tangwei/Desktop/小火车/5.jpeg");//读取一个图片
    
    cv::namedWindow("imageWindow");//创建一个窗口
    
    cv::imshow("imageWindow", img);//窗口中显示图片

    cv::waitKey(0);
    
//    cv::Mat img1(3000, 4000, CV_8UC3, cv::Scalar(0, 0, 255));
//    
//    int size = img1.rows * img1.cols * img1.elemSize();
//    
//    for(int i=0; i<size; i=i+3){
//    
//        img1.data[i] = 0;
//        
//        img1.data[i+1] = 0;
//        
//        img1.data[i+2] = 0;
//        
//    }
//    
//    cv::namedWindow("imageWindow");//创建一个窗口
//    
//    cv::imshow("imageWindow", img1);//窗口中显示图片
//    
//    cv::waitKey(0);
//    
//    return 0;

    
    
}
