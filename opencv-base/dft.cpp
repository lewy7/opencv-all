#include <stdlib.h>
#include <stdio.h>
#include "cv.h"
#include "highgui.h"

#pragma comment(lib,"cv.lib")
#pragma comment(lib,"cxcore.lib")
#pragma comment(lib,"highgui.lib")

void printMat(CvMat* mat)
{
	CvSize size = cvGetSize(mat);
	for (int i=0;i<size.width;i++)
	{
		printf("%0.1f\t",CV_MAT_ELEM(*mat,float,0,i));
	}
	printf("\nsize %d %d\n\n",size.width,size.height); 
}

void printMat32F(CvMat* mat)
{
	CvSize size = cvGetSize(mat);
	for (int i=0;i<size.width;i++)
	{
		printf("%0.1f\t",CV_MAT_ELEM(*mat,double,0,i));
	}
	printf("\nsize %d %d\n\n",size.width,size.height); 
}

void printMat8U(CvMat* mat)
{
	CvSize size = cvGetSize(mat);
	for (int i=0;i<size.width;i++)
	{
		printf("%02x\t",CV_MAT_ELEM(*mat,unsigned char,0,i));
	}
	printf("\nsize %d %d\n\n",size.width,size.height); 
}


void seq_demo()
{
	float a[12] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12 };
	CvMemStorage* storage = cvCreateMemStorage();
	CvSeq* seq = cvCreateSeq(0, sizeof(CvSeq), sizeof(float), storage );

	int i;
	for( i = 0; i < 12; i++ )
	{
		cvSeqPush( seq, &a[i] );
	}
	float* b;
	for( i = 0; i < seq->total; i++ )
	{
		b = (float*)cvGetSeqElem(seq,i);
		printf("%f\t",*b);
	}
	printf("\ntotal %d \n",seq->total);
	cvReleaseMemStorage(&storage);
}

void drawDft(CvMat* mat)
{
	
	CvFont font;
	char text[8];
	cvInitFont( &font, CV_FONT_HERSHEY_SIMPLEX,0.3, 0.3, 0, 1, CV_AA );

	IplImage *img = cvCreateImage(cvSize(1024,600),8,3);
	cvZero(img);

	double max,min;
	CvPoint maxloc;
	cvMinMaxLoc(mat,&min,&max,0,&maxloc);
	printf("max %f min %f %d\n",max,min,maxloc.x);
	
	//ȥ����ֵ
	CvMat *temp = cvCreateMat(mat->rows,mat->cols,mat->type);
	if(min<0){
		cvAddS(mat,cvScalar(-min),temp);
	}else{
		cvCopy(mat,temp);
	}
	printMat32F(temp);

	//ӳ�䵽ͼ������
	cvNormalize(temp,temp,0,img->height,CV_MINMAX);
	//printMat32F(temp);
	//cvConvertScale(temp,temp,img->height);
	printMat32F(temp);
	
	
	int offset = 10;
	int distance = 20;
	CvPoint xy = cvPoint(0,0);
	int hr = cvRound(img->height/(max-min)*max);
	cvLine(img,cvPoint(0,hr),cvPoint(img->width,hr),CV_RGB(0,255,0),1);
	cvNamedWindow("monitor",CV_WINDOW_AUTOSIZE);
	for(int i=0;i<mat->width;i++){
		xy = cvPoint(offset+i*distance,img->height-cvRound(CV_MAT_ELEM(*temp,double,0,i)));
		if(i==maxloc.x){
			cvLine(img,xy,cvPoint(offset+i*distance,hr),CV_RGB(0,255,255),3);
		}else{
			cvLine(img,xy,cvPoint(offset+i*distance,hr),CV_RGB(255,255,255),3);
		}
		
		cvCircle(img,xy,3,CV_RGB(255,255,255),2);
		sprintf(text,"%.1f",CV_MAT_ELEM(*mat,double,0,i));
		cvPutText( img, text, cvPoint(xy.x+6,xy.y), &font, CV_RGB(0,255,0));

	}
	
	cvShowImage("monitor",img);

	cvWaitKey();
}

