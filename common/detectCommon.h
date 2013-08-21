#include <vector>
#include <string>
#include <map>
using namespace std;

#include <stdio.h>
#include <stdlib.h>

#define PT_per_L 40.0
#define Stasdd_Num 26
#define stdd_X 100.0
#define stdd_Y 40.0
#define featureDimension 128

#define feaNumlimit 21

struct obj
{
	int xmin;
	int ymin;
	int xmax;
	int ymax;
	int tror;
	string pos;

	static void initOne(FILE* fp,obj &rslt)
	{
		char ts[30];
		fscanf(fp,"%d %d %d %d %d %s\n",&rslt.xmin,&rslt.ymin,&rslt.xmax,&rslt.ymax,&rslt.tror,&ts);
		rslt.pos=ts;
	};
	
};

struct sDetRec
{
	int w;
	int h;
	int xstp;
	int ystp;
	static void initOne(FILE* fp,sDetRec &rslt)
	{
		
		fscanf(fp,"%d %d %d %d",&rslt.w,&rslt.h,&rslt.xstp,&rslt.ystp);
		
	};
	static void printOne(FILE* fp,const sDetRec &rslt)
	{
		
		fprintf(fp,"%d %d %d %d",rslt.w,rslt.h,rslt.xstp,rslt.ystp);
		
	};
};

struct DetRec
{
	int w;
	int h;
	int xstp;
	int ystp;
	int stp;
	int fnumlim;
	static void initOne(FILE* fp,DetRec &rslt)
	{
		
		fscanf(fp,"%d %d %d %d %d %d",&rslt.w,&rslt.h,&rslt.xstp,&rslt.ystp,&rslt.stp,&rslt.fnumlim);
		
	};
	static void printOne(FILE* fp,const DetRec &rslt)
	{
		
		fprintf(fp,"%d %d %d %d %d %d",rslt.w,rslt.h,rslt.xstp,rslt.ystp,rslt.stp,rslt.fnumlim);
		
	};
};

obj smaller(obj to)
{
	obj o=to;
	int xs=o.xmax-o.xmin;
	int ys=o.ymax-o.ymin;
	
	xs = xs/20<3?3:xs/20;
	ys = ys/20<3?3:ys/20;

	o.xmax-=xs;
	o.xmin+=xs;

	o.ymax-=xs;
	o.ymin+=xs;
	return o;
}
struct sdtctHy
{
	int sx;
	int sy;
	int w;
	int h;

	
};



static void setUsed(sdtctHy o, map<int,map<int,bool> >& daset )
{

	
	int xmin(o.sx),ymin(o.sy),ymax(o.sy+o.h),xmax(o.sx+o.w);

	
	auto st=daset.lower_bound (ymin);

	while( st!=daset.end() && st->first<=ymax )
	{
		auto st1=st->second.lower_bound(xmin);

		while ( st1!=st->second.end() && st1->first<=xmax  )
		{
		
			daset[st->first][st1->first]=true;
			
			st1++;
		}
		st++;
	}

	
}

template<class T>
static pair<vector<vector<int> >, vector<T> > rangequery(obj o,const map<int,map<int,T> >& daset, const map<int, map<int, bool> >& used )
{

	
	int xmin(o.xmin),ymin(o.ymin),ymax(o.ymax),xmax(o.xmax);

	vector<vector<int> > r1;

	vector<T> rslt;
	rslt.clear();

	auto st=daset.lower_bound (ymin);

	while( st!=daset.end() && st->first<=ymax )
	{
		auto st1=st->second.lower_bound(xmin);

		while ( st1!=st->second.end() && st1->first<=xmax  )
		{
			vector<int> tvi(2,0);
			tvi[0]=st1->first;
			tvi[1]=st->first;
			
			if( used.at( tvi[1] ).at( tvi[0] )==false)
			{
				r1.push_back(tvi);
				rslt.push_back(st1->second);
			}
			st1++;
		}
		st++;
	}

	return pair<vector<vector<int> >, vector<T>> (r1, rslt);
}

