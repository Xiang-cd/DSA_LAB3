#include <iostream>
#include "BinTree.hpp"
#include "BST.hpp"
#include "AVL.hpp"
#include "Splay.hpp"
#include "RedBlack.hpp"

using namespace std;
int n;

void p(int a) {
    cout << a << " ";
}


int main() {
#ifndef _OJ_
    freopen("/Users/xxy/CLionProjects/DSA/LAB3/input.txt", "r", stdin);
//    freopen("/Users/xxy/CLionProjects/DSA/LAB3/output.txt", "w", stdout);
#endif
    Splay<int> a;
    a = Splay<int>();
    scanf("%d", &n);
    char type;
    int num;
    for (int i = 0; i < n; ++i) {
        if (Debug)cout<<"i "<<i<<endl;
        cin >> type >> num;
        if (type == 'A') {
            if (Debug)cout << "ins" << num << endl;
            a.insert(num);
            if (Debug) {
//                a.travLevel(p);
//                cout << endl;
                a.travIn(p);
                cout<<endl;
            }
        } else if (type == 'B') {
            if (Debug)cout<<"delete "<<num<<endl;
            a.remove(num);
            if (Debug) {
//                a.travLevel(p);
                a.travIn(p);
                cout << endl;
            }
        } else {
            auto find = a.search(num);
            if (find) {
                if (find->data == num)cout << num << endl;
                else {
                    if (find->data < num)cout << find->data << endl;
                    else {
                        Posi<int> pre = a._hot->pre();
                        if (pre)cout << pre->data << endl;
                        else cout << -1 << endl;
                    }
                }
            } else {
                if (!a._hot) {
                    cout << -1 << endl;
                    continue;
                }
                if (num > a._hot->data) {
                    cout << a._hot->data << endl;
                } else {
                    Posi<int> pre = a._hot->pre();
                    if (pre)cout << pre->data << endl;
                    else cout << -1 << endl;
                }
            }
        }
    }

    if (Debug)a.travLevel(p);

    return 0;
}
