#include <stdlib.h>
#include <stdio.h>
#include "cv.h"
#include "highgui.h"

#pragma comment(lib,"cv.lib")
#pragma comment(lib,"cxcore.lib")
#pragma comment(lib,"highgui.lib")

//开运算(先erode，再dilate)是消除凸点
void open_calc()
{
	IplImage* image = cvLoadImage("sharp.jpg", CV_LOAD_IMAGE_COLOR);
	cvNamedWindow( "origon", 1 );
    cvShowImage( "origon", image );
	
	int an = 10;
	IplConvKernel* element = cvCreateStructuringElementEx( an*2+1, an*2+1, an, an, CV_SHAPE_RECT, 0 );

	IplImage* erode = cvCreateImage(cvGetSize(image),8,3);
	cvErode(image,erode,element,1);
	cvNamedWindow( "erode", 1 );cvShowImage("erode", erode);

	IplImage* dilate = cvCreateImage(cvGetSize(image),8,3);
	cvDilate(erode,dilate,element,1);
	cvNamedWindow( "dilate", 1 );cvShowImage("dilate", dilate);

    cvWaitKey(0);

}


//闭运算(先dilate，再erode)是消除凹点
void close_calc()
{
	IplImage* image = cvLoadImage("sharp.jpg", CV_LOAD_IMAGE_COLOR);
	cvNamedWindow( "origon", 1 );
    cvShowImage( "origon", image );
	
	int an = 10;
	IplConvKernel* element = cvCreateStructuringElementEx( an*2+1, an*2+1, an, an, CV_SHAPE_RECT, 0 );

	IplImage* dilate = cvCreateImage(cvGetSize(image),8,3);
	cvDilate(image,dilate,element,1);
	cvNamedWindow( "dilate", 1 );cvShowImage("dilate", dilate);

	
	IplImage* erode = cvCreateImage(cvGetSize(image),8,3);
	cvErode(dilate,erode,element,1);
	cvNamedWindow( "erode", 1 );cvShowImage("erode", erode);
	
	
	
    cvWaitKey(0);
	
}



//tophat  = src - open(src),即把open消除的凸点给取出来
//blackhat = close(src) - src,即把close消除的凹点给取出来；blackhat类似，故不实现了
void tophat()
{
	IplImage* image = cvLoadImage("sharp.jpg", CV_LOAD_IMAGE_GRAYSCALE);
	cvNamedWindow( "origon", 1 );
    cvShowImage( "origon", image );
	
	int an = 10;
	IplConvKernel* element = cvCreateStructuringElementEx( an*2+1, an*2+1, an, an, CV_SHAPE_RECT, 0 );

	IplImage* erode = cvCreateImage(cvGetSize(image),8,1);
	cvErode(image,erode,element,1);
	cvNamedWindow( "erode", 1 );cvShowImage("erode", erode);

	IplImage* dilate = cvCreateImage(cvGetSize(image),8,1);
	cvDilate(erode,dilate,element,1);
	cvNamedWindow( "dilate", 1 );cvShowImage("dilate", dilate);


	IplImage* tophat = cvCreateImage(cvGetSize(image),8,1);
	//cvSub(image,dilate,tophat); //如果减成负值怎么办？用cvAbsDiff
	cvAbsDiff(image,dilate,tophat);
	cvNamedWindow( "tophat", 1 );cvShowImage("tophat", tophat);

    cvWaitKey(0);
	
}


void diff()
{
	IplImage* image1 = cvLoadImage("sharp.jpg", CV_LOAD_IMAGE_GRAYSCALE);
	IplImage* image2 = cvLoadImage("sharp0.jpg", CV_LOAD_IMAGE_GRAYSCALE);
	IplImage* diff = cvCreateImage(cvGetSize(image1),8,1);
	cvAbsDiff(image1,image2,diff);
	cvNamedWindow( "diff", 1 );
    cvShowImage( "diff", diff );
	cvWaitKey(0);
}

//同理cvSubS减掉
void adds()
{
	IplImage* image = cvLoadImage("pill.jpg", CV_LOAD_IMAGE_COLOR);
	IplImage* result = cvCreateImage(cvGetSize(image),8,3);
	cvAddS(image,cvScalar(0,0,100),result);
	cvNamedWindow( "adds", 1 );
    cvShowImage( "adds", result );
	cvWaitKey(0);
}

