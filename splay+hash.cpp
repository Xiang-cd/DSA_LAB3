//
// Created by 项小羽 on 2021/11/23.
//

//
// Created by Mac-2读者 on 2021/11/20.
//

#ifndef LAB3_SPLAY_HPP
#define LAB3_SPLAY_HPP

#include "BST.hpp"




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
    } else if (!HasLChild(*x)) succ = x = x->lc;
    else {
        w = w->succ();
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
    release(w->data);
    release(w);
    return succ;
}

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
                return this->_root;
            }
            for (_hot = this->_root;;) {
                Posi<T> &v = (e < _hot->data) ? _hot->lc : _hot->rc;
                if (!v || e == v->data) return v;
                _hot = v;
            }
        };

        virtual Posi<T> insert(const T &e) {
            Posi<T> &x = this->search(e);
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
        //TODO:move to binnode
        int ori1,ori2,rev1,rev2,rev;
        void update(){
            //TODO:update ori1-rev2
        }
        void add_tag(){
            rev ^= 1;
            //TODO:swap ori and rev
        }
        void pushdown(){
            if(rev == 1){
                rev = 0;
                if(lc != nullptr){
                    lc->add_tag();
                }
                if(rc != nullptr){
                    rc->add_tag();
                }
            }
        }
        Posi<T> splay(Posi<T> v, Posi<T> top = nullptr) { //将传入的节点伸展至根
            if (!v) return nullptr;
            Posi<T> p, g;
            while ((p = v->parent) != top && (g = p->parent) != top) {
                Posi<T> gg = g->parent;
                if (IsLChild(*v)) {
                    if (IsLChild(*p)) {
                        // 先折祖父
                        attachAsLC(p->rc, g);
                        attachAsLC(v->rc, p);
                        attachAsRC(p, g);
                        attachAsRC(v, p);
                    } else {
                        //                    this->connect34(g,v,p,g->lc,v->lc,v->rc,p->rc);
                        attachAsRC(g, v->lc);
                        attachAsLC(v->rc, p);
                        attachAsLC(g, v);
                        attachAsRC(v, p);
                    }
                } else {
                    if (IsLChild(*p)) {
                        //                    this->connect34(p,v,g,p->lc,v->lc,v->rc,g->rc);
                        attachAsLC(v->rc, g);
                        attachAsRC(p, v->lc);
                        attachAsLC(p, v);
                        attachAsRC(v, g);
                    } else {
                        attachAsRC(g, p->lc);
                        attachAsRC(p, v->lc);
                        attachAsLC(g, p);
                        attachAsLC(p, v);
                    }
                }
                if (!gg) v->parent = nullptr;
                else { if (IsLChild(*g)) attachAsLC(v, gg); else attachAsRC(gg, v); }
                this->updateHeight(g);
                this->updateHeight(p);
                this->updateHeight(v);
                g->update();
                p->update();
                v->update();
                // 这里更新高度的顺序是敏感的
                // v必然需要随后更新，因为旋转结束后v是子孙三代中最长的， 而g和p的顺序
                // 若 v中序遍历在中， 则g和p不是父子关系，顺序不敏感，若在旁， 则g必然在最远侧，必然是孙子，所以应当先给更行g
            }
            if ((p = v->parent) != top) {
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
        Posi<T> &search(const T &e){
            this->pushdown();
            //TODO:search
            //Posi<T> p = BST<T>::search(e);
            //this->_root = splay(p? p:this->_hot);
            //return this->_root;
        };

        Posi<T> &insert(const T &e){
            this->pushdown();
            //TODO:insert
            //Posi<T> p = BST<T>::insert(e);
            //this->_root = splay(p);
            //return this->_root;
        };

        bool remove(const T &e){
            pushdown();
            //TODO:splay(pre,nullptr),splay(suc,pre)
            //then remove pre->rc->lc(==suc->lc)
            //splay(this->_hot);
            return ans;
        };

        void rev(e1,e2){
            //TODO:pushdown
            //then splay pre of e1 to root, splay suc of e2 to e1
            //pre->rc->lc(==suc->lc)->add_tag()
        }

        void search_inter(e1,e2){
            //
            return pre->rc->lc;
        }
    };

#endif //LAB3_SPLAY_HPP
