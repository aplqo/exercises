#include<iostream>
#include<algorithm>
using namespace std;
typedef unsigned int num;

num a[50],mi;

bool cmp(num a,num b)
{
	return a>b;
}

int main()
{
	int n;
	cin>>n;
	for(num *i=a;i<a+n;i++)
		cin>>*i;
	sort(a,a+n);
	mi=a[n-1];
	for(int i=n-2;i>=0;i--)
		mi=mi*a[i]+1;
	make_heap(a,a+n,cmp);
	while(n-1)
	{
        /*
        A<B<C
        max=(AB+1)C+1=ABC+C+1;
        */
        /*
        a<b<c<d<e
        ab+1 may be greater than d:
            c<d<(ab+1)<e 
        */
		num x=a[0];
		pop_heap(a,a+n,cmp);
		num y=a[0];
		pop_heap(a,a+n-1,cmp);
		a[n-2]=x*y+1;
		n--;
		push_heap(a,a+n,cmp);
	}
	cout<<a[0]-mi;
	return 0;
}