//图像金字塔采样
void cvPyrDownUp()
{
	IplImage* image = cvLoadImage("pill.jpg", CV_LOAD_IMAGE_COLOR);
	CvSize sz = cvSize( image->width & -2, image->height & -2 ); //确保能被2整除

	IplImage* pyrDown = cvCreateImage( cvSize(sz.width/2, sz.height/2), 8, 3 );
	cvPyrDown(image,pyrDown); //向下采样 即尺寸减半
	cvNamedWindow( "cvPyrDown", CV_WINDOW_AUTOSIZE);
	cvShowImage("cvPyrDown",pyrDown);

    IplImage* pyrUp = cvCreateImage( sz , 8, 3 );
	cvPyrUp(pyrDown,pyrUp); //向上采样 即尺寸加倍
	cvNamedWindow( "cvPyrUp", CV_WINDOW_AUTOSIZE);
	cvShowImage("cvPyrUp",pyrUp);
	
	cvWaitKey();
}

void dark()
{
	IplImage* image = cvLoadImage("pill.jpg", CV_LOAD_IMAGE_COLOR);
	//药片中红色最强
	IplImage* red = cvCreateImage(cvGetSize(image),8,1);
	IplImage* blue = cvCreateImage(cvGetSize(image),8,1);
	IplImage* green = cvCreateImage(cvGetSize(image),8,1);
	cvSplit(image,red,blue,green,0);
	cvNamedWindow("red", CV_WINDOW_AUTOSIZE);
	cvShowImage("red",red);

	IplImage* dst = cvCreateImage(cvGetSize(image),8,1);
	
	cvCmpS(red,18.0,dst,CV_CMP_LT);//将红色小于18的找出来，在dst里的结果是1

	IplImage* tmp = cvCreateImage(cvGetSize(image),8,1);
	cvConvertScale(dst, tmp, 0xff);
	cvAdd(red,tmp,red);
	cvConvertScale(dst, tmp, 0xff);
	cvAdd(blue,tmp,blue);
	cvConvertScale(dst, tmp, 0x00);
	cvAdd(green,tmp,green);
	cvMerge(red, blue, green, 0 , image);
	cvNamedWindow( "dark", CV_WINDOW_AUTOSIZE);
	cvShowImage("dark",image);
	cvWaitKey();
	
	return;

	IplImage* dark = cvCreateImage(cvGetSize(image),8,3);
	cvConvertScale(dst, dst, -255);
	

	IplImage* r0 = cvCreateImage(cvGetSize(image),8,1);
	IplImage* b0 = cvCreateImage(cvGetSize(image),8,1);
	IplImage* g0 = cvCreateImage(cvGetSize(image),8,1);
	cvConvertScale(dst, r0, 0x9d);
	cvConvertScale(dst, b0, 0xa0);
	cvConvertScale(dst, g0, 0x99);

	dark = cvCreateImage(cvGetSize(image),8,3);
	cvMerge(r0, b0, g0, 0 , dark);
	cvAdd(image,dark,dark);

	cvNamedWindow( "dark", CV_WINDOW_AUTOSIZE);
	cvShowImage("dark",dark);
	cvWaitKey();

}


void canny(){
	//cvCanny操作的是单通道
	IplImage* image = cvLoadImage("car.jpg", CV_LOAD_IMAGE_GRAYSCALE);

	/*
	void cvCanny( const CvArr* image, CvArr* edges, double threshold1,
              double threshold2, int aperture_size=3 );
			threshold1 : 阀值，数值越大，边缘数量越少，即画面越干净
			threshold2 : 一般为threshold1的3-5倍
			aperture_size : 3或者5，3的画面比5干净
	*/	
	int thresh = 100;
	IplImage* target = cvCreateImage(cvGetSize(image),IPL_DEPTH_8U,1);
	cvCanny( image, target, thresh, thresh*3, 3 );

	cvNamedWindow( "canny", CV_WINDOW_AUTOSIZE);
	cvShowImage("canny",target);
 


	IplImage* color = cvCreateImage( cvGetSize(image), 8, 3 );
	cvCvtColor( target, color, CV_GRAY2BGR );
	int i;
	CvMemStorage* storage = cvCreateMemStorage(0);
	//用Hough变换在二值图像中寻找直线 貌似没什么鸟用
	CvSeq*  lines = cvHoughLines2( target, storage, CV_HOUGH_PROBABILISTIC , 1, CV_PI/180, 20, 1, 5 );

    for( i = 0; i < lines->total; i++ )
    {
        CvPoint* line = (CvPoint*)cvGetSeqElem(lines,i);
        cvLine( color, line[0], line[1], CV_RGB(255,0,0), 1, CV_AA, 0 );
    }

	cvNamedWindow( "line", CV_WINDOW_AUTOSIZE);
	cvShowImage("line",color);
	cvWaitKey();
}

