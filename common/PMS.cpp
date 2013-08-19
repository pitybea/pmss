#include "PMS.h"


#define  tHeE 2.718281828459


static __int64 pow2[]=
{
	1,
	2,
	4,
	8,
	16,
	32,
	64,
	128,
	256,
	512,
	1024,
	2048,
	4096,
	8192,
	16384,
	32768,
	65536,
	131072,
	262144,
	524288,
	1048576,
	2097152,
	4194304,
	8388608,
	16777216,
	33554432,
	67108864,
	134217728,
	268435456,
	536870912,
	1073741824,
	2147483648,
	4294967296,
	8589934592,
	17179869184,
	34359738368,
	68719476736,
	137438953472,
	274877906944,
	549755813888,
	1099511627776,
	2199023255552,
	4398046511104,
	8796093022208,
	17592186044416,
	35184372088832,
	70368744177664,
	140737488355328,
	281474976710656,
	562949953421312,
	1125899906842624,
	2251799813685248,
	4503599627370496,
	9007199254740992,
	18014398509481984,
	36028797018963968,
	72057594037927936,
	144115188075855872,
	288230376151711744,
	576460752303423488,
	1152921504606846976,
	2305843009213693952,
	4611686018427387904,
	9223372036854775808



};

/*
PMStruc::PMStruc()
{

	
	
}*/


PMStruc::PMStruc( int alvlimt,int plvlimt)
{

	aLevelLimit= alvlimt;
	pLevelLimit=plvlimt;
	paOrders=genPAorders(aLevelLimit,pLevelLimit);
	
	
}

template<class T>
void dtmMinMx(const vector<vector<T> >& data,vector<pair<T,T> >& minmaxs)
{

	for (int ind=0;ind<data[0].size();ind++)
	{
		minmaxs[ind].first=data[0][ind];
		minmaxs[ind].second=data[0][ind];
		for (int i=0;i<data.size();i++)
		{
			if (data[i][ind]<minmaxs[ind].first)
			{
				minmaxs[ind].first=data[i][ind];
			}
			else if (data[i][ind]>minmaxs[ind].second)
			{
				minmaxs[ind].second=data[i][ind];
			}
		}
	}
	
}




int PMStruc::generatePymFromdata(const vector<vector<double> >& data,int adm)
{

	adimension=adm;
	return dataToPym(data);

}

vector<pair<LInt,LInt> > PMStruc:: dataToTwoInxs(vector<double> data)
{


	vector<pair<LInt,LInt> > inxs;
	inxs.clear();
	for (int i = 0; i <aLevelLimit ; i++)
	{
		inxs.push_back( dataToTwoInxWthoutPos(i,data) );
	}

	
	return inxs;
}

pair<LInt,LInt> PMStruc::dataToTwoInxWthoutPos(int alvel,vector<double> data)
{

	int dimension=(int)data.size()+2;
	int haldim=dimension/2;

	LInt fisI(0),secI(0);


	
	for (int j=2;j<haldim;j++)
	{
		LInt indtem;
		indtem= (LInt) (aAbs[alvel][j].first*data[j-2]+aAbs[alvel][j].second);
		if (indtem<0)
		{
			indtem=0;
		}
		if (indtem>(pow2[alvel]-1))
		{
			indtem=(LInt)pow2[alvel]-1;
		}
		fisI+=twoExs[j]*indtem;
	}
	for (int j=haldim;j<dimension;j++)
	{
		LInt indtem;
		indtem=(LInt)(aAbs[alvel][j].first*data[j-2]+aAbs[alvel][j].second);
		if (indtem<0)
		{
			indtem=0;
		}
		if (indtem>(pow2[alvel]-1))
		{
			indtem=(LInt)pow2[alvel]-1;
		}
		secI+=twoExs[j]*indtem;
	}


	return pair<LInt,LInt>(fisI,secI);

	

		

}



pair<LInt,LInt> PMStruc::dataToTwoPosInx(int alvel,int plvel,vector<double> data,vector<pair<LInt,LInt> > inp)
{
	

	int dimension=(int)data.size();
	int haldim=dimension/2;

	LInt fisI(0),secI(0);


	for (int j = 0; j < 2; j++)
	{
		LInt indtem;
		indtem=(LInt)(aAbs[plvel][j].first*data[j]+aAbs[plvel][j].second);
		if (indtem<0)
		{
			indtem=0;
		}
		if (indtem>(twExMs-1))
		{
			indtem=twExMs-1;
		}
		fisI+=twoExs[j]*indtem;
	}
	return pair<LInt,LInt>(inp[alvel].first+fisI,inp[alvel].second);
}

vector<int> PMStruc::dataToAIntVec(int alvel,int plvel,vector<double> data)
{
	int dimension=(int)data.size();

	vector<int> rslt;

	rslt.resize(dimension,0);
	for (int j = 0; j < 2; j++)
	{
		int indtem;
		indtem=(int)(aAbs[plvel][j].first*data[j]+aAbs[plvel][j].second);
		if (indtem<0)
		{
			indtem=0;
		}
		if (indtem>(twExMs-1))
		{
			indtem=twExMs-1;
		}
		rslt[j]=indtem;	
	}

	for (int j=2;j<dimension;j++)
	{
		int indtem;
		indtem=(int)(aAbs[alvel][j].first*data[j]+aAbs[alvel][j].second);
		if (indtem<0)
		{
			indtem=0;
		}
		if (indtem>(twExMs-1))
		{
			indtem=twExMs-1;
		}
		rslt[j]=indtem;	
	}
	return rslt;
}

