# Lab3 报告

项晨东 2019011831

## 文件组织:

`AVL.hpp`:AVL树实现源代码<br>`BinTree.hpp`:树节点定义和二叉树实现源代码<br>`BST.hpp`:二叉搜索树实现源代码<br>`datagen.cpp`:数据生成器<br>`List.hpp`:链表的实现源代码<br>`main.cpp`:用于带IO操作测试的主函数源代码<br>`Queue.hpp`:队列实现源代码<br>`RedBlack.hpp`:红黑树实现源代码<br>`report.md`:报告(本文件)<br>`Splay.hpp`:伸展树实现源代码<br>`testmain.cpp`:用于不带IO操作的测试的主函数源代码<br>`utils.hpp`:通用宏定义等代码

## 数据结构实现:

在实现高级搜索树的过程中, 先实现了BinTree(二叉树)和BST(二叉搜索树), 随后基于BST进行派生实现三类高级搜索树, 采用了指针实现的方法, 具体的实现方法和算法思路见说明以及代码中的注释。

### BinTree:

主要实现了BinNode节点结构体, 主要的内容有节点的父亲指针, 左右孩子指针, 节点高度, 节点颜色, 节点数据等。主要实现的方法有寻找前驱和后继, 各类遍历方法。

BinTree中定义了根节点, 树规模等内容, 主要实现了更新高度, 从父亲到自己的引用等方法。

```c++
virtual int updateHeight(Posi<T> x) {
    return x->height = 1 + max(stature_nor(x->lc), stature_nor(x->rc));
}; //stature_nor为宏,判断是否为空节点,如果是,则返回-1, 否则返回节点本身高度

void updateHeightAbove(Posi<T> x) {
    while (x) {
        if (x->height == updateHeight(x))break; // 高度不变后, 祖先不必更新
        x = x->parent;
    }
};

inline Posi<T> &from_parent_to(Posi<T> &x) { //需要返回引用
        if (IsRoot(*x)) return _root;
        else if (x == x->parent->lc) return x->parent->lc;
        else return x->parent->rc;
}
```

### BST:

BST主要实现了`insert`(插入),`remove`( 删除), `search`(查找)三种方法。同时为了方便后续树调用改变树结构的方法, 实现了三加四重构`connect34`方法和基于三加四重构的旋转矫正`rotateAt`方法。

```c++
virtual Posi<T> &search(const T &e) {
        if (!this->_root || e == this->_root->data) {// 处理退化情况
            _hot = nullptr;
            return this->_root;
        }
        for (_hot = this->_root;;) { // 二叉搜索
            Posi<T> &v = (e < _hot->data) ? _hot->lc : _hot->rc;
            if (!v || e == v->data) return v;
            _hot = v;
        }
    };

    virtual Posi<T> insert(const T &e) {
        Posi<T> &x = search(e); //先搜索
        if (x) return x;       // 如果存在则直接返回
        x = new BinNode<T>(e, _hot); // 否则新建节点
        this->_size++; this->updateHeightAbove(x);
        return x;
    };

    virtual bool remove(const T &e) {
        Posi<T> &x = this->search(e); //先搜索
        if (!x) return false;    //未找到则返回
        removeAt(x, _hot);      //删除节点(包含后继交换等内容)
        this->_size--; this->updateHeightAbove(_hot);    //更新信息
        return true;
    };	
Posi<T> connect34(Posi<T> a, Posi<T> b, Posi<T> c, Posi<T> t0, Posi<T> t1, Posi<T> t2, Posi<T> t3) { // 三加四重构
        a->lc = t0; if (t0) t0->parent = a; a->rc = t1; if (t1) t1->parent = a;
        this->updateHeight(a);
        c->lc = t2; if (t2)t2->parent = c; c->rc = t3; if (t3)t3->parent = c;
        this->updateHeight(c);
        b->lc = a; a->parent = b; b->rc = c; c->parent = b;
        this->updateHeight(b);
        return b;
    };
Posi<T> rotateAt(Posi<T> x) { // 通过调用三加四重构实现树的重构(分四类情况)
        Posi<T> p = x->parent; Posi<T> g = p->parent;
        if (!x) cerr << "rotate at null" << endl;
        if (IsLChild(*p)) { // 父亲是左孩子
            if (IsLChild(*x)) { //自己是左孩子
                p->parent = g->parent;
                return connect34(x, p, g, x->lc, x->rc, p->rc, g->rc);
            } else { // 自己是右孩子
                x->parent = g->parent;
                return connect34(p, x, g, p->lc, x->lc, x->rc, g->rc);
            }
        } else { // 父亲是右孩子
            if (IsRChild(*x)) { // 自己是右孩子
                p->parent = g->parent;
                return connect34(g, p, x, g->lc, p->lc, x->lc, x->rc);
            } else { //自己是左孩子
                x->parent = g->parent;
                return connect34(g, x, p, g->lc, x->lc, x->rc, p->rc);
            }
        }
    }
```

