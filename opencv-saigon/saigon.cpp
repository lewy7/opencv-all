#include <stdlib.h>
#include <stdio.h>
#include "cv.h"
#include "highgui.h"

#pragma comment(lib,"cv.lib")
#pragma comment(lib,"cxcore.lib")
#pragma comment(lib,"highgui.lib")

#define MAX_CORNER 100
CvVideoWriter* videoWriter;
int writerSpeed = 25; 
void count_object()
{
	CvCapture* capture = cvCaptureFromAVI( "vietnam_xvid.avi" );
	IplImage* frame = cvQueryFrame(capture);
	if(!videoWriter){
		videoWriter = cvCreateVideoWriter("save.avi",CV_FOURCC('x','v','i','d'),writerSpeed, cvSize(frame->width,frame->height));
	}

	CvFont font;
	cvInitFont( &font, CV_FONT_HERSHEY_SIMPLEX,0.4, 0.4, 0, 1, CV_AA );
	
	while(1)  
	{  
		frame=cvQueryFrame(capture);
		
		if(!frame){
			printf("get frame error\n");
			break;
		}
		cvPutText( frame, "Count Object", cvPoint(5,cvGetSize(frame).height-20), &font, CV_RGB(0,255,0));

		IplImage*  gray = cvCreateImage(cvGetSize(frame),8,1);
		cvCvtColor(frame,gray,CV_BGR2GRAY);
		gray->origin = frame->origin;

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
		//会改变image，所以有必要的话可以先cvCloneImage
		cvFindContours( gray, storage, &contour);
		
		int count = 0;
		CvFont font;
		char text[8];
		CvRect rect;
		cvInitFont( &font, CV_FONT_HERSHEY_SIMPLEX,0.4, 0.4, 0, 1, CV_AA );
		for( ; contour != 0; contour = contour->h_next ){
			//printf("length : %.2f  area : %.2f\n",cvArcLength(contour),cvContourArea(contour));//轮廓长度,轮廓面积
			rect = cvBoundingRect(contour,0);
			if(rect.x<140 || rect.x>700  || rect.x>300 && rect.x<550 || rect.width+rect.height >250 || rect.width+rect.height <4)
				continue;
			cvRectangle( frame, cvPoint(rect.x,rect.y),cvPoint(rect.x+rect.width,rect.y+rect.height),CV_RGB(0,255,255), 1, 8, 0 );
			count++;
			sprintf(text,"%02d",count);
			cvPutText( frame, text, cvPoint(rect.x+5,rect.y+15), &font, CV_RGB(0,255,0));
		}
		
		cvShowImage("monitor",frame);
		cvWriteFrame(videoWriter,frame);
		
		if(cvWaitKey(20)>0) break; //这个必须要有，否则只有灰色窗口,数值可以控制播放速度
	}
}