void ploy(CvMat *mat,char *winName)
{
	IplImage *img = cvCreateImage(cvSize(1024,600),8,3);
	cvZero(img);

	double max,min;
	CvPoint maxloc;
	cvMinMaxLoc(mat,&min,&max,0,&maxloc);
	printf("max %f min %f %d\n",max,min,maxloc.x);

	//ȥ����ֵ
	CvMat *temp = cvCreateMat(mat->rows,mat->cols,mat->type);
	if(min<0){
		cvAddS(mat,cvScalar(-min),temp);
	}else{
		cvCopy(mat,temp);
	}
	//ӳ�䵽ͼ������
	cvNormalize(temp,temp,0,img->height,CV_MINMAX);

	int i;
	int offset = 10;
	int distance = 20;
	CvPoint point = cvPoint(offset,img->height-cvRound(CV_MAT_ELEM(*temp,double,0,0))); //��һ����
	cvNamedWindow(winName,CV_WINDOW_AUTOSIZE);
	for (i=1;i<temp->cols;i++)
	{
		cvLine(img,point,cvPoint(offset+i*distance,img->height-cvRound(CV_MAT_ELEM(*temp,double,0,i))),CV_RGB(255,255,255),2);
		point = cvPoint(offset+i*distance,img->height-cvRound(CV_MAT_ELEM(*temp,double,0,i)));
	}

	cvShowImage(winName,img);
	
	
	cvWaitKey();
	cvReleaseMat(&temp);
	cvReleaseImage(&img);

	//cvDestroyWindow(winName);
}


