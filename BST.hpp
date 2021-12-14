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
static Posi<T> removeAt(Posi<T> &x, Posi<T> &hot) {
    Posi<T> w = x;
    Posi<T> succ = nullptr;
    if (!HasLChild(*x)) {
        x = x->rc;
        succ = x;
    } else if (!HasRChild(*x)) succ = x = x->lc;
    else {
        if (Debug){
            printf("before %d ",w->data );
        }
        w = w->succ();
        if (Debug){printf("after %d \n",w->data );}
        T tmp = x->data;
        x->data = w->data;
        w->data = tmp;
        // 现在删除w 就可以了
        Posi<T> u = w->parent;
        ((u == x) ? u->rc : u->lc) = succ = w->rc;
//        ((IsRChild(*w))? u->rc:u->lc) = succ = w->rc;
//  首先如果是被找到的交换后继，必然没有左子树， 从而succ是w的右子树， 但是w即可能是父亲的左孩子，也可能是父亲的右边孩子
// 这里通过判断父亲和前前驱是否一致来区分是做孩子还是又孩子，从而确定节点的悬挂位置，当然，直接判断是做孩子还是右孩子更为直观
    }
    hot = w->parent;
    if (succ) succ->parent = hot;
//    release(w->data);
//    release(w);
    delete w;
    return succ;
}

template<typename T>
class BST : public BinTree<T> {
public:
    Posi<T> _hot;
    Posi<T> hot() {return  _hot;}
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
        this->updateHeight(c);
        b->lc = a;
        a->parent = b;
        b->rc = c;
        c->parent = b;
        this->updateHeight(b);
        return b;
    };

    Posi<T> rotateAt(Posi<T> x) {
        Posi<T> p = x->parent;
        Posi<T> g = p->parent;
        if (!x) cerr << "rotate at null" << endl;
        if (IsLChild(*p)) {
            if (IsLChild(*x)) {
                p->parent = g->parent;
                return connect34(x, p, g, x->lc, x->rc, p->rc, g->rc);
            } else {
                x->parent = g->parent;
                return connect34(p, x, g, p->lc, x->lc, x->rc, g->rc);
            }
        } else {
            if (IsRChild(*x)) {
                p->parent = g->parent;
                return connect34(g, p, x, g->lc, p->lc, x->lc, x->rc);
            } else {
                x->parent = g->parent;
                return connect34(g, x, p, g->lc, x->lc, x->rc, p->rc);
            }
        }
    }

public:
    BST() : BinTree<T>() {};

    virtual Posi<T> &search(const T &e) {
        if (!this->_root || e == this->_root->data) {
            _hot = nullptr;
            if (Debug)cout<<"no root or find root"<<endl;
            return this->_root;
        }
        for (_hot = this->_root;;) {
            Posi<T> &v = (e < _hot->data) ? _hot->lc : _hot->rc;
            if (!v || e == v->data) return v;
            _hot = v;
        }
    };

    virtual Posi<T> insert(const T &e) {
        Posi<T> &x = search(e); //???? 这里从this->search 改成 BST::search ,在splay中就对了
        if (x) return x;
        x = new BinNode<T>(e, _hot);
        this->_size++;
        this->updateHeightAbove(x);
        return x;
    };

    virtual bool remove(const T &e) {
        Posi<T> &x = this->search(e);
        if (!x) return false;
        removeAt(x, _hot);
        this->_size--;
        this->updateHeightAbove(_hot);
        return true;
    };

};

#endif //LAB3_BST_HPP
