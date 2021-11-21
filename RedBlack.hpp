//
// Created by Mac-2读者 on 2021/11/20.
//

#ifndef LAB3_REDBLACK_HPP
#define LAB3_REDBLACK_HPP

#include "BST.hpp"

#define IsBlack(p) ( ! (p) || p->is_black ) //外部节点也视作黑节点
#define IsRed(p) ( ! IsBlack(p) ) //非黑即红
#define BlackHeightUpdated(x) ( /*RedBlack高度更新条件*/ \
   ( stature_red_black( (x).lc ) == stature_red_black( (x).rc ) ) && \
   ( (x).height == ( IsRed(& x) ? stature_red_black( (x).lc ) : stature_red_black( (x).lc ) + 1 ) ) \
)

template<typename T>
class RedBlack : public BST<T> {
protected:
    void solveDoubleRed(Posi<T> x) {
        if (IsRoot(*x)) {
            this->_root->is_black = true;
            this->_root->height++;
            return;
        }
        Posi<T> p = x->parent;
        if (IsBlack(p))return;
        Posi<T> g = p->parent;
        Posi<T> u = uncle(x);
        if (IsBlack(u)) {
            if (IsLChild(*x) != (IsRChild(*p))) p->is_black = true;
            else x->is_black = true;
            g->is_black = false;
            Posi<T> gg = g->parent;
            Posi<T> r = FromParentTo(*g) = this->rotateAt(x);
            r->parent = gg;
        } else{
            // 只需要染色
            p->is_black = true;
            p->height++;
            u->is_black = true;
            u->height ++;
            if (!IsRoot(*g)) g->is_black = false;
            solveDoubleRed(g);
        }
    };

    void solveDoubleBlack(Posi<T> r){
        Posi<T> p = r ? r->parent : this->_hot;
        Posi<T> s = (r == p->lc) ? p->rc:p->lc;
        if (IsBlack(s)) {
            Posi<T> t = NULL;
            if (IsRed(s->rc)) t = s->rc;
            if (IsRed(s->lc)) t = s->lc;
            if (t) { // BB1 黑兄弟有红孩子
                bool origin_parent_color = p->is_black;
                Posi<T> b = FromParentTo(*p) = this->rotateAt(t); //??照理说这里应该不会出现没有孩子的情况
                if (HasLChild(*b)   ){b->lc->is_black = true;
                    updateHeight(b->lc);}
                if (HasRChild(*b)){b->rc->is_black = true;
                    updateHeight(b->rc);
                }
                b->is_black = origin_parent_color;
                updateHeight(b);
            } else {
                s->is_black = false;
                s->height --;
                if (IsRed(p))p->is_black = true; // 尽管转黑了但是黑高度不变因为两个子树的黑高度都减少了1
                else{
                    p->height--;
                    solveDoubleBlack(p);
                }
            }
        } else {
            // 兄弟为红
            s->is_black = true;
            p->is_black = false;
            Posi<T> t = IsLChild(*s)? s->lc:s->rc;
            this->_hot = p;
            FromParentTo(*p) = this->rotateAt(t);
            solveDoubleBlack(r);
        }
    };

    int updateHeight(Posi<T> x){
        return x->height = IsBlack ( x ) + max ( stature_red_black( x->lc ), stature_red_black( x->rc ) );
    };

public:
    Posi<T> insert(const T &e) {
        Posi<T> &x = search(e);
        if (x) return x;
        x = new BinNode<T>(e, this->_hot, NULL, NULL, 0);
        Posi<T> oldx = x; // ？？ 为什么又需要记录当前的指针呢
        solveDoubleRed(x);
        return oldx;
    };

    bool remove(const T &e){
        Posi<T> &x = this->search(e);
        if (! x) return false;
        Posi<T> r = removeAt(x,this->_hot);
        this->_size--;
        if ( this->empty()) return true;
        if (!this->_hot){
            this->_root->is_black = true;
            updateHeight(this->_root);
            return true;
        }
        if (BlackHeightUpdated(*this->_hot)) return true; //如果删除后 hot 依旧平衡，说明无需进行双黑矫正,这里意味着被删除的是红节点
        if (IsRed(r)){r->is_black = true; r->height++; return true;}
        //此时需要双黑矫正
        solveDoubleBlack(r); //?? 为什么入口处r必为空呢
        return true;
    }
};

#endif //LAB3_REDBLACK_HPP
