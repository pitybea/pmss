
#include <iostream>
#include <fstream>


#include <vector>
#include <string>
#include <windows.h>
#include <direct.h>
#include <map>
#include <time.h>
#include <unordered_map>
#include <unordered_set>
#include "..\..\..\fileIoinclude\FileInOut.h"

#include  "..\..\common\detectCommon.h"
#include "..\..\common\recdeal.h"

using namespace std;

static void kptstoNumKpFea( int thr, int stp,string s,triple< vector< vector<double>   > ,map<int,map<int, int > >, vector<vector<int> > >& rslt)
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

	rslt.feas.resize(tnum,vector<double>(featureDimension,0.0));

	fp=fopen((mys+"_feas.txt").c_str(),"r");
	for (int i = 0; i < rslt.feas.size(); i++)
	{
		for (int j = 0; j < featureDimension; j++)
		{
			fscanf(fp,"%lf ",&rslt.feas[i][j]);
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

double overlabRatio(obj a,obj b)
{
	return overlabRatio(Rect(a.xmin,a.ymin,a.xmax-a.xmin,a.ymax-a.ymin),Rect(b.xmin,b.ymin,b.xmax-b.xmin,b.ymax-b.ymin));
}

bool veryobj(obj o,const vector<obj>& ref)
{
	if (ref.size()==0)
	{
		return true;
	}

	for(auto s:ref)
	{
		if (overlabRatio(o,s)>0.2)
		{
			return false;
		}
	}

	return true;
}

void genpositive(int argc, char* argv[])
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

	vector<obj> objs=fileIOclass::InVector<obj>(s+"_objs.txt");
	//vector<obj> objs;
	/*objs.clear();
	for (int i = 0; i < bjs.size(); i++)
	{
		vector<obj> o=moveobj( bjs[i]);
		objs.insert(objs.end(),o.begin(),o.end());
	}
	*/
	
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

	triple<vector<vector<double> >,map<int,map<int, int > >, vector<vector<int> > > ms;
	vector<vector< triple<vector<vector<double> >,map<int,map<int, int > >, vector<vector<int> > > > > infos;

	infos.resize(kspt.size(),vector< triple<vector<vector<double> >,map<int,map<int, int > >, vector<vector<int> > > > (thrs.size(),ms));


	


	vector<int> ks;
	ks.resize(objs.size(),0);

	unordered_set<int> ks_neededs;

	for (int i = 0; i < objs.size(); i++)
	{
		int ksi(disp(objs[i],kspt));
		ks[i]=ksi;
		ks_neededs.insert(ksi);
	}
	
	for(auto kki:ks_neededs)
	{
		int j=kki;
		for (int i = 0; i < thrs.size(); i++)
		{
			kptstoNumKpFea(thrs[i],kspt[j],s,infos[j][i]);
		}
	}

	int gi(0);
	for (int i = 0; i < objs.size(); i++)
	{
		
		pair<int,int> gth=odisp( smaller( objs[i] ),infos[ks[i]],0);

		
		int thi(gth.first) ;

		auto oneok=rangequery(smaller( objs[i]),infos[ks[i]][thi].kptFea);

		writeOobj(objs[i],s,"",gi,oneok.first,oneok.second,infos[ks[i]][thi].feas);
		gi+=1;
		
		
	}
	
}


int  main(int argc, char* argv[])
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

	vector<DetRec> arcs =fileIOclass::InVector<DetRec>("..\\..\\detRecs.txt");

	vector<obj> pobjs=fileIOclass::InVector<obj>(s+"_objs.txt");
	vector<obj> objs;
	objs.clear();

	int heit,widh;

	FILE* fp=fopen((s+"_edge.txt").c_str(),"r");
	fscanf(fp,"%d %d",&heit,&widh);
	fclose(fp);

	srand((unsigned)time(0)); 
	for (int i = 0; i < 30; i++)
	{
	
		DetRec dr = arcs[rand()%arcs.size()];
		


		int cx=rand()%(widh-dr.w);

	
		int cy=rand()%(heit-dr.h);

		int hx=dr.w/2;
		int hy=dr.h/2;

		obj o;
		o.xmin= cx-hx;
		o.ymin= cy-hy;
		o.xmax= cx+hx;
		o.ymax= cy+hy;

		if (veryobj(o,pobjs))
		{
			objs.push_back(o);
		}

	}

	if(objs.size()==0)
		return -1;

	/*objs.clear();
	for (int i = 0; i < bjs.size(); i++)
	{
		vector<obj> o=moveobj( bjs[i]);
		objs.insert(objs.end(),o.begin(),o.end());
	}
	*/
	
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

	triple<vector<vector<double> >,map<int,map<int, int > >, vector<vector<int> > > ms;
	vector<vector< triple<vector<vector<double> >,map<int,map<int, int > >, vector<vector<int> > > > > infos;

	infos.resize(kspt.size(),vector< triple<vector<vector<double> >,map<int,map<int, int > >, vector<vector<int> > > > (thrs.size(),ms));


	


	vector<int> ks;
	ks.resize(objs.size(),0);

	unordered_set<int> ks_neededs;

	for (int i = 0; i < objs.size(); i++)
	{
		int ksi(disp(objs[i],kspt));
		ks[i]=ksi;
		ks_neededs.insert(ksi);
	}
	
	for(auto kki:ks_neededs)
	{
		int j=kki;
		for (int i = 0; i < thrs.size(); i++)
		{
			kptstoNumKpFea(thrs[i],kspt[j],s,infos[j][i]);
		}
	}

	int gi(0);
	for (int i = 0; i < objs.size(); i++)
	{
		
		pair<int,int> gth=odisp( smaller( objs[i] ),infos[ks[i]],0);

		if (gth.second>28)
		{
			int thi(gth.first) ;

			auto oneok=rangequery(smaller( objs[i]),infos[ks[i]][thi].kptFea);

			writeOobj(objs[i],s,"neg",gi,oneok.first,oneok.second,infos[ks[i]][thi].feas);
			gi+=1;
		}
		
		
		
	}

	return 0;
	
}
