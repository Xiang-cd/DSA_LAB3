//
// Created by Mac-2读者 on 2021/11/18.
//

#ifndef LAB3_BINTREE_HPP
#define LAB3_BINTREE_HPP
#include <iostream>
#include "utils.hpp"
#include "Queue.hpp"


using namespace std;

template<typename T>
struct BinNode;
template<typename T> using Posi = BinNode<T> *;

template<typename T>
struct BinNode {
public:
    T data;
    Posi<T> parent, lc, rc;
    bool is_black;
    int height;

    int size() {
        int ans = 1;
        if (lc)ans += lc->size();
        if (rc) ans += rc->size();
        return ans;
    };

    Posi<T> insertAsLC(T const &e) { return lc = new BinNode(e, this); };

    Posi<T> insertAsRC(T const &e) { return rc = new BinNode(e, this); };

    Posi<T> succ() { // O(h)
        Posi<T> tmp = this;
        if (rc) {
            tmp = rc;
            while (HasLChild(*tmp)) tmp = tmp->lc;
        } else {
            while (IsRChild(*tmp)) tmp = tmp->parent; // 到最左上方， 然后再上一步，
            tmp = tmp->parent;
        }
        return tmp;
    };
    Posi<T> pre() {
        Posi<T> tmp = this;
        if (lc) {
            tmp = lc;
            while (HasRChild(*tmp))tmp = tmp->rc;
        } else {
            while (IsLChild(*tmp)) tmp = tmp->parent;
            tmp = tmp->parent;
        }
        return tmp;
    }

    BinNode() : parent(nullptr), lc(nullptr), rc(nullptr), height(0), is_black(false) {}

    BinNode(T e, Posi<T> p = nullptr, Posi<T> lc = nullptr, Posi<T> rc = nullptr, int h = 0,
            bool isblack = false) : data(e), parent(p), lc(lc), rc(rc), height(h),
                                    is_black(isblack) {}


    template<class VST>
    void travLevel(VST &visit) {
        Queue<Posi<T> > Q;
        Q.enqueue(this);
        while (!Q.empty()) {
            Posi<T> x = Q.dequeue();
            visit(x->data);
            if (HasLChild(*x)) {
                Q.enqueue(x->lc);
//                if (Debug) printf("%d lc is %d \n", x->data, x->lc->data);
            }
            if (HasRChild(*x)) {
                Q.enqueue(x->rc);
//                if (Debug) printf("%d rc is %d \n", x->data, x->rc->data);
            }
        }
    }; //层次遍历
    template<class VST>
    void travPre(VST &visit) { travPre_R(this, visit); };//先

    template<class VST>
    void travIn(VST &visit) { travIn_R(this, visit); };//中
    template<class VST>
    void travPost(VST &visit) { travPos_R(this, visit); };//后
    bool operator<(BinNode const &bn) { return data < bn.data; }

    bool operator==(BinNode const &bn) { return data == bn.data; }

    bool operator<=(BinNode const &bn) { return data <= bn.data; }

    bool operator>(BinNode const &bn) { return data > bn.data; }

    bool operator>=(BinNode const &bn) { return data >= bn.data; }

    Posi<T> zig() {
        Posi<T> lChild = lc; //为什么可以保证左孩子不为空呢 ??
        if (!lChild)return this; // 额外加个判断
        lChild->parent = this->parent;
        if (lChild->parent) { if (IsLChild(*this))lChild->parent->lc = lChild; else lChild->parent->rc = lChild; }
        lc = lChild->rc;
        if (lc) lc->parent = lChild;
        lChild->rc = this;
        this->parent = lChild;
        // 开始更行高度
        height = 1 + max(stature_nor(lc), stature_nor(rc));
        lChild->height = 1 + max(stature_nor(lChild->lc), stature_nor(lChild->rc));
        for (Posi<T> x; x; x = x->parent) {
            if (x->height == 1 + max(stature_nor(x->lc), stature_nor(x->rc)))break;
            else x->height = 1 + max(stature_nor(x->lc), stature_nor(x->rc));
        }
        return lChild;
    };

    Posi<T> zag() {
        Posi<T> Rc = rc;
        Rc->parent = this->parent;
        if (Rc->parent) { if (IsLChild(*this)) Rc->parent->lc = Rc; else Rc->parent->rc = Rc; }
        rc = Rc->lc;
        if (rc)rc->parent = this;
        Rc->lc = this;
        this->parent = Rc;
        height = 1 + max(stature_nor(lc), stature_nor(rc));
        Rc->height = 1 + max(stature_nor(Rc->rc), stature_nor(Rc->lc));
        for (Posi<T> x = Rc->parent; x; x = x->parent) {
            if (x->height == 1 + max(stature_nor(x->lc), stature_nor(x->rc)))break;
            else x->height = 1 + max(stature_nor(x->lc), stature_nor(x->rc));
        }
        return Rc;
    };
};

