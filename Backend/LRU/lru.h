#pragma once
#include<iostream>
#include<string>
#include "../../Networking/CommandImp/CommAbs.h"
#include<unordered_map>

class Entry{
    public:
    std::string key;
    std::string val;
    Entry(std::string& key,std::string& val){
        this->key = key;
        this->val = val;
    }
};

class Node{
    public:
    Node* left;
    Node* right;
    Node(){
        left = NULL;
        right = NULL;
    }
};
class Doubly_Linked_List{
    public:
    Node* head;
    Node* tail;
    u_int16_t size;
    void remove(Node* node){
        Node* left = node->left;
        Node* right = node->right;
        left->right = right;
        right->left = left;
        delete node;
    }
    void insert(Node* node){
        Node* left = tail->left;
        Node* right = tail;
        left->right = node;
        right->left = node;
        node->left =left;
        node->right = right;
    }
    Doubly_Linked_List(){
        head = new Node();
        tail = new Node();
        head->right = tail;
        tail->left = head;
        size = 2;
    }
};
class LRU_CACHE{
    private:
    Doubly_Linked_List* list;
    unordered_map<std::string,Node*> map;
    public:
    LRU_CACHE(){
        list = new Doubly_Linked_List();
    }
    void put(std::string& key,std::string& val){
        if(map.find(key)!=map.end()){
            Node* node = map[key];
            list->remove(node);
            list->insert(node);
        }else{
            Node* node = new Node();
            list->insert(node);
            map[key] = node;        
        }
    }


};