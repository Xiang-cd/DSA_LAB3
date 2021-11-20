//
// Created by Mac-2读者 on 2021/11/18.
//

#ifndef LAB3_BST_HPP
#define LAB3_BST_HPP

#include "BinTree.hpp"
#include "utils.hpp"
#include <iostream>

using namespace std;

template<typename T>
class BST : public BinTree<T> {
protected:
    Posi<T> _hot;

    Posi<T> connect34(Posi<T> a, Posi<T> b, Posi<T> c, Posi<T> t0, Posi<T> t1, Posi<T> t2, Posi<T> t3) {
        a->lc = t0;
        if (t0) t0->parent = a;
        a->rc = t1;
        if (t1) t1->parent = a;
        this->updateHeight(a);
        c->lc = t2;
        if (t2)t2->parent = c;
        c->rc = t3;
        if (t3)t3->parent = c;
        this->updateHeight(a);
        b->lc = a;
        a->parent = b;
        a->rc = c;
        c->parent = b;
        this->updateHeight(b);
    };

    Posi<T> rotateAt(Posi<T> x) {
        Posi<T> p = x->parent;
        Posi<T> g = p->parent;
        if (!x) cerr << "rotate at null" << endl;
        if (IsLChild(*x)) {

        } else {

        }
    }

public:
    virtual Posi<T> &search(const T &e) {

    };

    virtual Posi<T> insert(const T &e);

    virtual Posi<T> remove(const T &e);

};

#endif //LAB3_BST_HPP
