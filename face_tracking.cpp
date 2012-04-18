// arduinoComm.cpp : main project file.

#include "stdafx.h"
#include "cv.h"
#include "highgui.h"

#include <iostream>
#include <stdio.h>


using namespace std;
using namespace cv;

using namespace System;
using namespace System::IO::Ports;

int menu();
void connect(SerialPort^ &arduino);

/** Function Headers */
void detectAndDisplay( IplImage *img, SerialPort^ &arduino,int menucode);
void detectAndDisplayEyes(IplImage *img, CvRect *r);
void detectAndDisplayNose(IplImage *img, CvRect *r);


/** Global variables */
//cv::String face_cascade_name = "haarcascade_frontalface_alt.xml";
//String eyes_cascade_name = "haarcascade_eye_tree_eyeglasses.xml";
int startx=0;
int starty=0;
CascadeClassifier face_cascade;
CvHaarClassifierCascade *facecascade;
CvHaarClassifierCascade *eyecascade;
CvHaarClassifierCascade *nosecascade;
CvMemStorage            *storage;
//CascadeClassifier eyes_cascade;
string window_name = "Capture - Face detection";
RNG rng(12345);
int resX=320;
int resY=240;

/* Com glob variables */
ConsoleKeyInfo cki;

int baudRate=57600;
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
	//TODO - menu will return a value corresponding to a mode to enter.
	int menucode;
	menucode=menu();
	if(menucode==3)
		return 0;

	//-- 1. Load the cascades
	//if( !face_cascade.load( face_cascade_name ) ){ printf("--(!)Error loading\n"); return -2; };
	storage = cvCreateMemStorage( 0 );
	char      *filenameFace = "haarcascade_frontalface_alt.xml";
	char      *filenameEyes = "haarcascade_eye_tree_eyeglasses.xml";
	char      *filenameNose = "haarcascade_mcs_nose.xml";
	facecascade = ( CvHaarClassifierCascade* )cvLoad( filenameFace, 0, 0, 0 );
	eyecascade = ( CvHaarClassifierCascade* )cvLoad( filenameEyes, 0, 0, 0 );
	nosecascade = ( CvHaarClassifierCascade* )cvLoad( filenameNose, 0, 0, 0 );

	SerialPort^ arduino;
	connect( arduino );
	//initialize connection and send control mode to microcontroller
	if(connection)
		arduino->Write("i"+menucode+"e");

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
				detectAndDisplay( frame, arduino, menucode);
				
			}
			else
			{ printf(" --(!) No captured frame -- Break!"); break; }

			int c = waitKey(10);
			if( (char)c == 'c' ) { break; }
		}
	}
	if(connection)
		arduino->Close();
	return 0;
}

/** @function detectAndDisplay */
void detectAndDisplay( IplImage *img, SerialPort^ &arduino, int menucode)
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
        //r = ( CvRect* )cvGetSeqElem( faces, i );
        face=( CvRect* )cvGetSeqElem( faces, i );
		Point center( face->x + face->width*0.5, face->y + face->height*0.5 );
		cvEllipse( img, center, Size( face->width*0.5, face->height*0.5), 0, 0, 360, Scalar( 255, 0, 255 ), 4, 8, 0 );
		centerx = face->x + face->width*0.5;
		centery = face->y + face->height*0.5;
		//detectAndDisplayEyes(img, face);
		//detectAndDisplayNose(img, face);
		//displays rectangle for face tracking instead of ellipse
        /*cvRectangle( img,
                     cvPoint( r->x, r->y ),
                     cvPoint( r->x + r->width, r->y + r->height ),
                     CV_RGB( 255, 0, 0 ), 1, 8, 0 );*/
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
	//middle of control is center of video resolution
	//startx=resX/2;
	//starty=resY/2;

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
//	int fudgex = lastx - centerx;
	//int fudgey = lasty - centery;
	int fudgex=0;
	int fudgey=0;
	if(connection && storey[0]!=0 && counter==1 )//&& (fudgey > 2 || fudgey < -2) )
	{	
			fudgex = lastx - sendx;
			fudgey = lasty - sendy;
			
		
			//arduino->Write( ( servocenter + diffx ) +"b"+ ( servocenter + diffy ) +"e" );
			if(fudgey<2 && fudgey>-2) {
				sendx=lastx;
				sendy=lasty;
			}
			arduino->Write("s" + ( servocenter + (sendx) ) +"b"+ ( servocenter + (sendy*2) ) +"e" );

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

	//lastx = centerx;
	//lasty = centery;
 	/* Set the Region of Interest: estimate the eyes' position */


	/*if(face) {
		cvSetImageROI(
		    img,                    // the source image 
		    cvRect(
		        face->x,            // x = start from leftmost 
		        face->y + (face->height/5.5), // y = a few pixels from the top 
		        face->width,        // width = same width with the face 
		        face->height/3.0    // height = 1/3 of face height 
		    )
		);
		//find eyes and draw squares
		detectAndDisplayEyes(img);
	}*/
    


    
    // display video 
    cvShowImage( "video", img );
}

