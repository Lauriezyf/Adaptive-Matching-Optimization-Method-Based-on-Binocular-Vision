#include <sys/types.h>
#include <sys/stat.h>
#include <iostream>
#include <iomanip>
#include <opencv2/opencv.hpp>
#include "original.h"

using namespace std;
using namespace cv;

int main(int argc, char** argv)
{
	//Build a document to save the result
	mkdir("/Users/lauriezyf/Documents/result", S_IRWXU);


	//Read the original picture and turn it into gray one
	time_t start=time(NULL);
	Mat Left=imread("/Users/lauriezyf/Documents/Software/myvim/picture/teddy/im2.png",-1);
	Mat Right=imread("/Users/lauriezyf/Documents/Software/myvim/picture/teddy/im6.png",-1);
	Mat disparityLeft=turnIntoGray(Left,3);
	Mat disparityRight=turnIntoGray(Right,3);
	

	
	//Operation of SAD
	time_t startSAD=time(NULL);
	Mat SADLeft=SAD(disparityLeft, disparityRight, true, 11);
	imwrite("/Users/lauriezyf/Documents/result/SADLeft.png",SADLeft);

	time_t finishSAD=time(NULL);
	cout<<"Duration of SAD : "<<finishSAD-startSAD<<" seconds"<<endl;


	
	//Operation of SSD
	time_t startSSD=time(NULL);
	Mat SSDLeft=SSD(disparityLeft, disparityRight, true, 11);
	imwrite("/Users/lauriezyf/Documents/result/SSDLeft.png",SSDLeft);

        time_t finishSSD=time(NULL);
	cout<<"Duration of SSD : "<<finishSSD-startSSD<<" seconds" <<endl;


	//Operation of NCC
	time_t startNCC=time(NULL);
	Mat NCCLeft=NCC(disparityLeft, disparityRight, true, 11);
	imwrite("/Users/lauriezyf/Documents/result/NCCLeft.png",NCCLeft);
    
	time_t finishNCC=time(NULL);
	cout<<"Duration of NCC : "<< finishNCC-startNCC<<" seconds"<<endl;

	
	
	
	//Operation of ASW
	time_t startASW=time(NULL);
	Mat ASWLeft=ASW(Left, Right, true, 11, 36, 7);
	imwrite("/Users/lauriezyf/Documents/result/ASWLeft.png",ASWLeft);

	time_t finishASW=time(NULL);
	cout<<"Duration of ASW : "<<finishASW-startASW<<" seconds"<<endl;
	
	
	//Evaluate the quality of the disparity maps
	Mat disparity = imread("/Users/lauriezyf/Documents/Software/myvim/picture/teddy/disp2.png",-1);
	Mat nonocc = imread("/Users/lauriezyf/Documents/Software/myvim/picture/teddy/nonocc.png",-1);
	Mat all = imread("/Users/lauriezyf/Documents/Software/myvim/picture/teddy/all.png",-1);
	Mat disc = imread("/Users/lauriezyf/Documents/Software/myvim/picture/teddy/disc.png",-1);	

	//Evaluate the quality of SADLeft
	cout<<"nonocc. of SADLeft : ";
	Evaluate(disparity, SADLeft, nonocc);

	cout<<"all. of SADLeft : ";
        Evaluate(disparity, SADLeft, all);

	cout<<"disc. of SADLeft : ";
        Evaluate(disparity, SADLeft, disc);

	//Evaluate the quality of SSDLeft
	cout<<"nonocc. of SSDLeft : ";
	Evaluate(disparity, SSDLeft, nonocc);

	cout<<"all. of SSDLeft : ";
        Evaluate(disparity, SSDLeft, all);

	cout<<"disc. of SSDLeft : ";
        Evaluate(disparity, SSDLeft, disc);

	//Evaluate the quality of NCCLeft
	cout<<"nonocc. of NCCLeft : ";
	Evaluate(disparity, NCCLeft, nonocc);

	cout<<"all. of NCCLeft : ";
        Evaluate(disparity, NCCLeft, all);

	cout<<"disc. of NCCLeft : ";
        Evaluate(disparity, ASWLeft, disc);

	//Evaluate the quality of ASWLeft
	cout<<"nonocc. of ASWLeft : ";
	Evaluate(disparity, ASWLeft, nonocc);

	cout<<"all. of ASWLeft : ";
        Evaluate(disparity, ASWLeft, all);

	cout<<"disc. of ASWLeft : ";
        Evaluate(disparity, ASWLeft, disc);

		


	time_t finish=time(NULL);
    
	cout<<"Total time : "<<finish-start<<" seconds"<<endl;
	return 0;
}
