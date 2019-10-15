#include <stdlib.h>
#include <stdio.h>
#include "cv.h"
#include "highgui.h"

#pragma comment(lib,"cv.lib")
#pragma comment(lib,"cxcore.lib")
#pragma comment(lib,"highgui.lib")

#define MAX_CORNER 100

//��Ϊͨ��cvGoodFeaturesToTrack�ҳ��ǵ㣬�ʺϸ������嶯�������ӵ�ͼ���ҳ��Ľǵ�������׼
void lucas_kanade()
{
	CvCapture* capture = cvCaptureFromAVI( "vietnam_xvid.avi" );
	cvNamedWindow( "monitor", 1 );
	
	IplImage *frame, *image = 0,*grey, *prev_grey =0, *pyramid = 0, *prev_pyramid = 0, *swap_temp;
	CvPoint2D32f* points[2] = {0,0},*swap_points;
	
	int win_size = 10;
	char* status = 0;
	int flags = 0;
	frame=cvQueryFrame(capture);
	int i,k;
	int MAX_COUNT = 50;
	int count = MAX_COUNT;
	int first_flag=1;
	while(1)  
	{  
		frame=cvQueryFrame(capture);
		if(!frame)
		{
			printf("get frame error\n");
			break;
		}
		//cvShowImage("monitor",frame);
		if(!image){
			image = cvCreateImage( cvGetSize(frame),8,3);
			image->origin = frame->origin;
			grey = cvCreateImage(cvGetSize(frame),8,1);
			prev_grey = cvCreateImage( cvGetSize(frame), 8, 1 );
            pyramid = cvCreateImage( cvGetSize(frame), 8, 1 );
            prev_pyramid = cvCreateImage( cvGetSize(frame), 8, 1 );
			
			points[0] = (CvPoint2D32f*)cvAlloc(MAX_COUNT*sizeof(points[0][0]));
			points[1] = (CvPoint2D32f*)cvAlloc(MAX_COUNT*sizeof(points[0][0]));
			status = (char*)cvAlloc(MAX_COUNT);
			flags = 0;

		}
		cvCopy( frame, image, 0 );
		cvCvtColor( image, grey, CV_BGR2GRAY );
		
		if(first_flag == 1){
			IplImage *eig = cvCreateImage(cvGetSize(frame),32,1); //��Ϊʲô��32λ�ġ���X
			IplImage *temp = cvCreateImage(cvGetSize(frame),32,1);
			double quality = 0.01;
			double min_distance = 10;
		/*
		�ҳ������׸��ٵĽǵ�
		void cvGoodFeaturesToTrack( const CvArr* image, CvArr* eig_image, CvArr* temp_image,
					CvPoint2D32f* corners, int* corner_count,
					double quality_level, double min_distance,
					const CvArr* mask=NULL, int block_size=3,
                    int use_harris=0, double k=0.04 );
				image : ԭʼ
				eig_image,temp_image  : ��ʱ����������ûʲô ��
				corners : �õ��ĵ�
				corner_count : ��Ҫ�õ��������� �ͷ���ʵ�ʵĵ���
				quality_level : �ǵ����С����ֵ ��Ӧ����1 һ���� 0.1��0.01
				min_distance : �ǵ�֮��ľ��벻ӦС�� min_distance ����

		*/
 			cvGoodFeaturesToTrack( grey, eig, temp, points[1], &count,quality, min_distance);
		
		/*
		���������ؾ��Ƚǵ��λ��,��ʱ��ֵ������������ĳ�����ϣ������������غ�����֮�䣬�����ص�����ص�֮��ľ�����������
		void cvFindCornerSubPix( const CvArr* image, CvPoint2D32 f* corners,int count, 
						CvSize win, CvSize zero_zone, CvTermCriteria criteria );
					image��points ��count : �������Ǹ�һ����˼
					win : ��ʽ�������ڵĳߴ�
					zero_zone : ������һ�㲻��Ҫ ֱ�� cvSize(-1,-1)
					criteria : ��ֹ���� �����صľ���

		*/
			cvFindCornerSubPix( grey, points[1], count,cvSize(win_size,win_size), cvSize(-1,-1),cvTermCriteria(CV_TERMCRIT_ITER|CV_TERMCRIT_EPS,20,0.03));
			cvReleaseImage( &eig );
			cvReleaseImage( &temp );

		}
		

		if(first_flag == 0){
		/*
		void cvCalcOpticalFlowPyrLK( const CvArr* prev, const CvArr* curr, CvArr* prev_pyr, CvArr* curr_pyr,
						const CvPoint2D32f* prev_features, CvPoint2D32f* curr_features,int count, 
						CvSize win_size, int level, char* status,
                        float* track_error, CvTermCriteria criteria, int flags );
					prev : ��һ��ͼ��  ��ͨ��
					curr : ��ǰͼ��
					prev_pyramid  , curr_pyr : ��һ�� ������ͼ�͵�ǰ������ͼ
					curr_features , curr_features :  ��һ�� �Ľǵ�͵�ǰ�ǵ�
					count : �ǵ����� 
					win_size : �ֲ������˶��Ĵ��ڳߴ�
					level : ������ջ�Ĳ��� 0��ʾ��ʹ�ý�����
					status: ÿ��Ԫ�ر��ó�1��0  1 ��һ��ͼ�ж�Ӧ�ĵ��ڵ�ǰͼ�ҵ� 0 ��һ��ͼ�ж�Ӧ�ĵ��ڵ�ǰͼû�ҵ�
					track_error : �����ٵ������һ��ͼ�к͵���ͼ�е����������
					criteria : ������ֹ����
					flags : �����ڲ��Ŀ��� ��֪�������

		*/
		
			cvCalcOpticalFlowPyrLK( prev_grey, grey, prev_pyramid, pyramid,
				points[0], points[1], count, cvSize(win_size,win_size), 3, status, 0,
				cvTermCriteria(CV_TERMCRIT_ITER|CV_TERMCRIT_EPS,20,0.03), flags );

            flags |= CV_LKFLOW_PYR_A_READY;
			
			
			for( i = k = 0; i < count; i++ )
			{
				if( !status[i] )
					continue;
				printf("count = %d  [%.2f,%.2f] \n",count,points[1][i].x,points[1][i].y);
				points[1][k++] = points[1][i];

				cvCircle( image, cvPointFrom32f(points[1][i]), 10, CV_RGB(0,255,255), 1, 8,0);
			}
			count = k;
			
		}
		first_flag = 0;
		CV_SWAP( prev_grey, grey, swap_temp );
        CV_SWAP( prev_pyramid, pyramid, swap_temp );
        CV_SWAP( points[0], points[1], swap_points );
		cvShowImage( "monitor", image );
		
		if(cvWaitKey(500)>0) break; //�������Ҫ�У�����ֻ�л�ɫ����,��ֵ���Կ��Ʋ����ٶ�
	}

}

