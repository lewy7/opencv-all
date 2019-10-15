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
		thickness : ��ϸ����
		line_type : ���� CV_AA 8��ͨ 
		
	*/
	cvLine( image, cvPoint(50,50), cvPoint(250,250), CV_RGB(255,255,255), 1, CV_AA, 0 );

	/*
	void cvRectangle( CvArr* img, CvPoint pt1, CvPoint pt2, CvScalar color,
                  int thickness=1, int line_type=8, int shift=0 );
	     pt1 : ���Ͻǵ�
		 pt2 : ���½ǵ�
		 thickness : ��ϸ���� ,������CV_FILLED �����ñ߿���ɫ����������� ��ͬ��Ҳ���������з��ͼ��
	*/
	cvRectangle( image,cvPoint(120,120), cvPoint(150,150), CV_RGB(255,255,255), CV_FILLED, CV_AA, 0 );

	/*
	void cvCircle( CvArr* img, CvPoint center, int radius, CvScalar color,
               int thickness=1, int line_type=8, int shift=0 );
		 center : ����
		 radius : �뾶
	*/
	cvCircle( image,cvPoint(180,180), 20, CV_RGB(255,255,255), CV_FILLED, CV_AA, 0 );


	/*
	void cvEllipse( CvArr* img, CvPoint center, CvSize axes, double angle,
	double start_angle, double end_angle, CvScalar color,
                int thickness=1, int line_type=8, int shift=0 );
		 axes : �����̰��᳤�� 
		 angle : ����ƫ��x��ĽǶ�
		 start_angle ��end_angle �� �����Ҫ��յ�ͼ�Σ���Ҫ�����������0 �� 360
	*/
	cvEllipse( image, cvPoint(240,240), cvSize(50,20), 45, 0, 270,
                   CV_RGB(255,255,255), 1, CV_AA, 0 );

	/*
	void cvPolyLine( CvArr* img, CvPoint** pts, int* npts, int contours, int is_closed,
                 CvScalar color, int thickness=1, int line_type=8, int shift=0 );
		 pts : ������
		 npts : ���ٸ�����Σ�ÿ������εĵ���
		 contours : ���������� Ҳ���� npts������ά��
		 is_closed  : �Ƿ������� ���׵� ��β�� ���� 
	*/
	int  arr[2] = {3,3};//����3������
	CvPoint ptt[6] = {{5,5},{15,95},{25,25},
					{35,35},{45,95},{55,55}};//����εĵ�
	CvPoint* pt[2];
	pt[0] = &(ptt[0]); 
	pt[1] = &(ptt[3]);
	
	cvPolyLine( image, pt, arr, 2, 1, CV_RGB(255,255,255), 1, CV_AA, 0 );

	
	/*
	void cvFillPoly( CvArr* img, CvPoint** pts, int* npts, int contours,
                 CvScalar color, int line_type=8, int shift=0 );
		cvPolyLine ���ƣ�ֻ����������Ķ����
	*/
	cvFillPoly( image, pt, arr, 2, CV_RGB(255,255,255), CV_AA, 0 );
	


	/*
	void cvInitFont( CvFont* font, int font_face, double hscale,
				 double vscale, double shear=0,
                 int thickness=1, int line_type=8 );
		font_face : �������� CV_FONT_HERSHEY_SIMPLEX �����ߴ��sancserif����  CV_FONT_HERSHEY_COMPLEX .... ��֧�����ģ���Ҫ֧�֣������freetype������
		hscale vscale : ˮƽ�ʹ�ֱ�ߴ磬 1.0 ȫ�ߴ� 0.5Ϊ��ߴ�
		shear : б�� 0 ����б 1.0 ��б45��
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