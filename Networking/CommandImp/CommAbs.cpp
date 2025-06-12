#include "CommAbs.h"
#include<unordered_map>
using namespace std;

void get(HMap* db,std::string val,std::string& ans){
    Entry key;
    cout<<val<<endl;
    key.key.swap(val);
    key.node.hash = hashFun((string)key.key.data(),key.key.size());
    HNode *node = hm_lookup(db,&key.node,&entry_eq);
    if(!node){
        perror("not found\n");
        return;
    }
    Entry *nodes = container_of(node, Entry, node);
    std::string value = nodes->value;
    ans.assign(value.begin(),value.end());
}

void insert(HMap* db,std::string key,std::string val){
Entry* entry = new Entry();
entry->key.swap(key);      
entry->value.swap(val);
entry->node.hash = hashFun(entry->key.data(), entry->key.size());
HNode* vals = hm_lookup(db,&entry->node,entry_eq);
if(vals){
    cout<<"exists"<<endl;
    container_of(vals,Entry,node)->value.swap(val);
    return;
}
hm_insert(db, &entry->node);
}
bool deletesm(HMap* db,std::string key){
    Entry* entry = new Entry();
    entry->key.swap(key);
    entry->node.hash = hashFun(entry->key.data(),entry->key.size());
    hm_delete(db,&entry->node,entry_eq);
    return entry == NULL? 1 : 0;
}