/** @function detectAndDisplay */
void detectAndDisplayEyes( IplImage *img, CvRect *face)
{
	cvSetImageROI(img,                    /* the source image */ 
          cvRect
          (
              face->x,            /* x = start from leftmost */
              face->y, //+ (face->height/5.5), /* y = a few pixels from the top */
              face->width,// s/1.5,        /* width = same width with the face */
              face->height // /3.0    /* height = 1/3 of face height */
          )
      );
	/* detect the eyes */
		CvSeq *eyes = cvHaarDetectObjects(
		    img,            /* the source image, with the
                           estimated location defined */
		    eyecascade,      /* the eye classifier */
		    storage,        /* memory buffer */
		    1.2, 1, CV_HAAR_DO_CANNY_PRUNING,     /* tune for your app */
		    cvSize(25, 15)  /* minimum detection scale */
		);
 		int i;
		int distance=0;
		for( i = 0; i < (eyes ? eyes->total : 0); i++ ) {
			/* get one eye */
		
			CvRect *eye = (CvRect*)cvGetSeqElem(eyes, i);
			Point centereye( eye->x + eye->width*0.5, eye->y + eye->height*0.5 );
			int radius = cvRound( (eye->width + eye->height)*0.25 );
			cvCircle( img, centereye, radius, Scalar( 255, 0, 0 ), 4, 8, 0 );
			/*if(distance==0) 
			{
				distance=(eye->x+eye->width*0.5);
			}
			else {
				distance=distance-(eye->x+eye->width*0.5);
				printf("distance:%i\n",distance);
			}*/
		}


    
    /* reset region of interest */
    cvResetImageROI(img);
}
/** @function detectAndDisplayNose */
void detectAndDisplayNose( IplImage *img, CvRect *face)
{
	cvSetImageROI(img,                    /* the source image */ 
          cvRect
          (
              face->x + (face->width/3),            /* x = start from leftmost */
              face->y + (face->height/3) , /* y = a few pixels from the top */
              face->width/2,        /* width = same width with the face */
              face->height/3    /* height = 1/3 of face height */
          )
      );
	/* detect the eyes */
		CvSeq *noses = cvHaarDetectObjects(
		    img,            /* the source image, with the
                           estimated location defined */
		    nosecascade,      /* the nose classifier */
		    storage,        /* memory buffer */
		    1.2, 3, CV_HAAR_DO_CANNY_PRUNING,     /* tune for your app */
		    cvSize(25, 15)  /* minimum detection scale */
		);
 		int i;
		int distance=0;
		for( i = 0; i < (noses ? noses->total : 0); i++ ) {
			/* get one eye */
		
			CvRect *nose = (CvRect*)cvGetSeqElem(noses, i);
			Point centernose( nose->x + nose->width*0.5, nose->y + nose->height*0.5 );
			int radius = cvRound( (nose->width + nose->height)*0.25 );
			cvCircle( img, centernose, radius, Scalar( 255, 0, 0 ), 4, 8, 0 );
			/*if(distance==0) 
			{
				distance=(eye->x+eye->width*0.5);
			}
			else {
				distance=distance-(eye->x+eye->width*0.5);
				printf("distance:%i\n",distance);
			}*/
		}

    
    /* reset region of interest */
    cvResetImageROI(img);
}


//TODO - return a value corresponding to the mode the user selected
int menu()
{
	//determine resolution to display webcam video at
	/*std::cout << "Please select a video resolution:\n1. 160x120\n2. 320x240\n3. 480x360\n4. 640x480" << std::endl;
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
	}*/
	resX=480;
	resY=360;
	std::cout << "Select a face tracking control mode" << std::endl;
	std::cout << "1. Continuous(camera moves with head always)" << std::endl;
	std::cout << "2. Directional(camera stays in last position if head returned to center)" << std::endl;
	std::cout << "3. Quit" << std::endl;
	int mode;
	std::cin >> mode;

	//std::cout << mode << endl;
	return mode;
}

void connect(SerialPort^ &arduino)
{
	System::String^ portName;
	System::String^ response;
	//take in port name from user
	Console::WriteLine("Type in a port name and hit ENTER");
	portName=Console::ReadLine();

	// try opening port, give user the choice to keep trying to connect or just continue to face tracking
	do {
		try
		{
			arduino = gcnew SerialPort(portName, baudRate);
			arduino->Open();
			printf("Successfully connected to: %s\n",portName);
			connection=true;
			break;
		}
		catch (IO::IOException^ e  )
		{
			Console::WriteLine(e->GetType()->Name+": Port is not ready. Continue to face tracking? (yes/no)");
		}
		catch (ArgumentException^ e)
		{
			Console::WriteLine(e->GetType()->Name+": incorrect port name syntax, must start with COM/com. Continue to face tracking? (yes/no)");
		}
		response=Console::ReadLine();
		if(System::String::Compare(response,"no")==0 ||System::String::Compare(response,"No")==0 || System::String::Compare(response,"NO")==0) {
			Console::WriteLine("Type in a port name and hit ENTER");
			portName=Console::ReadLine();
		}
		else {
			connection=false;
			break;
		}
	} while(1);
}