template<class T>
static pair<vector<vector<int> >, vector<T> > rangequery(obj o,const map<int,map<int,T> >& daset)
{

	
	int xmin(o.xmin),ymin(o.ymin),ymax(o.ymax),xmax(o.xmax);

	vector<vector<int> > r1;

	vector<T> rslt;
	rslt.clear();

	auto st=daset.lower_bound (ymin);

	while( st!=daset.end() && st->first<=ymax )
	{
		auto st1=st->second.lower_bound(xmin);

		while ( st1!=st->second.end() && st1->first<=xmax  )
		{
			vector<int> tvi(2,0);
			tvi[0]=st1->first;
			tvi[1]=st->first;
			
			r1.push_back(tvi);
			rslt.push_back(st1->second);

			st1++;
		}
		st++;
	}

	return pair<vector<vector<int> >, vector<T>> (r1, rslt);
}

template<class T1,class T2, class T3>
struct triple
{
	T1 feas;
	T2 kptFea;
	T3 intmap;
};


template<class T1,class T2, class T3>
struct striple
{
	T1 first;
	T2 second;
	T3 third;
};




static int area(const obj& o)
{
	return (o.xmax-o.xmin)*(o.ymax-o.ymin);
}
static int disp(obj o, const vector<int>& ref)
{
	double av=(double) area(o);

	double st=sqrt(av/PT_per_L);

	double dis=abs(st-(double) ref[0]);
	int minx=0;
	for (int i = 1; i < ref.size(); i++)
	{
		double tdis=abs(st-(double) ref[i]);
		if (tdis<dis)
		{
			dis =tdis;
			minx=i;
		}
	}
	return minx;
}




static int disp(DetRec o, const vector<int>& ref)
{
	double av=sqrt(o.w*o.h);
	double st=av/PT_per_L;

	double dis=abs(st-(double) ref[1]);
	int minx=1;
	for (int i = 2; i < ref.size(); i++)
	{
		double tdis=abs(st-(double) ref[i]);
		if (tdis<dis)
		{
			dis =tdis;
			minx=i;
		}
	}
	return minx;
}

int countkptnumber(obj o,const vector<vector<int> >& intmap)
{
	int sx,sy,ex,ey;



	int h=intmap.size();
	int w=intmap[0].size();

	sx=o.xmin<0?0:o.xmin;
	sy=o.ymin<0?0:o.ymin;

	sx=sx<w?sx:(w-1);
	sy=sy<h?sy:(h-1);

	ex=o.xmax<w?o.xmax:(w-1);
	ey=o.ymax<h?o.ymax:(h-1);

	ex=ex<0?0:ex;
	ey=ey<0?0:ey;

	return intmap[ey][ex]+intmap[sy][sx]-intmap[ey][sx]-intmap[sy][ex];
}
template<class T>
pair<int,int> odisp(obj o,const vector< triple<vector<vector<T> >,map<int,map<int, int > >, vector<vector<int> > > >& ref,int flim)
{
	int feasnum=countkptnumber(o,ref[0].intmap);
	if (feasnum<flim)
	{
		return pair<int,int>(0,0);
	}
	if (feasnum<Stasdd_Num)
	{
		return pair<int,int>(0,feasnum);
	}
	int propernum=abs(feasnum-Stasdd_Num);
	int properindex=0;
	
	for (int i = 1; i < ref.size(); i++)
	{
		int tfen=countkptnumber(o,ref[i].intmap);
		int tpropernum=abs(tfen-Stasdd_Num);
		if (tpropernum<propernum)
		{
			propernum=tpropernum;
			properindex=i;
			feasnum=tfen;
		}
	}

	return pair<int,int>(properindex,feasnum);
}

static vector<pair<int,int> > inptsForObj(obj o, const string& s, const string& extrs,int wh)
{
	vector<pair<int,int> > rslt;

	char td[40];
	sprintf(td,"%s_obj%s_%d",s.c_str(),extrs.c_str(),wh);
	string mys(td);

	int siz_;
	FILE* fp;
	
	fp=fopen((mys+"_num.txt").c_str(),"r");
	fscanf(fp,"%d",&siz_);
	fclose(fp);

	rslt.resize(siz_,pair<int,int>(0,0));

	vector<	pair<double,double> > pts;
	pts.resize(siz_,pair<double,double>( 0.0,0.0));

	fp=fopen((mys+"_pts.txt").c_str(),"r");
	for (int i = 0; i < siz_; i++)
	{
		fscanf(fp,"%lf %lf\n",&pts[i].first,&pts[i].second);
	}
	fclose(fp);

	double ratiox(stdd_X/(double)(o.xmax-o.xmin)),ratioy(stdd_Y/(double)(o.ymax-o.ymin));

	for (int i = 0; i < siz_; i++)
	{
		rslt[i].first=o.xmin + pts[i].first/ratiox;
		rslt[i].second=o.ymin + pts[i].second/ratioy;
	}

	return rslt;
}

