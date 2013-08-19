#include "VecDeal.h"
#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif

#include "..\common\bigint\BigIntegerLibrary.hh"

typedef __int64 LInt;



static vector<vector<pair<int,int> > > genPAorders(int n)
{
	int m=2*(n-1);
	vector<vector<pair<int,int> > > rslt;

	for (int i = m; i >=0; i--)
	{
		vector<pair<int,int> > lvl;
		lvl.clear();

		int s=((i>=(n-1))?(n-1):i);
		int t=i-s;

		while ( (t>=0) && (s<=(n-1)) && (t<=(n-1)) && (s>=0) )
		{
			lvl.push_back(pair<int,int>(s,t));
			s-=1;
			t=i-s;
		}


		rslt.push_back(lvl);

	}
	return rslt;
};


static vector<pair<int,int> >  genPAorders(int n,int ann)
{
	int m=(n-1)+(ann-1);

	vector<pair<int,int> >  rslt;

	for (int i = m; i >=0; i--)
	{
	//	vector<pair<int,int> > lvl;
	//	lvl.clear();

		int s=((i>=(n-1))?(n-1):i);
	
		int t=i-s;

		while ( (t>=1) && (s<=(n-1)) && (t<=(ann-1)) && (s>=0) )
		{
			rslt.push_back(pair<int,int>(s,t));
			s-=1;
			t=i-s;
		}


	//	rslt.push_back(lvl);

	}
	return rslt;
};

class PMStruc
{
public:


	PMStruc(){};
	PMStruc(int,int);


	 
	int generatePymFromdata(const vector<vector<double> >& data,int);
	
	double givePyramidMatchScore(const vector<vector<double> >& dataset,bool ExcluMode,vector< double>  & scoreAllLevel);

	double givePyramidMatchScore(const vector<vector<pair<LInt,LInt> > >& vecFSs,const vector<vector<double> >& poss,bool ExcluMode,vector< double>  & scoreAllLevel);

	double givePyramidMatchScoreSpecial(const vector<vector<pair<LInt,LInt> > >& vecFSs,const vector<vector<double>>& poss,const vector<int>& indx,bool ExcluMode,vector<pair<int,int> >& every);

	double giveNewPyramidMatchScore(const vector<vector<double > >& vecFSs,bool ExcluMode,vector< double>  & scoreAllLevel);

	//int initPymWithABs(vector<vector<pair<double,double> > > abS,int dimension);
	int AddoneData(vector<double> data,bool AddOrMinus);
	int AddSeverlData(vector<vector<double> > data,bool AddorMinus);

	int GeneratePosWeightWithParameter(double a,double b,double c);

	int GeneratePosWeightWithParameter(double ratio);
	
	vector<pair<LInt,LInt> > dataToTwoInxs(vector<double> data);

	static void printTofile(const PMStruc& pm,string s);
	static void loadFromfile(PMStruc& pm,string s);

	vector<vector< double> > weights;
	int getadim(){return adimension;};
	//int getlvlmt(){return LevelLimit;};


private:

	int dataToPym(const vector<vector<double> >& data);

	bool dataToPymLvl(const vector<vector<double> >& datas,int alvel,int plvel);



	double MatchDttoPosPymSimple(const vector<vector<double> >& dataset,bool ExcluMode,vector< double>  & mnumbers,bool inverse);

	double MatchDttoPosPymSimple(const vector<vector<pair<LInt,LInt> > >& vecFSs,const vector<vector<double> >& poss,bool ExcluMode,vector< double>  & mnumbers,bool inverse);

	double MatchDttoPosPymSimple(const vector<vector<pair<LInt,LInt> > >& vecFSs,const vector<vector<double>>& poss,const vector<int>& indx,bool ExcluMode,vector<pair<int,int> >& every,bool inverse);


	double newMatchDttoPosPymSimple(const vector<vector<double > >& vecFSs,bool ExcluMode,vector< double>  & mnumbers,bool inverse);
	
	int matchDToOneLvSimple(const vector<vector<pair<LInt,LInt> > >& vecFSs,const vector<vector<double> >& poss,int alevl,int plevl, bool ExcluMode,vector<bool>& used );

	int newmatchDToOneLvSimple(const vector<vector<double > >& vecFSs,int alevl,int plevl, bool ExcluMode,vector<bool>& used );

	int matchDToOneLvSimple(const vector<vector<pair<LInt,LInt> > >& vecFSs,const vector<vector<double> >& poss,const vector<int>& indx ,int alevl,int plevl, bool ExcluMode,vector<bool>& used,vector<pair<int,int>>& every );

	pair<LInt,LInt>  dataToTwoInxWthoutPos(int alvel,vector<double> data);