pair<LInt,LInt> PMStruc::dataToTwoPosInx(int alvel,int plvel,vector<double> data)
{

	

	int dimension=(int)data.size();
	int haldim=dimension/2;

	LInt fisI(0),secI(0);


	for (int j = 0; j < 2; j++)
	{
		LInt indtem;
		indtem=(LInt)(aAbs[plvel][j].first*data[j]+aAbs[plvel][j].second);
		if (indtem<0)
		{
			indtem=0;
		}
		if (indtem>(twExMs-1))
		{
			indtem=twExMs-1;
		}
		fisI+=twoExs[j]*indtem;
	}
	for (int j=2;j<haldim;j++)
	{
		LInt indtem;
		indtem=(LInt)(aAbs[alvel][j].first*data[j]+aAbs[alvel][j].second);
		if (indtem<0)
		{
			indtem=0;
		}
		if (indtem>(twExMs-1))
		{
			indtem=twExMs-1;
		}
		fisI+=twoExs[j]*indtem;
	}
	for (int j=haldim;j<dimension;j++)
	{
		LInt indtem;
		indtem=(LInt)(aAbs[alvel][j].first*data[j]+aAbs[alvel][j].second);
		if (indtem<0)
		{
			indtem=0;
		}
		if (indtem>(twExMs-1))
		{
			indtem=twExMs-1;
		}
		secI+=twoExs[j]*indtem;
	}


	return pair<LInt,LInt>(fisI,secI);
}



void valueToInx(pair<double,double> minMax,pair<double,double>& aAndB,int levl)
{
	int totl=(int)pow2[levl];
	minMax.first-=0.001;
	minMax.second+=0.001;
	aAndB.first=(double)(totl)/(minMax.second-minMax.first);
	aAndB.second=0-aAndB.first*minMax.first;
}



bool PMStruc::dataToPymLvl(const vector<vector<double> >& datas,int alvel,int plvel)
{
	bool fineEngough=true;
	int dimension=(int)datas[0].size();
	
	int haldim=(int)datas[0].size()/2;


	for (int i=0;i<datas.size();i++)
	{
		pair<LInt,LInt> ss;
	
		ss= dataToTwoPosInx(alvel,plvel,datas[i]);
		LInt fisI(ss.first),secI(ss.second);

		if (pym[alvel][plvel].count(fisI)>0)
		{
			if (pym[alvel][plvel][fisI].count(secI)>0)
			{
				pym[alvel][plvel][fisI][secI]+=1;
				fineEngough=false;
			}
			else
			{
				pym[alvel][plvel][fisI].insert(pair<int,int>(secI,1));
			}
		}
		else
		{
			unordered_map<LInt,int> temlv;
			temlv.insert(pair<LInt,int>(secI,1));
			pym[alvel][plvel].insert(pair<LInt,unordered_map<LInt,int> >(fisI,temlv));
		}

		auto vk=dataToAIntVec(alvel,plvel,datas[i]);
		if (newpym[alvel][plvel].count(vk) )
		{
			newpym[alvel][plvel][vk]+=1;
		}
		else
			newpym[alvel][plvel][vk]=1;
		

	}
	return fineEngough;
}



int invdvalue(double a,vector<double> inv)
{
	int re=0;
	if (inv.size()==0)
	{
		return re;
	}
	if ((re<inv[0]))
	{
		return re;
	}
	if (inv.size()==1)
	{
		return (a>inv[0])?1:0;
	}
	for (int i=0;i<inv.size()-1;i++)
	{
		re+=1;
		if ((a>=inv[i])&&(a<inv[i+1]))
		{
			return re;
		}
	}
	return re;
}

unordered_map<LInt,unordered_map<LInt,int> > loadPyramidLv(FILE* fp)
{
	unordered_map<LInt,unordered_map<LInt,int> > result;
	int pymlvS;
	fscanf(fp,"%d\n",&pymlvS);
	for (int i = 0; i < pymlvS; i++)
	{
		int a,bS;
		fscanf(fp,"%d %d\n",&a,&bS);
		unordered_map<LInt,int> b;
		for (int j = 0; j < bS; j++)
		{
			int ta,tb;
			fscanf(fp,"%d %d ",&ta,&tb);
			b.insert(pair<int,int>(ta,tb));
		}

		result.insert(pair<LInt,unordered_map<LInt,int> >(a,b));
		fscanf(fp,"\n");
	}
	return result;
}

void printPyramidLv(unordered_map<LInt,unordered_map<LInt,int> > pymlv,FILE* fp)
{
	fprintf(fp,"%d\n",pymlv.size());
	for(auto ii=pymlv.begin(); ii!=pymlv.end(); ++ii)
	{
		auto a=(*ii).first;
		auto b=(*ii).second;
		fprintf(fp,"%d %d\n",a,b.size());
		for(auto ij=b.begin();ij!=b.end();++ij)
		{
			fprintf(fp,"%d %d ",(*ij).first,(*ij).second);
		}
		fprintf(fp,"\n");
	}

}

vector<pair<double,double> > loadAandBsOne(FILE* fp)
{
	vector<pair<double,double> > result;
	int abS;
	fscanf(fp,"%d\n",&abS);
	for (int i = 0; i < abS; i++)
	{
		double ta,tb;
		fscanf(fp,"%lf %lf ",&ta,&tb);
		result.push_back(pair<double,double>(ta,tb));
	}
	fscanf(fp,"\n");
	return result;
}