void writeOobj(obj o,const string& s, const string& extrs ,int wh, const vector<vector<int> > & cors, const vector<int>& inx,const vector<vector<double> > & feas)
{
	char td[40];
	sprintf(td,"%s_obj%s_%d",s.c_str(),extrs.c_str(),wh);
	string mys(td);
	FILE* fp;
	
	fp=fopen((mys+"_num.txt").c_str(),"w");
	fprintf(fp,"%d",inx.size());

	fclose(fp);
	

	fp=fopen((mys+"_pts.txt").c_str(),"w");

	double ratiox(stdd_X/(double)(o.xmax-o.xmin)),ratioy(stdd_Y/(double)(o.ymax-o.ymin));
	for(auto sth:cors)
	{
		fprintf(fp,"%lf %lf\n",(sth[0]-o.xmin)*ratiox,(sth[1]-o.ymin)*ratioy);
	}
	fclose(fp);
	
	fp=fopen((mys+"_feas.txt").c_str(),"w");
	for(auto sth:inx)
	{
		for (int i = 0; i < feas[sth].size(); i++)
		{
			fprintf(fp,"%lf ",feas[sth][i]);
		}
		fprintf(fp,"\n");
	}
	fclose(fp);


}

void chagobj(obj& o,int x,int y)
{
	o.xmin+=x;
	o.xmax+=x;
	o.ymin+=y;
	o.ymax+=y;
}

vector<obj> moveobj(obj o)
{
	vector<obj> os;
	os.resize(4,o);
	int w=o.xmax-o.xmin;
	int h=o.ymax-o.ymin;

	chagobj(os[0],w,h);
	chagobj(os[1],0-w,h);
	chagobj(os[2],w,0-h);
	chagobj(os[3],0-w,0-h);

	return os;
}
static int stp(int a)
{
	int b=a/10;
	if(b<2)
	{
		b=2;
	}
	if (b>10)
	{
		b=10;
	}
	return b;
}

static int fnlim(int a)
{
	int b=a/80;

	b=b<15?15:b;

	b=b>50?50:b;

	return b;

}
static void writeDetvecs()
{
	vector<int> thrs= fileIOclass::InVectorInt("..\\..\\bndThreshld.txt");

	vector<int> kspt=fileIOclass::InVectorInt("..\\..\\kptStep.txt");

	vector<int> ys=fileIOclass::InVectorInt("..\\..\\ylengths.txt");

	vector<double> rs=fileIOclass::InVectorDouble("..\\..\\ratios.txt");

	DetRec td;
	vector<DetRec > recs;
	recs.resize(ys.size()*rs.size(),td);

	for (int i = 0; i < ys.size(); i++)
	{
		for (int j = 0; j < rs.size(); j++)
		{
			int x=(double)ys[i]/rs[j];
			int y=ys[i];
			int inx=i*rs.size()+j;

			recs[inx].w=x;
			recs[inx].h=y;
			recs[inx].xstp=stp(x);
			recs[inx].ystp=stp(y);
			recs[inx].stp=kspt[disp(recs[inx],kspt)];
			recs[inx].fnumlim=fnlim(x*y);
		}
	}

	fileIOclass::OutVector< DetRec> ("..\\..\\detRecs.txt",recs);
}

struct dtctHy
{
	int sx;
	int sy;
	int w;
	int h;

	int thr;
	int stp;

	vector<int> inx;
	vector<int> score;
	static void printOne(FILE* fp,dtctHy d)
	{
		fprintf(fp,"%d %d %d %d %d %d\n",d.sx,d.sy,d.w,d.h,d.thr,d.stp);
		fprintf(fp,"%d\n",d.inx.size());
		for (int i = 0; i < d.inx.size(); i++)
		{
			fprintf(fp,"%d ",d.inx[i]);
		}
		fprintf(fp,"\n%d\n",d.score.size());
		for (int i = 0; i < d.score.size(); i++)
		{
			fprintf(fp,"%d ",d.score[i]);
		}
		fprintf(fp,"\n");
	};

};