void ploy2(CvMat *mat,char *winName)
{
	CvFont font;
	char text[20];
	cvInitFont( &font, CV_FONT_HERSHEY_SIMPLEX,0.3, 0.3, 0, 1, CV_AA );
	cvNamedWindow(winName,CV_WINDOW_AUTOSIZE);

	IplImage *img = cvCreateImage(cvSize(1024,600),8,3);
	cvZero(img);
	
	double max,min;
	CvPoint maxloc,minloc;
	cvMinMaxLoc(mat,&min,&max,&minloc,&maxloc);
	printf("max %f min %f %d %d\n",max,min,maxloc.x,minloc.x);
	double mid = (min + max) / 2; 

	CvScalar mean,dev; 
	cvAvgSdv(mat,&mean,&dev);
	printf("mean %f dev%f \n",mean.val[0],dev.val[0]);
	int i;
	CvMat *temp = cvCreateMat(mat->rows,mat->cols+1,mat->type);//mat->cols+1 ��Ϊ�˱������ֵ�ĵ��Ա��ܽ�x�������Ļ����
	for(i=0;i<mat->cols;i++){
		CV_MAT_ELEM(*temp,double,0,i) = CV_MAT_ELEM(*mat,double,0,i);
	}
	if(fabs(min) > fabs(max) ){
		CV_MAT_ELEM(*temp,double,0,temp->cols-1) = fabs(min);
	}else{
		CV_MAT_ELEM(*temp,double,0,temp->cols-1) = - max;
	}
	
	//ȥ����ֵ
	if(min<0){
		cvAddS(temp,cvScalar(-min),temp);
	}
	
	//ӳ�䵽ͼ������
	cvNormalize(temp,temp,0,img->height,CV_MINMAX);


	
	int hr = img->height/2;
	cvLine(img,cvPoint(0,hr),cvPoint(img->width,hr),CV_RGB(0,255,0),1);
	sprintf(text,"%.4f",0);
	cvPutText( img, text, cvPoint(img->width-200,hr+3), &font, CV_RGB(0,255,0));
	
	CvMat *mask = cvCreateMat(temp->rows,temp->cols,CV_8UC1);
	cvZero(mask);
	cvConvertScale(mask,mask,1,0xff);
	CV_MAT_ELEM(*mask,unsigned char,0,mask->cols-1) = 0x00;

	CvScalar mean_temp,dev_temp; 
	cvAvgSdv(temp,&mean_temp,&dev_temp,mask);

	double max_temp,min_temp;
	cvMinMaxLoc(temp,&min_temp,&max_temp,0,0,mask);
	int mid_temp = cvRound((abs(min_temp) + abs(max_temp)) / 2); 

	hr = img->height-cvRound(mean_temp.val[0]);
	cvLine(img,cvPoint(0,hr),cvPoint(img->width,hr),CV_RGB(255,255,0),1);
	sprintf(text,"mean %.4f",mean.val[0]);
	cvPutText( img, text, cvPoint(img->width-100,hr-1), &font, CV_RGB(255,255,0));

	hr = img->height-mid_temp;
	cvLine(img,cvPoint(0,hr),cvPoint(img->width,hr),CV_RGB(0,255,255),1);
	sprintf(text,"mid %.4f",mid);
	cvPutText( img, text, cvPoint(img->width-100,hr-1), &font, CV_RGB(0,255,255));
	
	sprintf(text,"dev %.4f",dev.val[0]);
	cvPutText( img, text, cvPoint(img->width-100,20), &font, CV_RGB(255,255,255));
	sprintf(text,"sum %.4f",cvSum(mat).val[0]);
	cvPutText( img, text, cvPoint(img->width-100,30), &font, CV_RGB(255,255,255));
	

	int offset = 10; //x��ƫ��ԭ��
	int distance = 20; //���Ƽ��
	CvPoint point; 
	for (i=0;i<temp->cols-1;i++)
	{
		if(i==0){
			point = cvPoint(offset,img->height-cvRound(CV_MAT_ELEM(*temp,double,0,0))); //��һ����
			if(i==maxloc.x){
				cvCircle(img,point,2,CV_RGB(255,0,255),2);
				sprintf(text,"%.4f",max);
				cvPutText( img, text, cvPoint(point.x+10,point.y+15), &font, CV_RGB(255,0,255));
			}
			if(i==minloc.x){
				cvCircle(img,point,2,CV_RGB(255,0,255),2);
				sprintf(text,"%.4f",min);
				cvPutText( img, text, cvPoint(point.x+10,point.y-3), &font, CV_RGB(0,255,255));
			}
			continue;
		}
		cvLine(img,point,cvPoint(offset+i*distance,img->height-cvRound(CV_MAT_ELEM(*temp,double,0,i))),CV_RGB(255,255,255),2);
		point = cvPoint(offset+i*distance,img->height-cvRound(CV_MAT_ELEM(*temp,double,0,i)));

		if(i==maxloc.x){
			cvCircle(img,point,2,CV_RGB(255,0,255),2);
			sprintf(text,"%.4f",max);
			cvPutText( img, text, cvPoint(point.x+10,point.y+15), &font, CV_RGB(255,0,255));
		}
		if(i==minloc.x){
			cvCircle(img,point,2,CV_RGB(255,0,255),2);
			sprintf(text,"%.4f",min);
			cvPutText( img, text, cvPoint(point.x+10,point.y-3), &font, CV_RGB(255,0,255));
		}
	}
	
	cvShowImage(winName,img);
	
	
	cvWaitKey();
	cvReleaseMat(&temp);
	cvReleaseMat(&mask);
	cvReleaseImage(&img);

	//cvDestroyWindow(winName);
}

