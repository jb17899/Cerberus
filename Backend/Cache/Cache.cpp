#include "Cache.h"
using namespace std;

//basis on which the heap is built
// static void heap_up(HeapItem* heap, u_int64_t i) {
//     HeapItem tmp = heap[i];
//     while(i>0&&heap[get_parent(i)].val < tmp.val){
//         heap[i] = heap[get_parent(i)];
//         i = get_parent(i);
//     }
//     heap[i] = tmp;
// }
// static void heap_down(HeapItem* heap,u_int64_t i,u_int64_t len){
//     HeapItem tmp =  heap[i];
//     while(true){
//         int left = get_child_left(i);
//         int right = get_child_right(i);
//         int min_val = tmp.val;
//         int min_index = i;
//         if(left<len && heap[left].val<min_val){
//             min_index = left;
//             min_val = heap[left].val;
//         }
//         if(right<len && heap[right].val<min_val){
//             min_index = right;
//             min_val = heap[right].val;
//         }
//         if(min_index == i) break;
//         heap[i] = heap[min_index];
//         i = min_index;
//     }
//     heap[i] = tmp;
// }

static int get_child_left(u_int64_t i) {
    return (i << 1) + 1;
};
static int get_child_right(u_int64_t i) {
    return (i << 1) + 2;
};
static int get_parent(u_int64_t i) {
    return ((i +1)>>2)-1;
};
static void heap_up(HeapItem* heap,u_int64_t i){
    HeapItem val = heap[i];
    while(i>0&&heap[get_parent(i)].val>val.val){
        heap[i] = heap[get_parent(i)];
        *heap[i].entry = get_parent(i);
        i = get_parent(i);
    }
    *val.entry = i;
    heap[i] = val;
}
static void heap_down(HeapItem* heap,u_int64_t i,u_int64_t len){
    HeapItem temp = heap[i];
    while(true){
        int l = get_child_left(i);
        int r = get_child_right(i);
        int min_val = heap[i].val;
        int min_pos = i;
        if(l<len&&heap[l].val<min_val){
            min_val = heap[l].val;
            min_pos = l;
        }
        if(r<len&&heap[r].val<min_val){
            min_val = heap[r].val;
            min_pos = r;
        }
        if(min_pos == i){
            break;
        }
        heap[i] = heap[min_pos];
        *heap[i].entry = min_pos;
        i = min_pos;
    }
    heap[i] = temp;
    *temp.entry = i;
}
void heap_update(HeapItem* heap, u_int64_t i, u_int64_t len) {
    if(i>0 &&heap[get_parent(i)].val > heap[i].val) {
        heap_up(heap, i);
    } else {
        heap_down(heap, i, len);
    }
}
static void heap_delete(std::vector<HeapItem> &a, size_t pos) {
    a[pos] = a.back();
    a.pop_back();
    if (pos < a.size()) {
        heap_update(a.data(), pos, a.size());
    }
}
