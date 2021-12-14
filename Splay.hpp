//
// Created by Mac-2读者 on 2021/11/20.
//

#ifndef LAB3_SPLAY_HPP
#define LAB3_SPLAY_HPP
#include <iostream>
#include "BST.hpp"

template<typename T>
inline
void attachAsLC(Posi<T> lc, Posi<T> p) {
    p->lc = lc;
    if (lc) lc->parent = p;
}

template<typename T>
inline
void attachAsRC(Posi<T> p, Posi<T> rc) {
    p->rc = rc;
    if (rc) rc->parent = p;
}

template<typename T>
class Splay : public BST<T> {
protected:
    Posi<T> splay(Posi<T> v) { //将传入的节点伸展至根
        if (!v) return nullptr;
        Posi<T> p, g;
        while ((p = v->parent) && (g = p->parent)) {
            Posi<T> gg = g->parent;
            if (IsLChild(*v)) {
                if (IsLChild(*p)) {
                    // 先折祖父
                    attachAsLC(p->rc, g);
                    attachAsLC(v->rc, p);
                    attachAsRC(p, g);
                    attachAsRC(v, p);
                } else {
                    attachAsLC(v->rc, p);
                    attachAsRC(g, v->lc);
                    attachAsLC(g, v);
                    attachAsRC(v, p);
                }
            } else {
                if (IsRChild(*p)) {
                    attachAsRC(g, p->lc);
                    attachAsRC(p, v->lc);
                    attachAsLC(g, p);
                    attachAsLC(p, v);

                } else {
                    attachAsRC(p, v->lc);
                    attachAsLC(v->rc, g);
                    attachAsRC(v, g);
                    attachAsLC(p, v);
                }
            }
            if (!gg) v->parent = nullptr;
            else { if (g == gg->lc) attachAsLC(v, gg); else attachAsRC(gg, v); }
            this->updateHeight(g);
            this->updateHeight(p);
            this->updateHeight(v);
            // 这里更新高度的顺序是敏感的
            // v必然需要随后更新，因为旋转结束后v是子孙三代中最长的， 而g和p的顺序
            // 若 v中序遍历在中， 则g和p不是父子关系，顺序不敏感，若在旁， 则g必然在最远侧，必然是孙子，所以应当先给更行g
        }
        if ((p = v->parent)) {
            if (IsLChild(*v)) {
                attachAsLC(v->rc, p);
                attachAsRC(v, p);
            } else {
                attachAsRC(p, v->lc);
                attachAsLC(p, v);
            }
            this->updateHeight(p);
            this->updateHeight(v);
        }
        v->parent = nullptr;
        return v;
    };

public:
    Posi<T> &search(const T &e) {
        Posi<T> &p = BST<T>::search(e);
        this->_root = splay(p ? p : this->_hot);
        return this->_root;
    };

    Posi<T> insert(const T &e) {
        if (!this->_root) {
            this->_size++;
            return this->_root = new BinNode<T>(e);
        }
        if (e == search(e)->data) return this->_root;
        this->_size++;
        Posi<T> t = this->_root;
        if (this->_root->data < e) {
            t->parent = this->_root = new BinNode<T>(e, nullptr, t, t->rc);
            if (HasRChild(*t)) {
                t->rc->parent = this->_root;
                t->rc = nullptr;
            }
        } else {
            t->parent = this->_root = new BinNode<T>(e, nullptr, t->lc, t);
            if (HasLChild (*t)) {
                t->lc->parent = this->_root;
                t->lc = nullptr;
            }
        }
        this->updateHeightAbove(t);
        return this->_root;
    };

    bool remove(const T &e) {
//        bool ans = BST<T>::remove(e);
//        splay(this->_hot);
//        return ans;
        if (!this->root() or (e != search(e)->data))return false;
        Posi<T> w = this->root();
        if (!HasLChild(*this->root())){
            this->_root = this->_root->rc;
            if (this->root()) this->_root->parent = nullptr;
        } else if (!HasRChild(*this->root())){
            this->_root = this->root()->lc;
            if (this->root())this->_root->parent = nullptr;
        } else{
            Posi<T> ltree = this->root()->lc;
            ltree->parent = nullptr;
            this->_root->lc = nullptr;
            this->_root = this->root()->rc; this->_root->parent = nullptr;
            search(e);
            this->_root->lc = ltree;
            ltree->parent = this->_root;
        }
        this->_size--;
        delete w;
        if (this->_root) this->updateHeight(this->_root);
        return true;
    };
};

#endif //LAB3_SPLAY_HPP
