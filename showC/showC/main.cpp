#include <stdio.h>
#include <Windows.h>
#include <direct.h>

#include <dos.h>
#include <conio.h>

#include <iostream>


#include "..\..\..\fileIoinclude\FileInOut.h"
#include  "..\..\common\detectCommon.h"
#include "..\..\common\myhog.h"

#include <fstream>
#include <stdio.h>
#include <cv.h>
#include <highgui.h>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>




#pragma comment(lib,"cxcore.lib")
#pragma comment(lib,"cv.lib") 
#pragma comment(lib,"ml.lib") 
#pragma comment(lib,"highgui.lib")  



char wndname[] = "Edge";

char tbarname[] = "Threshold";
int edge_thresh = 60;

vector<vector<double> > values;

IplImage *image = 0, *edge = 0, *sh=0, *bound=0,*pts=0;


vector<vector<int> > rects;

void drawrecs(IplImage* simg)
{
	for (int i = 0; i < rects.size(); i++)
	{
		if (rects[i][4]==1)
		{
			cvRectangle(simg,cvPoint(rects[i][0],rects[i][1]),cvPoint(rects[i][2],rects[i][3]),cvScalar(255,0,0),2,8,0);
		}
		else
		cvRectangle(simg,cvPoint(rects[i][0],rects[i][1]),cvPoint(rects[i][2],rects[i][3]),cvScalar(255,255,0),2,8,0);
	}
}

void drawobjs(IplImage*& igm,string s ,vector<obj> objs, vector<int> ks )
{
	for (int i = 0; i < objs.size(); i++)
	{
		auto pts=inptsForObj(objs[i],s,"",i);
		int wid=ks[i]>45?45:ks[i];
		for(auto pt:pts)
		{
			cvCircle(igm,cvPoint(pt.first,pt.second),wid,cvScalar(rand()%255,rand()%255,255),2,8,0);
		}
	}
}

void on_trackbar(int h)
{
	
	
	for (int i=0;i<image->height;i++)
	{
		for (int j=0;j<image->width;j++)
		{
			if (values[i][j]>edge_thresh*0.001)
			{
				CvScalar s;
				s.val[0]=255;
				s.val[1]=255;
				s.val[2]=255;
			//	cout<<"("<<i<<","<<j<<")";
				cvSet2D(edge,i,j,s); 
			}
			else
			{
				CvScalar s;
				s.val[0]=0;
				s.val[1]=0;
				s.val[2]=0;
				cvSet2D(edge,i,j,s); 
			}
		}
	}
	drawrecs(edge);

	cvZero(sh);

	cvSetImageROI(sh,cvRect(0,0,image->width,image->height));
	cvAdd(sh,image,sh,NULL);
	
	cvSetImageROI(sh,cvRect(image->width,0,image->width,image->height));
	cvAdd(sh,edge,sh,NULL);

	cvSetImageROI(sh,cvRect(0,image->height,image->width,image->height));
	cvAdd(sh,bound,sh,NULL);

	cvSetImageROI(sh,cvRect(image->width,image->height,image->width,image->height));
	cvAdd(sh,pts,sh,NULL);


	cvResetImageROI(sh);

	cvShowImage(wndname,sh);

}


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

int givepositivelist(int argc,char* argv[])
{
	char tD[20];
	if (argc>1)
	{
		sprintf_s(tD,"%s",argv[1]);

	}
	else
	{
		_chdir("E:\\carData\\voc2007\\training\\car");
		sprintf_s(tD,"007815");
	}

	string ns(tD);
	vector<obj> objs=fileIOclass::InVector<obj>(ns+"_objs.txt");


	for (int i = 0; i < objs.size(); i++)
	{
		auto sth=inptsForObj(objs[i],ns,"",i);
		if (sth.size()>= feaNumlimit && objs[i].tror==0)
		{
			char td[40];
			sprintf(td,"%s_obj%s_%d",ns.c_str(),"",i);
			printf("%s\n",td);
		}
	}
	//getchar();
	return 0;
}

