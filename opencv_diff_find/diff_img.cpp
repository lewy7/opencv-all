#include <stdlib.h>
#include <stdio.h>
#include "cv.h"
#include "highgui.h"

#pragma comment(lib,"cv.lib")
#pragma comment(lib,"cxcore.lib")
#pragma comment(lib,"highgui.lib")

void ps_diff()
{
	IplImage* imageA = cvLoadImage("64_ps.jpg", CV_LOAD_IMAGE_GRAYSCALE);
	IplImage* imageB = cvLoadImage("64.jpg", CV_LOAD_IMAGE_GRAYSCALE);
	
	IplImage *dst = cvCreateImage(cvGetSize(imageA),8,1);
	cvNamedWindow("monitor",CV_WINDOW_AUTOSIZE);
	
	cvAbsDiff(imageA,imageB,dst);
	int slider_pos = 5; 
	cvThreshold(dst,dst,slider_pos,255,CV_THRESH_BINARY);
	
	int an = 5;
	IplConvKernel* element = cvCreateStructuringElementEx( an*2+1, an*2+1, an, an, CV_SHAPE_RECT, 0 );
	cvDilate(dst,dst,element,1);
	//cvErode(dst,dst,element,1);
	
	CvMemStorage* storage = cvCreateMemStorage(0);
	CvSeq* contour = NULL;
	cvFindContours( dst, storage, &contour);
	
	imageA = cvLoadImage("64_ps.jpg", CV_LOAD_IMAGE_COLOR);
	imageB = cvLoadImage("64.jpg", CV_LOAD_IMAGE_COLOR);

	CvRect rect;
	for( ; contour != 0; contour = contour->h_next ){
		rect = cvBoundingRect(contour,0);
		cvRectangle( imageA, cvPoint(rect.x,rect.y),cvPoint(rect.x+rect.width,rect.y+rect.height),CV_RGB(0,255,0), 1, 8, 0 );
		cvRectangle( imageB, cvPoint(rect.x,rect.y),cvPoint(rect.x+rect.width,rect.y+rect.height),CV_RGB(0,255,0), 1, 8, 0 );
	}
	
	cvNamedWindow("monitorA",CV_WINDOW_AUTOSIZE);
	cvShowImage("monitorA",imageA);
	cvNamedWindow("monitorB",CV_WINDOW_AUTOSIZE);
	cvShowImage("monitorB",imageB);

	cvWaitKey();

}


