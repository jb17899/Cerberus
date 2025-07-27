#include "CommAbs.h"
#include<unordered_map>
using namespace std;

void get(HMap* db,std::string val,std::string& ans,bool isString){
    Entrys key;
    key.key.swap(val);
    key.node.hash = hashFun((string)key.key.data(),key.key.size());
    HNode *node = hm_lookup(db,&key.node,&entry_eq);
    if(!node){
        perror("not found\n");
        return;
    }

    Entrys *nodes = container_of(node, Entrys, node);
    std::string value = nodes->value;
    ans.assign(value.begin(),value.end());
    
}

void insert(HMap* db,std::string key,std::string val){
    Entrys* entry;
entry->key.swap(key);      
entry->value.swap(val);
entry->node.hash = hashFun(entry->key.data(), entry->key.size());
HNode* vals = hm_lookup(db,&entry->node,entry_eq);
if(vals){
    cout<<"exists"<<endl;
    container_of(vals,Entrys,node)->value.swap(val);
    return;
}
hm_insert(db, &entry->node);
}
bool deletesm(HMap* db,std::string key){
    Entrys* entry = new Entrys();
    entry->key.swap(key);
    entry->node.hash = hashFun(entry->key.data(),entry->key.size());
    hm_delete(db,&entry->node,entry_eq);
    return entry == NULL? 1 : 0;
}