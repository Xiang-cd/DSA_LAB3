//
// Created by Mac-2读者 on 2021/11/18.
//

#ifndef LAB3_QUEUE_HPP
#define LAB3_QUEUE_HPP


#include "List.hpp"
template<typename T> class Queue: public List<T>{
public:
    void enqueue(T const& e){ this->insertAsLast(e);}
    T dequeue(){return this->remove(this->first());}
    T & front(){return this->first()->data;}
};
#endif //LAB3_QUEUE_HPP
