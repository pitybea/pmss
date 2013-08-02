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

int findSeed(vector<vector<int>> ede_map,vector<CvPoint>& seed)
{
	seed.clear();
	for	(int i=0;i<ede_map.size();i++)
		for(int j=0;j<ede_map[0].size();j++)
		{
			if(ede_map[i][j]>0)
			{
				seed.push_back(cvPoint(j,i));
				return 0;
			}
		}

	return -1;
}
int voidadds(vector<CvPoint>& addto,CvPoint o,vector<vector<int>>& ede_map)
{
	if((o.x>=0)&&(o.y>=0)&&(o.x<ede_map[0].size())&&(o.y<ede_map.size()))
	{
		if(ede_map[o.y][o.x]>0)
		{
			addto.push_back(o);
			ede_map[o.y][o.x]=0;
		}
	}
	return 0;
}

int traceout(vector<vector<int>>& ede_map,vector<CvPoint> seeds,vector<CvPoint> templ,vector<CvPoint>& trac_)
{
	vector<CvPoint> firstbuffer;
	vector<CvPoint> secondbuffer;

	firstbuffer.swap(seeds);

	while(firstbuffer.size()>0)
	{
		for(int i=0;i<firstbuffer.size();i++)
		{
			trac_.push_back(firstbuffer[i]);
		//	ede_map[firstbuffer[i].y] [firstbuffer[i].x]=0;
			for(int j=0;j<templ.size();j++)
			{
				voidadds(secondbuffer,cvPoint(firstbuffer[i].x+templ[j].x,firstbuffer[i].y+templ[j].y),ede_map);
			}
		}
		firstbuffer.clear();
		firstbuffer.swap(secondbuffer);
		
	}
	return 0;
}

int edeto(int& totalnum,vector<vector<int>>& ede_map,vector<vector<CvPoint>>& traces)
{
	traces.clear();

	vector<CvPoint> templ1,templ2;
	templ1.clear();
	templ2.clear();
	templ1.push_back(cvPoint(0,1));
	templ1.push_back(cvPoint(1,1));
	templ1.push_back(cvPoint(1,0));
	templ1.push_back(cvPoint(1,-1));

	templ2.push_back(cvPoint(-1,0));
	templ2.push_back(cvPoint(-1,1));
	templ2.push_back(cvPoint(0,1));
	templ2.push_back(cvPoint(-1,-1));
	while(totalnum>0)
	{
		vector<CvPoint> trace;

		vector<CvPoint> seed;
		seed.clear();
		trace.clear();
		findSeed(ede_map,seed);
		if(seed.size()==0)
		{
			continue;
		}
		voidadds(trace,seed[0],ede_map);
//		trace.push_back(seed[0]);
		totalnum-=1;

		vector<CvPoint> rightseeds;
		vector<CvPoint> right;
		vector<CvPoint> leftseeds;
		vector<CvPoint> left;
		rightseeds.clear();
		right.clear();
		leftseeds.clear();
		left.clear();

		voidadds(rightseeds,cvPoint(seed[0].x+0,seed[0].y+1),ede_map);
		voidadds(rightseeds,cvPoint(seed[0].x+1,seed[0].y+1),ede_map);
		voidadds(rightseeds,cvPoint(seed[0].x+1,seed[0].y+0),ede_map);

		voidadds(leftseeds,cvPoint(seed[0].x+(-1),seed[0].y+1),ede_map);
		voidadds(leftseeds,cvPoint(seed[0].x+(-1),seed[0].y+0),ede_map);
		
		traceout(ede_map,rightseeds,templ1,right);
		totalnum-=right.size();
		traceout(ede_map,leftseeds,templ2,left);
		totalnum-=left.size();
		if(right.size()>0)
		for(int i=0;i<right.size();i++)
		{
			trace.push_back(right[i]);
		}
		if(left.size()>0)
		for(int i=0;i<left.size();i++)
		{
			trace.insert(trace.begin(),left[left.size()-1-i]);
		}

		traces.push_back(trace);

	}
	return 0;
}

int main(int argc,char* argv)
{
	_chdir("F:\\robuh\\images");
	char tD[40];
	if (argc>1)
	{
		sprintf_s(tD,"%s",argv[1]);

	}
	else
	{
		sprintf_s(tD,"motorbikes_rotated_1");
	}
	char tDf[40];
	//char tDi[40];
	sprintf_s(tDf,"%s.txt",tD);

	double** values;

	double thresh_edge(10);

	FILE* fp;
	fp=fopen(tDf,"r");
	int wid,heit;

	fscanf_s(fp,"%d %d\n",&heit,&wid);
	values=new double*[heit];

	vector<vector<int>> edemap;
	vector<int> edemap_;
	edemap_.resize(wid,0);
	edemap.resize(heit,edemap_);
	int totalnum(0);
	for (int i=0;i<heit;i++)
	{
		values[i]=new double[wid];
		for (int j=0;j<wid;j++)
		{
			fscanf_s(fp,"%lf\t",&values[i][j]);
			if (values[i][j]>thresh_edge*0.01)
			{
				totalnum+=1;
				edemap[i][j]=1;
			}
			
		}
		fscanf_s(fp,"\n");
	}
	fclose(fp);
	

	for (int i=0;i<heit;i++)
	{
		delete[] values[i];
	}
	delete[] values;

	vector<vector<CvPoint>> traces;
	edeto(totalnum,edemap,traces);

	char tDi[40];
	sprintf_s(tDi,"%s_edge.txt",tD);
	FILE* ouF;
	ouF=fopen(tDi,"w");
	if(traces.size()>0)
	{
		fprintf(ouF,"%d\n",traces.size());
		for(int i=0;i<traces.size();i++)
		{
			fprintf(ouF,"%d\t",traces[i].size());
		}
		fprintf(ouF,"\n");
		for(int i=0;i<traces.size();i++){
			for	(int j=0;j<traces[i].size();j++)
			{
				fprintf(ouF,"%d %d\t",traces[i][j].x,traces[i][j].y);
			}
			fprintf(ouF,"\n");
		}
	}
	fclose(ouF);
	return 0; 
}