//lucas_kanade
void orient_speed()
{
	CvCapture* capture = cvCaptureFromAVI( "vietnam_xvid.avi" );

	IplImage *frame=cvQueryFrame(capture);
	if(!videoWriter){
		videoWriter = cvCreateVideoWriter("save.avi",CV_FOURCC('x','v','i','d'),writerSpeed, cvSize(frame->width,frame->height));
	}
	
	int gap = 0;
	int first_flag = 1;
	int count ;
	int win_size = 10;

	IplImage *prev_gray = cvCreateImage(cvGetSize(frame),8,1);
	IplImage *curr_gray = cvCreateImage(cvGetSize(frame),8,1);

	CvPoint2D32f* prev_corner = (CvPoint2D32f*)cvAlloc(MAX_CORNER*sizeof(CvPoint2D32f));
	CvPoint2D32f* curr_corner = (CvPoint2D32f*)cvAlloc(MAX_CORNER*sizeof(CvPoint2D32f));

	IplImage *prev_pyramid = cvCreateImage(cvGetSize(frame),32,1);
	IplImage *curr_pyramid = cvCreateImage(cvGetSize(frame),32,1);

	char corner_found[MAX_CORNER];
	float corner_error[MAX_CORNER];

	IplImage *swap_image;
	CvPoint2D32f *swap_corner;


	int slider_pos = 150;
	int an = 5;

	CvFont font;
	char text[8];
	cvInitFont( &font, CV_FONT_HERSHEY_SIMPLEX,0.4, 0.4, 0, 1, CV_AA );

	while(1)  
	{  
		frame=cvQueryFrame(capture);
		
		if(!frame)
		{
			printf("get frame error\n");
			break;
		}
		cvPutText( frame, "Orient and Speed", cvPoint(5,cvGetSize(frame).height-20), &font, CV_RGB(0,255,0));
		//每隔10帧处理一下，拉开距离
		if(gap++ % 5 != 1)
			continue;

		//cvShowImage( "monitor", frame );

		if(first_flag){
			
			cvCvtColor(frame,prev_gray,CV_RGB2GRAY);
			prev_gray->origin = frame->origin;
			
			cvThreshold( prev_gray, prev_gray, slider_pos, 255, CV_THRESH_BINARY );
			IplConvKernel* element = cvCreateStructuringElementEx( an*2+1, an*2+1, an, an, CV_SHAPE_RECT, 0 );
			cvDilate(prev_gray,prev_gray,element,1);
			cvErode(prev_gray,prev_gray,element,1);
			
			IplImage *eig = cvCreateImage(cvGetSize(frame),32,1); //这为什么是32位的。我X
			IplImage *temp = cvCreateImage(cvGetSize(frame),32,1);

			count = MAX_CORNER;
			cvGoodFeaturesToTrack( prev_gray, eig, temp, prev_corner, &count, 0.01, 2);//后两个是特征值和最小距离
			cvFindCornerSubPix( prev_gray, prev_corner, count,cvSize(win_size,win_size), cvSize(-1,-1),cvTermCriteria(CV_TERMCRIT_ITER|CV_TERMCRIT_EPS,20,0.03));//这玩意可以去掉，好像也没什么影响
			cvReleaseImage( &eig );
			cvReleaseImage( &temp );

			first_flag = 0;
			continue;
		}
		cvCvtColor(frame,curr_gray,CV_RGB2GRAY);
		curr_gray->origin = frame->origin;
		
			cvThreshold( curr_gray, curr_gray, slider_pos, 255, CV_THRESH_BINARY );
			IplConvKernel* element = cvCreateStructuringElementEx( an*2+1, an*2+1, an, an, CV_SHAPE_RECT, 0 );
			cvDilate(curr_gray,curr_gray,element,1);
			cvErode(curr_gray,curr_gray,element,1);

			//cvShowImage( "monitor", curr_gray );
			
			cvCalcOpticalFlowPyrLK( prev_gray, curr_gray, prev_pyramid, curr_pyramid,
			prev_corner, curr_corner, count, cvSize(win_size,win_size), 3, corner_found, corner_error,
			cvTermCriteria(CV_TERMCRIT_ITER|CV_TERMCRIT_EPS,20,0.03), 0 );
		
			
			float distance;
		for(int i=0;i<count;i++){
			if(corner_found[i] == 0){
				//printf("corner lost\n");
				continue;
			}
				
			CvPoint p0 = cvPoint(cvRound(prev_corner[i].x), cvRound(prev_corner[i].y));
			CvPoint p1 = cvPoint(cvRound(curr_corner[i].x), cvRound(curr_corner[i].y));
			//printf("%d [%d,%d]\n",count,p1.x,p1.y);
			distance = sqrtf(powf((curr_corner[i].x - prev_corner[i].x),2) + powf((curr_corner[i].y - prev_corner[i].y),2));
			sprintf(text,"%.0f",distance);

			cvLine(frame,p0,p1,CV_RGB(0,255,255));
			cvCircle(frame, p0, 5, CV_RGB(0,255,255), 1, 8,0);
			cvPutText( frame, text, cvPoint(p1.x+5,p1.y+5), &font, CV_RGB(0,255,0));
		}
		
		CV_SWAP( prev_gray, curr_gray, swap_image );
        CV_SWAP( prev_pyramid, curr_pyramid, swap_image );
        CV_SWAP( prev_corner, curr_corner, swap_corner );

		cvShowImage( "monitor", frame );
		cvWriteFrame(videoWriter,frame);
		cvWriteFrame(videoWriter,frame);
		cvWriteFrame(videoWriter,frame);
		cvWriteFrame(videoWriter,frame);
		cvWriteFrame(videoWriter,frame);
		if(cvWaitKey(200)>0) break; //这个必须要有，否则只有灰色窗口,数值可以控制播放速度
	}
}