void printAandBs(vector<pair<double,double> > abs,FILE* fp)
{
	fprintf(fp,"%d\n",abs.size());
	for(int i=0;i<abs.size();i++)
	{
		fprintf(fp,"%lf %lf ",abs[i].first,abs[i].second);
	}
	fprintf(fp,"\n");
}

/*
vector<unordered_map<int,unordered_map<int,int> > > pym;

	vector<vector<pair<double,double> > > aAbs;

	int adimension;
	int LevelLimit;// 6
	int twExMs;
	vector<int> twoExs;

	vector<double> weights;
	
*/
void PMStruc:: printTofile(const PMStruc& pm,string s)
{
	FILE* fp;
	fp=fopen(s.c_str(),"w");
	fprintf(fp,"%d %d %d %d\n",pm.adimension,pm.aLevelLimit,pm.pLevelLimit,pm.twExMs);
	fprintf(fp,"%d\n",pm.twoExs.size());
	for (int i = 0; i < pm.twoExs.size(); i++)
	{
		fprintf(fp,"%d ",pm.twoExs[i]);
	}

	fprintf(fp,"\n");
	for (int i = 0; i < pm.aLevelLimit; i++)
	{
		for (int j = 0; j < pm.pLevelLimit; j++)
		{
				fprintf(fp,"%lf ",pm.weights[i][j]);
		}

	}
	fprintf(fp,"\n%d\n",pm.aAbs.size());
	for (int i = 0; i < pm.aAbs.size(); i++)
	{
		printAandBs(pm.aAbs[i],fp);
	}
	fprintf(fp,"\n");
	for (int i = 0; i < pm.aLevelLimit; i++)
	{
		for (int j = 0; j < pm.pLevelLimit; j++)
		{
			printPyramidLv(pm.pym[i][j],fp);
		}

	}

	fclose(fp);
}
void PMStruc:: loadFromfile(PMStruc& pm,string s)
{
	FILE* fp;
	fp=fopen(s.c_str(),"r");

	fscanf(fp,"%d %d %d %d\n",&pm.adimension,&pm.aLevelLimit,&pm.pLevelLimit,&pm.twExMs);
	pm.paOrders=genPAorders(pm.aLevelLimit,pm.pLevelLimit);
	int isz;
	fscanf(fp,"%d\n",&isz);
	pm.twoExs.resize(isz,0);
	for (int i = 0; i < pm.twoExs.size(); i++)
	{
		fscanf(fp,"%d ",&pm.twoExs[i]);
	}
	fscanf(fp,"\n");
//	pm.weights.resize(isz,0.0);
	pm.weights.resize(pm.aLevelLimit,vector<double>(pm.pLevelLimit,0.0));
	for (int i = 0; i < pm.aLevelLimit; i++)
	{
		for (int j = 0; j < pm.pLevelLimit; j++)
		{
			fscanf(fp,"%lf ",&pm.weights[i][j]);
		}
		
	}
	fscanf(fp,"\n%d\n",&isz);
	pm.aAbs.resize(isz,vector<pair<double,double> >());
	for (int i = 0; i < pm.aAbs.size(); i++)
	{
		pm.aAbs[i]=loadAandBsOne(fp);
		//printAandBs(pm.aAbs[i],fp);
	}
	fscanf(fp,"\n");
	pm.pym.resize(pm.aLevelLimit,vector<unordered_map<LInt,unordered_map<LInt,int> > >(pm.pLevelLimit,unordered_map<LInt,unordered_map<LInt,int> >() ));
	for (int i = 0; i < pm.aLevelLimit; i++)
	{
		for (int j = 0; j < pm.pLevelLimit; j++)
		{
				pm.pym[i][j]=loadPyramidLv(fp);
		}

//		printPyramidLv(pm.pym[i],fp);
	}
	fclose(fp);
	

}


int PMStruc::GeneratePosWeightWithParameter(double a,double b,double c)
{
	/*for (int i = 0; i < LevelLimit*LevelLimit; i++)
	{
		int pi(i/LevelLimit);int ai(i%LevelLimit);
		weights[i]= pow((double)pow2[pi],ratio)*pow( pow2[ai],1-ratio);

	}*/

	for (int i = 0; i < aLevelLimit; i++)
	{
		for (int j = 0; j < pLevelLimit; j++)
		{
			auto x=0-pow2[aLevelLimit-i];
			auto y=pow2[pLevelLimit- j];
			weights[i][j]=pow(tHeE,0-a*(x*x+b*y*y+c*x*y) );
		}
	}


	return 0;
}




int PMStruc::GeneratePosWeightWithParameter(double ratio)
{
	
	for (int i = 0; i < aLevelLimit; i++)
	{
		for (int j = 0; j < pLevelLimit; j++)
		{
			double x=1.0/pow2[i]*10;
			double y=1.0/pow2[j]*2;
			weights[i][j]= pow(tHeE,0.02*(0-x*x))*pow(tHeE,0.02*(0-y*y));
		}
	}

	

	return 0;
}

