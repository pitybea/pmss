
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


using namespace std;





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

	vector<obj> bjs=fileIOclass::InVector<obj>(s+"_objs.txt");
	vector<obj> objs;
	objs.clear();
	for (int i = 0; i < bjs.size(); i++)
	{
		vector<obj> o=moveobj( bjs[i]);
		objs.insert(objs.end(),o.begin(),o.end());
	}

	
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
		
		auto gth=odisp(objs[i],infos[ks[i]]);

		if (gth.second>120)
		{
			int thi(gth.first) ;

			auto oneok=rangequery(objs[i],infos[ks[i]][thi].kptFea);

			writeOobj(objs[i],s,gi,oneok.first,oneok.second,infos[ks[i]][thi].feas);
			gi+=1;
		}
		
	}
	
}

