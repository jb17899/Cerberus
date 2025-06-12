#pragma once
#include<iostream>
#include<vector>
#include<assert.h>
#include<math.h>

#include "../HashTableImp/Hash.h"
#define container_of(ptr, T, member) \
    ((T *)( (char *)ptr - offsetof(T, member) ))

class Node{
public:
    Node* left;
    Node* right;
    int key;
    Node(int k) : key(0),left(nullptr), right(nullptr) {}
};
template<class T>
class Data{
    public:
    Node node;
    T key;
};

class AVLNode{
    public:
    AVLNode *parent = NULL;
    AVLNode* left = nullptr;
    AVLNode* right = NULL;
    u_int32_t height =0;
    u_int32_t cnt = 0;
};
class ZNode{
    public:
    HNode hmap;
    AVLNode root;

    double score = 0;
    size_t len = 0;
    char name[0];
};
inline void Avl_init(AVLNode* node){
    if (!node) {
        std::cerr << "Error: Attempted to initialize a null AVLNode pointer." << std::endl;
        return;
    }
    node->left =NULL; node->right =nullptr; node->parent = NULL;

    node->height = 1;
    node->cnt = 1;
}
inline int get_heights(AVLNode* node){
    if(node == NULL){
        return 0;
    }
    return node->height;
}
inline u_int32_t get_counts(AVLNode* node){
    if(node == NULL)
        return 0;
    return node->cnt;
}
inline int ret_balance(AVLNode* node){
    if(node == NULL){
        return 0;
    }
    return get_heights(node->left) - get_heights(node->right);
}
inline void avl_update(AVLNode *node) {
    node->height = 1 + std::max(get_heights(node->left), get_heights(node->right));
    node->cnt = 1+get_counts(node->left)+get_counts(node->right);
}
AVLNode* avl_fix(AVLNode* node);
AVLNode* node_detachs(AVLNode* node);