int PMStruc::dataToPym(const vector<vector<double> >& data)
{
	if (data.size()>0)
	{
		auto dimension=data[0].size();
		vector<pair<double,double> > minmax;
		minmax.resize(data[0].size(),pair<double,double>(0.0,0.0));
		dtmMinMx(data,minmax);

	
		auto haldim=dimension/2;
		
		twExMs=(int)pow2[ max(aLevelLimit,pLevelLimit) -1];
		twoExs.resize(dimension,0);
		for (int ti=0;ti<haldim;ti++)
		{
			twoExs[ti]=twoExs[ti+haldim]=pow2[(max(aLevelLimit,pLevelLimit) -1)*ti];
		}

		for (int goodi = 0; goodi < max( aLevelLimit,pLevelLimit); goodi++)
		{


			vector<pair<double,double> > aAb;
			aAb.resize(data[0].size(),pair<double,double>(0.0,0.0));

			for (int i=0;i<data[0].size();i++)
			{
				valueToInx(minmax[i],aAb[i],goodi);
			}

			aAbs.push_back(aAb);
			
	

		}


	
		weights.resize(aLevelLimit,vector<double>(pLevelLimit,0.0));
		pym.resize(aLevelLimit,vector<unordered_map<LInt,unordered_map<LInt,int> > >(pLevelLimit,unordered_map<LInt,unordered_map<LInt,int> >() ));

		newpym.resize(aLevelLimit,vector<map<vector<int>,int> >(pLevelLimit,map<vector<int>,int>()));

		for (int i = 0; i < aLevelLimit; i++)
		{
			for (int j = 0; j < pLevelLimit; j++)
			{
				dataToPymLvl(data,i,j);
				double awt= sqrt(pow2[i]);
				double pwt= sqrt(pow2[j]);

				weights[i][j]=awt*pwt;
			}
		}
		
		


	
	}
	
	return 0;
}

/*
int PMStruc:: matchDToOneLv(vector<vector<double> > &dataset,int levl,map<int,map<int,int> > pmlv,vector<vector<double> > invs,bool ExcluMode )
{
	int haldim=dataset[0].size()/2;
	int dimension=dataset[0].size();
	int twExM=pow2[levl];

	vector<int> twoEx;
	twoEx.resize(dimension,0);
	for (int i=0;i<haldim;i++)
	{
		twoEx[i]=twoEx[i+haldim]=pow(pow2[levl],i);
	}

	int res(0);

	int siz=dataset.size();
	for (int i=siz-1;i>=0;i--)
	{
		int fisI(0),secI(0);
		for (int j=0;j<haldim;j++)
		{
			int indtem;
			//indtem=aAndB[j].first*dataset[i][j]+aAndB[j].second;
			indtem= invdvalue(dataset[i][j],invs[j]);
			if (indtem<0)
			{
				indtem=0;
			}
			if (indtem>(twExM-1))
			{
				indtem=twExM-1;
			}
			fisI+=twoEx[j]*indtem;
		}
		for (int j=haldim;j<dimension;j++)
		{
			int indtem;
			indtem= invdvalue(dataset[i][j],invs[j]);
			if (indtem<0)
			{
				indtem=0;
			}
			if (indtem>(twExM-1))
			{
				indtem=twExM-1;
			}
			secI+=twoEx[j]*indtem;
		}


		if (pmlv.count(fisI)>0)
		{
			if (pmlv[fisI].count(secI)>0)
			{
				if(ExcluMode)
				{
					if (pmlv[fisI][secI]>1)
					{
						res+=1;
						pmlv[fisI][secI]-=1;
						dataset.erase(dataset.begin()+i);
					}
				}
				else
				{	
					if (pmlv[fisI][secI]>0)
					{
						res+=1;
						pmlv[fisI][secI]-=1;
						dataset.erase(dataset.begin()+i);
					}
				}

			}
		}
	}
	return res;
}

*/

/*vector<int> PMStruc:: matchDToOneLv(vector<vector<double> > dataset,int levl,map<int,map<int,int> > pmlv,bool ExcluMode,set<int> elimitset )
{

	int haldim=dataset[0].size()/2;
	int dimension=dataset[0].size();

	int siz=dataset.size();
	vector<int> res;
	res.clear();
	for (int i=siz-1;i>=0;i--)
	{
		
		pair<int,int> ss;
	
		ss=dataToTwoPosInx(levl%LevelLimit,levl/LevelLimit,dataset[i]);

		int fisI(ss.first),secI(ss.second);

		if (pmlv.count(fisI)>0)
		{
			if (pmlv[fisI].count(secI)>0)
			{
				if(ExcluMode)
				{
					if (pmlv[fisI][secI]>1)
					{
					//	res+=1;
						pmlv[fisI][secI]-=1;
						if (elimitset.count(i)==0)
						{
							res.push_back(i);
						}
					//	dataset.erase(dataset.begin()+i);
					}
				}
				else 
					{
						if (pmlv[fisI][secI]>0)
						{
						//	res+=1;
							pmlv[fisI][secI]-=1;
							if (elimitset.count(i)==0)
							{
								res.push_back(i);
							}
						//	dataset.erase(dataset.begin()+i);
						}
				}
			}
		}
	}
	return res;
}
*/
int PMStruc::matchDToOneLvSimple(const vector<vector<pair<LInt,LInt> > >& vecFSs,const vector<vector<double>>& poss,const vector<int>& indx ,int alevl,int plevl, bool ExcluMode,vector<bool>& used,vector<pair<int,int> > & every )
{
	auto siz=indx.size();
	int res(0);

	//auto pym[levl]=pym[levl];

	for (auto i=siz-1;i>=0;i--)
	{
		if(!used[i])
		{
			pair<int,int> ss;
	
			ss=dataToTwoPosInx(alevl,plevl,poss[i],vecFSs[indx[i]]);

			int fisI(ss.first),secI(ss.second);

			if (pym[alevl][plevl].count(fisI)>0)
			{
				if (pym[alevl][plevl][fisI].count(secI)>0)
				{
					if(ExcluMode)
					{
						if (pym[alevl][plevl][fisI][secI]>1)
						{
						
							if (used[i]==false)
							{
								res+=1;
								used[i]=true;
								every[i].first=alevl;
								every[i].second=plevl;
							}
						}
					}
					else 
						{
							if (pym[alevl][plevl][fisI][secI]>0)
							{
								if (used[i]==false)
								{
									res+=1;
									used[i]=true;
									every[i].first=alevl; 
									every[i].second=plevl;
								}
						
							}
					}
				}
			}
		}
		
	}
	return res;


	

}

