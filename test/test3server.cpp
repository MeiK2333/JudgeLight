#include <iostream>
#include <string>
#include <unistd.h>
using namespace std;

/**
 * 交互评测器
 * 对于用户程序的每次提问，返回一个数字
 * -1代表小了，1代表大了
 * 当返回0时，代表猜对了
 * 程序结束
 */

int main()
{
    const int ans = 23333333;
    int t, cnt = 0;
    while (cin >> t)
    {
        cerr << "用户猜了：" << t << endl;
        cnt++;
        if (t < ans)
        {
            cout << -1 << endl;
        }
        else if (t > ans)
        {
            cout << 1 << endl;
        }
        else
        {
            cout << 0 << endl;
            cerr << "用户一共猜了：" << cnt << endl;
            break;
        }
        fflush(stdout);
    }
    if (cnt > 40)
        return 1;
    return 0;
}