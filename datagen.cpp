//
// Created by 项小羽 on 2021/12/13.
//
#include <set>
#include <iostream>
#include<algorithm>
#include<cstdio>
#include<ctime>
#include <fstream>

using namespace std;
#define IDE true
set<int> inserted;


void order_insert(int start, int end, int step) {
    for (int i = start; i < end; i += step) {
        cout << "A " << i << endl;
        inserted.insert(i);
    }
}

void order_search(int start, int end, int step) {
    for (int i = start; i < end; i += step) cout << "C " << i << endl;
}

void order_remove(int start, int end, int step) {
    for (int i = start; i < end; i += step) {
        cout << "B " << i << endl;
        inserted.erase(i);
    }
}

void rand_insert(int num) {
    int *a = new int[num];
    for (int i = 0; i < num; ++i) a[i] = i;
    random_shuffle(a, a + num);
    for (int i = 0; i < num; ++i) {
        cout << "A " << a[i] << endl;
    }
    delete []a;

}

void rand_search(int num) {
    int *a = new int[num];
    for (int i = 0; i < num; ++i) a[i] = i;
    random_shuffle(a, a + num);
    for (int i = 0; i < num; ++i) {
        cout << "C " << a[i] << endl;
    }
    delete []a;
}

void rand_remove(int num){
    int *a = new int[num];
    for (int i = 0; i < num; ++i) a[i] = i;
    random_shuffle(a, a + num);
    for (int i = 0; i < num; ++i) {
        cout << "B " << a[i] << endl;
    }
    delete []a;
}

int main(int argc, char *argv[]) {
    if (IDE) {
        freopen("/Users/xxy/CLionProjects/DSA/lab3/input.txt", "w", stdout);
        cout << 2000000 << endl;
        order_insert(0, 1000000, 1);
        order_search(0, 1000000, 1);
    } else {
        freopen(argv[1], "w", stdout);
        if (argv[2][0] == '0') { //顺序插入，顺序查找
            cout << 2000000 << endl;
            order_insert(0, 1000000, 1);
            order_search(0, 1000000, 1);
        } else if (argv[2][0] == '1') { // 顺序插入，逆序查找
            cout << 2000000 << endl;
            order_insert(0, 1000000, 1);
            order_search(1000000, 0, -1);
        } else if (argv[2][0] == '2') { // 顺序插入，正顺逆序各一次查找
            cout << 3000000 << endl;
            order_insert(0, 1000000, 1);
            order_search(0, 1000000, 1);
            order_search(1000000, 0, -1);
        } else if (argv[2][0] == '3') {// 随机插入，随机查找
            cout << 2000000 << endl;
            rand_insert(1000000);
            rand_search(1000000);
        } else if (argv[2][0] == '4') { // 随机插入，随机删除
            cout << 2000000 << endl;
            rand_insert(1000000);
            rand_remove(1000000);
        }
    }

    return 0;
}