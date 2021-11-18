//
// Created by Mac-2读者 on 2021/11/18.
//

#ifndef LAB3_LIST_HPP
#define LAB3_LIST_HPP

template<typename T>
struct ListNode;

template<typename T> using ListNodePosi = ListNode<T> *;

template<typename T>
struct ListNode {
    T data;
    ListNodePosi<T> pred;
    ListNodePosi<T> succ;

    ListNode() {};

    ListNode(T e, ListNodePosi<T> p = NULL, ListNodePosi<T> s = NULL) :
            data(e), pred(p), succ(s) {};

    ListNodePosi<T> insertAsPred(T const &e) {
        ListNodePosi<T> x = new ListNode(e, pred, this);
        pred->succ = x;
        x->pred = x;
    };

    ListNodePosi<T> insertAsSucc(T const &e) {
        ListNodePosi<T> x = new ListNode(e, this, succ);
        succ->pred = x;
        succ = x;
        return x;
    }
};

template<typename T>
class List {
private:
    int _size;
    ListNodePosi<T> header;
    ListNodePosi<T> trailer;
protected:
    void init() {
        header = new ListNode<T>;
        trailer = new ListNode<T>;
        header->succ = trailer;
        header->pred = NULL;
        trailer->pred = header;
        trailer->succ = NULL;
        _size = 0;
    };

    int clear() {
        int old = _size;
        while (0 < _size) remove(header->succ);
        return old;
    };

    void copyNodes(ListNodePosi<T> x, int num) {
        init();
        while (num--) {
            insertAsLast(x->data);
            x = x->succ;
        }
    };

//    ListNodePosi<T> merge(ListNodePosi<T>, List<T> &, ListNodePosi<T>, int);
    // !!！！几个排序先跳过去

public:
    List() { init(); }

    List(List<T> const &L) {
        copyNodes(L.first(), L.size());
    };

    ~List() {
        clear();
        delete header;
        delete trailer;
    };

    int size() const { return _size; }

    bool empty() const { return _size <= 0; }

    ListNodePosi<T> first() const { return header->succ; } //首节点位置
    ListNodePosi<T> last() const { return trailer->pred; } //末节点位置


    ListNodePosi<T> insertAsFirst(T const &e) {
        _size++;
        return header->insertAsSucc(e);
    }; //将e当作首节点插入
    ListNodePosi<T> insertAsLast(T const &e) {
        _size++;
        return trailer->insertAsPred(e);
    }; //将e当作末节点插入
    T remove(ListNodePosi<T> p) {
        T e = p->data;
        p->pred->succ = p->succ;
        p->succ->pred = p->succ;
        delete p;
        _size--;
        return e;
    };
};


#endif //LAB3_LIST_HPP