### AVL Tree:

AVL Tree在BST的基础上, 重载了插入方法和删除方法, 实现了通过判断平衡条件进行树的平衡调整。

```c++
Posi<T> insert(const T &e) {
    Posi<T> &x = this->search(e); // 搜索, 找到则直接返回
    if (x) return x; this->_size++; 
    Posi<T> xx = x = new BinNode<T>(e, this->_hot);
    for (Posi<T> g = this->_hot; g; g = g->parent) {
        if (!AvlBalanced(*g)) {
            this->from_parent_to(g) = this->rotateAt(tallerChild(tallerChild(g)));
            break; // 平衡一次，必然复原
        } else this->updateHeight(g);
    }
    return xx;
}
bool remove(const T &e) {
        Posi<T> &x = this->search(e);
        if (!x) return false;
        removeAt(x, this->_hot);  this->_size--; // 删除节点
        for (Posi<T> g = this->_hot; g; g = g->parent) {// 开始调整平衡
            if (!AvlBalanced(*g)) {
                g = this->from_parent_to(g) = this->rotateAt(tallerChild(tallerChild(g)));
                continue;
            }
// connect34 就自带高度更新，所以如果进行了旋转，接下来的一次高度更新是多余的
// 又因为删除可能旋转h次， 重复的高度更新也可能重复h次，所以可以直接加continue
            this->updateHeight(g);
        }
        return true;
    }
```

### Splay Tree:

Splay Tree在BST的基础上, 重载了搜索, 插入和删除三个方法, 由于Splay树调整树平衡的特殊需求, 主要通过节点attach的方法进行结构调整, 而非调用三加四重构, 通过attach方法实现splay过程。