int PMStruc::newmatchDToOneLvSimple(const vector<vector<double > >& vecFSs,int alevl,int plevl, bool ExcluMode,vector<bool>& used )
{
	int siz=(int)vecFSs.size();
	int res(0);

//	auto pym[levl]=pym[levl];

	for (int i=0;i<siz;i++)
	{
		if(!used[i])
		{
			
			auto ss=dataToAIntVec(alevl,plevl,vecFSs[i]);

			if (newpym[alevl][plevl].count(ss))
			{
				if(ExcluMode)
					{
						if (newpym[alevl][plevl][ss]>1)
						{
						
							if (used[i]==false)
							{
								res+=1;
								used[i]=true;
							}
						}
					}
					else 
						{
							if (newpym[alevl][plevl][ss]>0)
							{
								if (used[i]==false)
								{
									res+=1;
									used[i]=true;
								}
						
							}
					}
			}


		}
		
	}
	return res;
}

int PMStruc:: matchDToOneLvSimple(const vector<vector<pair<LInt,LInt> > >& vecFSs,const vector<vector<double> >& poss,int alevl,int plevl, bool ExcluMode,vector<bool>& used )
{

	


	int siz=(int)vecFSs.size();
	int res(0);

//	auto pym[levl]=pym[levl];

	for (int i=0;i<siz;i++)
	{
		if(!used[i])
		{
			pair<LInt,LInt> ss;
	
			ss=dataToTwoPosInx(alevl,plevl,poss[i],vecFSs[i]);

			LInt fisI(ss.first),secI(ss.second);

			if (pym[alevl][plevl].count(fisI)>0)
			{
				if (pym[alevl][plevl][fisI].count(secI)>0)
				{
					if(ExcluMode)
					{
						if (pym[alevl][plevl][fisI][secI]>1)
						{
						
							if (used[i]==false)
							{
								res+=1;
								used[i]=true;
							}
						}
					}
					else 
						{
							if (pym[alevl][plevl][fisI][secI]>0)
							{
								if (used[i]==false)
								{
									res+=1;
									used[i]=true;
								}
						
							}
					}
				}
			}
		}
		
	}
	return res;


	
}

double PMStruc::MatchDttoPosPymSimple(const vector<vector<pair<LInt,LInt> > >& vecFSs,const vector<vector<double>>& poss,const vector<int>& indx,bool ExcluMode,vector<pair<int,int> >& every,bool inverse)
{
	auto siz=indx.size();

	
	every.resize(siz,pair<int,int>(0,0));

	vector<bool> used;
	used.clear();
	used.resize(indx.size(),false);

	
	double reslt(0.0);
	for (int i = 0; i < paOrders.size(); i++)
	{
			
//				int tinx=*LevelLimit+paOrders[i][j].second;
				//MatchDttoPosPymSimple(const vector<vector<pair<int,int> > >& vecFSs,const vector<vector<double>>& poss,const vector<int>& indx,bool ExcluMode,vector<double>& every);
				//int matchDToOneLvSimple(const vector<vector<pair<int,int> > >& vecFSs,const vector<vector<double>>& poss,const vector<int>& indx ,int levl, bool ExcluMode,vector<bool>& used,vector<int>& every );
				int sscore= matchDToOneLvSimple(vecFSs,poss,indx,paOrders[i].first, paOrders[i].second,ExcluMode,used,every);
				if(!inverse)
				reslt+=sscore*weights[paOrders[i].first][paOrders[i].second];
					else
				reslt+=sscore*(1.0/weights[paOrders[i].first][paOrders[i].second]);

	}
	


	

	reslt/=sqrt(siz);
	if(inverse)
	{
		
		reslt=1.0/reslt;
	}

		
	return reslt;
	

}

double PMStruc:: newMatchDttoPosPymSimple(const vector<vector<double > >& vecFSs,bool ExcluMode,vector< double>  & mnumbers,bool inverse)
{
	auto siz=vecFSs.size();

	
	mnumbers.resize(paOrders.size(), 0.0);

	vector<bool> used;
	used.clear();
	used.resize(vecFSs.size(),false);



	for (int i = 0; i < paOrders.size(); i++)
	{

		mnumbers[i]=newmatchDToOneLvSimple(vecFSs,paOrders[i].first,paOrders[i].second,ExcluMode,used);

	}
	

	double reslt(0.0);
	for (int i=0;i<paOrders.size();i++)
	{
//		for (int j = 0; j < pLevelLimit; j++)
		{
			if(!inverse)
				reslt+=mnumbers[i]*weights[paOrders[i].first][paOrders[i].second];
			else
				reslt+=mnumbers[i]*(1.0/weights[paOrders[i].first][paOrders[i].second]);
		}
		
	}

	reslt/=siz;
	if(inverse)
	{
		
		reslt=1.0/reslt;
	}

		
	return reslt;
}

