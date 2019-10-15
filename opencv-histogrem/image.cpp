#include <stdlib.h>
#include <stdio.h>
#include "cv.h"
#include "highgui.h"

#pragma comment(lib,"cv.lib")
#pragma comment(lib,"cxcore.lib")
#pragma comment(lib,"highgui.lib")

void Histogrem()
{
	IplImage *src = cvLoadImage("85.jpg",1);
	
	/*
	IplImage *src = cvCreateImage(cvSize(640,480),8,3);
	cvZero( src );
	cvRectangle( src ,cvPoint(120,120), cvPoint(450,450), CV_RGB(48,23,20), CV_FILLED, CV_AA, 0 );
	cvRectangle( src ,cvPoint(10,10), cvPoint(150,150), CV_RGB(100,0,25), CV_FILLED, CV_AA, 0 );
	*/


	cvNamedWindow( "src", 1 );
	cvShowImage( "src", src );
	
	IplImage *hsv = cvCreateImage(cvGetSize(src),8,3);
	cvCvtColor(src,hsv,CV_BGR2HSV);//ת��src�� BGR��ɫ �� HSV��ɫ(ɫ��H[0,180),���Ͷ�S[0,255),����V[0,255))�����浽hsvͼ����
	//ת��HSV�ĺô� ͨ������£�����Ҫ�Ա�����ͼ��ֱ��ͼ������ͼ���ܺ����ƣ����ǹ���ǿ�ȣ�����V�����ܲ�ͬ�������ᵼ�¶ԱȽ��������Ԥ��
	//cvNamedWindow( "HSV", 1 );
	//cvShowImage( "HSV", hsv );

	IplImage* h_plane = cvCreateImage( cvGetSize(hsv), 8, 1 );
	IplImage* s_plane = cvCreateImage( cvGetSize(hsv), 8, 1 );
    IplImage* v_plane = cvCreateImage( cvGetSize(hsv), 8, 1 );
    cvCvtPixToPlane( hsv, h_plane, s_plane, v_plane, 0 );//cvSplit ��hsvͼ��� ɫ��H�����Ͷ�S������V �ֲ� h��s��v 3��ͼ����

	IplImage* planes[] = { h_plane, s_plane }; //����ֻ���� ɫ��H�����Ͷ�S
	int h_bins = 30, s_bins = 32; //����(ɫ��H)�̶ܿ�30   ����(���Ͷ�S)�̶ܿ�32 
	int hist_size[] = {h_bins, s_bins}; //��������Ϣ
	float h_ranges[] = { 0, 180 }; // ɫ��H ��ȡֵ��Χ�� [0,180)  0��ʾ��ɫ��180��ʾ��ɫ
	float s_ranges[] = { 0, 255 }; // ���Ͷ�S ��ȡֵ��Χ�� [0,255) 
    float* ranges[] = { h_ranges, s_ranges };//ȡֵ��Χ��Ϣ  
	//   ��ɫ��H=0 : h_ranges ��Ӧ0, h_bins��ӦҲ��0 , ��ȡֵ��Χ��ԭ��
	//   ��ɫ��H=90 : h_ranges ��Ӧ90, h_bins��ӦҲ��15 , ��ȡֵ��Χ���е�

	/*
	CvHistogram* cvCreateHist( int dims, int* sizes, int type,
                           float** ranges=NULL, int uniform=1 );
			dims : ֱ��ͼά��
			sizes : ���鳤�ȵ���dims
			type :  CV_HIST_ARRAY �ܼ�����
			ranges : ��ά�ȵ�ȡֵ��Χ
	*/
	CvHistogram* hist = cvCreateHist( 2, hist_size, CV_HIST_ARRAY, ranges, 1 );
	
	cvCalcHist( planes, hist, 0, 0 );//����ֱ��ͼ
	//cvNormalizeHist( hist, 1.0 );//��һ�� ��������Ԫ�صĺ�=1.0

	float max_value = 0;
	cvGetMinMaxHistValue( hist, 0, &max_value, 0, 0 );//ȡ��ֱ��ͼ�е����ֵ
	printf("max_value %.2f \n",max_value);

	int scale = 10; //ֱ��ͼ��ÿ������ı߳�10px
	IplImage* hist_img = cvCreateImage( cvSize(h_bins*scale,s_bins*scale), 8, 3 );//ֱ��ͼͼ��
	cvZero( hist_img );

	int h, s;
	for( h = 0; h < h_bins; h++ )//ɫ��H  ���������  ��ʾ �Ӻ쵽��
	{
		for( s = 0; s < s_bins; s++ ) //���Ͷ�S  ������ϵ��� ��ʾ ֵ��С����
		{
			float bin_val = cvQueryHistValue_2D( hist, h, s ); //����ֱ��ͼ��ֵ

			//ʹ�ð�ɫǿ������ʾ ���ֵ�Ƶ�ʣ�Խ�ױ�ʾԭʼͼ���и�����ɫ(ɫ��+���Ͷ�)����Ƶ��Խ�ߣ�����˵ռ��Խ��
			int intensity = cvRound(bin_val*255/max_value);
			if(bin_val!=0.0)
				printf("P(%d,%d) %f  %d\n",h,s,bin_val,intensity);
			cvRectangle( hist_img, cvPoint( h*scale, s*scale ),
				cvPoint( (h+1)*scale - 1, (s+1)*scale - 1),
				CV_RGB(intensity,intensity,intensity), CV_FILLED );
		
		}
    }

	
	
	cvNamedWindow( "H-S Histogram", 1 );
	cvShowImage( "H-S Histogram", hist_img );
	
    cvWaitKey(0);
}