	pair<LInt,LInt> dataToTwoPosInx(int alvel,int plvel,vector<double> data);

	pair<LInt,LInt> dataToTwoPosInx(int alvel,int plvel,vector<double> data,vector<pair<LInt,LInt> > inp);

	vector<int> dataToAIntVec(int alvel,int plvel,vector<double> data);


	vector<vector<  unordered_map<LInt,unordered_map<LInt,int> > > > pym;


	vector<vector<  map<vector<int>, int > > > newpym;

	vector<vector<pair<double,double> > > aAbs;

	int adimension;
	int aLevelLimit;
	int pLevelLimit;// 6

	int twExMs;
	vector<LInt> twoExs;
	

	vector<pair<int,int> >  paOrders;
};

class PMSEnsemble
{
public:
	PMSEnsemble();
	vector<vector<double>> weights;
	int generateAaBsFromdata(vector<vector<double> > data,int);

	//int generateStructureFromData(vector<vector<vector<double> > > data);
	double givePyramidMatchScore(vector<vector<double> > dataset);
	
	vector<vector<pair<double,double> > > aAbs;
	double threshold;


//private:

	vector<PMStruc> pyms;
	int dimension;
	
};

/*
class PMStrainer
{
public:
	enum TrainStrategy
	{
		allAtOnce,
		oneAtOnce

	};
	PMStrainer(PMStruc pm,TrainStrategy s,double d,double l,string ts);
	void Train(vector<vector<int> > posnums,vector<vector<int> > negnums);
	void OutTofile();
	string name;



private:
	PMStruc pmStrct;
	TrainStrategy myStrgy;
	double stepWidth;
	double largeBand;
	vector<double> bestWeights;
	double bestDivScore;
	int bestNumber;

	void trainOneAtOnce(vector<vector<int> > posnums,vector<vector<int> > negnums);
	void trainAllAtOnce(vector<vector<int> > posnums,vector<vector<int> > negnums);
	int DecideBestScore(vector<double> temWeight,vector<vector<int> > posnums,vector<vector<int> > negnums,double& divValue);
};
*/

class trainVecs
{
public:
	enum strategies{allAtOnce,oneAtonce};

	trainVecs(strategies strg,vector<vector<double> > lhbds,vector<double> stps,vector<vector<double> > pexs,vector<vector<double> > nexs);

	void train();
	void printWeights()
	{
		for(auto ss:bstWeights) 
		printf("%lf ",ss);
	};
private:
	strategies mystr;
	vector<vector<double> > posvecs;
	vector<vector<double> > negvecs;
	vector<double> bstWeights;
	vector<double> stps;
	vector<vector<double> > lowHighBands;

	void trainOneAtOnce();
	void trainAllAtOnce();
};


static vector<vector<double > > prepareData(vector<featype> allfeas, vector<vector<double> > TrmTx, bool addPos)
{
	vector<vector<double > >  dats;
	dats.resize(allfeas.size(),vector<double>(0,0.0));
	for (int i=0;i<dats.size();i++)
	{
		dats[i]=allfeas[i].toVdouble();
	}
	
	ZeroMnVec(dats);
	dats=TransitMtx(dats,TrmTx);
	if(addPos)
		dats=addPositionsToData(dats,allfeas);
	return dats;
}


static void printPR(vector<double> pos,vector<double> neg)
{
	vector<double> al;
	al.clear();

	vector<int> inx;
	inx.clear();

	al.insert(al.end(),pos.begin(),pos.end());
	al.insert(al.end(),neg.begin(),neg.end());

	vector<int> ti;
	ti.resize(pos.size(),1);

	inx.insert(inx.end(),ti.begin(),ti.end());

	ti.clear();

	ti.resize(neg.size(),-1);
	inx.insert(inx.end(),ti.begin(),ti.end());
	
	vector<int> i__;
	i__.resize(inx.size(),0);
	for (int i = 0; i < inx.size(); i++)
	{
		i__[i]=i;

	}

	prshl(al,(int)al.size(),i__);

	int nnum=0;
	int pnum=0;
	
	int allp=(int)pos.size();

	int az= (int)	inx.size();

	for (int i = 0; i < inx.size(); i++)
	{
		if (inx[i__[i]]==1)
		{
			pnum+=1;
		}
		else
		{
			nnum+=1;
		}
		
		int crrctNum=nnum+allp-pnum;

		int dtnum=allp-pnum;

		if (i>0 && i<(az-1))
		{
			printf("%lf %lf\n", (double)dtnum / (double)allp, (double)dtnum / (double)(az-i-1) );
		}

	}
}