```c++
inline void attachAsLC(Posi<T> lc, Posi<T> p) {
    p->lc = lc; if (lc) lc->parent = p;
}
inline void attachAsRC(Posi<T> p, Posi<T> rc) {
		p->rc = rc; if (rc) rc->parent = p;
}
Posi<T> splay(Posi<T> v) { //将传入的节点伸展至根
    if (!v) return nullptr;
    Posi<T> p, g;
    while ((p = v->parent) && (g = p->parent)) {
        Posi<T> gg = g->parent; // 分四种情况处理, 将v提高到最高的位置
        if (IsLChild(*v)) {
            if (IsLChild(*p)) { // 先折祖父
                attachAsLC(p->rc, g); attachAsLC(v->rc, p); attachAsRC(p, g); attachAsRC(v, p);
            } else {
                attachAsLC(v->rc, p); attachAsRC(g, v->lc); attachAsLC(g, v); attachAsRC(v, p);
            }
        } else {
            if (IsRChild(*p)) { 
              attachAsRC(g, p->lc); attachAsRC(p, v->lc); attachAsLC(g, p); attachAsLC(p, v);
            } else {
                attachAsRC(p, v->lc); attachAsLC(v->rc, g); attachAsRC(v, g); attachAsLC(p, v);
            }
        }
        if (!gg) v->parent = nullptr;
        else { if (g == gg->lc) attachAsLC(v, gg); else attachAsRC(gg, v); }
        this->updateHeight(g); this->updateHeight(p); this->updateHeight(v);
        // 这里更新高度的顺序是敏感的
// v必然需要最后更新，因为旋转结束后v是子孙三代中最长的。 而g和p的顺序, 若v中序遍历在中，则g和p不是父子关系，顺序不敏感，若在旁, 则g必然在最远侧，g必然是孙子，所以应当先更新g
    }
    if ((p = v->parent)) { // 处理退化情况
      if (IsLChild(*v)) {attachAsLC(v->rc, p); attachAsRC(v, p);} 
      else {attachAsRC(p, v->lc); attachAsLC(p, v);}
        this->updateHeight(p); this->updateHeight(v);
    }
    v->parent = nullptr; return v;
};

Posi<T> &search(const T &e) {
		Posi<T> &p = BST<T>::search(e); //先调用BST搜索
    this->_root = splay(p ? p : this->_hot); // 随后进行伸展
		return this->_root;
};

Posi<T> insert(const T &e) {
        if (!this->_root) { // 处理退化情况
            this->_size++; return this->_root = new BinNode<T>(e);
        }
        if (e == search(e)->data) return this->_root; 
        this->_size++; Posi<T> t = this->_root;
        if (this->_root->data < e) {//如果根的数据小于插入节点,则插入到根和根的右孩子之间
            t->parent = this->_root = new BinNode<T>(e, nullptr, t, t->rc);
            if (HasRChild(*t)) {
                t->rc->parent = this->_root; t->rc = nullptr;
            }
        } else {//反之插入到根和根的左节点之间
            t->parent = this->_root = new BinNode<T>(e, nullptr, t->lc, t);
            if (HasLChild (*t)) {
                t->lc->parent = this->_root; t->lc = nullptr;
            }
        }
        this->updateHeightAbove(t);
        return this->_root;
};
bool remove(const T &e) {
        if (!this->root() or (e != search(e)->data))return false; // 删除失败
        Posi<T> w = this->root();//记录根节点
        if (!HasLChild(*this->root())){ // 如果只有一个孩子, 则直接由孩子接替
            this->_root = this->_root->rc;
            if (this->root()) this->_root->parent = nullptr;
        } else if (!HasRChild(*this->root())){
            this->_root = this->root()->lc;
          	if (this->root())this->_root->parent = nullptr;
        } else{ // 如果有两个孩子
            Posi<T> ltree = this->root()->lc; //分离左子树
            ltree->parent = nullptr; this->_root->lc = nullptr;
            this->_root = this->root()->rc; this->_root->parent = nullptr;
            search(e); // 做一次搜索, 将右树最小值提至树根
            this->_root->lc = ltree; // 接入左子树
            ltree->parent = this->_root;
        }
        this->_size--; delete w;
        if (this->_root) this->updateHeight(this->_root);
        return true;
};
```

### RedBlack Tree:

Redblack Tree在BST的基础上, 重载了插入和删除两个方法, 为了保持红黑树的要求, 还需要额外实现解决双红修正和双黑修正两个方法。