double PMStruc::MatchDttoPosPymSimple(const vector<vector<pair<LInt,LInt> > >& vecFSs,const vector<vector<double>>& poss,bool ExcluMode,vector<double>  & mnumbers,bool inverse)
{
	auto siz=vecFSs.size();

	
	mnumbers.resize(paOrders.size(), 0.0);

	vector<bool> used;
	used.clear();
	used.resize(vecFSs.size(),false);


	for (int i = 0; i < paOrders.size(); i++)
	{

		mnumbers[i]=matchDToOneLvSimple(vecFSs,poss,paOrders[i].first,paOrders[i].second,ExcluMode,used);

	}
	

	double reslt(0.0);
	for (int i=0;i<paOrders.size();i++)
	{
//		for (int j = 0; j < pLevelLimit; j++)
		{
			if(!inverse)
				reslt+=mnumbers[i]*weights[paOrders[i].first][paOrders[i].second];
			else
				reslt+=mnumbers[i]*(1.0/weights[paOrders[i].first][paOrders[i].second]);
		}
		
	}

	reslt/=siz;
	if(inverse)
	{
		
		reslt=1.0/reslt;
	}

		
	return reslt;
	

}






double PMStruc::MatchDttoPosPymSimple(const vector<vector<double> >& dataset,bool ExcluMode,vector< double> & mnumbers,bool inverse)
{
	auto apPs=toTwo(dataset,2);

	vector<vector<pair<LInt,LInt> > > faSs;
	faSs.clear();
	for (int i = 0; i < apPs.second.size(); i++)
	{
		faSs.push_back( dataToTwoInxs (apPs.second[i]) );
	}

	

		
	return MatchDttoPosPymSimple(faSs,apPs.first,ExcluMode,mnumbers,inverse);
}
/*double PMStruc::MatchDttoPosPym(vector<vector<double> > dataset,bool ExcluMode,vector<double> & mnumbers,bool inverse)
{
	set<int> used;
	used.clear();
	mnumbers.resize(pym.size(),0.0);
	double reslt(0.0);
	for (int i = 0; i < paOrders.size(); i++)
	{
		vector<vector<int> > lrslt;
		lrslt.resize(paOrders[i].size(),vector<int>());


		for (int j = 0; j < paOrders[i].size(); j++)
		{
			int tinx=paOrders[i][j].first*LevelLimit+paOrders[i][j].second;
			lrslt[j]=matchDToOneLv(dataset,tinx,pym[tinx],ExcluMode,used);
		}
		
		vector<vector<int> > belongTo;
		belongTo.resize(dataset.size(),vector<int>());
		
		for (int j = 0; j < lrslt.size(); j++)
		{
			for (int k = 0; k < lrslt[j].size(); k++)
			{
				belongTo[lrslt[j][k]].push_back(j);
			}
		}

		for (int j = 0; j < belongTo.size(); j++)
		{
			if (belongTo[j].size()>0)
			{
				for (int k = 0; k < belongTo[j].size(); k++)
				{
					int mj=belongTo[j][k];
					int tinx=paOrders[i][mj].first*LevelLimit+paOrders[i][mj].second;
					mnumbers[tinx]+=1.0/(double)belongTo[j].size();
				}
				used.insert(j);
			}
		}
	}
	int dim=dataset[0].size();
	for (int i = 0; i < mnumbers.size(); i++)
	{
		if(!inverse)
			reslt+=mnumbers[i]*weights[i]*dim;
		else
			reslt+=mnumbers[i]*(1.0/weights[i])*dim;
	}
	int siz=dataset.size();
	reslt/=siz;
	if(inverse)
	{
		
		reslt=1.0/reslt;
	}
	return reslt;
}
*/

double multiIVecDvec(vector<int> ivec,vector<double> dvec)
{
	assert(ivec.size()==dvec.size());
	double result=0.0;
	for (int i = 0; i < ivec.size(); i++)
	{
		result+=ivec[i]*dvec[i];
	}
	return result;
}

void setDoubleVec(vector<double>& to,vector<double> frm)
{
	to.clear();
	to.insert(to.end(),frm.begin(),frm.end());
}


bool endWhile(vector<double> jg,vector<double> tojg)
{
	assert(jg.size()==tojg.size());
	for (int i = 0; i < jg.size(); i++)
	{
		if(tojg[i]<jg[i])
			return false;
	}
	return true;
}


void nextweights(vector<double> bd,vector<double>& wts,double stw)
{
	auto idx=wts.size()-1;
	bool stpPr=false;
	while(!stpPr)
	{
		wts[idx]+=stw;
		if(wts[idx]<bd[idx])
		{
			stpPr=true;
			if(idx!=(wts.size()-1))
			{
				for(auto i=idx+1;i<wts.size();i++)
					wts[i]=wts[idx];							
			}
		}
		else
		{
			idx-=1;
		}
		if(idx==0)
		{
			stpPr=true;
			break;
		}

		
	}
}

