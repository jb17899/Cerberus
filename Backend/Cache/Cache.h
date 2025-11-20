#include<iostream>
#include "../HashTableImp/Hash.h"
#define MAX_SIZE 1000

// class HeapItem{
//     public:
//     u_int64_t val;
// };

class HeapItem {
    public:
    u_int64_t val;
    size_t* entry;                               //using intrusive dsa
};
class Entry{
    public:
    HNode* hnode;
    std::string key;
    size_t heap_idx=-1;
};
class Cache{
    private:
    std::vector<HeapItem> cache;
    size_t size_cache;
    public:
    Cache(){
    size_cache = 0;    
    cache.resize(MAX_SIZE);
    }
    int get_child_left(u_int64_t i);
    int get_child_right(u_int64_t i);
    int get_parent(u_int64_t i);
    void heap_up(u_int64_t i);
    void heap_down(u_int64_t i,u_int64_t len);
    void heap_update(u_int64_t i, u_int64_t len);
    void heap_delete(size_t pos);
    void add_item_to_cache(u_int64_t value);
};