void lock_on()
{
	CvCapture* capture = cvCaptureFromAVI( "vietnam_xvid.avi" );
	IplImage *frame=cvQueryFrame(capture);
	if(!videoWriter){
		videoWriter = cvCreateVideoWriter("save.avi",CV_FOURCC('x','v','i','d'),writerSpeed, cvSize(frame->width,frame->height));
	}
	
	IplImage *target = cvLoadImage("greencar.jpg",CV_LOAD_IMAGE_COLOR);
	//cvNamedWindow( "origon", CV_WINDOW_AUTOSIZE );cvShowImage( "origon", target );
	IplImage *hsv = cvCreateImage(cvGetSize(target),8,3);
	IplImage *hue = cvCreateImage(cvGetSize(target),8,1);
	
	cvCvtColor(target,hsv,CV_BGR2HSV);
	cvSplit( hsv, hue, 0, 0, 0 );
	
	int hist_size[]={255};
	float range[] = {0,360};
	float *ranges[]={ range };
	CvHistogram *hist=cvCreateHist(1,hist_size,CV_HIST_ARRAY,ranges,1);
	cvCalcHist(&hue, hist, 0, 0);
	
	IplImage *backproject = cvCreateImage( cvGetSize(frame), 8, 1 );
	IplImage *image = cvCreateImage( cvGetSize(frame), 8, 3 );
	IplImage *image_hue = cvCreateImage(cvGetSize(frame),8,1);
	
	CvRect track_window = {608,204,60,60};//初始搜索框非常重要，这个要先在大图片上定位好，不然搜不到
	CvBox2D track_box;
	CvConnectedComp track_comp;

	CvFont font;
	char text[8] ={0};
	cvInitFont( &font, CV_FONT_HERSHEY_SIMPLEX,0.4, 0.4, 0, 1, CV_AA );

	CvPoint last_position = {0,0};
	float distance = 0;
	int gap = 1;
	
	while(1)  
	{  
		frame=cvQueryFrame(capture);
		
		if(!frame)
		{
			printf("get frame error\n");
			break;
		}
		cvPutText( frame, "Lock On", cvPoint(5,cvGetSize(frame).height-20), &font, CV_RGB(0,255,0));
		//cvShowImage( "monitor", frame );
		cvCvtColor(frame,image,CV_BGR2HSV);
		
		cvSplit(image,image_hue,0,0,0);
		//cvShowImage( "monitor", image_hue );
		
		/*
		根据获得的色彩直方图将原始图像转化成色彩概率分布图像，这个过程就被称作"Back Projection"
		void cvCalcBackProjectPatch( IplImage** images, CvArr* dst,
		CvSize patch_size, CvHistogram* hist,int method, float factor );
		1. IplImage** img:存放原始图像，输入。
		2. CvArr** backproject:存放Back Projection结果，输出。
		3. CvHistogram* hist:存放直方图，输入
		*/ 
		cvCalcBackProject(&image_hue,backproject,hist);
		backproject->origin = frame->origin;
		//cvShowImage( "monitor", backproject );
		
		/*
		cvCamShift算法
		1.选择窗的大小和初始位置.
		2.计算此时窗口内的Mass Center.
		3.调整窗口的中心到Mass Center.
		4.重复2和3，直到窗口中心"会聚"，即每次窗口移动的距离小于一定的阈值。
		
		  cvCamShift就是再视频每一帧间的cvMeanShift的循环迭代版
		  int cvCamShift( const CvArr* prob_image, CvRect window, CvTermCriteria criteria,
		  CvConnectedComp* comp, CvBox2D* box=NULL );
		  prob_image ： backproject 图像
		  window     : 首次搜索窗口 对复杂图像查找成功非常重要
		  criteria  : 搜寻中止条件
		  comp ：保存运算结果,包括新的Search Window的位置和面积。
		  box：包含被跟踪物体的最小矩形。
		*/
		
		cvCamShift( backproject, track_window,cvTermCriteria( CV_TERMCRIT_EPS | CV_TERMCRIT_ITER, 10, 0.1 ),
			&track_comp, &track_box );
		
		CvPoint p = cvPoint(cvRound(track_box.center.x), cvRound(track_box.center.y));
		int width = cvRound(track_box.size.width);
		int height = cvRound(track_box.size.height);
		//printf("%.2f\n",track_box.angle);
		cvCircle(frame,p,width,CV_RGB(255,0,255),2); 
		cvLine(frame,cvPoint(p.x-width,p.y),cvPoint(p.x+width,p.y),CV_RGB(255,0,255),1);
		cvLine(frame,cvPoint(p.x,p.y-width),cvPoint(p.x,p.y+width),CV_RGB(255,0,255),1);
		cvLine( frame, p, cvPoint( cvRound( p.x + 25*cos(track_box.angle*CV_PI/180)),
			cvRound( p.y - 25*sin(track_box.angle*CV_PI/180))), CV_RGB(255,0,255), 2, CV_AA, 0 );
		
		if(gap++ % 5 == 0){
			if(last_position.x != 0 && last_position.y != 0){
				distance = sqrtf(powf((p.x - last_position.x),2) + powf((p.y - last_position.y),2));
				sprintf(text,"%.0f",distance);
			}

			last_position.x = p.x;
			last_position.y = p.y;

		}
		cvPutText( frame, text, cvPoint( cvRound( p.x + 27*cos(track_box.angle*CV_PI/180)),
					cvRound( p.y - 27*sin(track_box.angle*CV_PI/180))), &font, CV_RGB(0,255,0));
		
		//cvEllipseBox( frame, track_box, CV_RGB(255,255,0), 1, CV_AA, 0 );
		cvShowImage( "monitor", frame );
		cvWriteFrame(videoWriter,frame);
        track_window= track_comp.rect;
		
		
		if(cvWaitKey(30)>0) break; //这个必须要有，否则只有灰色窗口,数值可以控制 播放速度
	}
}

int main()
{
	cvNamedWindow( "monitor", 1 );
	
	count_object();
	orient_speed();
	lock_on();

	cvReleaseVideoWriter(&videoWriter);
	return 0;
}