PMSEnsemble::PMSEnsemble()
{
	pyms.clear();
}

int PMSEnsemble::generateAaBsFromdata(vector<vector<double> > data,int LevelLimit)
{
	if (data.size()>0)
	{
		dimension=(int)data[0].size();
		vector<pair<double,double> > minmax;
		minmax.resize(data[0].size(),pair<double,double>(0.0,0.0));
		dtmMinMx(data,minmax);
	
		int goodi=0;
		while(goodi<LevelLimit)
		{
			vector<pair<double,double> > aAb;
			aAb.resize(data[0].size(),pair<double,double>(0.0,0.0));
			for (int i=0;i<data[0].size();i++)
			{
				valueToInx(minmax[i],aAb[i],goodi);
			}			
			aAbs.push_back(aAb);	
			goodi+=1;

		}
	
	
	}
	
	return 0;
};
/*
int PMStruc::initPymWithABs(vector<vector<pair<double,double> > > abS,int dimension)
{
	if (abS.size()>0)
	{


	
		int haldim=dimension/2;

		aAbs.clear();
		aAbs.insert(aAbs.end(),abS.begin(),abS.end());

		twoExs.clear();
	
		twExMs=pow2[LevelLimit-1];
		twoExs.resize(dimension,0);
		for (int ti=0;ti<haldim;ti++)
		{
			twoExs[ti]=twoExs[ti+haldim]=pow(twExMs,ti);
		}

		weights.resize(LevelLimit*LevelLimit,0.0);
		unordered_map<int,unordered_map<int,int>> tum;
		pym.resize(LevelLimit*LevelLimit,tum);
		
		for (int i = 0; i < LevelLimit*LevelLimit; i++)
		{
				int ai(i/LevelLimit);int aj(i%LevelLimit);
				weights[i]=sqrt((double)(pow2[ai]*pow2[aj]));
		}
		

	
	
	}
	
	return 0;
};*/
int PMStruc::AddoneData(vector<double> data,bool AddOrMinus)
{

	
	auto dimension=data.size();
	auto haldim=dimension/2;

	for (int levi = 0; levi < aLevelLimit; levi++)
	{
		for (int levj = 0; levj < pLevelLimit; levj++)
		{
			pair<int,int> ss;
	
			ss= dataToTwoPosInx(levi,levj,data);
			LInt fisI(ss.first),secI(ss.second);
		

		
			if (pym[levi][levj].count(fisI)>0)
			{
				if (pym[levi][levj][fisI].count(secI)>0)
				{
					if(AddOrMinus)
						pym[levi][levj][fisI][secI]+=1;
					else
						pym[levi][levj][fisI][secI]-=1;
				
				}
				else
				{
					if(AddOrMinus)
						pym[levi][levj][fisI].insert(pair<int,int>(secI,1));
					else
						pym[levi][levj][fisI].insert(pair<int,int>(secI,-1));
				}
			}
			else
			{
				unordered_map<LInt,int> temlv;
				if(AddOrMinus)
					temlv.insert(pair<LInt,int>(secI,1));
				else
					temlv.insert(pair<LInt,int>(secI,-1));
				pym[levi][levj].insert(pair<LInt,unordered_map<LInt,int> >(fisI,temlv));
			}
		}
	//	int lvel=levi;
	
		
	}
	


	return 0;
};



int PMStruc::AddSeverlData(vector<vector<double> > data,bool AddorMinus)
{
	if(data.size()>0)
		for(auto d :data)
			AddoneData(d,AddorMinus);

	return 0;
}

/*
int PMSEnsemble::generateStructureFromData(vector<vector<vector<double> > > data)
{

	if (data.size()>0)
		for(auto d :data)
			if(pyms.size()==0)
			{
				PMStruc tp;
				tp.initPymWithABs(aAbs,dimension);
				tp.AddSeverlData(d,true);
				pyms.push_back(tp);
			}
			else
			{
				vector<double> tr;
				tr.resize(pyms.size(),0.0);
				for(int i=0;i<tr.size();i++)
				{
					vector<double> temiv;
					tr[i]=pyms[i].givePyramidMatchScore(d,false,temiv);
					
				}
				auto sd=maxminValAndInx(tr,true);
				if(sd.second>threshold)
					pyms[sd.first].AddSeverlData(d,true);
				else
				{
					
					PMStruc tp;
					tp.initPymWithABs(aAbs,dimension);
					tp.AddSeverlData(d,true);
					pyms.push_back(tp);
				}


			}

	

	return 0;
}*/

double PMStruc::givePyramidMatchScore(const vector<vector<double> >& dataset,bool ExcluMode,vector< double>  & scoreAllLevel)
{

	return MatchDttoPosPymSimple(dataset,ExcluMode,scoreAllLevel,false);

}

double PMStruc::givePyramidMatchScore(const vector<vector<pair<LInt,LInt> > >& vecFSs,const vector<vector<double>>& poss,bool ExcluMode,vector< double>  & scoreAllLevel)
{
	assert(vecFSs.size()==poss.size());
//double MatchDttoPosPymSimple(vector<pair<int,int>> vecFSs,vector<vector<double>> poss,bool ExcluMode,vector<double> & mnumbers,bool inverse);
	return  MatchDttoPosPymSimple(vecFSs,poss,ExcluMode,scoreAllLevel,true);
}

