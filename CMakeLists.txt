cmake_minimum_required(VERSION 3.20)
project(LAB3)

set(CMAKE_CXX_STANDARD 14)

add_executable(LAB3 main.cpp BinTree.hpp BST.hpp List.hpp Queue.hpp Stack.hpp utils.hpp AVL.hpp RedBlack.hpp Splay.hpp)
#add_executable(LAB3 datagen.cpp)
#add_executable(LAB3 testmain.cpp)