void stem(CvMat* mat,char *winName)
{
	IplImage *img = cvCreateImage(cvSize(1024,600),8,3);
	cvZero(img);
	
	int i;
	double max,min;
	CvPoint maxloc,minloc;
	cvMinMaxLoc(mat,&min,&max,&minloc,&maxloc);
	printf("max %f min %f %d\n",max,min,maxloc.x);

	
	CvMat *temp = cvCreateMat(mat->rows,mat->cols+1,mat->type);//mat->cols+1 ��Ϊ�˱������ֵ�ĵ��Ա��ܽ�x�������Ļ����
	for(i=0;i<mat->cols;i++){
		CV_MAT_ELEM(*temp,double,0,i) = CV_MAT_ELEM(*mat,double,0,i);
	}
	if(fabs(min) > fabs(max) ){
		CV_MAT_ELEM(*temp,double,0,temp->cols-1) = fabs(min);
	}else{
		CV_MAT_ELEM(*temp,double,0,temp->cols-1) = - max;
	}

	//ȥ����ֵ
	if(min<0){
		cvAddS(temp,cvScalar(-min),temp);
	}

	//ӳ�䵽ͼ������
	cvNormalize(temp,temp,0,img->height,CV_MINMAX);

	int offset = 10;
	int distance = 20;
	CvPoint xy = cvPoint(0,0);
	int hr = img->height/2;
	cvLine(img,cvPoint(0,hr),cvPoint(img->width,hr),CV_RGB(0,255,0),1);
	cvNamedWindow(winName,CV_WINDOW_AUTOSIZE);
	for(i=0;i<mat->width;i++){
		xy = cvPoint(offset+i*distance,img->height-cvRound(CV_MAT_ELEM(*temp,double,0,i)));
		if(i==maxloc.x){
			cvLine(img,xy,cvPoint(offset+i*distance,hr),CV_RGB(0,255,255),3);
		}else if(i==minloc.x){
			cvLine(img,xy,cvPoint(offset+i*distance,hr),CV_RGB(255,255,0),3);
		}else{
			cvLine(img,xy,cvPoint(offset+i*distance,hr),CV_RGB(255,255,255),3);
		}
		
		cvCircle(img,xy,3,CV_RGB(255,255,255),2);
		//sprintf(text,"%.1f",CV_MAT_ELEM(*mat,double,0,i));
		//cvPutText( img, text, cvPoint(xy.x+6,xy.y), &font, CV_RGB(0,255,0));
		
	}
	
	cvShowImage(winName,img);
	
	cvWaitKey();
	cvReleaseMat(&temp);
	cvReleaseImage(&img);

}

//dft ���˲�������
//�Ա�matlab��������վ������
/*
https://octave-online.net
n=0:31;
xn=cos(2*pi*n*0.1);
stem(n,xn);
fft(xn);
a=fft(xn);
stem(n,real(a));

xn=cos(1*pi*n*0.1);
stem(n,xn);
*/
void dft_demo()
{

	double a[] = {
		// 1.00000,0.80902,0.30902,-0.30902,-0.80902,-1.00000,-0.80902,-0.30902,0.30902,0.80902
		  1.00000, 0.80902, 0.30902,-0.30902,-0.80902,-1.00000,-0.80902,-0.30902
		, 0.30902, 0.80902, 1.00000, 0.80902, 0.30902,-0.30902,-0.80902,-1.00000
		,-0.80902,-0.30902, 0.30902, 0.80902, 1.00000, 0.80902, 0.30902,-0.30902
		,-0.80902,-1.00000,-0.80902,-0.30902, 0.30902, 0.80902, 1.00000, 0.80902
	};

	double b[] = {
		 1,0.95106,0.80902,0.58779,0.30902,0,-0.30902,-0.58779,-0.80902,-0.95106
		,-1,-0.95106,-0.80902,-0.58779,-0.30902,0,0.30902,0.58779,0.80902,0.95106
		,1,0.95106,0.80902,0.58779,0.30902,0,-0.30902,-0.58779,-0.80902,-0.95106
		,-1,-0.95106
	};
	double c[32];
	for(int i=0;i<32;i++)
	{
		c[i] = a[i]+b[i];
	}

	CvMat mat = cvMat(1,sizeof(a)/sizeof(double),CV_64FC1,c);
	CvMat *data = &mat;
	ploy(data,"cԭʼͼ��");
	CvMat* result = cvCreateMat( 1, data->cols, data->type ); 
	cvZero(result);
	cvDFT( data, result, CV_DXT_FORWARD);
	printMat32F(result);
	//drawDft(result); drawDft������
	stem(result,"cƵ����Ϣ");

	//��Ƶ�����bƵ���˵� ,ֻ����ʾ�� ʵ����Ҫ�˲���
	CvMat bMat = cvMat(1,sizeof(b)/sizeof(double),CV_64FC1,b);
	CvMat* bFreq = cvCreateMat( 1, bMat.cols, bMat.type ); 
	cvDFT( &bMat, bFreq, CV_DXT_FORWARD);
	cvSub(result,bFreq,result);//����˵�b
	cvReleaseMat(&bFreq);

	//CV_DXT_INVERSE ��任����Ϊ�����Ѿ��˵�bƵ�ʣ����Խ����ʾ��a��ʱ��ͼ��
	cvDFT( result, result, CV_DXT_INVERSE);
	printMat32F(result);
	ploy(result,"�˲���ͼ��");
	cvReleaseMat(&result);
	cvDestroyAllWindows();
}


