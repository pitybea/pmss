#include <stdio.h>
#include <Windows.h>
#include <direct.h>

#include <dos.h>
#include <conio.h>

#include <iostream>


#include "..\..\..\fileIoinclude\FileInOut.h"
#include  "..\..\common\detectCommon.h"
#include "..\..\common\myhog.h"
#include "..\..\common\recdeal.h"


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

template<class T>
static void prshl(vector<T> p,int n,vector<int>& index)
{
	int k,j,i;
	T t;
	int ii;
	k=n/2;
	while(k>0)
	{
		for(j=k;j<=n-1;j++)
		{
			t=p[j];  ii=index[j];  i=j-k;
			while((i>=0)&&(p[i]>t))
			{
				p[i+k]=p[i];  index[i+k]=index[i];  i=i-k;
			}
			p[i+k]=t;  index[i+k]=ii;
		}
		k=k/2;
	}
};

static void printPR(vector<double> pos,vector<double> neg)
{
	vector<double> al;
	al.clear();

	vector<int> inx;
	inx.clear();

	al.insert(al.end(),pos.begin(),pos.end());
	al.insert(al.end(),neg.begin(),neg.end());

	vector<int> ti;
	ti.resize(pos.size(),1);

	inx.insert(inx.end(),ti.begin(),ti.end());

	ti.clear();

	ti.resize(neg.size(),-1);
	inx.insert(inx.end(),ti.begin(),ti.end());
	
	vector<int> i__;
	i__.resize(inx.size(),0);
	for (int i = 0; i < inx.size(); i++)
	{
		i__[i]=i;

	}

	prshl(al,(int)al.size(),i__);

	int nnum=0;
	int pnum=0;
	
	int allp=(int)pos.size();

	int az= (int)	inx.size();

	for (int i = 0; i < inx.size(); i++)
	{
		if (inx[i__[i]]==1)
		{
			pnum+=1;
		}
		else
		{
			nnum+=1;
		}
		
		int crrctNum=nnum+allp-pnum;

		int dtnum=allp-pnum;

		if (i>0 && i<(az-1))
		{
			printf("%lf %lf\n", (double)dtnum / (double)allp, (double)dtnum / (double)(az-i-1) );
		}

	}
}
Rect toR(CvRect o)
{
	return Rect(o.x,o.y,o.width,o.height);
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

void drawRcs(IplImage* img, vector<CvRect> rcs,CvScalar cl )
{
	for (int i = 0; i < rcs.size(); i++)
	{
		cvRectangle(img,cvPoint(rcs[i].x,rcs[i].y),cvPoint(rcs[i].x+rcs[i].width,rcs[i].y+rcs[i].height),cl,1,8,0);
	}
}

void simplecollect(string s,	vector<CvRect>& gts, 	vector<CvRect>& dpms,vector<double>&	dscore,vector<CvRect>& dps,vector<double>& pscore)
{
	int on;
	
	FILE* fp=fopen((s+"_gtn.txt").c_str(),"r");
	fscanf(fp,"%d",&on);
	fclose(fp);


	fp=fopen((s+"_gt.txt").c_str(),"r");
	gts.resize(on,cvRect(0,0,0,0));
	for (int i = 0; i < on; i++)
	{
		fscanf(fp,"%d %d %d",&gts[i].y,&gts[i].x,&gts[i].width);
		gts[i].height=gts[i].width*0.4;
	}
	fclose(fp);


	fp=fopen((s+"_dpm.txt").c_str(),"r");
	fscanf(fp,"%d\n",&on);
	dpms.resize(on,cvRect(0,0,0,0));
	dscore.resize(on,0.0);
	vector<double> tm;
	tm.resize(6,0.0);
	for (int i = 0; i < on; i++)
	{
		for (int j = 0; j < 6; j++)
		{
			fscanf(fp, "%lf ",&tm[j]); 
		}
		fscanf(fp,"\n");
		dpms[i].x=tm[0];
		dpms[i].y=tm[1];
		dpms[i].width=tm[2];
		dpms[i].height=tm[3];
		dscore[i]=tm[5];
	}
	fclose(fp);


	fp=fopen((s+"_pms.txt").c_str(),"r");
	fscanf(fp,"%d\n",&on);
	dps.resize(on,cvRect(0,0,0,0));
	pscore.resize(on,0.0);
	double dtm;
	for (int i = 0; i < on; i++)
	{
		fscanf(fp,"%d %d %d %d %lf",&dps[i].x ,&dps[i].y ,&dps[i].width ,&dps[i].height ,&dtm);
		pscore[i]=dtm;
	}

	fclose(fp);
}

void dcideNegPos(const vector<CvRect>& gts,const vector<CvRect>& dts,vector<double> scs, vector<double>& pss,vector<double>& ngs)
{
	vector<bool> lps;
	lps.resize(dts.size(),false);
	
	
	for(auto ss:gts)
	{
		double currentOlpR=0.0;
		for (int i = 0; i < dts.size(); i++)
		{
			double ro=RealoverlabRatio(toR(ss),toR(dts[i]));
			if(ro>currentOlpR && lps[i]==false)
			{
				currentOlpR=ro;
				if (ro>0.1)
				{
					lps[i]=true;
				}
			}
		}
	}
	for (int i = 0; i < lps.size(); i++)
	{
		if (lps[i])
		{
			pss.push_back(scs[i]);
		}
		else
			ngs.push_back(scs[i]);
	}

}

void collectall(vector<string> fls,vector<double>& dpos,vector<double>& dneg,vector<double>& ppos,vector<double>& pneg)
{

	for(auto s:fls)
	{
		vector<CvRect> gts; 	vector<CvRect> dpms; vector<double>	dscore; vector<CvRect> dps;vector<double> pscore;
		simplecollect(s,gts,dpms,dscore,dps,pscore);

		dcideNegPos(gts,dpms,dscore,dpos,dneg);
		dcideNegPos(gts,dps,pscore,ppos,pneg);
	}
}
int givePR(int argc,char* argv[])
{
	string s;
	if (argc>1)
	{
	
		//sprintf_s(tD,"%s",argv[1]);

	}
	else
	{
	//	_chdir("E:\\CarData\\TestImages_Scale");

		//sprintf_s(tD,"007815");
	}
	vector<string> flNms;
	flNms.clear();
	flNms=fileIOclass::InVectorString("car.lst");

	vector<double> dpos;
	vector<double> dneg;
	dpos.clear();
	dneg.clear();

	vector<double> ppos;
	vector<double> pneg;
	ppos.clear();
	pneg.clear();

	collectall(flNms,dpos,dneg,ppos,pneg);

	printPR(dpos,dneg);

	printf("\n______________________________________\n");
	printPR(ppos,pneg);

	return 0;
}



int showcarscale(int argc,char* argv[])
{

	string s;
	if (argc>1)
	{
		s=argv[1];
		//sprintf_s(tD,"%s",argv[1]);

	}
	else
	{
		_chdir("E:\\CarData\\TestImages_Scale");
		s="test-0";
		//sprintf_s(tD,"007815");
	}

	IplImage* img,* imgg,*imgd,*imgp,*ph;

	img=cvLoadImage((s+".pgm").c_str() ,-1);
	imgg=cvLoadImage((s+".pgm").c_str() ,-1);
	imgd=cvLoadImage((s+".pgm").c_str() ,-1);
	imgp=cvLoadImage((s+".pgm").c_str() ,-1);

	int on;
	
	FILE* fp=fopen((s+"_gtn.txt").c_str(),"r");
	fscanf(fp,"%d",&on);
	fclose(fp);

	vector<CvRect> gts;
	fp=fopen((s+"_gt.txt").c_str(),"r");
	gts.resize(on,cvRect(0,0,0,0));
	for (int i = 0; i < on; i++)
	{
		fscanf(fp,"%d %d %d",&gts[i].y,&gts[i].x,&gts[i].width);
		gts[i].height=gts[i].width*0.4;
	}
	fclose(fp);

	vector<CvRect> dpms;
	fp=fopen((s+"_dpm.txt").c_str(),"r");
	fscanf(fp,"%d\n",&on);
	dpms.resize(on,cvRect(0,0,0,0));
	vector<double> tm;
	tm.resize(6,0.0);
	for (int i = 0; i < on; i++)
	{
		for (int j = 0; j < 6; j++)
		{
			fscanf(fp, "%lf ",&tm[j]); 
		}
		fscanf(fp,"\n");
		dpms[i].x=tm[0];
		dpms[i].y=tm[1];
		dpms[i].width=tm[2];
		dpms[i].height=tm[3];
	}
	fclose(fp);

	vector<CvRect> dps;
	fp=fopen((s+"_pms.txt").c_str(),"r");
	fscanf(fp,"%d\n",&on);
	dps.resize(on,cvRect(0,0,0,0));
	double dtm;
	for (int i = 0; i < on; i++)
	{
		fscanf(fp,"%d %d %d %d %lf",&dps[i].x ,&dps[i].y ,&dps[i].width ,&dps[i].height ,&dtm);

	}

	fclose(fp);

	drawRcs(imgg,gts,CV_RGB(255,255,0));
	drawRcs(imgd,dpms,CV_RGB(255,255,0));
	drawRcs(imgp,dps,CV_RGB(255,255,0));

	ph=cvCreateImage(cvSize(img->width*2,img->height*2), IPL_DEPTH_8U, 1);

	cvZero(ph);

	cvSetImageROI(ph,cvRect(0,0,img->width,img->height));
	cvAdd(ph,img,ph,NULL);
	
	cvSetImageROI(ph,cvRect(img->width,0,img->width,img->height));
	cvAdd(ph,imgg,ph,NULL);

	cvSetImageROI(ph,cvRect(0,img->height,img->width,img->height));
	cvAdd(ph,imgd,ph,NULL);

	cvSetImageROI(ph,cvRect(img->width,img->height,img->width,img->height));
	cvAdd(ph,imgp,ph,NULL);


	cvResetImageROI(ph);

	cvNamedWindow("w1", 1);
	cvShowImage("w1",ph);/*
	cvNamedWindow("w2", 1);
	cvShowImage("w2",imgg);
	cvNamedWindow("w3", 1);
	cvShowImage("w3",imgd);
	cvNamedWindow("w4", 1);
	cvShowImage("w4",imgp);*/
	cvWaitKey(0);

	cvReleaseImage(&img);
	cvReleaseImage(&imgg);
	cvReleaseImage(&imgd);
	cvReleaseImage(&imgp);
	return 0;
}
int main(int argc,char* argv[])
{
//	showmain(argc,argv);

//	testmyhog(argc,argv);
//	 showcarscale(argc,argv);
	givePR(argc,argv);
	return 0;
}