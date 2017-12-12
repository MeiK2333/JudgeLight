#include <iostream>
using namespace std;

int main()
{
    int n;
    cin >> n;
    if (n > 256)
    {
        cout << "多了" << endl;
        return 1;
    }
    else if (n < 128)
    {
        cout << "少了" << endl;
        return 1;
    }
    else
    {
        cout << "正好" << endl;
    }
    return 0;
}