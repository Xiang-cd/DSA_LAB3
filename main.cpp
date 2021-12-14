#include <iostream>
#include <ctime>
#include "BinTree.hpp"
#include "BST.hpp"
#include "AVL.hpp"
#include "Splay.hpp"
#include "RedBlack.hpp"

#define Time true
#define IDE true

using namespace std;
int n;
clock_t startT, endT;
BST<int> *a;


void p(int num) {
    cout << num << " ";
}

int main(int argc, char *argv[]) {
    if (Time) startT = clock();
#ifndef _OJ_
    if (IDE) {
        freopen("/Users/xxy/CLionProjects/DSA/LAB3/input.txt", "r", stdin);
        freopen("/Users/xxy/CLionProjects/DSA/LAB3/output.txt", "w", stdout);
        a = new AVL<int>();
    } else {
        freopen(argv[1], "r", stdin);
        freopen(argv[2], "r", stdout);
        if (argv[3][0] == '0') {

        } else if (argv[3][0] == '1') {
            a = new Splay<int>();
        } else if (argv[3][0] == '2') {

        }
    }

#endif
    scanf("%d", &n);
    char type;
    int num;
    for (int i = 0; i < n; ++i) {
        if (Debug)cout << "i " << i << endl;
        cin >> type >> num;
        if (type == 'A') {
            if (Debug)cout << "ins" << num << endl;
            a->insert(num);
            if (Debug) {
//                a->travLevel(p);
//                cout << endl;
                a->travIn(p);
                cout << endl;
            }
        } else if (type == 'B') {
            if (Debug)cout << "delete " << num << endl;
            a->remove(num);
            if (Debug) {
//                a->travLevel(p);
                a->travIn(p);
                cout << endl;
            }
        } else {
            auto find = a->search(num);
            if (find) {
                if (find->data == num)cout << num << endl;
                else {
                    if (find->data < num)cout << find->data << endl;
                    else {
                        Posi<int> pre = a->_hot->pre();
                        if (pre)cout << pre->data << endl;
                        else cout << -1 << endl;
                    }
                }
            } else {
                if (!a->_hot) {
                    cout << -1 << endl;
                    continue;
                }
                if (num > a->_hot->data) {
                    cout << a->_hot->data << endl;
                } else {
                    Posi<int> pre = a->_hot->pre();
                    if (pre)cout << pre->data << endl;
                    else cout << -1 << endl;
                }
            }
        }
    }
    if (Debug)a->travLevel(p);
    if (Time) {
        endT = clock();
        printf("cost %f\n", (float) (float(endT - startT) / CLOCKS_PER_SEC));
    }
    return 0;
}