/*
将指定点的，以及临近点涂成指定的颜色
void cvFloodFill( CvArr* image, CvPoint seed_point, CvScalar new_val,
				  CvScalar lo_diff=cvScalarAll(0), CvScalar up_diff=cvScalarAll(0),
                  CvConnectedComp* comp=NULL, int flags=4, CvArr* mask=NULL );
	seed_point 指定点的颜色
	new_val    涂成什么新的颜色
	lo_diff up_diff  指定 周围点高于 、低于 什么值之内 也涂成指定颜色
	comp  不知道
	flags 4(默认) 只考虑垂直和水平的点      8 还包含对角的点
	mask 掩码
*/
void floodfill()
{
	IplImage* image = cvLoadImage("pill.jpg", CV_LOAD_IMAGE_COLOR);
	cvFloodFill(image,cvPoint(90,94),CV_RGB(255,0,0),cvScalarAll(12),cvScalarAll(12));
	cvNamedWindow( "floodfill", 1 );
    cvShowImage( "floodfill", image );
	cvWaitKey(0);
	
}

void number()
{
	IplImage* image = cvLoadImage("pill.jpg", CV_LOAD_IMAGE_GRAYSCALE);

	int slider_pos = 140;
	cvThreshold( image, image, slider_pos, 255, CV_THRESH_BINARY );
	cvNamedWindow( "Threshold", 1 );cvShowImage("Threshold", image);
	
	//经过分析阀值结果，决定用闭运算消除凹点
	int an = 5;
	IplConvKernel* element = cvCreateStructuringElementEx( an*2+1, an*2+1, an, an, CV_SHAPE_RECT, 0 );

	cvDilate(image,image,element,1);
	cvNamedWindow( "Dilate", 1 );cvShowImage("Dilate", image);

	cvErode(image,image,element,1);
	cvNamedWindow( "Erode", 1 );cvShowImage("Erode", image);
	
	CvMemStorage* storage = cvCreateMemStorage(0);
	CvSeq* contour = NULL;
	//会改变image，所以有必要的话可以先cvCloneImage
	cvFindContours( image, storage, &contour);
	cvDrawContours( image , contour, cvScalarAll(255), cvScalarAll(255), 100);
	cvNamedWindow( "Contour", 1 );cvShowImage("Contour", image);
	
	int count = 0;
	CvFont font;
	char text[8];
	cvInitFont( &font, CV_FONT_HERSHEY_SIMPLEX,0.4, 0.4, 0, 1, CV_AA );
	image = cvLoadImage("pill.jpg", CV_LOAD_IMAGE_COLOR);
	for( ; contour != 0; contour = contour->h_next ){
		printf("length : %.2f  area : %.2f\n",cvArcLength(contour),cvContourArea(contour));//轮廓长度,轮廓面积
		CvRect rect = cvBoundingRect(contour,0);
		cvRectangle( image, cvPoint(rect.x,rect.y),cvPoint(rect.x+rect.width,rect.y+rect.height),CV_RGB(0,255,255), 1, 8, 0 );
		count++;
		sprintf(text,"%02d",count);
		cvPutText( image, text, cvPoint(rect.x+5,rect.y+15), &font, CV_RGB(0,255,0));
	}
	cvNamedWindow( "final", 1 );cvShowImage("final", image);
	//整幅图也是一个轮廓，所以要减掉1
	printf("Number of pills : %d\n",count-1);

	
	cvWaitKey();
}

int main()
{
	//开闭运算
	//open_calc();
	//close_calc();

	//礼帽和黑帽运算
	//tophat();

	//查找尺寸相同 两图中的精确差异
	//diff();
	
	//给图像像素点加强，加亮度，
	//adds(); 
	
	//没鸟用
	//dark();
	
	//查找边缘
	canny();

	//颜色填充指定点，以及临近点
	//floodfill();


	//途径图像中的元素个数
	//number();

	//图像金字塔采样,有吊用，还不如直接smooth
    //cvPyrDownUp();
	return 0;
}