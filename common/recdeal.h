int pow2(int n)
{
	return (int)pow(2.0,n);
}

struct Point
{
	double x;
	double y;
	Point(double a,double b)
	{
		x=a;
		y=b;
	}
};
struct Rect
{
	double x;
	double y;
	double width;
	double height;

	Rect(double p1,double p2,double p3,double p4)
	{
		x=p1;
		y=p2;
		width=p3;
		height=p4;
	}

	bool contains(Point pt)
	{
		return (pt.x>=x)&&(pt.x<=(x+width))&&(pt.y>=y)&&(pt.y<=(y+height));
	}
};



double overlabRatio(Rect a,Rect b)
{
	vector<Point> inps;
	inps.clear();
	vector<Point> vinps;
	vinps.clear();
	inps.push_back(Point(b.x,b.y));
	inps.push_back(Point(b.x+b.width,b.y));
	inps.push_back(Point(b.x+b.width,b.y+b.height));
	inps.push_back(Point(b.x,b.y+b.height));
	
	int situa(0);

	for (int i=0;i<inps.size();i++)
	{
		if (a.contains(inps[i]))
		{
			vinps.push_back(inps[i]);
			situa+=pow2(i);
		}
	
	}
	double ovAre(0.0);
	switch(situa)
	{
	case 0:
		if (b.contains(Point(a.x,a.y)))
		{
			ovAre=a.width*a.height;
		}
		break;
	case 1:
		ovAre=(a.x+a.width-b.x)*(a.y+a.height-b.y);
		break;
	case 2:
		ovAre=(b.x+b.width-a.x)*(a.y+a.height-b.y);
		break;
	case 3:
		ovAre=b.width*(a.y+a.height-b.y);
		break;
	case 4:
		ovAre=(b.x+b.width-a.x)*(b.y+b.height-a.y);
		break;
	
	case 6:
		ovAre=b.height*(b.x+b.width-a.x);
		break;
	case 8:
		ovAre=(a.x+a.width-b.x)*(b.y+b.height-a.y);
		break;
	case 9:
		ovAre=b.height*(a.x+a.width-b.x);
		break;
	case 12:
		ovAre=b.width*(b.y+b.height-a.y);
		break;
	case 15:
		ovAre=b.height*b.width;
		break;
	default:
		
		break;
	}
	
	double r1=(double)ovAre/(double)(a.width*a.height);
	double r2=(double)ovAre/(double)(b.width*b.height);
	return r1>r2?r1:r2;
}