#include <stdlib.h>
#include <stdio.h>
#include "cv.h"
#include "highgui.h"

#pragma comment(lib,"cv.lib")
#pragma comment(lib,"cxcore.lib")
#pragma comment(lib,"highgui.lib")

int main()
{
	IplImage* image = cvCreateImage( cvSize(640,480), 8, 3 );

	cvNamedWindow("A",CV_WINDOW_AUTOSIZE);
	cvZero(image);
	/*
	void cvLine( CvArr* img, CvPoint pt1, CvPoint pt2, CvScalar color,
             int thickness=1, int line_type=8, int shift=0 );
		thickness : 粗细像素
		line_type : 线形 CV_AA 8联通 
		
	*/
	cvLine( image, cvPoint(50,50), cvPoint(250,250), CV_RGB(255,255,255), 1, CV_AA, 0 );

	/*
	void cvRectangle( CvArr* img, CvPoint pt1, CvPoint pt2, CvScalar color,
                  int thickness=1, int line_type=8, int shift=0 );
	     pt1 : 左上角点
		 pt2 : 右下角点
		 thickness : 粗细像素 ,如果设成CV_FILLED ，可用边框颜色填充整个矩形 ，同样也可用于所有封闭图形
	*/
	cvRectangle( image,cvPoint(120,120), cvPoint(150,150), CV_RGB(255,255,255), CV_FILLED, CV_AA, 0 );

	/*
	void cvCircle( CvArr* img, CvPoint center, int radius, CvScalar color,
               int thickness=1, int line_type=8, int shift=0 );
		 center : 中心
		 radius : 半径
	*/
	cvCircle( image,cvPoint(180,180), 20, CV_RGB(255,255,255), CV_FILLED, CV_AA, 0 );


	/*
	void cvEllipse( CvArr* img, CvPoint center, CvSize axes, double angle,
	double start_angle, double end_angle, CvScalar color,
                int thickness=1, int line_type=8, int shift=0 );
		 axes : 长、短半轴长度 
		 angle : 长轴偏离x轴的角度
		 start_angle ，end_angle ： 如果想要封闭的图形，需要将这两个设成0 和 360
	*/
	cvEllipse( image, cvPoint(240,240), cvSize(50,20), 45, 0, 270,
                   CV_RGB(255,255,255), 1, CV_AA, 0 );

	/*
	void cvPolyLine( CvArr* img, CvPoint** pts, int* npts, int contours, int is_closed,
                 CvScalar color, int thickness=1, int line_type=8, int shift=0 );
		 pts : 点数组
		 npts : 多少个多边形，每个多边形的点数
		 contours : 轮廓的数量 也就是 npts的数组维度
		 is_closed  : 是否封闭曲线 即首点 和尾点 相连 
	*/
	int  arr[2] = {3,3};//两个3点多边形
	CvPoint ptt[6] = {{5,5},{15,95},{25,25},
					{35,35},{45,95},{55,55}};//多边形的点
	CvPoint* pt[2];
	pt[0] = &(ptt[0]); 
	pt[1] = &(ptt[3]);
	
	cvPolyLine( image, pt, arr, 2, 1, CV_RGB(255,255,255), 1, CV_AA, 0 );

	
	/*
	void cvFillPoly( CvArr* img, CvPoint** pts, int* npts, int contours,
                 CvScalar color, int line_type=8, int shift=0 );
		cvPolyLine 类似，只是这个是填充的多边形
	*/
	cvFillPoly( image, pt, arr, 2, CV_RGB(255,255,255), CV_AA, 0 );
	


	/*
	void cvInitFont( CvFont* font, int font_face, double hscale,
				 double vscale, double shear=0,
                 int thickness=1, int line_type=8 );
		font_face : 字体类型 CV_FONT_HERSHEY_SIMPLEX 正常尺寸的sancserif字体  CV_FONT_HERSHEY_COMPLEX .... 不支持中文，如要支持，请查找freetype库下载
		hscale vscale : 水平和垂直尺寸， 1.0 全尺寸 0.5为半尺寸
		shear : 斜体 0 不倾斜 1.0 倾斜45度
	*/

	
	CvFont font;
	cvInitFont( &font, CV_FONT_HERSHEY_SIMPLEX,1.0, 1.0, 0, 1, CV_AA );
	cvPutText( image, "OPENCV 1.0!", cvPoint(300,50), &font, CV_RGB(255,255,255));


	cvShowImage("A",image);
	cvSaveImage("draw.jpg",image);
	cvWaitKey(0);
	
	cvReleaseImage(&image);
	cvDestroyWindow("A");
	
	return 0;
}