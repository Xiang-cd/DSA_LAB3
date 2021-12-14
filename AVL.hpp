//
// Created by 项小羽 on 2021/11/19.
//

#ifndef LAB3_AVL_HPP
#define LAB3_AVL_HPP

#define Balanced(x) ( stature( (x).lc ) == stature( (x).rc ) ) //理想平衡条件
#define BalFac(x) ( stature_nor( (x).lc ) - stature_nor( (x).rc ) ) //平衡因子
#define AvlBalanced(x) ( ( -2 < BalFac(x) ) && ( BalFac(x) < 2 ) ) //AVL平衡条件
#include <iostream>
#include "BST.hpp"

template<typename T>
class AVL : public BST<T> {
public:
    Posi<T> insert(const T &e) {
        Posi<T> &x = this->search(e);
        if (x) return x;
        this->_size++;
        Posi<T> xx = x = new BinNode<T>(e, this->_hot);
        if (Debug)if (this->_hot)cout << "hot " << this->_hot->data << endl;
        for (Posi<T> g = this->_hot; g; g = g->parent) {
            if (!AvlBalanced(*g)) {
                this->from_parent_to(g) = this->rotateAt(tallerChild(tallerChild(g)));
                if (Debug)cout << "root" << this->_root->data << endl;
                break; // 平衡一次，必然复原
            } else this->updateHeight(g);
        }
        return xx;
    }

//    Posi<T> tallerChild(Posi<T> x) {
//        if (stature_nor(x->lc) > stature_nor(x->rc)) {
//            return x->lc;
//        } else if (stature_nor(x->lc) < stature_nor(x->rc)) {
//            return x->rc;
//        } else {
//            if (IsLChild(*x))return x->lc;
//            else return x->rc;
//        }
//    }

    bool remove(const T &e) {
        Posi<T> &x = this->search(e);
        if (!x) return false;
        removeAt(x, this->_hot);
//        if(Debug){cout<<this->root()->data<<endl;}
        this->_size--;
        for (Posi<T> g = this->_hot; g; g = g->parent) {
            if (!AvlBalanced(*g)) {
                g = this->from_parent_to(g) = this->rotateAt(tallerChild(tallerChild(g)));
                continue;
            }
            // 为什么在旋转之后只有旋转子树的根需要更新高度，而别的节点不需要??
            // 原因是connect34 就自带高度更新，所以如果进行了旋转，接下来的一次高度更新是多余的
            // 又因为删除可能旋转h次， 重复的高度更新也可能重复h次，所以可以直接加continue
            this->updateHeight(g);
        }
        return true;
    }

};

#endif //LAB3_AVL_HPP
