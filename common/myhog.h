#define Number_SignleHog 8
#define HogsmallDouble 0.0001

#include<math.h>

static int around[8][2]=
{
	{-1,-1},
	{-1,0},
	{-1,1},
	{0,-1},
	{0,1},
	{1,-1},
	{1,0},
	{1,1}


};

static int fromAroundToInx(int i1,int i2)
{
	int y= around[i1][0]-around[i2][0] ;
	int x= around[i1][1]-around[i2][1];

	if (y==0)
	{
		return 0;
	}
	if (x==0)
	{
		return 1;
	}
	if (x==y)
	{
		return 2;
	}
	if ((x*(-1))==y)
	{
		return 3;
	}
	if ((x*2)==y)
	{
		return 4;
	}
	if ((x*(-2))==y)
	{
		return 5;
	}
	if ((y*2)==x)
	{
		return 6;
	}
	if ((y*(-2))==x)
	{
		return 7;
	}

}

static int findDirection(int y,int x,const vector<vector<double> > & emap)
{
	double l1 = emap[y + around[0][0] ][x + around[0][1] ];
	double l2 = emap[y + around[1][0] ][x + around[1][1] ];
	int in1=0;
	int in2=1;
	for (int i = 2; i < 8; i++)
	{
		if (emap[y + around[i][0] ][x + around[i][1] ] > l1)
		{
			l1=emap[y + around[i][0] ][x + around[i][1] ];
			in1=i;
			continue;
		}
		else if(emap[y + around[i][0] ][x + around[i][1] ] > l2)
		{
			l2 = emap[y + around[i][0] ][x + around[i][1] ] ;
			in2=i;
		}
	}
	if ((l1<HogsmallDouble)||(l2<HogsmallDouble))
	{
		return -1;
	}
	else
	{
		return fromAroundToInx(in1,in2);
	}

}

static vector<vector<vector< int > > > generateHogFromEdgeMap(const vector<vector<double> > & emap)
{
	vector<vector<vector<int> > >  rslt;
	rslt.resize(emap.size(), vector<vector<int> > (emap[0].size(), vector<int> (Number_SignleHog, 0) ));

	for (int i = 1; i < emap.size()-1; i++)
	{
		for (int j = 1; j < emap[0].size()-1; j++)
		{
			int dr=findDirection(i,j,emap);
			if (dr>=0)
			{
				rslt[i][j][dr]+=1;
			}
		}
	}

	return rslt;
}

/*
	for (int i = 1; i < heit; i++)
	{
		for (int j = 1; j < wid; j++)
		{
			intmap[i][j]=intmap[i][j]+intmap[i-1][j]+intmap[i][j-1]-intmap[i-1][j-1];
		}
	}
*/
static vector<vector<vector<int> > > generateHogIntegrateFromEdgeMap(const vector<vector<double> > & emap)
{
	vector<vector<vector<int> > > rslt;

	//rslt.resize(emap.size(), vector<vector<int> > (emap[0].size(), vector<int> (Number_SignleHog, 0) ));
	
	rslt=generateHogFromEdgeMap(emap);

	for (int i = 1; i < emap.size(); i++)
	{
		for (int j = 1; j < emap[0].size(); j++)
		{
			for (int k = 0; k < Number_SignleHog ; k++)
			{
				rslt[i][j][k]=rslt[i][j][k]+rslt[i-1][j][k]+rslt[i][j-1][k]-rslt[i-1][j-1][k];
			}
		}
	}


	return rslt;
}