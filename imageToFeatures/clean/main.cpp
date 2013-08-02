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



class featype
{
	public:
	featype()
	{

	};
	Point pos;
	double feature[featureDimension];

	static void initOne(FILE* fp,featype &t)
	{
		fscanf(fp,"%d %d\n",&t.pos.x,&t.pos.y);
		for (int i=0;i<featureDimension;i++)
		{
			fscanf(fp,"%lf ",&t.feature[i]);
		}
	};
	static void printOne(FILE* fp, featype t)
	{
		fprintf(fp,"%d %d\n",t.pos.x,t.pos.y);
		for (int i=0;i<featureDimension;i++)
		{
			fprintf(fp,"%lf ",t.feature[i]);
		}
	};

};
void main(int argc, char* argv[])
{
	string s;
#ifdef debugMode
	_chdir("E:\\project\\pmsvoc");
	s="000245";
#else
	if (argc<2)
	{
		printf("not enough parameters");
	//	return -1;
	}
	
	s=argv[1];
#endif	
	
	
		//string s=img_names[img_index];
		
		Mat iptimg=imread(s+".jpg");

	
	
		
		
		vector<float> fsth;


		

		SurfDescriptorExtractor extractor;

		Mat descriptor;

		vector<KeyPoint> temkpts;

		//SIFT(int nfeatures=0, int nOctaveLayers=3, double contrastThreshold=0.04, double edgeThreshold=10, double sigma=1.6)

	

		SurfFeatureDetector detector;
		detector.detect(iptimg,temkpts);

	/*	temkpts.clear();
		
		string kpfn=s+"_kpts.txt";
		FILE* fp;
		fopen_s(&fp,kpfn.c_str(),"r");

		int kptn;
		fscanf_s(fp,"%d\n",&kptn);
		KeyPoint kpt;
		temkpts.resize(kptn,kpt);
		for (int i = 0; i < kptn; i++)
		{
			int tx,ty;
			double ts;
			fscanf_s(fp,"%d %d %lf\n",&tx,&ty,&ts);
			temkpts[i].pt.x=tx;
			temkpts[i].pt.y=ty;
			temkpts[i].size=ts;
		}

		fclose(fp);

		*/

		

		extractor.compute(iptimg,temkpts,descriptor);

		
	
		vector<featype> feas;
		feas.clear();
		for (auto i=0;i<descriptor.rows;i++)
		{
			featype temfes;

			temfes.pos.x=(int)temkpts[i].pt.x;
			temfes.pos.y=(int)temkpts[i].pt.y;
			Scalar ss(255,0,0);
		
			//temfes.feature.resize(128,0.0);
			double temsum(0.000001);
			for (auto j=0;j<descriptor.cols;j++)
			{
				temfes.feature[j]=descriptor.at<float>(i,j);
				temsum+=temfes.feature[j];
			}
			for (auto j=0;j<descriptor.cols;j++)
			{
				temfes.feature[j]/=temsum;
	
			}
	
	
			feas.push_back(temfes);
		}
		fileIOclass::OutVector(s+"_surf.txt",feas);
	
		
	
  //  return 0;

}

