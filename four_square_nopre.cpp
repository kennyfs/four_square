#include <iostream>
#include <tuple>
#include <vector>
using namespace std;
const int N=100000000;
int n;
long long my_abs(long long a){//絕對值
	return (a>0)?a:-a;
}
struct four{
	long long a,b,c,d;
	four():a(0),b(0),c(0),d(0){}
	four(tuple<int,int,int,int> t){//用tuple<int,int,int,int>初始化一個four
		a=get<0>(t);
		b=get<1>(t);
		c=get<2>(t);
		d=get<3>(t);
	}
	long long value()const{//以存的a,b,c,d值算出a^2+b^2+c^2+d^2
		return a*a+b*b+c*c+d*d;
	}
	void operator=(four f){//複製一個four
		a=f.a;b=f.b;c=f.c;d=f.d;
	}
	four operator*(four& f){//利用尤拉恆等式，回傳值為value等於自己的value*f的value的four
		four ans;
		ans.a=a*f.a+b*f.b+c*f.c+d*f.d;//z=x*y
		ans.b=a*f.b-b*f.a+c*f.d-d*f.c;
		ans.c=a*f.c-c*f.a+d*f.b-b*f.d;
		ans.d=a*f.d-d*f.a+b*f.c-c*f.b;
		return ans;
	}
	four operator%(long long& m){//把a,b,c,d都設為除以m的餘數
		four ans;
		ans.a=a%m;
		ans.b=b%m;
		ans.c=c%m;
		ans.d=d%m;
		return ans;
	}
	void minus_m(long long& m){//把原本0～m-1的餘數變成(-m+1)/2~(m-1)/2
		if(a>m/2)a-=m;
		if(b>m/2)b-=m;
		if(c>m/2)c-=m;
		if(d>m/2)d-=m;
	}
	void operator*=(four& f){//把自己設為自己*f
		four ans=(*this)*f;
		*this=ans;
	}
	void operator/=(long long& m){//把a,b,c,d都除以m
		a/=m;b/=m;c/=m;d/=m;
	}
	void abs(){//把a,b,c,d設為絕對值a,b,c,d，不影響平方和，也比較好看
		a=my_abs(a);
		b=my_abs(b);
		c=my_abs(c);
		d=my_abs(d);
	}
	void divide2(){//value是偶數，把value除以2
		long long na,nb,nc,nd;
		if(a%2!=b%2){
			if(a%2==c%2)swap(b,c);
			else if(a%2==d%2)swap(b,d);
		}//用交換的確保a同餘b(mod 2)
		na=(a+b)/2;
		nb=(a>b)?(a-b)/2:(b-a)/2;
		nc=(c+d)/2;
		nd=(c>d)?(c-d)/2:(d-c)/2;
		a=na,b=nb,c=nc,d=nd;
	}
};
//以下是線性時間篩質數（參考演算法筆記http://web.ntnu.edu.tw/~algo/Prime.html#5）
//一邊製作質數表，一邊刪掉每個數的質數倍。每個合數只遇到最小質因數（次小因數），每個合數只讀取一次、刪除一次。時間複雜度O(N)。
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
//自定義輸出four
ostream& operator<<(ostream& out,four &f){
	out<<f.a<<' '<<f.b<<' '<<f.c<<' '<<f.d;
	return out;
}
bool done[5800000];//10^8以下有5761455個質數
four solve[5800000];
four get(int i){//算質數的four
	//i是prime的序號，回傳value為prime[i]的four
	if(done[i])return solve[i];//做過就回傳原本的值
	int p=prime[i];
	if(p==2)return four(make_tuple(1,1,0,0));//直接回傳
	long long x,y;
	bool find=0;
	for(x=0;2*x<=p-1;++x){//暴力O(p^2)算出x,y，使得1^2+x^2+y^2是p的倍數
		for(y=0;2*y<=p-1;++y){
			if((1+x*x+y*y)%p==0){
				find=1;
				break;//找到就離開
			}
		}
		if(find)break;
	}//1+x^2+y^2=mp
	long long m=(1+x*x+y*y)/p;//用x,y算出m
	four ans,yy,zz;//ans為答案（一直更新），yy、zz是暫存值
	ans.a=1,ans.b=x,ans.c=y,ans.d=0;
	while(m>1){//O(m) 屬於 O(p)
		if(!(m&1)){//如果m是偶數（聽說這樣寫比較快）
			ans.divide2();
			m/=2;
		}else{
			yy=ans%m;//先把yy設為ans除以m的餘數
			yy.minus_m(m);//把原本0～m-1的餘數變成(-m+1)/2~(m-1)/2，所以|y_i|<m/2
			zz=yy*ans;//算出zz，zz.value=yy.value*ans.value=nm*mp=nm^2p
			zz/=m;//zz.value從nm^2p變成np(n<m)
			zz.abs();//絕對值，不影響，但比較好看
			ans=zz;//用value更小的four取代ans
			m=(ans.value())/p;//更新m
		}
		//m嚴格遞減，所以一定會最終變為1(m的最小值)
	}
	solve[i]=ans;
	done[i]=1;
	return ans;
}
int main(){
	cout<<"max prime range?(0 means MAX N)\n";
	cin>>n;
	if(!n)n=N;
	linear_sieve();//n=10^8大約1.27秒
	int q;
	cout<<"How many quests?\n";
	cin>>q;
	while(q--){
		long long a;
		size_t p=0;//p是prime的序號
		cin>>a;//輸入
		four ans,fp;
		bool err=0;
		ans.a=1;
		while(a>1){//a是目前要做的值
			//看所有的質數，如果整除就除，除到不能除為止，然後跳到下一個質數
			while(p<prime.size()&&a%prime[p])p++;
			if(p>=prime.size()){cout<<"too big error\n";err=1;break;}
			//如果列表內的質數都不行整除p，代表列表範圍不夠大，無法做，暫時不做動態擴增質數，因為太大的質數複雜度一樣會太大
			fp=get(p);
			ans*=fp;//*=是用參考傳入，所以要先存在fp裡
			ans.abs();
			a/=prime[p];
		}
		if(!err)cout<<ans<<" : "<<ans.value()<<'\n';//沒有錯誤才輸出
	}
}
