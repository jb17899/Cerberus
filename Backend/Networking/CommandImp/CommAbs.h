#pragma once
#include<iostream>
#include<string>
#include<vector>
#include<string>
#include <cstddef> 
#include <cstdint>
#include <cstring>
#include "../../SortedSet/SoSet.h"
#define container_of(ptr, T, member) \
    ((T *)( (char *)ptr - offsetof(T, member) ))
#define Entrys Entry<std::string>
class commAbs{
    public:
    HMap db;
};
template<class T>
class Entry{
   public:
   std::string key;
   T value;
   HNode node; 
};
enum {
    TAG_NIL = 0,    // nil
    TAG_ERR = 1,    // error code + msg
    TAG_STR = 2,    // string
    TAG_INT = 3,    // int64
    TAG_DBL = 4,    // double
    TAG_ARR = 5,    // array
};



static bool entry_eq(HNode *lhs, HNode *rhs) {
    Entrys *le = container_of(lhs, Entrys, node);
    Entrys *re = container_of(rhs, Entrys, node);
    return le->key == re->key;
}

void get(HMap* db,std::string val,std::string& ans);
void insert(HMap* db,std::string key,std::string val);
bool deletesm(HMap* db,std::string key);
