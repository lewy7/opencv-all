#include <stdlib.h>
#include <stdio.h>
#include "cv.h"
#include "highgui.h"

#pragma comment(lib,"cv.lib")
#pragma comment(lib,"cxcore.lib")
#pragma comment(lib,"highgui.lib")

#define MAX_CORNER 100

//因为通过cvGoodFeaturesToTrack找出角点，适合跟踪人体动作，复杂的图像找出的角点往往不准
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
			IplImage *eig = cvCreateImage(cvGetSize(frame),32,1); //这为什么是32位的。我X
			IplImage *temp = cvCreateImage(cvGetSize(frame),32,1);
			double quality = 0.01;
			double min_distance = 10;
		/*
		找出最容易跟踪的角点
		void cvGoodFeaturesToTrack( const CvArr* image, CvArr* eig_image, CvArr* temp_image,
					CvPoint2D32f* corners, int* corner_count,
					double quality_level, double min_distance,
					const CvArr* mask=NULL, int block_size=3,
                    int use_harris=0, double k=0.04 );
				image : 原始
				eig_image,temp_image  : 临时变量，好像没什么 用
				corners : 得到的点
				corner_count : 想要得到的最多点数 和返回实际的点数
				quality_level : 角点的最小特征值 不应超过1 一半是 0.1或0.01
				min_distance : 角点之间的距离不应小于 min_distance 像素

		*/
 			cvGoodFeaturesToTrack( grey, eig, temp, points[1], &count,quality, min_distance);
		
		/*
		查找亚像素精度角点的位置,有时峰值不是正好落在某像素上，而是落在像素和像素之间，这像素点和像素点之间的距离是亚像素
		void cvFindCornerSubPix( const CvArr* image, CvPoint2D32 f* corners,int count, 
						CvSize win, CvSize zero_zone, CvTermCriteria criteria );
					image，points ，count : 和上面那个一样意思
					win : 等式产生窗口的尺寸
					zero_zone : 禁区，一般不需要 直接 cvSize(-1,-1)
					criteria : 终止条件 亚像素的精度

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
					prev : 上一幅图像  单通道
					curr : 当前图像
					prev_pyramid  , curr_pyr : 上一幅 金字塔图和当前金字塔图
					curr_features , curr_features :  上一幅 的角点和当前角点
					count : 角点数量 
					win_size : 局部连续运动的窗口尺寸
					level : 金字塔栈的层数 0表示不使用金字塔
					status: 每个元素被置成1或0  1 上一幅图中对应的点在当前图找到 0 上一幅图中对应的点在当前图没找到
					track_error : 被跟踪点的在上一幅图中和当期图中的区域差数组
					criteria : 迭代中止条件
					flags : 函数内部的控制 不知道干嘛的

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
		
		if(cvWaitKey(500)>0) break; //这个必须要有，否则只有灰色窗口,数值可以控制播放速度
	}

}

//可以从物体和历史位置的连线查看加速的状态，但是由于角点自动选择的问题，效果还是不太理想
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
			cvGoodFeaturesToTrack( prev_gray, eig, temp, prev_corner, &count, 0.01, 5);//后两个是特征值和最小距离
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
		if(cvWaitKey(200)>0) break; //这个必须要有，否则只有灰色窗口,数值可以控制播放速度
	}

}

//注意，这个是根据颜色定位的，所以需要定位的物体颜色一定要有特点 可以配合floodfill填充
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

	CvRect track_window = {608,204,60,60};//初始搜索框非常重要，这个要先在大图片上定位好，不然搜不到
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


		//cvEllipseBox( frame, track_box, CV_RGB(255,255,0), 1, CV_AA, 0 );
		cvShowImage( "monitor", frame );
        track_window= track_comp.rect;


		if(cvWaitKey(50)>0) break; //这个必须要有，否则只有灰色窗口,数值可以控制 播放速度
	}
}

int main()
{
	

	//lucas_kanade();
	lucas_kanade2(); //这个好用


	camShift();
	return 0;
	
}