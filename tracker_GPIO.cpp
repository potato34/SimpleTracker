// Object tracking algorithm using Hash or pHash code
// Author : zouxy
// Date   : 2013-12-21
// HomePage : http://blog.csdn.net/zouxy09
// Email  : zouxy09@qq.com
// http://blog.csdn.net/zouxy09/article/details/13358977
// http://blog.csdn.net/zouxy09/article/details/17471401

#include <opencv2/opencv.hpp>
#include <stdio.h>
#include <stdlib.h>
#define MODE_FILE_FORMAT "/sys/devices/virtual/misc/gpio/mode/gpio%d"
#define PIN_FILE_FORMAT  "/sys/devices/virtual/misc/gpio/pin/gpio%d"
using namespace cv;
using namespace std;

// Global variables
Rect box;
// tracker: get search patches around the last tracking box,
// and find the most similar one
void tracking(Mat frame, Mat &model, Rect &trackBox)
{
	Mat gray;
	cvtColor(frame, gray, CV_RGB2GRAY);

	Rect searchWindow;
	searchWindow.width = trackBox.width * 3;
	searchWindow.height = trackBox.height * 3;
	searchWindow.x = trackBox.x + trackBox.width * 0.5 - searchWindow.width * 0.5;
	searchWindow.y = trackBox.y + trackBox.height * 0.5 - searchWindow.height * 0.5;
	searchWindow &= Rect(0, 0, frame.cols, frame.rows);

	Mat similarity;
	matchTemplate(gray(searchWindow), model, similarity, CV_TM_CCOEFF_NORMED);

	double mag_r;
	Point point;
	minMaxLoc(similarity, 0, &mag_r, 0, &point);
	trackBox.x = point.x + searchWindow.x;
	trackBox.y = point.y + searchWindow.y;

	//*model = gray(trackBox);
}


void trackALL(Mat frame, Mat &model, Rect &trackBox)
{
	Mat gray;
	cvtColor(frame, gray, CV_RGB2GRAY);

	Rect searchWindow;
	searchWindow.width = 640;
	searchWindow.height = 480;
	searchWindow.x = 0;
	searchWindow.y = 0;
	searchWindow &= Rect(0, 0, frame.cols, frame.rows);

	Mat similarity;
	matchTemplate(gray(searchWindow), model, similarity, CV_TM_CCOEFF_NORMED);

	double mag_r;
	Point point;
	minMaxLoc(similarity, 0, &mag_r, 0, &point);
	trackBox.x = point.x + searchWindow.x;
	trackBox.y = point.y + searchWindow.y;

	//*model = gray(trackBox);
}

const char* keys =
{
	"{1|  | 0 | camera number}"
};

int  setpinmod(pin,){

}

int main( int argc, const char** argv )
{

    char pathstring[128];
    FILE *modefile1, *pinfile1,*modefile2, *pinfile2;
    int pin1,pin2;
    cout<<"test2"<<endl;
	//********************gpio init*********************
	pin1 = 2;
	pin2 = 3;
//********pin1******
	sprintf(pathstring,MODE_FILE_FORMAT,pin1);
    modefile1 = fopen(pathstring,"w");
    cout<<"test1"<<endl;
    if(modefile1 == NULL)
    {
        fprintf(stderr,"Unable to open mode file: %s\n",pathstring);
        exit(2);
    }

    sprintf(pathstring,PIN_FILE_FORMAT,pin1);
    pinfile1 = fopen(pathstring,"w");
    if(pinfile1 == NULL)
    {
        fprintf(stderr,"Unable to open pin file: %s\n",pathstring);
        exit(2);
    }
//********GPIO 2******
    sprintf(pathstring,MODE_FILE_FORMAT,pin2);
    modefile2 = fopen(pathstring,"w");
    if(modefile2 == NULL)
    {
        fprintf(stderr,"Unable to open mode file: %s\n",pathstring);
        exit(2);
    }

    sprintf(pathstring,PIN_FILE_FORMAT,pin2);
    pinfile2 = fopen(pathstring,"w");
    if(pinfile2 == NULL)
    {
        fprintf(stderr,"Unable to open pin file: %s\n",pathstring);
        exit(2);
    }

    fwrite("1",1,1,modefile1);
    fwrite("1",1,1,modefile2);
    //init output signal
    fwrite("0",1,1,pinfile1);
    fflush(pinfile1);
    fwrite("1",1,1,pinfile2);
    fflush(pinfile2);

    //**********************************************************
	VideoCapture capture;
	CommandLineParser parser(argc, argv, keys);
	int camNum = parser.get<int>("1");
    cout<<"test3"<<endl;
	capture.open(camNum);
    //capture.set(CV_CAP_PROP_FRAME_WIDTH, 1024);
    //capture.set(CV_CAP_PROP_FRAME_HEIGHT, 768);
	//Init camera
	if (!capture.isOpened())
	{
		cout << "capture device failed to open!" << endl;
		return -1;
	}

	cvNamedWindow("Tracker", CV_WINDOW_AUTOSIZE);

	Mat frame, model;
	capture >> frame;

	box = Rect( 170, 184, 108, 80 );
	Mat img=imread("Tracker.jpg");
	Mat gray;
	cvtColor(img, gray, CV_RGB2GRAY);
	model = gray(box);

	int frameCount = 0;

	while (1)
	{
		capture >> frame;
		if (frame.empty())
			return -1;
		double t = (double)cvGetTickCount();
		frameCount++;

        if(frameCount==100)
        {
            //imwrite("Tracker.jpg",frame);
        }
		// tracking
        if(frameCount<5)
        {
            trackALL(frame, model, box);
        }
        else
        {
            tracking(frame, model, box);
        }
		double xx=box.x+box.width*0.5;
        cout << "x: " << xx<<  endl;
        if(xx<230)
        {
            //turn Left
            fwrite("1",1,1,pinfile1);
            fflush(pinfile1);
            //sleep(1);
            fwrite("0",1,1,pinfile2);
            fflush(pinfile2);
            //sleep(1);
            cout << "Turn Left" <<endl;

        }
        else if(xx<410)
        {
            //forward
            fwrite("1",1,1,pinfile1);
            fflush(pinfile1);
           // sleep(1);
            fwrite("1",1,1,pinfile2);
            fflush(pinfile2);
           // sleep(1);
           cout << "Forward" <<endl;
        }
        else
        {
            //trun Right
            fwrite("0",1,1,pinfile1);
            fflush(pinfile1);
            //sleep(1);
            fwrite("1",1,1,pinfile2);
            fflush(pinfile2);
            // sleep(1);
            cout << "Turn Right" <<endl;
        }

		// show
		stringstream buf;
		buf << frameCount;
		string num = buf.str();
		putText(frame, num, Point(20, 20), FONT_HERSHEY_SIMPLEX, 1, Scalar(0, 0, 255), 3);
		rectangle(frame, box, Scalar(0, 0, 255), 2);
		imshow("Tracker", frame);

		t = (double)cvGetTickCount() - t;
		cout << "cost time: " << t / ((double)cvGetTickFrequency()*1000.) << endl;

		if ( cvWaitKey(1) == 27 )
			break;
	}

	return 0;
}
