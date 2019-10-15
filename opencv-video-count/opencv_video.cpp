#include <stdlib.h>
#include <stdio.h>
#include "cv.h"
#include "highgui.h"

#pragma comment(lib,"cv.lib")
#pragma comment(lib,"cxcore.lib")
#pragma comment(lib,"highgui.lib")


/*
double t = (double)getTickCount();
sleep(2);
t = ((double)getTickCount() - t) / getTickFrequency();
printf("Times passed in seconds: %lf\n",t);

*/

//��Ե���Ҳ��ʺϸ��ӵĳ�������ȡС�͸���
void proccess2(IplImage* frame)
{
	//cvShowImage("monitor",frame);

	IplImage*  gray = cvCreateImage(cvGetSize(frame),8,1);
	cvCvtColor(frame,gray,CV_BGR2GRAY);
	gray->origin = frame->origin;

	int thresh = 20;
	cvCanny( gray, gray, thresh, thresh*3, 3 );

	int an = 2;
	IplConvKernel* element = cvCreateStructuringElementEx( an*2+1, an*2+1, an, an, CV_SHAPE_RECT, 0 );
	cvDilate( gray, gray, element, 1 ); 

	//cvShowImage("monitor",gray);

	CvMemStorage* storage = cvCreateMemStorage(0);
	CvSeq* contour = NULL;
	//��ı�image�������б�Ҫ�Ļ�������cvCloneImage
	cvFindContours( gray, storage, &contour);

	int count = 0;
	CvFont font;
	char text[8];
	CvRect rect;
	cvInitFont( &font, CV_FONT_HERSHEY_SIMPLEX,0.4, 0.4, 0, 1, CV_AA );
	for( ; contour != 0; contour = contour->h_next ){
		printf("length : %.2f  area : %.2f\n",cvArcLength(contour),cvContourArea(contour));//��������,�������
		if(cvArcLength(contour)>180 || cvArcLength(contour)<10)
			continue;
		rect = cvBoundingRect(contour,0);
		if(rect.x<120 || rect.x>700  || rect.x>300 && rect.x<550 )
			continue;
		cvRectangle( frame, cvPoint(rect.x,rect.y),cvPoint(rect.x+rect.width,rect.y+rect.height),CV_RGB(0,255,255), 1, 8, 0 );
		count++;
		sprintf(text,"%02d",count);
		cvPutText( frame, text, cvPoint(rect.x+5,rect.y+15), &font, CV_RGB(0,255,0));
	}

	cvShowImage("monitor",frame);


}


void proccess(IplImage* frame)
{
	//cvShowImage("monitor",frame);
	
	IplImage*  gray = cvCreateImage(cvGetSize(frame),8,1);
	cvCvtColor(frame,gray,CV_BGR2GRAY);
	gray->origin = frame->origin;
	
	//cvSmooth(gray,gray);
	
	int slider_pos = 150;
	cvThreshold( gray, gray, slider_pos, 255, CV_THRESH_BINARY );
	//cvNot(gray,gray);
	//cvShowImage("monitor",gray);
	
	int an = 5;
	IplConvKernel* element = cvCreateStructuringElementEx( an*2+1, an*2+1, an, an, CV_SHAPE_RECT, 0 );
	
	cvDilate(gray,gray,element,1);
	cvErode(gray,gray,element,1);
	
	//cvShowImage("monitor",gray);
	
	CvMemStorage* storage = cvCreateMemStorage(0);
	CvSeq* contour = NULL;
	//��ı�image�������б�Ҫ�Ļ�������cvCloneImage
	cvFindContours( gray, storage, &contour);
	
	int count = 0;
	CvFont font;
	char text[8];
	CvRect rect;
	cvInitFont( &font, CV_FONT_HERSHEY_SIMPLEX,0.4, 0.4, 0, 1, CV_AA );
	for( ; contour != 0; contour = contour->h_next ){
		printf("length : %.2f  area : %.2f\n",cvArcLength(contour),cvContourArea(contour));//��������,�������
		rect = cvBoundingRect(contour,0);
		if(rect.x<140 || rect.x>700  || rect.x>300 && rect.x<550 || rect.width+rect.height >250 || rect.width+rect.height <4)
			continue;
		cvRectangle( frame, cvPoint(rect.x,rect.y),cvPoint(rect.x+rect.width,rect.y+rect.height),CV_RGB(0,255,255), 1, 8, 0 );
		count++;
		sprintf(text,"%02d",count);
		cvPutText( frame, text, cvPoint(rect.x+5,rect.y+15), &font, CV_RGB(0,255,0));
	}
	
	cvShowImage("monitor",frame);
	
	
}

int main()
{
	//������� ��Ҫ���ؽ�����http://www.xvidmovies.com/codec/�������ļ���ʽת��������Ӧѡ��avi---xvid��ʽ
	CvCapture* capture = cvCaptureFromAVI( "vietnam_xvid.avi" );
	//CvCapture* capture = cvCaptureFromCAM(0);
	if(capture==NULL){
		printf("open file error\n");
		return -1;
	}
	cvNamedWindow( "monitor", 1 );
	IplImage* frame = 0;

	frame=cvQueryFrame(capture);

	while(1)  
	{  
		frame=cvQueryFrame(capture);
		if(!frame)
		{
			printf("get frame error\n");
			break;
		}
		proccess(frame);

		if(cvWaitKey(50)>0) break; //�������Ҫ�У�����ֻ�л�ɫ����,��ֵ���Կ��Ʋ����ٶ�
	}



	return 0;
}