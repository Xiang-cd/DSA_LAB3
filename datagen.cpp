//
// Created by 项小羽 on 2021/12/13.
//
#include <set>
#include <iostream>
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
    for (int i = start; i < end; i += step){
        cout << "B " << i << endl;
        inserted.erase(i);
    }
}


int main(int argc, char *argv[]) {
    if (IDE) {
        freopen("/Users/xxy/CLionProjects/DSA/LAB3/input.txt", "w", stdout);
        cout << 20 << endl;
        order_insert(0, 10, 2);
        order_remove(0, 10, 2);
//        order_search(1, 10, 2);

    } else {

    }

    return 0;
}