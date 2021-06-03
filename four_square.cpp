#include <iostream>
#include <vector>
using namespace std;
const int N = 10000;
struct four{
	int a,b,c,d;
	bool ok;
	void check(int n){
		ok=((a*a+b*b+c*c+d*d)==n);
	}
};
four solve[10000];

bool sieve[N];
vector<int> prime;
void linear_sieve(){//O(N) sieve prime
	fill(sieve,sieve+N,0);
	for(int i=2;i<N;++i){
		if(!sieve[i])prime.push_back(i);
		for(int j=0;i*prime[j]<N;++j){
			sieve[i*prime[j]]=true;
			if(i%prime[j]==0)break;
		}
	}
}
int my_abs(int a){
	return (a>0)?a:-a;
}
int main(){
	linear_sieve();
	for(int p:prime){
		if(p==2)continue;
		cout<<p<<'\n';
		int x,y;
		bool find=0;
		for(x=0;2*x<=p-1;++x){
			for(y=0;2*y<=p-1;++y){
				if((1+x*x+y*y)%p==0){
					find=1;
					break;
				}
			}
			if(find)break;
		}
		int m=(1+x*x+y*y)/p;
		int a=0,b=1,c=x,d=y;
		int na,nb,nc,nd;
		int ya,yb,yc,yd;
		int za,zb,zc,zd;
		while(m>1){
			if(!(m&1)){
				if(a%2!=b%2){
					if(a%2==c%2)swap(b,c);
					else if(a%2==d%2)swap(b,d);
				}//ensure a%2==b%2
				na=(a+b)/2;
				nb=(a>b)?(a-b)/2:(b-a)/2;
				nc=(c+d)/2;
				nd=(c>d)?(c-d)/2:(d-c)/2;
				a=na,b=nb,c=nc,d=nd;
				m/=2;
			}else{
				ya=a%m;if(ya>m/2)ya-=m;
				yb=b%m;if(yb>m/2)yb-=m;
				yc=c%m;if(yc>m/2)yc-=m;
				yd=d%m;if(yd>m/2)yd-=m;
				za=my_abs((a*ya+b*yb+c*yc+d*yd)/m);
				zb=my_abs((a*yb-b*ya+c*yd-d*yc)/m);
				zc=my_abs((a*yc-c*ya+b*yd-d*yb)/m);
				zd=my_abs((a*yd-d*ya+b*yc-c*yb)/m);
				a=za,b=zb,c=zc,d=zd;
				m=(a*a+b*b+c*c+d*d)/p;
			}
		}
		solve[p].a=a;
		solve[p].b=b;
		solve[p].c=c;
		solve[p].d=d;
	}
}
