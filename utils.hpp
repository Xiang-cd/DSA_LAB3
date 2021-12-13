//
// Created by Mac-2读者 on 2021/11/18.
//

#ifndef LAB3_UTILS_HPP
#define LAB3_UTILS_HPP

#define IsRoot(x) ( ! ( (x).parent ) )
#define IsLChild(x) ( ! IsRoot(x) && ( & (x) == (x).parent->lc ) )
#define IsRChild(x) ( ! IsRoot(x) && ( & (x) == (x).parent->rc ) )
#define HasParent(x) ( ! IsRoot(x) )
#define HasLChild(x) ( (x).lc )
#define HasRChild(x) ( (x).rc )
#define HasChild(x) ( HasLChild(x) || HasRChild(x) ) //至少拥有一个孩子
#define HasBothChild(x) ( HasLChild(x) && HasRChild(x) ) //同时拥有两个孩子
#define IsLeaf(x) ( ! HasChild(x) )

#define stature_nor(p) ((p) ? (p)->height : -1)  //普通高度定义
#define stature_red_black(p) ((p) ? (p)->height : 0)  // 红黑树的高度定义

#define sibling(p) ( IsLChild( * (p) ) ? (p)->parent->rc : (p)->parent->lc ) /*兄弟*/
#define uncle(x) ( sibling( (x)->parent ) ) /*叔叔*/
#define FromParentTo(x)  ( IsRoot(x) ? this->_root : ( IsLChild(x) ? (x).parent->lc : (x).parent->rc ) )
/*来自父亲的引用*/
#define tallerChild(x) ( \
stature_nor( (x)->lc ) > stature_nor( (x)->rc ) ? (x)->lc : ( /*左高*/ \
stature_nor( (x)->lc ) < stature_nor( (x)->rc ) ? (x)->rc : ( /*右高*/ \
IsLChild( * (x) ) ? (x)->lc : (x)->rc /*等高：与父亲x同侧者（zIg-zIg或zAg-zAg）优先*/ \
) \
) \
)

#define Debug false


template <typename T> struct Cleaner { // release ？？??release 其实没懂
    static void clean ( T x ) { //相当于递归基
#ifdef _DEBUG
        static int n = 0;
      if ( 7 > strlen ( typeid ( T ).name() ) ) { //复杂类型一概忽略，只输出基本类型
         printf ( "\t<%s>[%d]=", typeid ( T ).name(), ++n );
         print ( x );
         printf ( " purged\n" );
      }
#endif
    }
};

template <typename T> struct Cleaner<T*> {
    static void clean ( T* x ) {
        if ( x ) { delete x; } //如果其中包含指针，递归释放
#ifdef _DEBUG
        static int n = 0;
      printf ( "\t<%s>[%d] released\n", typeid ( T* ).name(), ++n );
#endif
    }
};

template <typename T> void release ( T x ) { Cleaner<T>::clean ( x ); }


#endif //LAB3_UTILS_HPP
