#include <iostream>
#include <string>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
using namespace std;

/**
 * 用户提交程序
 * 尝试猜数字，并根据返回结果再次尝试，直到猜正确
 */

int main()
{
    int l = 0, r = 2147483647;
    srand((unsigned)time(NULL));
    while (l < r)
    {
        // int ans = (l + r) / 2;
        int ans = (rand() % (r - l + 1)) + l - 1;
        cout << ans << endl;
        fflush(stdout);
        int t;
        cin >> t;
        if (t < 0)
        {
            l = ans;
        }
        else if (t > 0)
        {
            r = ans;
        }
        else
        {
            l = r = ans;
        }
    }
    cout << l << endl;
    return 0;
}