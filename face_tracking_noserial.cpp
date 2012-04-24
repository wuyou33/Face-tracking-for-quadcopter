// face_tracking_noserial.cpp : main project file.

//#include "stdafx.h"
#include <cv.h>
#include <highgui.h>

#include <iostream>
#include <stdio.h>


using namespace std;
using namespace cv;


void menu();

/** Function Headers */
void detectAndDisplay( IplImage *img,int menucode);


/** Global variables */
int startx=0;
int starty=0;
CascadeClassifier face_cascade;
CvHaarClassifierCascade *facecascade;
CvMemStorage            *storage;
//CascadeClassifier eyes_cascade;
string window_name = "Capture - Face detection";
RNG rng(12345);
int resX=320;
int resY=240;


bool connection=false;

int counter=0;
int flagUp = 0;
int flagDown = 0;
int storex[2] = {0,0};
int storey[2] = {0,0};
int lastx=0;
int lasty=0;


int main( int argc, char** argv )
{
	int menucode=1;
	menu();
	//if(menucode==3)
		//return 0;

	//-- 1. Load the cascades
	//if( !face_cascade.load( face_cascade_name ) ){ printf("--(!)Error loading\n"); return -2; };
	storage = cvCreateMemStorage( 0 );
	char      *filenameFace = "haarcascade_frontalface_alt.xml";
	facecascade = ( CvHaarClassifierCascade* )cvLoad( filenameFace, 0, 0, 0 );
	
	//-- 2. Read the video stream
	CvCapture* capture;
	capture = cvCaptureFromCAM( -1 );
	int centerx, centery;
    if( capture != NULL )
	{
		//set the size of the video frame displayed to users to 80px by 60px. If you want to increase it, do it in multiples, like 160x120 or 320x240
		cvSetCaptureProperty( capture, CV_CAP_PROP_FRAME_WIDTH, resX );
		cvSetCaptureProperty( capture, CV_CAP_PROP_FRAME_HEIGHT, resY );
		//continuously call detectanddisplay function to detect face, display video, and send info to arduino
		while( true )
		{
			//Mat frame;
			IplImage  *frame;
			frame = cvQueryFrame( capture );
			cvFlip( frame, frame, 1 );
			frame->origin = 0;
 
			
			//-- 3. Apply the classifier to the frame
			if( frame )
			{ 
				// detect faces and display video and send coordinates to arduino
				detectAndDisplay( frame, menucode);
				
			}
			else
			{ printf(" --(!) No captured frame -- Break!"); break; }

			int c = waitKey(10);
			if( (char)c == 'c' ) { break; }
		}
	}
	return 0;
}

/** @function detectAndDisplay */
void detectAndDisplay( IplImage *img, int menucode)
{
    int i,centerx=0,centery=0;
 
    // detect faces 
    CvSeq *faces = cvHaarDetectObjects(
            img,
            facecascade,
            storage,
            1.3,
            1,
            CV_HAAR_DO_CANNY_PRUNING,
            cvSize( 40, 40 ) );
	CvRect *face;
    // for each face found, draw a red box 

    for( i = 0 ; i < ( faces ? faces->total : 0 ) ; i++ ) {
        face=( CvRect* )cvGetSeqElem( faces, i );
		Point center( face->x + face->width*0.5, face->y + face->height*0.5 );
		cvEllipse( img, center, Size( face->width*0.5, face->height*0.5), 0, 0, 360, Scalar( 255, 0, 255 ), 4, 8, 0 );
		centerx = face->x + face->width*0.5;
		centery = face->y + face->height*0.5;
		
        /* reset buffer for the next object detection */
    	cvClearMemStorage(storage);
    }

	if(faces && centerx!=0 && centery!=0) {
		if(startx==0 || starty==0) {
			startx=centerx;
			starty=centery;
			lastx = startx;
			lasty = starty;
			
		}
		int servocenter=90;
		int diffx = startx - centerx;
		int diffy = starty - centery;
		int sendy=diffy;
		int sendx=diffx;
		if(storex[0]!=0 && storey[0]!=0 && storey[1]!=0 && storex[1]!=0)
		{
			sendy=(storey[0]+storey[1]+diffy)/3;
			sendx=(storex[0]+storex[1]+diffx)/3;
			/*if((storey[0]>diffy && storey[0] > storey[1]) || (storey[0] < diffy && storey[0] < storey[1])) 
			{
				storey[0]=storey[1];
			}
			if((storex[0]>diffx && storex[0] > storex[1]) || (storex[0] < diffx && storex[0] < storex[1])) 
			{
				storey[0]=storey[1];
			}*/
		}
		
		int fudgex=0;
		int fudgey=0;
		if(connection && storey[0]!=0 && counter==1 )//&& (fudgey > 2 || fudgey < -2) )
		{	
				fudgex = lastx - sendx;
				fudgey = lasty - sendy;
				
			
				if(fudgey<2 && fudgey>-2) {
					sendx=lastx;
					sendy=lasty;
				}
				//arduino->Write("s" + ( servocenter + (sendx) ) +"b"+ ( servocenter + (sendy*2) ) +"e" );

				lastx=sendx;
				lasty=sendy;
		}
		if(counter<1)
			counter++;
		else
			counter=0;
		storex[1]=storex[0];storex[0]=diffx;
		storey[1]=storey[0];storey[0]=diffy;
	}
    
    // display video 
    cvShowImage( "video", img );
}

//TODO - return a value corresponding to the mode the user selected
void menu()
{
	//determine resolution to display webcam video at
	std::cout << "Please select a video resolution:\n1. 160x120\n2. 320x240\n3. 480x360\n4. 640x480" << std::endl;
	int res;
	std::cin >> res;
	switch(res) {
		case 1:	//160x120
			resX=160;
			resY=120;
			break;
		case 2:	//320x240
			resX=320;
			resY=240;
			break;
		case 3:	//480x360
			resX=480;
			resY=360;
			break;
		case 4:	//640x480
			resX=640;
			resY=480;
			break;
	}
	/*
	std::cout << "Select a face tracking control mode" << std::endl;
	std::cout << "1. Continuous(camera moves with head always)" << std::endl;
	std::cout << "2. Directional(camera stays in last position if head returned to center)" << std::endl;
	std::cout << "3. Quit" << std::endl;
	int mode;
	std::cin >> mode;

	//std::cout << mode << endl;
	return mode;*/
}

