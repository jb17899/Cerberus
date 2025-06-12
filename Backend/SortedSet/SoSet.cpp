#pragma once
#include "SoSet.h"
using namespace std;
ZNode* val;
ZNode* zset_lookup(Zset*zset,const char* name,size_t len){
    if(!zset->root){
        return NULL;
    }
    Hkey key;
    key.node.hash = hashFun((std::string)name,len);
    key.name = name;
    key.len = len;
    HNode* found = hm_lookup(&zset->hmap,&key.node,&hcmp);
    return found == NULL?NULL:container_of(found,ZNode,hmap);
}
static void tree_insert(Zset* zset,ZNode* znode){
    AVLNode* parent = NULL;
    AVLNode** from = &zset->root;
    while(*from){
        parent = *from;
        from = zless(&znode->root,parent)?&parent->left:&parent->right;
    }
    *from = &znode->root;
    (znode)->root.parent = parent;
    zset->root = avl_fix(& znode->root);
    ZNode* val,*val1 = NULL,*val2 = NULL;   
}
bool zset_insert(Zset *zset, const char *name, size_t len, double score){
    if(ZNode* node = zset_lookup(zset,name,len)){
         zset_update(zset, node, score);
        return false;
    }

    ZNode* node = znode_new(name,len,score);

    hm_insert(&zset->hmap,&node->hmap);
    val = node;
    tree_insert(zset,node);
    return true;
}
void zset_update(Zset* zset,ZNode* node,double score){
    zset->root = node_detachs(&node->root);
    Avl_init(&(node)->root);
    node->score = score;
    tree_insert(zset,node);
}
void zset_delete(Zset *zset, ZNode *node) {
    Hkey key;
    key.len = node->len;
    key.name = node->name;
    key.node.hash = node->hmap.hash;
    HNode* found = hm_delete(&zset->hmap,&key.node,&hcmp);
    assert(found);
    zset->root = node_detachs(&node->root);
    free(node);
}
ZNode *zset_seekge(Zset *zset, double score, const char *name, size_t len){
    AVLNode* found = NULL;
    for(AVLNode* node = zset->root;node;){
        if(zless(node,name,len,score)){
            node = node->right;
        }
        else{
            found = node;
            node = node->left;
        }
    }
    return found?container_of(found,ZNode,root):NULL;
}
ZNode *znode_offset(ZNode *node, int64_t offset){
    AVLNode* off = node?avl_offset(&node->root,offset):NULL;
    return off?container_of(off,ZNode,root):NULL;
}
string do_zquery(Zset* zset,string name,double score,int offset,int limit){
    ZNode* val = zset_seekge(zset,score,name.data(),name.size());
    val = znode_offset(val,offset);
    int n = 0;
    string s = "";
    while(val&&n<limit){
        s+=to_string(n)+"#*^";
        s+=val->name;
        s+=" ";
        s+=" st &*";
        s+=to_string(val->score);
        val = znode_offset(val, +1);
        n += 1;
    }

    return s;
}
AVLNode* avl_offset(AVLNode* node,u_int64_t offset){
    int val = 0;
    while(val!=offset){
        if(val<offset&&val+get_counts(node->right)>=offset){
            node = node->right;
            val+=get_counts(node->left)+1;
        }
        else if(val>offset&&val-get_counts(node->left)<offset){
            node = node->left;
            val-=get_counts(node->right)+1;
        }else {
            // go to the parent
            AVLNode *parent = node->parent;
            if (!parent) {
                return NULL;
            }
            if (parent->right == node) {
                val -= get_counts(node->left) + 1;
            } else {
                val += get_counts(node->right) + 1;
            }
            node = parent;
        }
    }
    return node;
}