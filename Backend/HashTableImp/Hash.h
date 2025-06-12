#pragma once
#include<iostream>
#include<vector>
#include<assert.h>
#include<math.h>
#include<stddef.h>
class HNode
{
public:
HNode* next = NULL;
u_int64_t hash = 0;
};

class HTable{
    public:
    HNode** val = NULL;
    size_t mask = 0;
    size_t size = 0;
};
class HMap{
    public:
    HTable older;
    HTable newer;
    size_t migrate_pos =0;
};
const int transferValue = 128;
static int starCount = 2;
static int car = 0;

static void h_init(HTable * htab,size_t n);
static void h_insert(HTable *htab,HNode* node);
static HNode** h_lookup(HTable* htab,HNode* key,bool* eq(HNode*,HNode*));
static HNode* detach(HTable* htab,HNode** from);
u_int64_t hashFun(std::string keyToBeCoded,int maxSize);
static void hm_trigger_rehashing(HMap* hmap);
void hm_help_rehashing(HMap* hmap);
void hm_init(HMap* hmap);
HNode *hm_lookup(HMap *hmap, HNode *key, bool (*eq)(HNode *, HNode *));
void   hm_insert(HMap *hmap, HNode *node);
HNode *hm_delete(HMap *hmap, HNode *key, bool (*eq)(HNode *, HNode *));
const int max_threshold = 8;

