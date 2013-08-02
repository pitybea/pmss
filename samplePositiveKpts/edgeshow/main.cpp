#include <stdio.h>
#include <Windows.h>
#include <direct.h>

#include <dos.h>
#include <conio.h>

#include <iostream>
using namespace std;
#include <fstream>
#include <stdio.h>
#include <cv.h>
#include <highgui.h>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <vector>
#include <string>

#pragma comment(lib,"cxcore.lib")
#pragma comment(lib,"cv.lib") 
#pragma comment(lib,"ml.lib") 
#pragma comment(lib,"highgui.lib")  

char wndname[] = "Edge";

char tbarname[] = "Threshold";
int edge_thresh = 0;

vector<vector<int> > rects;


IplImage *image = 0, *edge = 0, *sh=0;


static int label_color[21][3]=
{
	242,13,40,
	242,13,110,
	242,13,212,
	45,13,242,
	13,242,229,
	0,222,21,
	24,242,13,
	234,242,13,
	242,67,13,
	239,99,116,
	239,99,162,
	239,99,211,
	239,99,162,
	112,99,239,
	99,239,132,
	237,239,99,
	106,106,72,
	247,247,243,
	10,10,0,
	51,128,129,
	245,245,245


};
void drawp(CvPoint p,int inde_,IplImage *frame1)
{
	int cT=inde_%21;
	CvScalar sc=CV_RGB(label_color[cT][0],label_color[cT][1],label_color[cT][2]);
	cvCircle(frame1,p,1,sc,1,8,0);
}
struct mypoint
{
	CvPoint pt;
	double score;
	mypoint()
	{
		pt.x=0;
		pt.y=0;
		score=0.0;
	};
};

int main(int argc,char* argv[])
{

	//D:\ethzshAllAngle\0
	//_chdir("E:\\CarData\\voc2007\\testing\\car");
	char tD[40];
	if (argc>1)
	{
		sprintf_s(tD,"%s",argv[1]);

	}
	else
	{
		sprintf_s(tD,"009963");
	}
	char tDf[40];
	char tDi[40];
	char tDa[40];

	sprintf_s(tDa,"%s_objs.txt",tD);
	sprintf_s(tDf,"%s_boundary.txt",tD);
	sprintf_s(tDi,"%s.jpg",tD);
	vector<int> vecSiz;
	vecSiz.clear();
	vector<vector<mypoint> > traces;
	vector<mypoint> tr_;
	tr_.clear();

	FILE* ouF;
	ouF=fopen(tDf,"r");
	int temI;
	fscanf(ouF,"%d\n",&temI);
	vecSiz.resize(temI,0);
	traces.resize(temI,tr_);
	for(int i=0;i<temI;i++)
	{
		fscanf(ouF,"%d\t",&vecSiz[i]);
		traces[i].resize(vecSiz[i],mypoint());
	}
	fscanf(ouF,"\n");
	for(int i=0;i<vecSiz.size();i++){
		for	(int j=0;j<vecSiz[i];j++)
		{
			fscanf(ouF,"%d %d %lf\t",&traces[i][j].pt.x,&traces[i][j].pt.y,&traces[i][j].score);
		}
		fscanf(ouF,"\n");
	}

	
	fclose(ouF);

	if( (image = cvLoadImage(tDi, 1)) == 0 )
		return -1;
	sh = cvCreateImage(cvSize(image->width*2,image->height), IPL_DEPTH_8U, 3);
	edge = cvCreateImage(cvSize(image->width,image->height), IPL_DEPTH_8U, 3);
	cvNamedWindow(wndname, 1);
	

	cvZero(sh);
	cvSetImageROI(sh,cvRect(0,0,image->width,image->height));
	cvAdd(sh,image,sh,NULL);
	cvSetImageROI(sh,cvRect(image->width,0,image->width,image->height));
	cvZero(edge);
	int index_c(0);
	if (traces.size()>0)
	{
		for (int i=0;i<traces.size();i++)
		{
			if (traces[i].size()>20)
			{
				for (int j=0;j<traces[i].size();j++)
				{
					drawp(traces[i][j].pt,index_c,edge);
				}
				index_c+=1;
			}
			
		}
	}
	cvAdd(sh,edge,sh,NULL);

	cvResetImageROI(sh);

	cvShowImage(wndname,sh);
	// Create a window


	cvWaitKey(0);
	cvReleaseImage(&image);
	cvReleaseImage(&edge);
	cvReleaseImage(&sh);

	cvDestroyWindow(wndname);

	return 0;
}