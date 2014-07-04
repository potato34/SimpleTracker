// Object tracking algorithm using Hash or pHash code
// Author : zouxy
// Date   : 2013-12-21
// HomePage : http://blog.csdn.net/zouxy09
// Email  : zouxy09@qq.com
// http://blog.csdn.net/zouxy09/article/details/13358977
// http://blog.csdn.net/zouxy09/article/details/17471401

#include <opencv2/opencv.hpp>

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

	model = gray(trackBox);
}

const char* keys =
{
	"{1|  | 0 | camera number}"
};

int main( int argc, const char** argv )
{
	VideoCapture capture;
	CommandLineParser parser(argc, argv, keys);
	int camNum = parser.get<int>("1");
	capture.open(camNum);
	bool fromfile = true;
	//Init camera
	if (!capture.isOpened())
	{
		cout << "capture device failed to open!" << endl;
		return -1;
	}

	cvNamedWindow("Tracker", CV_WINDOW_AUTOSIZE);

	Mat frame, model;
	capture >> frame;

	box = Rect( 250, 200, 40, 40 );
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

		// tracking
		tracking(frame, model, box);
		printf( "x: %d y:%d\n", box.x+box.width*0.5, box.y+box.height*0.5);
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
