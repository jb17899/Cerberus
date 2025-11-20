#include "Cache.h"
using namespace std;

//basis on which the heap is built
// static void heap_up(HeapItem* heap, u_int64_t i) {
//     HeapItem tmp = cache[i];
//     while(i>0&&cache[get_parent(i)].val < tmp.val){
//         cache[i] = cache[get_parent(i)];
//         i = get_parent(i);
//     }
//     cache[i] = tmp;
// }
// static void heap_down(HeapItem* heap,u_int64_t i,u_int64_t len){
//     HeapItem tmp =  cache[i];
//     while(true){
//         int left = get_child_left(i);
//         int right = get_child_right(i);
//         int min_val = tmp.val;
//         int min_index = i;
//         if(left<len && cache[left].val<min_val){
//             min_index = left;
//             min_val = cache[left].val;
//         }
//         if(right<len && cache[right].val<min_val){
//             min_index = right;
//             min_val = cache[right].val;
//         }
//         if(min_index == i) break;
//         cache[i] = cache[min_index];
//         i = min_index;
//     }
//     cache[i] = tmp;
// }

int Cache::get_child_left(u_int64_t i) {
    return (i << 1) + 1;
};
int Cache::get_child_right(u_int64_t i) {
    return (i << 1) + 2;
};
int Cache::get_parent(u_int64_t i) {
    return ((i +1)>>2)-1;
};
void Cache::heap_up(u_int64_t i){
    HeapItem val = cache[i];
    while(i>0&&cache[get_parent(i)].val>val.val){
        cache[i] = cache[get_parent(i)];
        *cache[i].entry = get_parent(i);
        i = get_parent(i);
    }
    *val.entry = i;
    cache[i] = val;
}
void Cache::heap_down(u_int64_t i,u_int64_t len){
    HeapItem temp = cache[i];
    while(true){
        int l = get_child_left(i);
        int r = get_child_right(i);
        int min_val = cache[i].val;
        int min_pos = i;
        if(l<len&&cache[l].val<min_val){
            min_val = cache[l].val;
            min_pos = l;
        }
        if(r<len&&cache[r].val<min_val){
            min_val = cache[r].val;
            min_pos = r;
        }
        if(min_pos == i){
            break;
        }
        cache[i] = cache[min_pos];
        *cache[i].entry = min_pos;
        i = min_pos;
    }
    cache[i] = temp;
    *temp.entry = i;
}
void Cache::heap_update(u_int64_t i, u_int64_t len) {
    if(i>0 &&cache[get_parent(i)].val > cache[i].val) {
        heap_up(i);
    } else {
        heap_down(i, len);
    }
}
void Cache::heap_delete(size_t pos) {
    cache[pos] = cache.back();
    cache.pop_back();
    if (pos < cache.size()) {
        heap_update(pos, cache.size());
    }
}
void Cache::add_item_to_cache(u_int64_t value){
    
}