void Histogrem_EMD()
{
    IplImage *src1 = cvLoadImage("84.jpg", 1);
	IplImage *src2 = cvLoadImage("86.jpg", 1);


	IplImage* hsv1 = cvCreateImage( cvGetSize(src1), 8, 3 );
	cvCvtColor( src1, hsv1, CV_BGR2HSV);
	
	
	IplImage* h_plane1 = cvCreateImage( cvGetSize(src1), 8, 1 );
	IplImage* s_plane1 = cvCreateImage( cvGetSize(src1), 8, 1 );
	IplImage* v_plane1 = cvCreateImage( cvGetSize(src1), 8, 1 );
	IplImage* planes1[] = { h_plane1, s_plane1 };
	cvCvtPixToPlane( hsv1, h_plane1, s_plane1, v_plane1, 0 );
	
	IplImage* hsv2 = cvCreateImage( cvGetSize(src2), 8, 3 );
	cvCvtColor( src2, hsv2, CV_BGR2HSV);
	
	
	IplImage* h_plane2 = cvCreateImage( cvGetSize(src2), 8, 1 );
	IplImage* s_plane2 = cvCreateImage( cvGetSize(src2), 8, 1 );
	IplImage* v_plane2 = cvCreateImage( cvGetSize(src2), 8, 1 );
	IplImage* planes2[] = { h_plane2, s_plane2 };
	cvCvtPixToPlane( hsv2, h_plane2, s_plane2, v_plane2, 0 );
	
	int h_bins = 30, s_bins = 32;
	int hist_size[] = {h_bins, s_bins};
	float h_ranges[] = { 0, 180 }; 
	float s_ranges[] = { 0, 255 };
    float* ranges[] = { h_ranges, s_ranges };

	CvHistogram* hist1 = cvCreateHist( 2, hist_size, CV_HIST_ARRAY, ranges, 1 );
	CvHistogram* hist2 = cvCreateHist( 2, hist_size, CV_HIST_ARRAY, ranges, 1 );
	
	cvCalcHist( planes1, hist1, 0, 0 );
	cvCalcHist( planes2, hist2, 0, 0 );

	CvMat *sig1 = cvCreateMat(h_bins * s_bins, 3, CV_32FC1);
    CvMat *sig2 = cvCreateMat(h_bins * s_bins, 3, CV_32FC1);

	for ( int h = 0; h < h_bins; h ++ ) {
		for ( int s = 0; s < s_bins ; s++ ) {
			// Signature
			cvSet2D(sig1, h*s_bins+s, 0, cvScalar(cvQueryHistValue_2D( hist1, h, s ))); // bin value
			cvSet2D(sig1, h*s_bins+s, 1, cvScalar(h)); // Coord 1
			cvSet2D(sig1, h*s_bins+s, 2, cvScalar(s)); // Coord 2
			cvSet2D(sig2, h*s_bins+s, 0, cvScalar(cvQueryHistValue_2D( hist2, h, s ))); // bin value
			cvSet2D(sig2, h*s_bins+s, 1, cvScalar(h)); // Coord 1
			cvSet2D(sig2, h*s_bins+s, 2, cvScalar(s)); // Coord 2
		}
    }

	float emd = cvCalcEMD2( sig1, sig2, CV_DIST_L2);
    printf("EMD : %f ;", emd);//���Խ�ӽ�0Խ����



}