void analy_demo()
{
	double a[] = {
		32.0,20.7,7.9,-4.7,-15.6,-23.5,-27.9,-28.6,-25.9,-20.7,-14.0,-6.9,-0.4,4.7, 8.1, 9.7, 9.9, 9.1, 7.9,6.9
		,6.5,6.9, 7.9, 9.1, 9.9, 9.7, 8.1, 4.7, -0.4, -6.9,-14.0,-20.7
	};
	CvMat mat = cvMat(1,sizeof(a)/sizeof(double),CV_64FC1,a);
	CvMat *data = &mat;
	
	ploy2(data,"analy");
	cvDestroyAllWindows();
}

void base_demo()
{
	
    CvMat* mat = cvCreateMat( 1, 9, CV_32FC1 ); // 32FC1 : 32=32bit  F=float  C1=1channel
	cvZero(mat);
	
	CV_MAT_ELEM(*mat,float,0,3) = 1.2f;
	printf("[0,3] = %f\n",CV_MAT_ELEM(*mat,float,0,3));
	
	printMat(mat);
	cvReleaseMat(&mat);
	
	float a[] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
	CvMat matInit = cvMat(1,10,CV_32FC1,a); //ע�⣬����û���Ǻ�ָ��
	printMat(&matInit);
	//cvReleaseMat(&mat);�����ͷ�
	
	
	CvMat* matA = cvCloneMat(&matInit);
	CvMat* matB = cvCreateMat( 1, 10, CV_32FC1 ); 
	CvMat* result = cvCreateMat( 1, 10, CV_32FC1 ); 
	
	
	
	printf("cvConvertScale\n");
	cvConvertScale(matA,matB,5);//ÿ��Ԫ�س���5
	printMat(matB);
	
	printf("cvAbsDiff\n");
	cvAbsDiff(matA,matB,result);//��������Ԫ�ز�ľ���ֵ
	printMat(result);
	
	printf("cvNormalize\n");
	cvNormalize( matA, result, 0, 255, CV_MINMAX );//�߶Ȼ� matA��С��1���0,����12���255�����������������仯
	printMat(result);
	
	
	printf("mask\n");
	CvMat* mask = cvCreateMat( 1, 10,  CV_8UC1 ); //���� ������CV_8UC1���� 8UC1 : 8=8bit  U=unsigned  C1=1channel
	cvSetZero(mask);
	cvSetZero(result);
	CV_MAT_ELEM(*mask,unsigned char,0,0) = 5;//������mask[0]��0,����ֻ����matA[0]-matB[0],������浽result[0],result���಻��
	cvSub(matA,matB,result,mask);
	printMat(matA);
	printMat(matB);
	printMat(result);
	
	
	
	
	printf("cvInRangeS\n");
	unsigned char b[] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
	matInit = cvMat(1,10,CV_8UC1,b);
	CvMat* gray = &matInit; 
	CvMat* range = cvCreateMat( 1, 10,  CV_8UC1 ); 
	cvInRangeS(gray,cvScalar(3),cvScalar(7),range);//matAԪ��ֵ��С��[3,7)(����3,����7)֮���,������Ϊ0xFF,��֮Ϊ0x00
	printMat8U(gray);
	printMat8U(range);
	
	printf("And\n");
	cvAnd(gray,range,gray);//���� ��grayԪ���а�ֵ��[3,7)��Χ���Ԫ����0x00,���Ҫ��ֵ��[3,7)��0������ִ��cvNot(range,range);
	printMat8U(gray);


}
int main(int argc, char ** argv)
{

	//base_demo();
	//seq_demo();
	dft_demo();
	analy_demo();

	
	
    return 0;
}
