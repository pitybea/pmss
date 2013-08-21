
#include <iostream>
#include <fstream>


#include <vector>
#include <string>
#include <windows.h>
#include <direct.h>
#include <map>
#include <unordered_map>
#include <unordered_set>
#include "..\..\..\fileIoinclude\FileInOut.h"

#include  "..\..\common\detectCommon.h"
#include "..\..\common\PMS.h"

#define at_l_f_n 25
using namespace std;


striple<vector<vector<pair<LInt,LInt> > >,map<int,map<int, int > >,map<int,map<int,bool> >>  kptstoNumKpFea( string mys, PMStruc& ped)
{
	striple<vector<vector<pair<LInt,LInt> > >,map<int,map<int, int > >,map<int,map<int,bool> >>  rlst;
	int tnum;

	FILE *fp;
	fp=fopen((mys+"_num.txt").c_str(),"r");
	fscanf(fp,"%d",&tnum);
	fclose(fp);

	vector<pair<int,int> > temkpts;

	temkpts.clear();
	temkpts.resize(tnum,pair<int,int>(0,0));

	fp=fopen((mys+"_pts.txt").c_str(),"r");
	for (int i = 0; i < temkpts.size(); i++)
	{
		fscanf(fp,"%d %d\n",&temkpts[i].first,&temkpts[i].second);
		rlst.second[temkpts[i].second][temkpts[i].first]=i;
		rlst.third[temkpts[i].second][temkpts[i].first]=false;
	}
	fclose(fp);

	vector<vector<double> > feas;
	feas.resize(tnum,vector<double>(ped.getadim(),0.0));

	fp=fopen((mys+"_dimrd.txt").c_str(),"r");

	double temdd;
	for (int i = 0; i < feas.size(); i++)
	{
		for (int j = 0; j < featureDimension-1; j++)
		{
			if(j < ped.getadim())
				fscanf(fp,"%lf ",&feas[i][j]);
			else
				fscanf(fp,"%lf ",&temdd);
		}
		fscanf(fp,"\n");
	}
	fclose(fp);

	rlst.first.clear();
	
//	fss.clear();
//	fss.resize(feas.size(),vector<pair<int,int> > ());

	rlst.first.resize(feas.size(),vector<pair<LInt,LInt> >());
	for (int i = 0; i < feas.size(); i++)
	{
		rlst.first[i]=ped.dataToTwoInxs(feas[i]);
		
	}


	return rlst;
}


pair<sdtctHy,double > detectBst(sDetRec drc, int height,int width,const striple<vector<vector<pair<LInt,LInt> > >,map<int,map<int, int > >, map<int,map<int,bool> > > & ref,PMStruc& pmd)
{
	sdtctHy rslt;
	double alss=0.0;

	int startx=0;
	int starty=0;
	int endx=width-drc.w;
	int endy=height-drc.h;

	obj o;
	double ratiox(stdd_X/(double)(drc.w)),ratioy(stdd_Y/(double)(drc.h));

	for (int i =starty; i < endy; i+=drc.ystp)
	{
		for (int j = startx; j < endx; j+=drc.xstp)
		{
			
			o.xmin=j;
			o.xmax=j+drc.w;
			o.ymin=i;
			o.ymax=i+drc.h;
		
		
	
			auto oneok=rangequery(o,ref.second,ref.third);

			if(oneok.second.size()>at_l_f_n )
			{
				sdtctHy dt;
				dt.sx=j;
				dt.sy=i;
				dt.w=drc.w;
				dt.h=drc.h;

				vector<vector<double> > pss;
				pss.resize(oneok.first.size(),vector<double>(2,0.0));
				for (int i = 0; i < oneok.first.size(); i++)
				{
					pss[i][0] = (oneok.first[i][0]-j)*ratiox;
					pss[i][1] = (oneok.first[i][1]-i)*ratioy;
				}

				double tsc=pmd.givePyramidMatchScoreSpecial(ref.first,pss,oneok.second,false,vector<pair<int,int> >());

			//	printf("%d %d %lf\n",dt.sx,dt.sy,tsc);
				if (tsc>alss)
				{
					rslt=dt;
					alss=tsc;
				}
			}
		}
	}
	return pair<sdtctHy,double >(rslt,alss);
}

