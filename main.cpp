#include <iostream>
#include "AVL.hpp"
#include "Splay.hpp"
#include "RedBlack.hpp"
#include <cstdio>

using namespace std;
int n;
BST<int> *a;

int main(int argc, char *argv[]) {
    a = new AVL<int>();
#ifndef _OJ_
    freopen(argv[1], "r", stdin);
    freopen(argv[2], "w", stdout);
//    freopen("/Users/xxy/CLionProjects/DSA/LAB3/input.txt", "r", stdin);
//    freopen("/Users/xxy/CLionProjects/DSA/LAB3/output.txt", "w", stdout);
    if (argv[3][0] == '0') {
        a = new AVL<int>();
        printf("avl\n");
    } else if (argv[3][0] == '1') {
        a = new Splay<int>();
        printf("splay\n");
    } else if (argv[3][0] == '2') {
        a = new RedBlack<int>();
        printf("redblack\n");
    }
#endif

    scanf("%d", &n);
    char *buffer = new char[2];
    char type;
    int num;
    for (int i = 0; i < n; ++i) {
        scanf("%s%d", buffer, &num);
        type = buffer[0];
        if (type == 'A') {
            a->insert(num);
        } else if (type == 'B') {
            a->remove(num);
        } else {
            Posi<int> &find = a->search(num);
            if (find) {
                if (find->data == num)printf("%d\n", num);
                else {
                    if (find->data < num)printf("%d\n", find->data);
                    else {
                        Posi<int> pre = a->_hot->pre();
                        if (pre)printf("%d\n", pre->data);
                        else printf("-1\n");
                    }
                }
            } else {
                if (!a->_hot) {
                    printf("-1\n");
                    continue;
                }
                if (num > a->_hot->data) {
                    printf("%d\n", a->_hot->data);
                } else {
                    Posi<int> pre = a->_hot->pre();
                    if (pre)printf("%d\n", pre->data);
                    else printf("-1\n");
                }
            }
        }
    }
    return 0;
}
