
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


using namespace std;



static void feasToIntPairs( int thr, int stp,string s,PMStruc& ped)
{

	
	char tD[40];
	sprintf(tD,"%s_bt%d_stp%d",s.c_str(),thr,stp);
	string mys(tD);

	int tnum;

	FILE *fp;
	fp=fopen((mys+"_num.txt").c_str(),"r");
	fscanf(fp,"%d",&tnum);
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

	vector<vector<pair<int,int> > > fss;
	fss.clear();
//	fss.resize(feas.size(),vector<pair<int,int> > ());
	for (int i = 0; i < feas.size(); i++)
	{
		fss.push_back(ped.dataToTwoInxs(feas[i]));
	}
	//map<int,map<int, int > > kptFea;

	
	

	//auto sth=rangequery(10,0,200,300,rslt.kptFea);

	//vector<vector<int> > intmap;

	FILE* ouF;
	ouF=fopen((mys+"_fss.txt").c_str(),"w");
	
	fprintf(ouF,"%d\n",fss.size());
	for (int i = 0; i < fss.size(); i++)
	{
		for (int j = 0; j < fss[i].size(); j++)
		{
			fprintf(ouF,"%d %d ",fss[i][j].first,fss[i][j].second);
		}
		fprintf(ouF,"\n");
	}


		
	
	fclose(ouF);
	

}


static void kptstoNumKpFea( int thr, int stp,string s,triple<vector<vector<pair<int,int> > >,map<int,map<int, int > >, vector<vector<int> > >& rslt, PMStruc& ped)
{


	vector<pair<int,int> > temkpts;

	temkpts.clear();

	rslt.feas.clear();


	
	char tD[40];
	sprintf(tD,"%s_bt%d_stp%d",s.c_str(),thr,stp);
	string mys(tD);

	int tnum;

	FILE *fp;
	fp=fopen((mys+"_num.txt").c_str(),"r");
	fscanf(fp,"%d",&tnum);
	fclose(fp);

	temkpts.resize(tnum,pair<int,int>(0,0));


	fp=fopen((mys+"_fpts.txt").c_str(),"r");
	for (int i = 0; i < temkpts.size(); i++)
	{
		fscanf(fp,"%d %d\n",&temkpts[i].first,&temkpts[i].second);
	}
	fclose(fp);

	rslt.feas.resize(tnum,vector<pair<int,int> >(ped.getlvlmt(),pair<int,int>(0,0)));

	fp=fopen((mys+"_fss.txt").c_str(),"r");
	fscanf(fp,"%d",&tnum);
	for (int i = 0; i < rslt.feas.size(); i++)
	{
		for (int j = 0; j <rslt.feas[i].size(); j++)
		{
			//fscanf(fp,"%lf ",&rslt.feas[i][j]);
			fscanf(fp,"%d %d ",&rslt.feas[i][j].first,&rslt.feas[i][j].second);
		}
		fscanf(fp,"\n");
	}
	fclose(fp);


	//map<int,map<int, int > > kptFea;

	rslt.kptFea.clear();

	for (int i = 0; i < temkpts.size(); i++)
	{
		int a=temkpts[i].second;
		if (rslt.kptFea.count(a)>0)
		{
			int b=temkpts[i].first;
			rslt.kptFea[a].insert(pair<int,int>(b,i));
		}
		else
		{
			map<int, int > sth;
			int b=temkpts[i].first;
			sth.insert(pair<int,int>(b,i));
			rslt.kptFea.insert(pair<int,map<int, int > >(a,sth));
		}
	}

	

	//auto sth=rangequery(10,0,200,300,rslt.kptFea);

	//vector<vector<int> > intmap;

	FILE* ouF;
	ouF=fopen((mys+"_kptintg.txt").c_str(),"r");
	
	int heit,wid;
	fscanf(ouF,"%d %d\n",&heit,&wid);
	rslt.intmap.resize(heit,vector<int>(wid,0));

	for (int i = 0; i < heit; i++)
	{
		for (int j = 0; j < wid; j++)
		{
			fscanf(fp,"%d ",&rslt.intmap[i][j]);
		}
		fscanf(fp,"\n");
	}
		
	
	fclose(ouF);
	
//	return rslt;
}

