#include<iostream>
#include<cstring>
#include<string>
#include<algorithm>
#include<numeric>
using namespace std;
const int stn=100;

int n[25],*cur=n;
/*--Trie--*/
struct num
{
    const char *s;
    unsigned int num;
}str1[]={
    {"one",1},{"two",2},{"three",3},{"four",4},{"five",5},
    {"six",6},{"seven",7},{"eight",8},{"nine",9},{"ten",10},
    {"eleven",11},{"twelve",12},{"thirteen",13} ,{"fourteen",14},{"fifteen",15},
    {"sixteen",16},{"seventeen",17},{"eighteen",18},{"nineteen",19},{"twenty",20},
    {"a",1},{"both",2},{"another",1},{"first",1},{"second",2},{"third",3}};
int ch[stn][26];
unsigned int bo[stn];
void insert(const num &st)
{
    static int cur=0;
    unsigned int n=strlen(st.s),u=0;
    for(unsigned int i=0;i<n;i++)
    {
        unsigned int t=st.s[i]-'a';
        if(!ch[u][t]) ch[u][t]=++cur;
        u=ch[u][t];
    }
    bo[u]=(st.num*(st.num))%100+1;
}
int find(const string &s)
{
    unsigned int n=s.size(),u=0;
    for(unsigned int i=0;i<n;i++)
    {
        int t=s[i]-'a';
        if(!ch[u][t]) return -1;
        u=ch[u][t];
    }
    return bo[u]-1;
}

int main()
{
    for(const num& i:str1)
        insert(i);
    for(int i=0;i<6;i++)
    {
        string s;
        cin>>s;
        for_each(s.begin(),s.end(),[](char &i){i=tolower(i);});
        int t=find(s);
        if(t>=0) *(cur++)=t;
    }
    sort(n,cur,[](const int a,const int b){return a*100+b<b*100+a;});
    unsigned long long i=accumulate(n,cur,0ull,[](unsigned long long r,int i){return r*100+i;});
    cout<<i;
    return 0;
}
