#include <iostream>
#include "BinTree.hpp"
#include "BST.hpp"

using namespace std;

void p(int a){
    cout<<a<<" ";
}


int main() {
    BST<int> a;
    a = BST<int>();
    a.insert(20);
    a.insert(39);
    a.insert(2048);
    a.insert(-149);
    a.travIn(p);
    a.travLevel(p);
    return 0;
}