int showmain(int argc,char* argv[])
{

	//D:\ethzshAllAngle\0
	//_chdir("E:\\carData\\voc2007\\training\\car");
	char tD[20];
	if (argc>1)
	{
		sprintf_s(tD,"%s",argv[1]);

	}
	else
	{
		_chdir("E:\\carData\\voc2007\\training\\car");
		sprintf_s(tD,"000007");
	}

	string ns(tD);
	vector<obj> objs=fileIOclass::InVector<obj>(ns+"_objs.txt");
	vector<int> kspt=fileIOclass::InVectorInt("..\\..\\kptStep.txt");

	vector<int> ks;
	ks.resize(objs.size(),0);



	for (int i = 0; i < objs.size(); i++)
	{
		int ksi(disp(objs[i],kspt));
		ks[i]=kspt[ ksi];
	}

	

	char tDf[20];
	char tDi[20];
	char tDa[20];
	char tDff[20];

//	sprintf_s

	sprintf_s(tDf,"%s_edge.txt",tD);
	sprintf_s(tDi,"%s.jpg",tD);
	sprintf_s(tDa,"%s_objs.txt",tD);


	sprintf_s(tDff,"%s_boundary.txt",tD);

	FILE* fp;
	fp=fopen(tDf,"r");
	int wid,heit;

	fscanf_s(fp,"%d %d\n",&heit,&wid);

	vector<vector<int> > v_;

//	values=new double*[heit];
	values.resize(heit,vector<double>(wid,0.0));
	
	v_.resize(heit,vector<int>(wid,0));

	for (int i=0;i<heit;i++)
	{
		//values[i]=new double[wid];
		for (int j=0;j<wid;j++)
		{
			double temt;
			fscanf_s(fp,"%lf ",&temt);
			values[i][j]=temt;

		}
		fscanf_s(fp,"\n");
	}
	fclose(fp);

	fp=fopen(tDa,"r");
	int objn;
	fscanf_s(fp,"%d\n",&objn);

	rects.resize(objn,vector<int>(5,0));
	for (int i = 0; i < objn; i++)
	{
	//	int ti;
		string ts;
		fscanf_s(fp,"%d %d %d %d %d %s\n",&rects[i][0],&rects[i][1],&rects[i][2],&rects[i][3],&rects[i][4],&ts);
	}

	fclose(fp);

	// (Xmin, Ymin) - (Xmax, Ymax) : (292, 115) - (381, 406)


	

	vector<int> vecSiz;
	vecSiz.clear();
	vector<vector<mypoint> > traces;
	vector<mypoint> tr_;
	tr_.clear();

	FILE* ouF;
	ouF=fopen(tDff,"r");
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
	if( (pts = cvLoadImage(tDi, 1)) == 0 )
		return -1;

	drawrecs(image);

	sh = cvCreateImage(cvSize(image->width*2,image->height*2), IPL_DEPTH_8U, 3);
	edge = cvCreateImage(cvSize(image->width,image->height), IPL_DEPTH_8U, 3);
	bound = cvCreateImage(cvSize(image->width,image->height), IPL_DEPTH_8U, 3);
//	pts = cvCreateImage(cvSize(image->width,image->height), IPL_DEPTH_8U, 3);

	int index_c(0);
	if (traces.size()>0)
	{
		for (int i=0;i<traces.size();i++)
		{
		//	if (traces[i].size()>20)
			{
				for (int j=0;j<traces[i].size();j++)
				{
					drawp(traces[i][j].pt,index_c,bound);
				}
				index_c+=1;
			}
			
		}
	}
	drawrecs(bound);

	drawobjs(pts,ns,objs,ks);

	cvNamedWindow(wndname, 1);
	cvCreateTrackbar(tbarname, wndname, &edge_thresh, 1000, on_trackbar);

	on_trackbar(60);
	// Create a window


	cvWaitKey(0);
	cvReleaseImage(&image);
	cvReleaseImage(&edge);
	cvReleaseImage(&sh);
	cvReleaseImage(&pts);

	cvDestroyWindow(wndname);

	return 0;
}

void testmyhog(int argc,char* argv[])
{
	char tD[20];
	if (argc>1)
	{
		sprintf(tD,"%s",argv[1]);

	}
	else
	{
		_chdir("E:\\carData\\voc2007\\training\\car");
		sprintf(tD,"000007");
	}

	string ns(tD);
	char tDf[20];
	sprintf(tDf,"%s_edge.txt",tD);
	FILE* fp;
	fp=fopen(tDf,"r");
	int wid,heit;

	fscanf(fp,"%d %d\n",&heit,&wid);



//	values=new double*[heit];
	values.resize(heit,vector<double>(wid,0.0));
	


	for (int i=0;i<heit;i++)
	{
		//values[i]=new double[wid];
		for (int j=0;j<wid;j++)
		{
			//double temt;
			fscanf(fp,"%lf ",&values[i][j]);
			//=temt;

		}
		fscanf(fp,"\n");
	}
	fclose(fp);
	vector<vector<vector<int> > > hogIntr=generateHogIntegrateFromEdgeMap(values);

	char tDff[20];
	sprintf(tDff,"%s_hogInte.txt",tD);
	//FILE* fp;
	fp=fopen(tDff,"w");
	int sa=Number_SignleHog;
	fprintf(fp,"%d %d %d\n",heit,wid,sa);
	for (int i = 0; i < heit; i++)
	{
		for (int j = 0; j < wid; j++)
		{
			for (int k = 0; k < sa; k++)
			{
				fprintf(fp,"%d ",hogIntr[i][j][k]);
			}

		}
		fprintf(fp,"\n");
	}
	fclose(fp);

}

int main(int argc,char* argv[])
{
//	showmain(argc,argv);

//	testmyhog(argc,argv);
	givepositivelist(argc,argv);
	return 0;
}