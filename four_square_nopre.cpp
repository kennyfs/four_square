#include <iostream>
#include <tuple>
#include <vector>
#define tii tuple<int,int,int,int>
const int N=100000000;
using namespace std;
int n;
int my_abs(int a){
	return (a>0)?a:-a;
}
struct four{
	int a,b,c,d;
	bool ok;
	four():a(0),b(0),c(0),d(0){}
	four(tii t){
		a=get<0>(t);
		b=get<1>(t);
		c=get<2>(t);
		d=get<3>(t);
	}
	int value()const{
		return a*a+b*b+c*c+d*d;
	}
	void check(int n){
		ok=(this->value()==n);
	}
	void operator=(four f){
		a=f.a;b=f.b;c=f.c;d=f.d;
	}
	four operator*(four& f){
		four ans;
		ans.a=a*f.a+b*f.b+c*f.c+d*f.d;
		ans.b=a*f.b-b*f.a+c*f.d-d*f.c;
		ans.c=a*f.c-c*f.a+d*f.b-b*f.d;
		ans.d=a*f.d-d*f.a+b*f.c-c*f.b;
		return ans;
	}
	void operator*=(four& f){
		four ans=(*this)*f;
		*this=ans;
	}
	void operator/=(int m){
		a/=m;b/=m;c/=m;d/=m;
	}
	void abs(){
		a=my_abs(a);
		b=my_abs(b);
		c=my_abs(c);
		d=my_abs(d);
	}
	void divide2(){//m is even, divide m by 2
		int na,nb,nc,nd;
		if(a%2!=b%2){
			if(a%2==c%2)swap(b,c);
			else if(a%2==d%2)swap(b,d);
		}//ensure a%2==b%2
		na=(a+b)/2;
		nb=(a>b)?(a-b)/2:(b-a)/2;
		nc=(c+d)/2;
		nd=(c>d)?(c-d)/2:(d-c)/2;
		a=na,b=nb,c=nc,d=nd;
	}
};
bool sieve[N];
vector<int> prime;
void linear_sieve(){//O(N) sieve prime
	fill(sieve,sieve+N,0);
	for(int i=2;i<n;++i){
		if(!sieve[i])prime.push_back(i);
		for(int j=0;i*prime[j]<n;++j){
			sieve[i*prime[j]]=true;
			if(i%prime[j]==0)break;
		}
	}
}
ostream& operator<<(ostream& out,four &f){
	out<<f.a<<' '<<f.b<<' '<<f.c<<' '<<f.d;
	return out;
}
bool done[100000];
four solve[100000];
four get(int i){//p is index of a prime
	if(done[i])return solve[i];
	int p=prime[i];
	if(p==2)return four(make_tuple(1,1,0,0));
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
	four a,yy,zz;
	a.a=1,a.b=x,a.c=y,a.d=0;
	while(m>1){
		if(!(m&1)){
			a.divide2();
			m/=2;
		}else{
			yy.a=a.a%m;if(yy.a>m/2)yy.a-=m;
			yy.b=a.b%m;if(yy.b>m/2)yy.b-=m;
			yy.c=a.c%m;if(yy.c>m/2)yy.c-=m;
			yy.d=a.d%m;if(yy.d>m/2)yy.d-=m;
			zz=yy*a;
			zz/=m;
			zz.abs();
			a=zz;
			m=(a.value())/p;
		}
	}
	solve[i]=a;
	done[i]=1;
	return a;
}
int main(){
	cout<<"max prime range?(0 means MAX N)\n";
	cin>>n;
	if(!n)n=N;
	linear_sieve();
	int q;
	cout<<"How many quests?\n";
	cin>>q;
	while(q--){
		int a;
		size_t p=0;//p is index of prime;
		cin>>a;
		four ans,fp;
		ans.a=1;
		while(a>1){
			while(p<prime.size()&&a%prime[p])p++;
			if(p>=prime.size())break;
			fp=get(p);
			ans*=fp;
			ans.abs();
			a/=prime[p];
		}
		cout<<ans<<" : "<<ans.value()<<'\n';
	}
}