void find_partx()
{
	IplImage *image = cvLoadImage("63.jpg", CV_LOAD_IMAGE_COLOR);//如果想要加速，可以用灰色来操作,灰色匹配很差
	IplImage *part = cvLoadImage("63_part_large.jpg", CV_LOAD_IMAGE_COLOR);//一般尺寸大小差距20%可以找出来
	cvNamedWindow("monitor",CV_WINDOW_AUTOSIZE);


	IplImage *part_temp ;

	int i_width;
	int i_height;
	double min_val;
	double max_val;
	CvPoint min_loc;
	CvPoint max_loc;
	IplImage *ftmp,*diff,*diff_gray;
	
	CvRect rect;
	CvRect minRect;
	double sum ;
	double lastSum  = -1;
	double scale = 1.1;
	int width = part->width;
	int height = part->height;
	
	int change_way = 0; //先缩小查找，再放大查找

	while(1){
		part_temp = cvCreateImage(cvSize( cvRound (width),cvRound (height)),8, 3);
		
		printf("%d %d \n",part_temp->width,part_temp->height); //fix size 81*65

		
		cvResize( part, part_temp, CV_INTER_LINEAR );
		i_width = image->width - part_temp->width + 1;
		i_height = image->height - part_temp->height + 1;

		ftmp = cvCreateImage( cvSize(i_width, i_height), 32, 1 );
		cvMatchTemplate( image, part_temp, ftmp, CV_TM_SQDIFF); //CV_TM_SQDIFF_NORMED  CV_TM_CCORR CV_TM_CCOEFF... totoal 6 methon
		cvMinMaxLoc(ftmp, &min_val, &max_val, &min_loc, &max_loc, NULL);
		rect = cvRect(min_loc.x, min_loc.y,part_temp->width,part_temp->height);
		cvSetImageROI(image,rect);
		diff = cvCreateImage( cvGetSize(part_temp), 8, 3 );
		diff_gray = cvCreateImage( cvGetSize(part_temp), 8, 1 );
		cvAbsDiff(image,part_temp,diff);
		cvResetImageROI(image);
		cvCvtColor(diff,diff_gray,CV_BGR2GRAY);
		sum = cvSum(diff_gray).val[0]/(part_temp->width*part_temp->height);
		printf("sum %.2f\n",sum);
		if(lastSum < 0){
			lastSum  = sum;
			minRect.x = rect.x;
			minRect.y = rect.y;
			minRect.width = rect.width;
			minRect.height = rect.height;
		}else if(lastSum > sum){
			lastSum  = sum;
			minRect.x = rect.x;
			minRect.y = rect.y;
			minRect.width = rect.width;
			minRect.height = rect.height;
		}

		//这里画图会干扰图像
		//cvRectangle(image, cvPoint(min_loc.x, min_loc.y), cvPoint((min_loc.x + part_temp->width),(min_loc.y + part_temp->height)), CV_RGB(0,255,0), 1);
		
		
		cvReleaseImage(&diff);
		cvReleaseImage(&diff_gray);
		cvReleaseImage(&ftmp);
		cvReleaseImage(&part_temp);
		
		
		if(change_way == 0){
			width = cvRound (width/scale);
            height = cvRound (height/scale);
		}else{
            width = cvRound (width*scale);
			height = cvRound (height*scale);
		}
		
		if(width >= image->width || height >= image->height ||  width <= 50 || height <= 50 ){
			//break;
			if(change_way == 0){
				change_way = 1;
				width = part->width;
				height = part->height;
				printf("change  way \n");
				continue;
			}

			break;
		}
			
	}
	
	cvRectangle(image, cvPoint(minRect.x, minRect.y), cvPoint((minRect.x + minRect.width),(minRect.y + minRect.height)), CV_RGB(255,255,0), 1);
	printf("match ====== : %.2f =====",lastSum);	
	cvShowImage("monitor",image);
	cvWaitKey(0);
}


