#include "VecDeal.h"





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

class PMStruc
{
public:


	PMStruc(){};
	PMStruc(int);


	 
	int generatePymFromdata(const vector<vector<double> >& data,int);
	
	double givePyramidMatchScore(const vector<vector<double> >& dataset,bool ExcluMode,vector<double>& scoreAllLevel);

	double givePyramidMatchScore(const vector<vector<pair<int,int> > >& vecFSs,const vector<vector<double> >& poss,bool ExcluMode,vector<double>& scoreAllLevel);

	double givePyramidMatchScoreSpecial(const vector<vector<pair<int,int> > >& vecFSs,const vector<vector<double>>& poss,const vector<int>& indx,bool ExcluMode,vector<int>& every);

	int initPymWithABs(vector<vector<pair<double,double> > > abS,int dimension);
	int AddoneData(vector<double> data,bool AddOrMinus);
	int AddSeverlData(vector<vector<double> > data,bool AddorMinus);

	int GeneratePosWeightWithParameter(double a,double b,double c);

	int GeneratePosWeightWithParameter(double ratio);
	
	vector<pair<int,int> > dataToTwoInxs(vector<double> data);

	static void printTofile(const PMStruc& pm,string s);
	static void loadFromfile(PMStruc& pm,string s);

	vector<double> weights;
	int getadim(){return adimension;};
	int getlvlmt(){return LevelLimit;};


private:

	int dataToPym(const vector<vector<double> >& data);

	bool dataToPymLvl(const vector<vector<double> >& datas,int lvel);



	double MatchDttoPosPymSimple(const vector<vector<double> >& dataset,bool ExcluMode,vector<double> & mnumbers,bool inverse);

	double MatchDttoPosPymSimple(const vector<vector<pair<int,int> > >& vecFSs,const vector<vector<double> >& poss,bool ExcluMode,vector<double> & mnumbers,bool inverse);

	double MatchDttoPosPymSimple(const vector<vector<pair<int,int> > >& vecFSs,const vector<vector<double>>& poss,const vector<int>& indx,bool ExcluMode,vector<int>& every,bool inverse);

	
	int matchDToOneLvSimple(const vector<vector<pair<int,int> > >& vecFSs,const vector<vector<double> >& poss,int levl, bool ExcluMode,vector<bool>& used );

	int matchDToOneLvSimple(const vector<vector<pair<int,int> > >& vecFSs,const vector<vector<double> >& poss,const vector<int>& indx ,int levl, bool ExcluMode,vector<bool>& used,vector<int>& every );

	pair<int,int>  dataToTwoInxWthoutPos(int alvel,vector<double> data);

	pair<int,int> dataToTwoPosInx(int lvel,vector<double> data);
	pair<int,int> dataToTwoPosInx(int lvel,vector<double> data,vector<pair<int,int> > inp);

	pair<int,int> lvelToAPlvel(int);


	vector<unordered_map<int,unordered_map<int,int> > > pym;

	vector<vector<pair<double,double> > > aAbs;

	int adimension;
	int LevelLimit;// 6
	int twExMs;
	vector<int> twoExs;
	

	vector<vector<pair<int,int> > > paOrders;
};

class PMSEnsemble
{
public:
	PMSEnsemble();
	vector<vector<double>> weights;
	int generateAaBsFromdata(vector<vector<double> > data,int);

	int generateStructureFromData(vector<vector<vector<double> > > data);
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