//����ٶ�Ҫ�� cvCalcEMD2 ��ܶ࣬��Ч����˵��һЩ
void Histogrem_compare()
{
	IplImage *src1 = cvLoadImage("84.jpg", CV_LOAD_IMAGE_GRAYSCALE);
	IplImage *src2 = cvLoadImage("87.jpg", CV_LOAD_IMAGE_GRAYSCALE);

	IplImage* planes1[] = {src1};
	IplImage* planes2[] = {src2};

	int bins = 256;
    int hist_size[]  = {bins};
    float range[] = {0,255};
	float *ranges[] = {range};

    CvHistogram *hist1=cvCreateHist(1,hist_size,CV_HIST_ARRAY,ranges,1);    
    CvHistogram *hist2=cvCreateHist(1,hist_size,CV_HIST_ARRAY,ranges,1);

	cvCalcHist( planes1, hist1, 0, 0 );cvNormalizeHist(hist1,1.0); //����ֱ��ͼ����һ��
	cvCalcHist( planes2, hist2, 0, 0 );cvNormalizeHist(hist2,1.0);

	printf("CV_COMP_CORREL : %.4f\n",cvCompareHist(hist1,hist2,CV_COMP_CORREL));//��رȽ�
	printf("CV_COMP_CHISQR : %.4f\n",cvCompareHist(hist1,hist2,CV_COMP_CHISQR));//�����Ƚ�
	printf("CV_COMP_INTERSECT : %.4f\n",cvCompareHist(hist1,hist2,CV_COMP_INTERSECT));//ֱ��ͼ�ཻ
	printf("CV_COMP_BHATTACHARYYA : %.4f\n",cvCompareHist(hist1,hist2,CV_COMP_BHATTACHARYYA));//BHATTACHARYYA����
	
	//��һ���� ����ͼ��ȫ��ͬ�ķ���ֵ 
// 	CV_COMP_CORREL=1 
// 	CV_COMP_CHISQR =0
// 	CV_COMP_INTERSECT=1
// 	CV_COMP_BHATTACHARYYA=0

}


// ͨ������ͼ�����ƥ��
void MatchTemplate()
{
    IplImage *src = cvLoadImage("84.jpg", 1);
	IplImage *templ = cvLoadImage("84_part.jpg", 1);

	int i_width = src->width - templ->width + 1;
	int i_height = src->height - templ->height + 1;

	IplImage *ftmp = cvCreateImage( cvSize(i_width, i_height), 32, 1 );
	cvMatchTemplate( src, templ, ftmp, CV_TM_SQDIFF); //CV_TM_SQDIFF_NORMED... totoal 6 methon
	//cvNormalize( ftmp, ftmp, 1, 0, CV_MINMAX );

	double min_val;
	double max_val;
	CvPoint min_loc;
	CvPoint max_loc;

	cvMinMaxLoc(ftmp, &min_val, &max_val, &min_loc, &max_loc, NULL);

	cvRectangle(src, cvPoint(min_loc.x, min_loc.y), 
		cvPoint((min_loc.x + templ->width),(min_loc.y + templ->height)), CV_RGB(0,255,0), 1);
	cvNamedWindow("src",1);
	cvShowImage("src",src);
	cvWaitKey(0);

}

int main()
{
	//Histogrem();
	//Histogrem_EMD();
	//Histogrem_compare();
	MatchTemplate();
	return 0;
}