```c++
void solveDoubleRed(Posi<T> x) {
        if (IsRoot(*x)) { // 如果是根,直接染色
            this->_root->is_black = true; this->_root->height++; return;
        }
        Posi<T> p = x->parent; if (IsBlack(p))return; // 父亲为红,直接返回
        Posi<T> g = p->parent; // 否则为双红,并且祖父必然存在
        Posi<T> u = uncle(x); // 寻找叔叔
        if (IsBlack(u)) { // 如果叔叔是黑色
            if (IsLChild(*x) != (IsRChild(*p))) p->is_black = true;//自己和父亲同侧,则父亲染黑, 否则自己染黑
            else x->is_black = true;
            g->is_black = false; //祖父染红
            Posi<T> gg = g->parent; // 进行三加四重构后恢复平衡
            Posi<T> r = this->from_parent_to(g) = this->rotateAt(x);
            r->parent = gg;
        } else{ // 叔叔为红色,只需要染色
            p->is_black = true; p->height++; u->is_black = true; u->height++;
            if (!IsRoot(*g)) g->is_black = false;
            solveDoubleRed(g); // 双红可能会继续上传
        }
};


void solveDoubleBlack(Posi<T> r){
        Posi<T> p = r ? r->parent : this->_hot;// 如果r为空,则找到hot,否则找到r的父亲
        if (!p) return;
        Posi<T> s = (r == p->lc) ? p->rc:p->lc; // 找到兄弟,兄弟必然存在,否则黑高度不一致
        if (IsBlack(s)) { // 如果兄弟为黑
          Posi<T> t = nullptr; // 寻找兄弟的红孩子
          if (IsRed(s->rc)) t = s->rc; if (IsRed(s->lc)) t = s->lc;
            if (t) { // BB1 黑兄弟有红孩子
                bool origin_parent_color = p->is_black;
                Posi<T> b = this->from_parent_to(p) = this->rotateAt(t);
                if (HasLChild(*b)){b->lc->is_black = true; updateHeight(b->lc);}
                if (HasRChild(*b)){b->rc->is_black = true; updateHeight(b->rc); }
                b->is_black = origin_parent_color;
                updateHeight(b);
            } else { // 没有红孩子, 兄弟先染红, 此时兄弟和自己高度一致
                s->is_black = false;
                s->height --;
                if (IsRed(p))p->is_black = true; //兄弟和自己的高度都加一, 原树高度恢复
                else{
                    p->height--; solveDoubleBlack(p); // 否则双黑上传
                }
            }
        } else {
            // 兄弟为红, 三加四重构后使得父亲为红, 再次进行双黑修正
            s->is_black = true;
            p->is_black = false;
            Posi<T> t = IsLChild(*s)? s->lc:s->rc;
            this->_hot = p;
            this->from_parent_to(p) = this->rotateAt(t);
            solveDoubleBlack(r);
        }
    };

Posi<T> insert(const T &e) {
        Posi<T> &x = search(e); if (x) return x; // 先搜索
        x = new BinNode<T>(e, this->_hot, nullptr, nullptr, 0);
        Posi<T> oldx = x; solveDoubleRed(x); // 进行双红修正
        return oldx;
};

bool remove(const T &e){
		Posi<T> &x = this->search(e); if (! x) return false;//先搜索, 未找到则直接返回
		Posi<T> r = removeAt(x,this->_hot); this->_size--;
		if ( this->empty()) return true;
		if (!this->_hot){ // 若接替者成为根节点,则直接染色,更新高度后返回
        this->_root->is_black = true; updateHeight(this->_root);
        return true;
    }
    if (BlackHeightUpdated(*this->_hot)) return true; //如果删除后 hot 依旧平衡，说明无需进行双黑矫正,这里意味着被删除的是红节点
    if (IsRed(r)){r->is_black = true; r->height++; return true;}// 接替者为红,直接染色
    solveDoubleBlack(r);//此时需要双黑矫正
    return true;
}
```

## 复杂度分析

### AVL Tree

查找: 查找过程中在每层比较的次数最多为1次, 所以复杂度不会超过O(h)

插入: 首先会调用一次查找操作, 复杂度为O(h), 随后可能会进行树的结构调整, 检查是否平衡最多进行h次, 而调整树的结构最多进行1次, 总体复杂度为O(h)

删除: 首先会调用一次查找, 复杂度为O(h), 随后进行平衡的调整, 平衡的调整可能会不断上传, 调整次数最多进行h次, 总体复杂度为O(h)

