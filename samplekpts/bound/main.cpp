#include <stdio.h>
#include <Windows.h>
#include <direct.h>

#include <dos.h>
#include <conio.h>
#include <iostream>

#include <fstream>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <vector>
#include <string>

using namespace std;

struct Point
{
	int x;
	int y;
};

inline int dist(Point a,Point b)
{
	int w=a.x-b.x;
	int h=a.y-b.y;
	return (w*w+h*h);
}
bool okpts(const vector<Point>& gkpt,const Point& o,int thresh)
{
	int lth=gkpt.size();
	for (int i = lth-1; i >=0 ; i--)
	{
		if (dist(gkpt[i],o)<thresh)
		{
			return false;
		}
	}
	return true;
}

void findkpts(const vector<vector<int> >& ref,vector<Point>& kpts,int thresh)
{
	kpts.clear();
	int width=ref[0].size();
	for (int i = 0; i < ref.size(); i++)
	{
		for (int j = 0; j <width ; j++)
		{
			if (ref[i][j]>0)
			{
				Point o;
				o.x=j;
				o.y=i;
				if (okpts(kpts,o,thresh))
				{
					kpts.push_back(o);
				}
			}
		}
	}
}

int main(int argc,char* argv[])
{
	//
	char tD[40];
	double thresh_edge;

	int bthresh;
	int kptstep;

	if (argc>1)
	{
		sprintf_s(tD,"%s",argv[1]);
		bthresh=atoi(argv[2]);
		thresh_edge=(double)atoi(argv[2])/10.0;
		kptstep=atoi(argv[3]);
	}
	else
	{
		_chdir("E:\\CarData\\voc2007\\training\\car");
		sprintf_s(tD,"000007");
		bthresh=30;
		thresh_edge=5.0;
		kptstep=7;
	}
	char tDf[40];
	//char tDi[40];
	sprintf_s(tDf,"%s_edge.txt",tD);

	

	

	FILE* fp;
	fp=fopen(tDf,"r");
	int wid,heit;

	fscanf_s(fp,"%d %d\n",&heit,&wid);


	vector<vector<int>> edemap;
	vector<int> edemap_;
	edemap_.resize(wid,0);
	edemap.resize(heit,edemap_);
	int totalnum(0);

	double value;
	for (int i=0;i<heit;i++)
	{
	
		for (int j=0;j<wid;j++)
		{
			fscanf_s(fp,"%lf\t",&value);
			if (value>thresh_edge*0.01)
			{
				totalnum+=1;
				edemap[i][j]=1;
			}
			
		}
		fscanf_s(fp,"\n");
	}
	fclose(fp);
	

	
	vector<Point> kpts;

	findkpts(edemap,kpts,kptstep*kptstep);

	char tDi[40];
	sprintf_s(tDi,"%s_bt%d_stp%d_kpts.txt",tD,bthresh,kptstep);
	FILE* ouF;
	ouF=fopen(tDi,"w");
	if(kpts.size()>0)
	{
		fprintf(ouF,"%d\n",kpts.size());
		for (int i = 0; i < kpts.size(); i++)
		{
			fprintf(ouF,"%d %d\t",kpts[i].x,kpts[i].y);
		}
		
	}
	fclose(ouF);


	return 0; 
}