#include<iostream>
#include "../HashTableImp/Hash.h"

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

static int get_child_left(u_int64_t i) ;
static int get_child_right(u_int64_t i);
static int get_parent(u_int64_t i);
static void heap_up(HeapItem* heap,u_int64_t i);
static void heap_delete(std::vector<HeapItem> &a, size_t pos);
void heap_update(HeapItem* heap, u_int64_t i, u_int64_t len);
static void heap_down(HeapItem* heap,u_int64_t i,u_int64_t len);