/*
pair<vector<dtctHy>,vector<double> > detectBst100(DetRec drc, int height,int width,const vector< triple<vector<vector<pair<int,int> > >,map<int,map<int, int > >, vector<vector<int> > > >& ref,PMStruc& pmd,const vector<int>& thrs)
{
	vector<dtctHy> rslt;
	vector<double> alss;

	int startx=0-drc.w/2;
	int starty=0-drc.h/2;
	int endx=width+drc.w/2;
	int endy=height+drc.h/2;

	vector<double> scores;
	vector<dtctHy> dts;
	vector<int> inx;
	
	
	double ratiox(stdd_X/(double)(drc.w)),ratioy(stdd_Y/(double)(drc.h));
	obj o;
	for (int i =starty; i < endy; i+=drc.ystp)
	{
		for (int j = startx; j < endx; j+=drc.xstp)
		{
			
			o.xmin=j;
			o.xmax=j+drc.w;
			o.ymin=i;
			o.ymax=i+drc.h;
			auto gth=odisp(o,ref,drc.fnumlim);
			if(gth.second>=drc.fnumlim)
			{
				int thi(gth.first);
				auto oneok=rangequery(o,ref[thi].kptFea);
				dtctHy dt;

				dt.sx=j;
				dt.sy=i;
				dt.w=drc.w;
				dt.h=drc.h;
				dt.thr=thrs[thi];
				dt.stp=drc.stp;

				dt.inx=oneok.second;
				vector<vector<double> > pss;
				pss.resize(oneok.first.size(),vector<double>(2,0.0));
				for (int i = 0; i < oneok.first.size(); i++)
				{
					pss[i][0] = (oneok.first[i][0]-j)*ratiox;
					pss[i][1] = (oneok.first[i][1]-i)*ratioy;
				}

				scores.push_back(pmd.givePyramidMatchScoreSpecial(ref[thi].feas,pss,oneok.second,false,dt.score));

				dts.push_back(dt);
				inx.push_back(inx.size());
			}
		}
	}
	if (scores.size()>0)
	{
		prshl(scores,scores.size(),inx);
		for (int i = 0; i < 100 && i<inx.size(); i++)
		{
			int dxi=inx.size()-1-i;
			rslt.push_back(dts[inx[dxi]]);
			alss.push_back(scores[inx[dxi]]);
		}
	}

	return pair<vector<dtctHy>,vector<double> >( rslt,alss);
}*/
vector<vector<double> >  getAllfeas(int dim)
{
	vector<vector<double> > rslt;
	rslt.clear();



	vector<string> flNms;
	flNms.clear();
	flNms=fileIOclass::InVectorString("positive.lst");
	
	for(auto s : flNms)
	{
		vector<vector<double> > tvd;
		tvd.clear();
		tvd=fileIOclass::InVectorSDouble(s+"_ptscpp.txt");

		auto tvm=selectVecButLstTwo(tvd,dim);

		rslt.insert(rslt.end(),tvm.begin(),tvm.end());

	}


	return   rslt;
	
}

void main(int argc, char* argv[])
{
	string s;
	
	//s=argv[1];
	
	//_chdir("E:\\car");
	
	if (argc==1)
	{
		_chdir("E:\\CarData\\TestImages_Scale");
		s="test-0";

	}
	else
	{
		s=argv[1];
	}
	
	//_chdir("E:\\carData\\TrainImages");
	

	PMStruc pedmd;
	PMStruc::loadFromfile(pedmd,"pmsuiuc");

	vector<sDetRec> arcs =fileIOclass::InVector<sDetRec>("..\\detRecs.txt");

	auto ss =	kptstoNumKpFea(s,pedmd);

	int heit,widt;

	FILE* fp;
	fp=fopen((s+"_size.txt").c_str(),"r");
	fscanf(fp,"%d %d",&heit,&widt);
	fclose(fp);

	vector<sdtctHy> fnlrslt;
	fnlrslt.clear();
	vector<double> fnlscore;
	fnlscore.clear();

	while(fnlrslt.size()<20)
	{
		printf("%d  ",fnlrslt.size());
		vector<double> allscores;
		vector<sdtctHy> alldts;
		for (int i = 0; i < arcs.size(); i++)
		{
			
			auto mydt=detectBst(arcs[i],heit,widt,ss,pedmd);
			alldts.push_back(mydt.first);
			allscores.push_back(mydt.second);
			
		//	printf("%d %lf\n",i,mydt.second);
		}
	
		vector<int> indx_(allscores.size(),0);
		for (int i = 0; i < allscores.size(); i++)
		{
			indx_[i]=i;
		}
		prshl(allscores,allscores.size(),indx_);
		int dxi=indx_[indx_.size()-1];

		
		
		double lscore=allscores[dxi];
		if (lscore<0.0001)
		{
			break;
		}
		sdtctHy fdt = alldts[dxi];
		setUsed(fdt,ss.third);

		fnlrslt.push_back(fdt);
		fnlscore.push_back(lscore);
	//	printf("%lf\n",lscore);
	//	printf("%d %d %d %d \n",fdt.sx,fdt.sy,fdt.w,fdt.h );
	//	getchar();

	}
	fp=fopen((s+"_pms.txt").c_str(),"w");
	fprintf(fp,"%d\n",fnlrslt.size());
	for (int i = 0; i < fnlrslt.size(); i++)
	{
		fprintf(fp,"%d %d %d %d ",fnlrslt[i].sx,fnlrslt[i].sy,fnlrslt[i].w,fnlrslt[i].h );
		fprintf(fp,"%lf\n",fnlscore[i]);
	}
	fclose(fp);

//	getchar();
	
}

