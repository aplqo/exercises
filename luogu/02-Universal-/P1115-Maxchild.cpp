#include <algorithm>
#include <climits>
#include <iostream>
using namespace std;

long long sum[200005][2];

int main()
{
    int t, n;
    cin >> n;
    cin >> sum[0][1];
    sum[0][0] = LLONG_MIN;
    for (int i = 1; i < n; i++)
    {
        cin >> t;
        sum[i][0] = max(sum[i - 1][0], sum[i - 1][1]);
        if (t > sum[i - 1][1] + t)
            sum[i][1] = t;
        else
            sum[i][1] = sum[i - 1][1] + t;
    }
    cout << max(sum[n - 1][0], sum[n - 1][1]);
    return 0;
}