void writefss(int argc, char* argv[])
{
	string s;
	
	//s=argv[1];
	
	//_chdir("E:\\car");
	
	if (argc==1)
	{
		_chdir("E:\\CarData\\voc2007\\demo\\car");
		s="000007";

	}
	else
	{
		s=argv[1];
	}

	PMStruc ped;
	PMStruc::loadFromfile(ped,"vospms.txt");


	vector<int> thrs= fileIOclass::InVectorInt("..\\..\\bndThreshld.txt");

	vector<int> kspt=fileIOclass::InVectorInt("..\\..\\kptStep.txt");

	for(int j=0;j<kspt.size();j++)
	{	
		for (int i = 0; i < thrs.size(); i++)
		{
			feasToIntPairs(thrs[i],kspt[j],s,ped);
		}
	}

}

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
}


void main(int argc, char* argv[])
{
	string s;
	
	//s=argv[1];
	
	//_chdir("E:\\car");
	
	if (argc==1)
	{
		_chdir("E:\\CarData\\voc2007\\demo\\car");
		s="000007";

	}
	else
	{
		s=argv[1];
	}
	
	

	
	vector<int> thrs= fileIOclass::InVectorInt("..\\..\\bndThreshld.txt");

	vector<int> kspt=fileIOclass::InVectorInt("..\\..\\kptStep.txt");

	
	writeDetvecs();

	vector<DetRec> arcs;
	arcs=fileIOclass::InVector<DetRec>("..\\..\\detRecs.txt");


	
	unordered_map<int,int> thrs_um;
	unordered_map<int,int> kspt_um;
	for (int i = 0; i < thrs.size(); i++)
	{
		thrs_um[thrs[i]]=i;
	}
	for (int i = 0; i < kspt.size(); i++)
	{
		kspt_um[kspt[i]]=i;
	}

	triple<vector<vector<pair<int,int> > >,map<int,map<int, int > >, vector<vector<int> > > ms;
	vector<vector< triple<vector<vector<pair<int,int> > >,map<int,map<int, int > >, vector<vector<int> > > > > infos;
	infos.resize(kspt.size(),vector< triple<vector<vector<pair<int,int> > >,map<int,map<int, int > >, vector<vector<int> > > > (thrs.size(),ms));

	PMStruc ped;
	PMStruc::loadFromfile(ped,"vospms.txt");


	for(int j=0;j<kspt.size();j++)
	{
	//int j=1;
		for (int i = 0; i < thrs.size(); i++)
		{
			kptstoNumKpFea(thrs[i],kspt[j],s,infos[j][i],ped);
		}
	}
	int height=infos[1][0].intmap.size();
	int width=infos[1][0].intmap[0].size();
	
	vector<double> allscores;
	vector<dtctHy> alldts;
	for (int i = 0; i < arcs.size(); i++)
	{
		printf("%d of %d\n",i,arcs.size());
		auto mydt =		detectBst100(arcs[i],height,width,infos[kspt_um[arcs[i].stp]],ped,thrs);
		alldts.insert(alldts.end(),mydt.first.begin(),mydt.first.end());
		allscores.insert(allscores.end(),mydt.second.begin(),mydt.second.end());
	}

	vector<int> indx_(allscores.size(),0);
	for (int i = 0; i < allscores.size(); i++)
	{
		indx_[i]=i;
	}
	prshl(allscores,allscores.size(),indx_);

	FILE* fp=fopen((s+"_dtrslts.txt").c_str(),"w");
	fprintf(fp,"%d\n",indx_.size()>3000?3000:indx_.size());
	for (int i = 0; i < 3000 && i<indx_.size(); i++)
	{
		int dxi=indx_[indx_.size()-1-i];
		fprintf(fp,"%lf\n",allscores[dxi]);
		dtctHy::printOne(fp,alldts[dxi]);
	}
	fclose(fp);

	
}