double PMStruc:: giveNewPyramidMatchScore(const vector<vector<double > >& vecFSs,bool ExcluMode,vector< double>  & scoreAllLevel)
{
//	assert(vecFSs.size()==poss.size());
	return newMatchDttoPosPymSimple(vecFSs,ExcluMode,scoreAllLevel,true);
}

double PMStruc::givePyramidMatchScoreSpecial(const vector<vector<pair<LInt,LInt> > >& vecFSs,const vector<vector<double>>& poss,const vector<int>& indx,bool ExcluMode,vector<pair<int,int> >& every)
{
	assert(indx.size()==poss.size());
	//	double MatchDttoPosPymSimple(const vector<vector<pair<int,int> > >& vecFSs,const vector<vector<double>>& poss,const vector<int>& indx,bool ExcluMode,vector<int>& every,bool inverse);
	return MatchDttoPosPymSimple(vecFSs,poss,indx,ExcluMode,every,true);

}

double PMSEnsemble::givePyramidMatchScore(vector<vector<double> > dataset)
{
	if(pyms.size()==0)
		return 0.0;
	else
	{
		vector<double> rslts;
		rslts.clear();
		for(auto s:pyms)
			rslts.push_back(s.givePyramidMatchScore(dataset,false,vector<double> ()));

		return maxminValAndInx(rslts,true).second;
	}
}


trainVecs:: trainVecs(trainVecs::strategies str,vector<vector<double> > lhb,vector<double> st,vector<vector<double> > pv,vector<vector<double> > nv)
{
	mystr=str;
	lowHighBands=lhb;
	stps=st;
	posvecs=pv;
	negvecs=nv;
}

void trainVecs::train()
{
	switch (mystr)
	{
	case trainVecs::allAtOnce:
		trainAllAtOnce();
		break;
	case trainVecs::oneAtonce:
		trainOneAtOnce();
		break;
	default:
		break;
	}
}

void trainVecs::trainOneAtOnce()
{
}

vector<double> nextweights(vector<vector <double> > bds, vector<double> owts,vector<double> stws,int &which)
{

	vector<double> wts(owts);

	auto idx=wts.size()-1;
	bool stpPr=false;
	while(!stpPr)
	{
		wts[idx]+=stws[idx];
		if(wts[idx]<bds[1][idx])
		{
			stpPr=true;
			which=(int)idx;
			if(idx!=(wts.size()-1))
			{
				for(auto i=idx+1;i<wts.size();i++)
					wts[i]=bds[0][i];							
			}

		}
		else
		{
			idx-=1;
		}
		if(idx==0)
		{
			stpPr=true;
			break;
		}

		
	}
	return wts;
}


int DecideBestScore(vector<double> allWeights,vector<int> lblIdx,int allp,int alln)
{
	
	prshl(allWeights,(int)allWeights.size(),lblIdx);
	int bestNum=-1;
	int nnum=0;
	int pnum=0;
	
	for (int i = 0; i < lblIdx.size(); i++)
	{
		if (lblIdx[i]==1)
		{
			pnum+=1;
		}
		else
		{
			nnum+=1;
		}
		
		int crrctNum=nnum+allp-pnum;

		if(crrctNum>bestNum)
		{
			bestNum=crrctNum;
		}


	}

	return bestNum;
}

double multiIVecDvec(vector<double> ivec,vector<double> dvec,int frm)
{

	double result=0.0;
	for (int i = 0; i < dvec.size(); i++)
	{
		result+=ivec[frm+i]*dvec[i];
	}
	return result;
}

void trainVecs::trainAllAtOnce()
{
	vector<double> tweights;
	tweights.clear();
	tweights.insert(tweights.end(),lowHighBands[0].begin(),lowHighBands[0].end());

	vector<double> allScores;
	allScores.resize(posvecs.size()+negvecs.size());

	auto allp=posvecs.size();
	auto alln=negvecs.size();

	

	vector<vector<double> > allvects;
	allvects.clear();
	allvects.insert(allvects.end(),posvecs.begin(),posvecs.end());
	allvects.insert(allvects.end(),negvecs.begin(),negvecs.end());

	vector<int> lblIdx;
	lblIdx.clear();
	vector<int> plbs(allp,1);
	vector<int> nlbs(alln,-1);
	lblIdx.insert(lblIdx.end(),plbs.begin(),plbs.end());
	lblIdx.insert(lblIdx.end(),nlbs.begin(),nlbs.end());

	for (int i = 0; i < allvects.size(); i++)
	{
		allScores[i]=multiIVecDvec(allvects[i],tweights,0);
		
	}
	
	vector<double> newweights;

	auto dim=posvecs[0].size();

	int bestnum=-1;

	while(!endWhile(lowHighBands[1],tweights))
	{
		int inx;
		newweights=nextweights(lowHighBands,tweights,stps,inx);
		
		vector<double> diff;
		diff.resize(dim-inx,0.0);
		
		for (int i = 0; i < diff.size(); i++)
		{
			diff[i]=newweights[inx+i]-tweights[inx+i];
		}
		
		if (inx<4)
		{
			printf("%d ",inx);
		}

		for (int i = 0; i < allScores.size(); i++)
		{
			 allScores[i]+=multiIVecDvec(allvects[i],diff,inx);
		}

		auto tnum=DecideBestScore(allScores,lblIdx,(int)allp,(int)alln);
		
		if (tnum>bestnum)
		{
			setDoubleVec(bstWeights,newweights);
		}

		setDoubleVec(tweights,newweights);

	}

}