//���Դ��������ʷλ�õ����߲鿴���ٵ�״̬���������ڽǵ��Զ�ѡ������⣬Ч�����ǲ�̫����
void lucas_kanade2()
{
	CvCapture* capture = cvCaptureFromAVI( "vietnam_xvid.avi" );
	cvNamedWindow( "monitor", 1 );
	IplImage *frame=cvQueryFrame(capture);
	
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

	while(1)  
	{  
		frame=cvQueryFrame(capture);
		
		if(!frame)
		{
			printf("get frame error\n");
			break;
		}
		//ÿ��10֡����һ�£���������
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
			
			IplImage *eig = cvCreateImage(cvGetSize(frame),32,1); //��Ϊʲô��32λ�ġ���X
			IplImage *temp = cvCreateImage(cvGetSize(frame),32,1);

			count = MAX_CORNER;
			cvGoodFeaturesToTrack( prev_gray, eig, temp, prev_corner, &count, 0.01, 5);//������������ֵ����С����
			cvFindCornerSubPix( prev_gray, prev_corner, count,cvSize(win_size,win_size), cvSize(-1,-1),cvTermCriteria(CV_TERMCRIT_ITER|CV_TERMCRIT_EPS,20,0.03));//���������ȥ��������ҲûʲôӰ��
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
		
			CvFont font;
			char text[8];
			cvInitFont( &font, CV_FONT_HERSHEY_SIMPLEX,0.4, 0.4, 0, 1, CV_AA );
			float distance;
		for(int i=0;i<count;i++){
			if(corner_found[i] == 0){
				printf("corner lost\n");
				continue;
			}
				
			CvPoint p0 = cvPoint(cvRound(prev_corner[i].x), cvRound(prev_corner[i].y));
			CvPoint p1 = cvPoint(cvRound(curr_corner[i].x), cvRound(curr_corner[i].y));
			printf("%d [%d,%d]\n",count,p1.x,p1.y);
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
		if(cvWaitKey(200)>0) break; //�������Ҫ�У�����ֻ�л�ɫ����,��ֵ���Կ��Ʋ����ٶ�
	}

}

//ע�⣬����Ǹ�����ɫ��λ�ģ�������Ҫ��λ��������ɫһ��Ҫ���ص� �������floodfill���
void camShift()
{
	CvCapture* capture = cvCaptureFromAVI( "vietnam_xvid.avi" );
	cvNamedWindow( "monitor", 1 );
	IplImage *frame=cvQueryFrame(capture);

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

	CvRect track_window = {608,204,60,60};//��ʼ������ǳ���Ҫ�����Ҫ���ڴ�ͼƬ�϶�λ�ã���Ȼ�Ѳ���
	CvBox2D track_box;
	CvConnectedComp track_comp;
	
	while(1)  
	{  
		frame=cvQueryFrame(capture);
		
		if(!frame)
		{
			printf("get frame error\n");
			break;
		}
		
		//cvShowImage( "monitor", frame );
		cvCvtColor(frame,image,CV_BGR2HSV);
		
		cvSplit(image,image_hue,0,0,0);
		//cvShowImage( "monitor", image_hue );
		
		/*
			���ݻ�õ�ɫ��ֱ��ͼ��ԭʼͼ��ת����ɫ�ʸ��ʷֲ�ͼ��������̾ͱ�����"Back Projection"
			void cvCalcBackProjectPatch( IplImage** images, CvArr* dst,
			CvSize patch_size, CvHistogram* hist,int method, float factor );
			1. IplImage** img:���ԭʼͼ�����롣
			2. CvArr** backproject:���Back Projection����������
			3. CvHistogram* hist:���ֱ��ͼ������
		*/ 
		cvCalcBackProject(&image_hue,backproject,hist);
		backproject->origin = frame->origin;
		//cvShowImage( "monitor", backproject );
		
		/*
		cvCamShift�㷨
		1.ѡ�񴰵Ĵ�С�ͳ�ʼλ��.
		2.�����ʱ�����ڵ�Mass Center.
		3.�������ڵ����ĵ�Mass Center.
		4.�ظ�2��3��ֱ����������"���"����ÿ�δ����ƶ��ľ���С��һ������ֵ��

		cvCamShift��������Ƶÿһ֡���cvMeanShift��ѭ��������
		int cvCamShift( const CvArr* prob_image, CvRect window, CvTermCriteria criteria,
                CvConnectedComp* comp, CvBox2D* box=NULL );
			prob_image �� backproject ͼ��
			window     : �״��������� �Ը���ͼ����ҳɹ��ǳ���Ҫ
			criteria  : ��Ѱ��ֹ����
			comp ������������,�����µ�Search Window��λ�ú������
             box�������������������С���Ρ�
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


		//cvEllipseBox( frame, track_box, CV_RGB(255,255,0), 1, CV_AA, 0 );
		cvShowImage( "monitor", frame );
        track_window= track_comp.rect;


		if(cvWaitKey(50)>0) break; //�������Ҫ�У�����ֻ�л�ɫ����,��ֵ���Կ��� �����ٶ�
	}
}

int main()
{
	

	//lucas_kanade();
	lucas_kanade2(); //�������


	camShift();
	return 0;
	
}