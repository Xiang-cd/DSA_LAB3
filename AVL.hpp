//
// Created by 项小羽 on 2021/11/19.
//

#ifndef LAB3_AVL_HPP
#define LAB3_AVL_HPP

#define Balanced(x) ( stature( (x).lc ) == stature( (x).rc ) ) //理想平衡条件
#define BalFac(x) ( stature( (x).lc ) - stature( (x).rc ) ) //平衡因子
#define AvlBalanced(x) ( ( -2 < BalFac(x) ) && ( BalFac(x) < 2 ) ) //AVL平衡条件

#include "BST.hpp"

template<typename T>
class AVL : public BST<T> {
public:
    Posi<T> insert(const T &e) {
        Posi<T> x = this->search(e);
        if (x) return x;
        x = new BinNode<T>(e, this->_hot);
        this->_size++;
        Posi<T> xx = x; //？？为什么这里要记录下这个指针呢，难道旋转之后x不再指向原来的部分了吗
        for (Posi<T> g = this->_hot; g; g = g->parent) {
            if (!AvlBalanced(*g)) {
                FromParentTo(*g) = this->rotateAt(this->tallerChild(this->tallerChild(g)));
                break; // 平衡一次，必然复原
            } else this->updateHeight(g);
        }
        return xx;
    }

    Posi<T> tallerChild(Posi<T> x) {
        if (stature_nor(x->lc) > stature_nor(x->rc)) {
            return x.lc;
        } else if (stature_nor(x->lc) < stature_nor(x->rc)) {
            return x->rc;
        } else {
            if (IsLChild(*x))return x->lc;
            else return x->rc;
        }
    }

    bool remove(const T &e) {
        Posi<T> &x = search(e);
        if (!x) return false;
        removeAt(x, this->_hot);
        this->_size--;
        for (Posi<T> g = this->_hot; g; g = g->parent) {
            if (!AvlBalanced(*g)) {
                g = FromParentTo(*g) = this->rotateAt(this->tallerChild(this->tallerChild(g)));
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