template<typename T, typename VST>
void travPre_R(Posi<T> x, VST &visit) {
    if (!x)return;
    visit(x->data);
    travPre_R(x->lc, visit);
    travPre_R(x->rc, visit);
}

template<typename T, typename VST>
void travIn_R(Posi<T> x, VST &visit) {
    if (!x)return;
    travIn_R(x->lc, visit);
    visit(x->data);
    travIn_R(x->rc, visit);
}

template<typename T, typename VST>
void travPos_R(Posi<T> x, VST &visit) {
    if (!x)return;
    travPos_R(x->lc, visit);
    travPos_R(x->rc, visit);
    visit(x->data);
}


template<typename T>
static int removeAt(Posi<T> x) {
    if (!x) return 0;
    int n = 1 + removeAt(x->lc) + removeAt(x->rc);
    release(x->data);
    release(x);
    return n;
}

template<typename T>
class BinTree {
//protected:
public:
    int _size;
    Posi<T> _root;

    virtual int updateHeight(Posi<T> x) {
        return x->height = 1 + max(stature_nor(x->lc), stature_nor(x->rc));
    };

    void updateHeightAbove(Posi<T> x) {
        //x 本身 的高度也会被更新 修改了
        while (x) {
            if (x->height == updateHeight(x))break;
            x = x->parent;
        }
    };

    BinTree() : _size(0), _root(nullptr) {}

    ~BinTree() { if (0 < _size)remove(_root); }

    int size() const { return _size; }

    bool empty() const { return !_root; }

    Posi<T> root() const { return _root; }

    virtual Posi<T> insert(T const &e) {
        _size = 1;
        return _root = new BinNode<T>(e);
    };

    Posi<T> insert(T const &e, Posi<T> x) {
        _size++;
        x->insertAsLC(e);
        updateHeightAbove(x);
        return x->lc;
    };

    Posi<T> insert(Posi<T> x, T const &e) {
        _size++;
        x->insertAsRC(e);
        updateHeightAbove(x);
        return x->rc;
    };

    inline Posi<T> &from_parent_to(Posi<T> &x) {
        if (IsRoot(*x)) return _root;
        else if (x == x->parent->lc) return x->parent->lc;
        else return x->parent->rc;
    }

    Posi<T> attach(BinTree<T> *&
    sub_tree, Posi<T> position) {// 使用时候必须保证左孩子为空

        if ((position->lc = sub_tree->_root))position->lc->parent = position; //？？这是看是否赋值成功吗
        _size += sub_tree->size();
        updateHeightAbove(position);
        sub_tree->_root = nullptr;
        sub_tree->_size = 0;
        release(sub_tree);
        sub_tree = nullptr;
        return position;
    };

    Posi<T> attach(Posi<T> position, BinTree<T> *&subtree) {
        if ((position->rc = subtree->_root))position->lc->parent = position;
        _size += subtree->size();
        updateHeightAbove(position);
        subtree->_root = nullptr;
        subtree->_size = 0;
        release(subtree);
        subtree = nullptr;
        return position;
    };

    int remove(Posi<T> x) {
        this->from_parent_to(x) = nullptr;
        updateHeightAbove(x->parent);
        int n = removeAt(x);
        _size -= n;
        return n;
    };

    BinTree<T> *secede(Posi<T> x) { //子树分离
        this->from_parent_to(x) = nullptr;
        updateHeightAbove(x->parent);
        BinTree<T> *S = new BinTree<T>();
        S->_root = x;
        x->parent = nullptr;
        S->_size = x->size();
        _size -= S->_size;
        return S;
    };

    template<typename VST>
    void travLevel(VST &visit) { if (_root) _root->travLevel(visit); } //层次遍历
    template<typename VST>
    void travPre(VST &visit) { if (_root) _root->travPre(visit); } //先序遍历
    template<typename VST>
    void travIn(VST &visit) { if (_root) _root->travIn(visit); } //中序遍历
    template<typename VST>
    void travPost(VST &visit) { if (_root) _root->travPost(visit); } //后序遍历
    // 树的比较算子就不做了
};

#endif //LAB3_BINTREE_HPP
