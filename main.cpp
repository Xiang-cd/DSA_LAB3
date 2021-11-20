#include <iostream>
#include "BinTree.hpp"

using namespace std;

void p(int a){
    cout<<a<<" ";
}


int main() {
    BinTree<int> a = BinTree<int>();
    a.insert(50)   ;
    a.insert(a.root(),100);
    a.insert(20,a.root());
    a.travLevel(p  );
    return 0;
}
