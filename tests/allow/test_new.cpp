#include <iostream>
using namespace std;

int main() {
    string *str = new string("Hello World!");
    cout << *str << endl;
    return 0;
}