void find_part()
{
	IplImage *image = cvLoadImage("754.jpg", CV_LOAD_IMAGE_COLOR);//如果想要加速，可以用灰色来操作,灰色匹配很差
	IplImage *part = cvLoadImage("754_part.jpg", CV_LOAD_IMAGE_COLOR);//一般尺寸大小差距20%可以找出来
	cvNamedWindow("monitor",CV_WINDOW_AUTOSIZE);
	IplImage *part_temp ;

	int i_width;
	int i_height;
	double min_val;
	double max_val;
	CvPoint min_loc;
	CvPoint max_loc;
	IplImage *ftmp,*diff,*diff_gray;
	
	CvRect rect;
	CvRect minRect;
	double sum ;
	double lastSum  = -1;
	double scale = 1.05;
	int width = part->width;
	int height = part->height;
	
	int change_way = 0; //先缩小查找，再放大查找
	int writerSpeed = 5; 
	CvVideoWriter* videoWriter = cvCreateVideoWriter("picture.avi",CV_FOURCC('x','v','i','d'),writerSpeed, cvSize(image->width,image->height));

	IplImage *police = cvLoadImage("police.jpg", CV_LOAD_IMAGE_COLOR);
	cvWriteFrame(videoWriter,police);
	cvWriteFrame(videoWriter,police);
	cvWriteFrame(videoWriter,police);
	cvWriteFrame(videoWriter,police);
	cvWriteFrame(videoWriter,police);

	CvFont font;
	char text[8];
	cvInitFont( &font, CV_FONT_HERSHEY_SIMPLEX,0.4, 0.4, 0, 1, CV_AA );

	while(1){
		part_temp = cvCreateImage(cvSize( cvRound (width),cvRound (height)),8, 3);
		
		printf("%d %d \n",part_temp->width,part_temp->height); //fix size 81*65

		
		cvResize( part, part_temp, CV_INTER_LINEAR );
		i_width = image->width - part_temp->width + 1;
		i_height = image->height - part_temp->height + 1;

		ftmp = cvCreateImage( cvSize(i_width, i_height), 32, 1 );
		cvMatchTemplate( image, part_temp, ftmp, CV_TM_SQDIFF); //CV_TM_SQDIFF_NORMED  CV_TM_CCORR CV_TM_CCOEFF... totoal 6 methon
		cvMinMaxLoc(ftmp, &min_val, &max_val, &min_loc, &max_loc, NULL);
		rect = cvRect(min_loc.x, min_loc.y,part_temp->width,part_temp->height);
		cvSetImageROI(image,rect);
		diff = cvCreateImage( cvGetSize(part_temp), 8, 3 );
		diff_gray = cvCreateImage( cvGetSize(part_temp), 8, 1 );
		cvAbsDiff(image,part_temp,diff);
		cvResetImageROI(image);
		cvCvtColor(diff,diff_gray,CV_BGR2GRAY);
		sum = cvSum(diff_gray).val[0]/(part_temp->width*part_temp->height);
		printf("sum %.2f\n",sum);
		if(lastSum < 0){
			lastSum  = sum;
			minRect.x = rect.x;
			minRect.y = rect.y;
			minRect.width = rect.width;
			minRect.height = rect.height;
		}else if(lastSum > sum){
			lastSum  = sum;
			minRect.x = rect.x;
			minRect.y = rect.y;
			minRect.width = rect.width;
			minRect.height = rect.height;
		}

		//这里画图会干扰图像,所以要重新加载,纯属绘图需要，可删除
		sprintf(text,"%.0f",sum);
		cvPutText( image, text, cvPoint(min_loc.x+part_temp->width, min_loc.y), &font, CV_RGB(0,255,0));
		cvRectangle(image, cvPoint(min_loc.x, min_loc.y), cvPoint((min_loc.x + part_temp->width),(min_loc.y + part_temp->height)), 
			CV_RGB(0,255,0), 2);
		cvShowImage("monitor",image);cvWaitKey(10);
		cvWriteFrame(videoWriter,image);
		cvReleaseImage(&image);
		image = cvLoadImage("754.jpg", CV_LOAD_IMAGE_COLOR);
		
		cvReleaseImage(&diff);
		cvReleaseImage(&diff_gray);
		cvReleaseImage(&ftmp);
		cvReleaseImage(&part_temp);
		
		
		if(change_way == 0){
			width = cvRound (width/scale);
            height = cvRound (height/scale);
		}else{
            width = cvRound (width*scale);
			height = cvRound (height*scale);
		}
		
		if(width >= image->width || height >= image->height ||  width <= 20 || height <= 20 ){
			//break;
			if(change_way == 0){
				change_way = 1;
				width = part->width;
				height = part->height;
				printf("change  way \n");
				continue;
			}

			break;
		}
			
	}
	
	sprintf(text,"%.0f",lastSum);
	cvPutText( image, text, cvPoint(minRect.x+minRect.width, minRect.y), &font, CV_RGB(255,255,0));
	cvRectangle(image, cvPoint(minRect.x, minRect.y), cvPoint((minRect.x + minRect.width),(minRect.y + minRect.height)), 
		CV_RGB(255,255,0), 2);
	cvLine(image,cvPoint(minRect.x+(minRect.width/2)-20, minRect.y+minRect.height/2),
				 cvPoint(minRect.x+(minRect.width/2)+20, minRect.y+minRect.height/2),
				 CV_RGB(255,255,0), 2);
	cvLine(image,cvPoint(minRect.x+(minRect.width/2), minRect.y+minRect.height/2-25),
		cvPoint(minRect.x+(minRect.width/2), minRect.y+minRect.height/2+25),
				 CV_RGB(255,255,0), 2);
	printf("match ====== : %.2f =====",lastSum);	
	cvShowImage("monitor",image);
	cvWriteFrame(videoWriter,image);
	cvWriteFrame(videoWriter,image);
	cvWriteFrame(videoWriter,image);
	cvWriteFrame(videoWriter,image);cvWriteFrame(videoWriter,image);
	cvWriteFrame(videoWriter,image);
	cvWriteFrame(videoWriter,image);

	cvWaitKey(0);
	cvSaveImage("output.jpg",image);
	cvReleaseVideoWriter(&videoWriter);
}


int main()
{	
	//ps_diff();
	find_part();
	return 0;
}