根据教材证明, AVL Tree在高度一定, 节点最少的情况下, 节点数是高度的斐波那契函数值, 从而可知h = O(log((N)), 可知AVL所有操作的复杂度为O(log((N))。

###  Splay Tree

查找: 查找过程中在每层比较的次数最多为1次, 所以复杂度不会超过O(h), 随后进行splay, 操作的过程长度也不会超过h, 所以总体复杂度为O(h)

插入: 首先会调用一次查找(包括splay), 随后进行根节点的插入O(1), 总体复杂度为O(h)

删除: 首先调用一次查找(包括splay), 最后进行根节点的删除,  因为为了使得右树的最小孩子提至树根, 会额外调用依次查找, 总体提复杂度为O(h), 但常数略大。

Splay树在极端条件下h和N同阶, 但是在均摊意义下h = O(log(N)), 所以Splay树在均摊意义下所有操作的复杂度为O(log(N))。

### Redbalck Tree

查找: 查找过程中在每层比较的次数最多为1次, 所以复杂度不会超过O(h)

插入: 首先调用一次查找, 随后进行双红修正, 双红修正可能会上传, 但最多不超过h, 并且上传过程中只需染色, 总体复杂度为O(h)。

删除: 首先调用一次查找, 随后进行双黑修正, 双黑修正也可能上传, 但最多不超过h, 并且上传过程中只需染色, 总体复杂度为O(h)。

将红黑树理解为(2,4)B树, 很容易证明高度为log(N) 量级, 所以红黑树各个操作的复杂度为O(log(N))



## 测试部分

为了测试三棵树的性能, 主要按照是否随机来生成测例, 为此主要构建了七种测试样例, 其中按序依次为

1. 顺序插入,正序查找; 从0顺序插入2000000个节点, 随后从0顺序查找2000000个节点。 
2. 顺序插入,逆序查找;从0顺序插入2000000个节点, 随后从2000000-1逆序查找到0, 共2000000个节点。
3. 顺序插入, 正序查找后逆序查找, 从0顺序插入2000000个节点, 随后从0顺序查找2000000个节点, 随后从2000000-1逆序查找到0。
4. 顺序插入, 顺序删除; 从0顺序插入2000000个节点, 随后从0顺序删除2000000个节点。
4. 顺序插入,随机查找; 从0顺序插入2000000个节点, 随后随机查找这2000000个节点。

以上顺序的测试样例主要为了测试各个树在有序的查询和接口调用下的表现情况, 其中主要关注Splay Tree 和其他树的区别, 因为Splay Tree的树形态是与查询顺序相关的, 从理论上说, 相近查询会让Splay Tree 表现的更出色。

6. 随机插入,随机查找; 随机插入2000000个节点, 随后随机查询这2000000个节点, 这是比较自然的数据形式, 在很多情况下随机的数据插入和查询也是合理的。
7. 随机插入,随机查找; 随机插入2000000个节点, 随后随机删除这2000000个节点, 此测试样例主要比较树之间删除性能之间的差别。

通过脚本按照相关参数运行后得到如下结果(共进行了两次实验. 以减少实验过程中的随机因素影响):

单位为秒(s)

| Data\tree | AVL  | Splay | RedBlack |
| --------- | ---- | ----- | -------- |
|顺序插入,正序查找|1.120|0.755|1.287|
|顺序插入,逆序查找|1.109|0.556|1.208|
|顺序插入,正序查找后逆序查找|1.570|1.083|1.670|
|顺序插入,正序删除|1.312|0.722|1.200|
|顺序插入,随机查找|1.910|3.561|2.156|
|随机插入,随机查找|3.243|8.221|2.975|
|随机插入,随机删除|4.506|8.539|3.501|

| Data\tree | AVL  | Splay | RedBlack |
| --------- | ---- | ----- | -------- |
|顺序插入,正序查找|1.046|0.725|1.196|
|顺序插入,逆序查找|1.020|0.539|1.180|
|顺序插入,正序查找后逆序查找|1.794|1.043|1.683|
|顺序插入,正序删除|1.350|0.784|1.245|
|顺序插入,随机查找|2.356|4.001|2.232|
|随机插入,随机查找|5.147|11.240|3.292|
|随机插入,随机删除|5.862|9.155|4.188|

在实验的过程中注意到系统输入输出的IO操作所占用的时间在一些情况下占比比较高, 为了更好的比较树本身的性能， 再次进行测试时在**原位生成数组和打乱操作, 不进行写入和输出操作**, 这样能更好的比较树之间的操作效率, 减少IO操作对实验的影响，相关代码于`testmain.cpp`中, 以下是测试两次的结果。

单位为秒(s)


| Data\tree | AVL  | Splay | RedBlack |
| -------- | ---- | ----- | -------- |
|顺序插入,正序查找|0.7160|0.3825|0.9099|
|顺序插入,逆序查找|0.7150|0.2555|0.9443|
|顺序插入,正序查找后逆序查找|0.9528|0.4323|1.1830|
|顺序插入,正序删除|1.1386|0.4848|1.0776|
|顺序插入,随机查找|1.5515|3.0878|1.8099|
|随机插入,随机查找|3.0067|7.9402|3.4970|
|随机插入,随机删除|4.3812|8.5836|3.3330|

| Data\tree | AVL  | Splay | RedBlack |
| -------- | ---- | ----- | -------- |
|顺序插入,正序查找|0.6799|0.3962|0.9643|
|顺序插入,逆序查找|0.6822|0.2248|0.8590|
|顺序插入,正序查找后逆序查找|0.9226|0.4440|1.0965|
|顺序插入,正序删除|1.0571|0.5329|0.9648|
|顺序插入,随机查找|1.6379|3.2051|1.7863|
|随机插入,随机查找|3.2877|8.6135|3.1134|
|随机插入,随机删除|4.1614|8.8328|3.7976|

### 结果分析:

#### 横向对比树之间的区别:

首先分析有序数据:

在有序数据中, Splay Tree表现出明显的优越性, 比AVL Tree和RedBlack Tree快不少。这说明Splay Tree 的伸展操作对连续访问和邻近访问的加速作用明显。AVL Tree和RedBlack Tree之间总体AVL更快一些, 只有在顺序插入, 正序删除时RedBlack Tree略有优势; 可能的原因是在顺序插入的条件下AVL树的拓扑变化不大(调整平衡一次后就可以退出循环) , 从而AVL Tree插入操作的常数相对RedBlack Tree更小, 所以表现出AVL Tree的效率更高。

无序数据分析:

在无序数据中, 可以明显注意到Splay Tree的效率不如其他两颗树, 主要的原因是在无序状态 下Splay 树不能发挥其邻近查找的优势, 而旋转到根的操作尽管在复杂度上一致, 但是在常数上略大, 所以效率下降。AVL Tree 和RedBlack Tree的性能相差不大, 总体来说Redblack Tree更有优势, 在随机删除方面优势较为明显, 原因可能是RedBlack Tree在随机插入和删除的条件下对树结构的维护更为高效, 因为RedBlack Tree的插入和删除的结构差异不超过O(1)。

#### 纵向对比不同数据下的相同树的表现:

注意到所有树在有序数据下的查询速度更快, 但是在随机数据下性能明显下降, 从理论上来说, 对比顺序插入,顺序查找和顺序插入,随机查找, 仅仅查找方式的不同, 时间相差很大, 一个可能的原因是顺序查找缓存命中的概率很高, 而随机查找的缓存命中率低, 从而增加了缓存IO的负担。



## 其他说明

主体的代码参考自邓公源代码，均由本人亲手键入。

非常感谢老师和助教的辛苦付出~



