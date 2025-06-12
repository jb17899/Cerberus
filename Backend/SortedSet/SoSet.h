#pragma once
#include<iostream>
#include<vector>
#include<assert.h>
#include<math.h>
#include<string.h>
#include <cstddef> 
#include <cstdint>
#include <cstring>
#include "Tree.h"
using namespace std;

static bool min(int a,int b){
    return a<b;
}
class Zset{
    public:
    HMap hmap;
    AVLNode* root = NULL;
};
static void zset_clear(Zset* zset){
    delete(zset);
}
enum {
    T_INIT  = 0,
    T_STR   = 1,    // string
    T_ZSET  = 2,    // sorted set
    T_INT = 3
};
// class Entry{
//     public:
//     HNode node;
//     std::string key;
//     u_int32_t type =0;
//     virtual ~Entry(){
//     }
// };
// class strEntry:Entry{
//     public:
//     std::string str;
// };
// class SsetEntry:Entry{
//     public:
//     Zset zset;
//     virtual ~SsetEntry(){
//         zset_clear(&zset);
//     }
// };
class Hkey{
    public:
    HNode node;
    const char* name = NULL;
    size_t len = 0;
};

static bool hcmp(HNode* node,HNode* key){
    ZNode* val = container_of(node,ZNode,hmap);
    Hkey* val1 = container_of(key,Hkey,node);
    if(val->len!=val1->len){
        return false;
    }
    return !strncmp(val1->name,val->name,min(val1->len,val->len));
}

static bool zless(AVLNode* lhs,AVLNode* rhs){
    ZNode* hl = container_of(lhs,ZNode,root);
    ZNode* rl = container_of(rhs,ZNode,root);

    if(hl->score!=rl->score){
        return hl->score<rl->score;
    }
    int rs = strncmp(hl->name,rl->name,min(hl->len,rl->len));
    return rs == 0?hl->len<rl->len:(rs<0);
}
static ZNode *znode_new(const char *name, size_t len, double score) {
    ZNode *node = (ZNode *)malloc(sizeof(ZNode) + len + 1); // +1 for null terminator
    Avl_init(&node->root); // optional back-link if needed
    node->score = score;
    node->len = len;
    memcpy(node->name, name, len);
    node->name[len] = '\0';

    node->hmap.hash = hashFun(std::string(name), len);
    node->hmap.next = NULL;

    return node;
}

ZNode* zset_lookup(Zset*zset,const char* name,size_t len);
static void tree_insert(Zset* zset,ZNode* znode);
bool zset_insert(Zset *zset, const char *name, size_t len, double score);
void zset_delete(Zset *zset, ZNode *node);
ZNode *zset_seekge(Zset *zset, double score, const char *name, size_t len);
ZNode *znode_offset(ZNode *node, int64_t offset);
static AVLNode* avl_offset(AVLNode* node,u_int64_t offset);
static void zset_update(Zset* zset,ZNode* node,double score);
string do_zquery(Zset* zset,string name,double score,int offset,int limit);
static bool zless(AVLNode* node,const char* name,size_t len,double score){
    ZNode* val = container_of(node,ZNode,root);
    if(val->score!=score){
        return (val->score<score);
    }
    int rv = strncmp(val->name,name,min(len,val->len));
    return rv==0?val->len<len:rv<0;
}

static bool sorted_entry_eq(HNode* lhs,HNode* rhs){
    ZNode * le = container_of(lhs,ZNode,hmap);
    ZNode *re =container_of(rhs,ZNode,hmap);
    return le->name == re->name&&le->score == re->score&&le->len == re->len;
}