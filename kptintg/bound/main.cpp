#include <stdio.h>
#include <Windows.h>
#include <direct.h>

#include <dos.h>
#include <conio.h>

#include <iostream>
using namespace std;
#include <fstream>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <vector>
#include <string>

#include <set>


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

struct CvPoint
{
	int x;
	int y;
};

void funct( int bthresh, int kptstep,string s)
{
	char tDf[40];
	//char tDi[40];
	sprintf_s(tDf,"%s_edge.txt",s.c_str());
	

	FILE* fp;
	fp=fopen(tDf,"r");
	int wid,heit;

	fscanf_s(fp,"%d %d\n",&heit,&wid);

	fclose(fp);
	

	vector<vector<int> > intmap;
	intmap.resize(heit,vector<int>(wid,0));
	


	vector<CvPoint> kpts;
	kpts.clear();


	


	char tDi[40];
	
	int kptn;

	FILE* ouF;
	sprintf_s(tDi,"%s_bt%d_stp%d_num.txt",s.c_str(),bthresh,kptstep);
	ouF=fopen(tDi,"r");
	fscanf(ouF,"%d\n",&kptn);
	fclose(ouF);


	sprintf_s(tDi,"%s_bt%d_stp%d_fpts.txt",s.c_str(),bthresh,kptstep);
	ouF=fopen(tDi,"r");
	
	
	
	CvPoint p;
	kpts.resize(kptn,p);
	

	for (int i = 0; i < kpts.size(); i++)
	{
		
		fscanf(ouF,"%d %d\t",&kpts[i].x,&kpts[i].y);
		intmap[kpts[i].y][kpts[i].x]=1;
	
	}
		
	
	fclose(ouF);

	for (int i = 1; i < heit; i++)
	{
		intmap[i][0]=intmap[i-1][0]+intmap[i][0];
	}
	for (int i = 1; i < wid; i++)
	{
		intmap[0][i]=intmap[0][i-1]+intmap[0][i];
	}
	for (int i = 1; i < heit; i++)
	{
		for (int j = 1; j < wid; j++)
		{
			intmap[i][j]=intmap[i][j]+intmap[i-1][j]+intmap[i][j-1]-intmap[i-1][j-1];
		}
	}



	sprintf_s(tDi,"%s_bt%d_stp%d_kptintg.txt",s.c_str(),bthresh,kptstep);
//	FILE* ouF;
	ouF=fopen(tDi,"w");
	
	fprintf(ouF,"%d %d\n",intmap.size(),intmap[0].size());
	for (int i = 0; i < heit; i++)
	{
		for (int j = 0; j < wid; j++)
		{
			fprintf(fp,"%d ",intmap[i][j]);
		}
		fprintf(fp,"\n");
	}
		
	
	fclose(ouF);
}


void genInt(string s)
{
	char tDf[40];
	//char tDi[40];
	sprintf_s(tDf,"%s_size.txt",s.c_str());
	

	FILE* fp;
	fp=fopen(tDf,"r");
	int wid,heit;

	fscanf_s(fp,"%d %d\n",&heit,&wid);

	fclose(fp);
	

	vector<vector<int> > intmap;
	intmap.resize(heit,vector<int>(wid,0));
	
	//vector<vector<int> > tintmap;

	vector<CvPoint> kpts;
	kpts.clear();


	


	char tDi[40];
	
	int kptn;

	FILE* ouF;
	sprintf_s(tDi,"%s_num.txt",s.c_str());
	ouF=fopen(tDi,"r");
	fscanf(ouF,"%d\n",&kptn);
	fclose(ouF);


	sprintf_s(tDi,"%s_pts.txt",s.c_str());
	ouF=fopen(tDi,"r");
	
	
	
	CvPoint p;
	kpts.resize(kptn,p);
		set<int> cnt;
		
	for (int i = 0; i < kpts.size(); i++)
	{
		fscanf(ouF,"%d %d\n",&kpts[i].x,&kpts[i].y);
		intmap[kpts[i].y][kpts[i].x]=1;
		cnt.insert(kpts[i].y*wid+kpts[i].x);
	}
		
	
	fclose(ouF);

	for (int i = 1; i < heit; i++)
	{
		intmap[i][0]=intmap[i-1][0]+intmap[i][0];
	}
	for (int i = 1; i < wid; i++)
	{
		intmap[0][i]=intmap[0][i-1]+intmap[0][i];
	}
	for (int i = 1; i < heit; i++)
	{
		for (int j = 1; j < wid; j++)
		{
			intmap[i][j]=intmap[i][j]+intmap[i-1][j]+intmap[i][j-1]-intmap[i-1][j-1];
		}
	}



	sprintf_s(tDi,"%s_kptintg.txt",s.c_str());
//	FILE* ouF;
	ouF=fopen(tDi,"w");
	
	fprintf(ouF,"%d %d\n",intmap.size(),intmap[0].size());
	for (int i = 0; i < heit; i++)
	{
		for (int j = 0; j < wid; j++)
		{
			fprintf(fp,"%d ",intmap[i][j]);
		}
		fprintf(fp,"\n");
	}
		
	
	fclose(ouF);
}

int main(int argc,char* argv[])
{
	string s;
	if (argc>1)
	{
		s=argv[1];
	}
	else
	{
		_chdir("E:\\CarData\\TestImages_Scale");
		s="test-95";
	}

	genInt(s);
	return 0;
}

int maina(int argc,char* argv[])
{
	//
//	char tD[40];



	string s;
	if (argc>1)
	{
		s=argv[1];
	}
	else
	{
		_chdir("E:\\CarData\\voc2007\\training\\car");
		s="003007";
	}
	vector<int> thrs=infromstring("..\\..\\bndThreshld.txt");

	vector<int> kspt=infromstring("..\\..\\kptStep.txt");

	for (int i = 0; i < thrs.size(); i++)
	{
		for (int j = 0; j < kspt.size(); j++)
		{
			funct(thrs[i],kspt[j],s);
		}
	}
	
	return 0; 
}