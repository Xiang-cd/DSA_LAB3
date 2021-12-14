#include <iostream>
#include <ctime>
#include "BinTree.hpp"
#include "BST.hpp"
#include "AVL.hpp"
#include "Splay.hpp"
#include "RedBlack.hpp"

#define Time false
#define IDE false

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
        freopen(argv[2], "w", stdout);
        if (argv[3][0] == '0') {
            a = new AVL<int>();
        } else if (argv[3][0] == '1') {
            a = new Splay<int>();
        } else if (argv[3][0] == '2') {
            a = new RedBlack<int>();
        }
    }
#endif

    scanf("%d", &n);
    char type;
    int num;
    for (int i = 0; i < n; ++i) {
        cin >> type >> num;
        if (type == 'A') {
            a->insert(num);
        } else if (type == 'B') {
            a->remove(num);
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
    if (Time) {
        endT = clock();
        printf("cost %f\n", (float) (float(endT - startT) / CLOCKS_PER_SEC));
    }
    return 0;
}
