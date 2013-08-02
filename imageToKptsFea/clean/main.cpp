#include <opencv2/core/core.hpp>
#include <opencv2/features2d/features2d.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>
#include <fstream>


#include <vector>
#include <string>
#include <windows.h>
#include <direct.h>

#include "FileInOut.h"
using namespace std;
using namespace cv;
//#define debugMode

#define featureDimension 64

vector<int> infromstring(string s)
{
	vector<int> rslt;
	rslt.clear();
	FILE* fp=fopen(s.c_str(),"r");
	int tn;
	fscanf(fp,"%d\n",&tn);
	rslt.resize(tn,0);
	for (int i = 0; i < tn; i++)
	{
		fscanf(fp,"%d\n",&rslt[i]);
	}
	fclose(fp);
	return rslt;
}



template<class T>
vector<vector<T> > TransitMtx(vector<vector<T> > oData,vector<vector<T> >trasM)
{
	vector<vector<T> > tData;
	tData.resize(oData.size(),vector<T>(trasM[0].size(),0.0));
	
	for (int i=0;i<oData.size();i++)
	{

		for (int j=0;j<trasM[0].size();j++)
		{
			for (int k=0;k<trasM.size();k++)
			{
				tData[i][j]+=oData[i][k]*trasM[k][j];
			}
		}
	}
	return tData;
}


template<class T>
int ZeroMnVec(vector<vector<T> >& dataset)
{
	if (dataset.size()>0)
	{
		if (dataset[0].size()>0)
		{
			for (int i=0;i<dataset.size();i++)
			{
				double temtol(0.0);
				for (int j=0;j<dataset[i].size();j++)
				{
					temtol+=dataset[i][j];
				}
				temtol/=dataset[0].size();
				for (int j=0;j<dataset[i].size();j++)
				{
					dataset[i][j]-=temtol;
				}
			}
		}
	}
	return 0;
}

vector<vector<double> > pcamatrix;

void kptstoNumKpFea(Mat iptimg, int thr, int stp,string s)
{
	vector<KeyPoint> temkpts;
	char tm[40];
	sprintf(tm,"%s_bt%d_stp%d_kpts.txt",s.c_str(),thr,stp);

	FILE* ft;
	ft=fopen(tm,"r");
	int tnn;
	
	fscanf(ft,"%d\n",&tnn);
	KeyPoint tkpt;
	//tnn=10;
	temkpts.resize(tnn,tkpt);
	for (int i = 0; i < tnn; i++)
	{
		int tmx,tmy;
		fscanf(ft,"%d %d\t",&tmx,&tmy);
		temkpts[i].pt.x=tmx;
		temkpts[i].pt.y=tmy;
		temkpts[i].size=stp;
	}
	fclose(ft);

	
	SurfDescriptorExtractor extractor;

	Mat descriptor;

	vector<vector<double> > feas;
	feas.clear();

	
	extractor.compute(iptimg, temkpts ,descriptor);
	feas.resize(descriptor.rows,vector<double>(featureDimension,0.0));
	for(int i=0;i<descriptor.rows;i++)
	{
		
		
		//temfes.feature.resize(128,0.0);
		double temsum(0.000001);
		for (int j=0;j<descriptor.cols;j++)
		{
			feas[i][j]=descriptor.at<float>(i,j);
			temsum+=feas[i][j];
		}
		for (int j=0;j<descriptor.cols;j++)
		{
			feas[i][j]/=temsum;
	
		}
	
	
	//	feas.push_back(temfes);
	}
	char tD[40];
	sprintf(tD,"%s_bt%d_stp%d",s.c_str(),thr,stp);
	string mys(tD);
	FILE *fp;
	fp=fopen((mys+"_num.txt").c_str(),"w");
	fprintf(fp,"%d",temkpts.size());
	fclose(fp);
	fp=fopen((mys+"_fpts.txt").c_str(),"w");
	for (int i = 0; i < temkpts.size(); i++)
	{
		fprintf(fp,"%d %d\n",(int)temkpts[i].pt.x,(int)temkpts[i].pt.y);
	}
	fclose(fp);
	fp=fopen((mys+"_feas.txt").c_str(),"w");
	for (int i = 0; i < feas.size(); i++)
	{
		for (int j = 0; j < featureDimension; j++)
		{
			fprintf(fp,"%lf ",feas[i][j]);
		}
		fprintf(fp,"\n");
	}
	fclose(fp);
	/*
	ZeroMnVec(feas);

	vector<vector<double> > opp=TransitMtx(feas,pcamatrix);

	fp=fopen((mys+"_dimrd.txt").c_str(),"w");
	for (int i = 0; i < opp.size(); i++)
	{
		for (int j = 0; j < opp[i].size(); j++)
		{
			fprintf(fp,"%lf ",feas[i][j]);
		}
		fprintf(fp,"\n");
	}
	fclose(fp);*/
	
	

}

void main(int argc, char* argv[])
{
	string s;
	
	s=argv[1];
	
	//_chdir("E:\\CarData\\voc2007\\training\\car");
	
	//s="001119";
	Mat iptimg=imread(s+".jpg");


	vector<int> thrs=infromstring("..\\..\\bndThreshld.txt");

	vector<int> kspt=infromstring("..\\..\\kptStep.txt");
	/*
	pcamatrix.clear();
	pcamatrix.resize(64,vector<double>(63,0.0));
	FILE* fp=fopen("pcaMatrix.txt","r");
	for (int i = 0; i < 64; i++)
	{
		for (int j = 0; j < 63; j++)
		{
			fscanf(fp,"%lf ",&pcamatrix[i][j]);
		}
		fscanf(fp,"\n");
	}
	fclose(fp);*/

	for (int i = 0; i < thrs.size(); i++)
	{
		for (int j = 0; j < kspt.size(); j++)
		{
			kptstoNumKpFea(iptimg,thrs[i],kspt[j],s);
		}
	}

	
}

