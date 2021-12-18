//
// Created by 项小羽 on 2021/12/18.
//

#include <iostream>
#include <ctime>
#include "BST.hpp"
#include "AVL.hpp"
#include "Splay.hpp"
#include "RedBlack.hpp"
#include <cstdio>
#include <algorithm>
#define Time true
using namespace std;
int n;
clock_t startT, endT;
BST<int> *a;
int lists[2000010] = {0};

int main(int argc, char *argv[]) {
    int tree, type;
    for (tree = 0; tree < 3; ++tree) {
        if (tree == 0) {cout << "AVL" << endl;}
        else if (tree == 1) {cout << "Spaly" << endl;}
        else if (tree == 2) {cout << "RedBlack" << endl;}
        for (type = 0; type < 7; ++type) {
            if (Time) startT = clock();
            for (int i = 0; i < 2000000; ++i)lists[i] = i;
            if (tree == 0) {a = new AVL<int>();}
            else if (tree == 1) {a = new Splay<int>();}
            else if (tree == 2) {a = new RedBlack<int>();}
            if (type == 0) {
                for (int i = 0; i < 2000000; ++i) { a->insert(lists[i]); } //顺序插入
                for (int i = 0; i < 2000000; ++i) { a->search(lists[i]); } // 顺序查找
                random_shuffle(lists, lists + 2000000); // 做两次打乱, 统一测例之间的打乱次数
                a->search(lists[0]); // 防止打乱被编译器优化, 添加数组使用
                random_shuffle(lists, lists + 2000000);
                a->search(lists[0]);
            } else if (type == 1) {
                for (int i = 0; i < 2000000; ++i) { a->insert(lists[i]); } //顺序插入
                for (int i = 2000000 - 1; i >= 0; --i) { a->search(lists[i]); } //  逆序查找
                random_shuffle(lists, lists + 2000000); // 做两次打乱, 统一测例之间的打乱次数
                a->search(lists[0]); // 防止打乱被编译器优化, 添加数组使用
                random_shuffle(lists, lists + 2000000);
                a->search(lists[0]);
            } else if (type == 2) {
                for (int i = 0; i < 2000000; ++i) { a->insert(lists[i]); } //顺序插入
                for (int i = 0; i < 2000000; ++i) { a->search(lists[i]); } // 顺序查找
                for (int i = 2000000 - 1; i >= 0; --i) { a->search(lists[i]); } // 后逆序查找
                random_shuffle(lists, lists + 2000000); // 做两次打乱, 统一测例之间的打乱次数
                a->search(lists[0]); // 防止打乱被编译器优化, 添加数组使用
                random_shuffle(lists, lists + 2000000);
                a->search(lists[0]);
            } else if (type == 3) {
                for (int i = 0; i < 2000000; ++i) { a->insert(lists[i]); } //顺序插入
                for (int i = 0; i < 2000000; ++i) { a->remove(lists[i]); } // 顺序删除
                random_shuffle(lists, lists + 2000000);
                a->search(lists[0]);
                random_shuffle(lists, lists + 2000000);
                a->search(lists[0]);
            } else if (type == 4) {
                for (int i = 0; i < 2000000; ++i) { a->insert(lists[i]); } //顺序插入
                random_shuffle(lists, lists + 2000000);
                for (int i = 0; i < 2000000; ++i) { a->search(lists[i]); } //随机查找
                a->search(lists[0]);
                random_shuffle(lists, lists + 2000000);
                a->search(lists[0]);
            } else if (type == 5) {
                random_shuffle(lists, lists + 2000000);
                for (int i = 0; i < 2000000; ++i) { a->insert(lists[i]); } //随机插入
                random_shuffle(lists, lists + 2000000);
                for (int i = 0; i < 2000000; ++i) { a->search(lists[i]); } //随机查找
                a->search(lists[0]);
                a->search(lists[0]);
            } else if (type == 6) {
                random_shuffle(lists, lists + 2000000);
                for (int i = 0; i < 2000000; ++i) { a->insert(lists[i]); } //随机插入
                random_shuffle(lists, lists + 2000000);
                for (int i = 0; i < 2000000; ++i) { a->remove(lists[i]); } //随机删除
                a->search(lists[0]);
                a->search(lists[0]);
            }
            if (Time) {
                endT = clock();
                printf("%.4f\n", (float) (float(endT - startT) / CLOCKS_PER_SEC));
            }
        }
    }
    return 0;
}
