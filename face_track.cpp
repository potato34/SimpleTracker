#include "cv.h" 
#include "highgui.h"

#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include <assert.h> 
#include <math.h> 
#include <float.h> 
#include <limits.h> 
#include <time.h> 
#include <ctype.h>

#ifdef _EiC 
//#define WIN32
#endif

static CvMemStorage* storage = 0; 
static CvHaarClassifierCascade* cascade = 0;

void detect_and_draw( IplImage* image );

/*const char* cascade_name = "/Users/ang/Documents/workspace/eye/src/haarcascade_frontalface_alt.xml";
/*    "haarcascade_profileface.xml";*/
const char* cascade_name = "D:/Program Files/opencv/data/haarcascades/haarcascade_frontalface_alt.xml";

int main( int argc, char** argv ) 
{ 
	//   cascade_name = "haarcascade_frontalface_alt2.xml"; 
	cascade = (CvHaarClassifierCascade*)cvLoad( cascade_name, 0, 0, 0 ); 

	if( !cascade ) 
	{ 
		fprintf( stderr, "ERROR: Could not load classifier cascade\n" ); 
		return -1; 
	} 
	storage = cvCreateMemStorage(0); 
	cvNamedWindow( "result", 1 ); 
	cvNamedWindow("source",1);
	CvCapture *capture = cvCaptureFromCAM(0);
	if(!capture)
		printf("\n");
	IplImage* image = NULL;
	char key = cvWaitKey(10);
	while( key != 'Q' && key != 'q' ) 
	{ 
		double t = (double)cvGetTickCount();
		image = cvQueryFrame(capture);
		if(!image)
		{
			printf("image NULL");
			continue;
		}
		std::cout<<"width"<<image->width<<std::endl;
		std::cout<<"height"<<image->height<<std::endl;
		//cvShowImage("source",image);
		detect_and_draw( image ); 
		key = cvWaitKey(10);
		t = (double)cvGetTickCount() - t;
		printf("cost time: %f",t / ((double)cvGetTickFrequency()*1000.));
	}
	cvReleaseImage( &image );  
	cvDestroyWindow("source"); 
	cvDestroyWindow("result"); 
	return 0; 
}

void detect_and_draw(IplImage* img ) 
{ 
	double scale=1.2; 
	static CvScalar colors[] = 
	{ 
		{{0,0,255}},{{0,128,255}},{{0,255,255}},{{0,255,0}}, 
		{{255,128,0}},{{255,255,0}},{{255,0,0}},{{255,0,255}} 
	};//Just some pretty colors to draw with

	//Image Preparation 
	// 
	IplImage* gray = cvCreateImage(cvSize(img->width,img->height),8,1); 
	IplImage* small_img=cvCreateImage(cvSize(cvRound(img->width/scale),cvRound(img->height/scale)),8,1); 
	cvCvtColor(img,gray, CV_BGR2GRAY); 
	cvResize(gray, small_img, CV_INTER_LINEAR);

	cvEqualizeHist(small_img,small_img);

	//Detect objects if any 
	// 
	cvClearMemStorage(storage); 
	double t = (double)cvGetTickCount(); 
	CvSeq* objects = cvHaarDetectObjects(small_img, 
										 cascade, 
										 storage,
										 1.1,
										 2,
										 0/*CV_HAAR_DO_CANNY_PRUNING*/,
										 cvSize(30,30));
	t = (double)cvGetTickCount() - t; 
	//// printf( "detection time = %gms\n", t/((double)cvGetTickFrequency()*1000.) );

	//Loop through found objects and draw boxes around them 
	for(int i=0;i<(objects? objects->total:0);++i) 
	{ 
		CvRect* r=(CvRect*)cvGetSeqElem(objects,i); 
		cvRectangle(img, cvPoint(r->x*scale,r->y*scale), cvPoint((r->x+r->width)*scale,(r->y+r->height)*scale), colors[i%8]); 
	} 
	for( int i = 0; i < (objects? objects->total : 0); i++ ) 
	{ 
		CvRect* r = (CvRect*)cvGetSeqElem( objects, i ); 
		CvPoint center; 
		int radius; 
		center.x = cvRound((r->x + r->width*0.5)*scale); 
		center.y = cvRound((r->y + r->height*0.5)*scale); 
		radius = cvRound((r->width + r->height)*0.25*scale); 
		cvCircle( img, center, radius, colors[i%8], 3, 8, 0 ); 
	}

	cvShowImage( "result", img ); 
	cvReleaseImage(&gray); 
	cvReleaseImage(&small_img); 
}
