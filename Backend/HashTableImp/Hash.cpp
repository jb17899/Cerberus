#include "Hash.h"
using namespace std;



// uint64_t fnv1a(const std::string& s) {
//     const uint64_t offset = 0xcbf29ce484222325ULL;
//     const uint64_t prime  = 0x100000001b3ULL;

//     uint64_t hash = offset;
//     for (unsigned char c : s) {
//         hash ^= c;
//         hash *= prime;
//     }
//     return hash;
// }

u_int64_t hashFun(std::string keyToBeCoded,int maxSize){
    u_int64_t h = 0x811C9DC5;
    for(int c=0;c<maxSize;c++){
        h = (h + keyToBeCoded[c]) * 0x01000193;
    }
    return h;
}



void h_init(HTable * htab,size_t n){
assert(n>0&&(((n-1)&n) == 0));
htab->val = (HNode**)calloc(n,sizeof(HNode*));
htab->size = 0;
htab->mask = n-1; 
}
static void h_insert(HTable *htab,HNode* node){
    size_t pos = (node->hash)&(htab->mask);
    HNode* next = htab->val[pos];
    node->next = next;
    htab->val[pos] = node;
    htab->size++;
}
static HNode** h_lookup(HTable* htab,HNode* key,bool (*eq)(HNode*,HNode*)){
    if(!htab->val){
        return NULL;
    }
    size_t pos = key->hash&htab->mask;
    HNode** from = &htab->val[pos];
    for(HNode* cur;(cur = *from)!=NULL;from = &cur->next){
        if(cur->hash==key->hash&&eq(cur,key)){
            return from;
        }
    }
    return NULL;
}

static HNode* detach(HTable* htab,HNode** from){
    HNode* node = *from;
    *from = node->next;
    htab->size--;
    return node;
}


static void hm_trigger_rehashing(HMap* hmap){
    hmap->older = hmap->newer;
    h_init(&hmap->newer,(hmap->newer.mask+1)*2);
    hmap->migrate_pos = 0;
}

HNode *hm_lookup(HMap *hmap, HNode *key, bool (*eq)(HNode *, HNode *)){
HNode** from = h_lookup(&hmap->newer,key,eq);

if(!from){
    from = h_lookup(&hmap->older,key,eq);
}
return from?*from:NULL;
}

HNode *hm_delete(HMap *hmap, HNode *key, bool (*eq)(HNode *, HNode *)){
    if(HNode** from = h_lookup(&hmap->newer,key,eq)){
        return detach(&hmap->newer,from);
    }
    if(HNode** from = h_lookup(&hmap->older,key,eq)){
        return detach(&hmap->older,from);    
    }
    return NULL;
}
void hm_insert(HMap *hmap, HNode *node){
    if(!hmap->newer.val){
        h_init(&hmap->newer,64);
    }
    h_insert(&hmap->newer,node);
    if(!hmap->older.val){
        size_t threSize = (hmap->newer.mask+1)*max_threshold;
        if(hmap->newer.size>threSize){
           hm_trigger_rehashing(hmap); 
        }
    }
    hm_help_rehashing(hmap);
}
extern void hm_help_rehashing(HMap *hmap) {
    size_t nwork = 0;
    while (nwork < transferValue && hmap->older.size > 0) {
        // find a non-empty slot
        HNode **from = NULL;
        from = &hmap->older.val[hmap->migrate_pos];
        
        if (!*from) {
            hmap->migrate_pos++;
            continue;   // empty slot
        }
        // move the first list item to the newer table
        h_insert(&hmap->newer, detach(&hmap->older, from));
        nwork++;
    }
    // discard the old table if done
    if (hmap->older.size == 0 && hmap->older.val) {
        free(hmap->older.val);
        hmap->older = HTable{};
    }
}

