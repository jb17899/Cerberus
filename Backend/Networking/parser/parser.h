#pragma once
#include <iostream>
#include <vector>
#include <cstring>
#include <string>
#include <functional>
#include "../SortedSet/SoSet.h"
#include "../CommandImp/CommAbs.h"
#include <fstream>
#include <time.h>
#include "init/init.h"
#define MAX_VAL 1000000
enum
{
    create = 0,
    put = 1,
    put_chained = 2,
    get = 3,
    get_chained = 4,
    del = 5,
    del_chained = 6,
    zadd=7,
    zscore=8,
    zrem=9,
    zquery=10,
    zscan=11,
    zrangebyscore= 12,
    zrev = 13,
    success=14,
    failure=15
};
class parser
{
private:
    bool isSortedSetCommand = false;
    bool toLog = true;
    bool chainedQuery = false;
    char user_db[256];
    char pass_db[256];
    int command;
    std::string inVal = "";
    std::string dbName = "";
    std::string keyName = "";
    std::string value = "";
    int score;
    int offset = 0;
    std::string prefix = "";
    std::vector<std::string> parsedVal;
    long long total_input = 0;
    init* initial = nullptr;
    Error errorHandler;
public:
    std::string output = "";
    int success = 10;
    ~parser();
    std::string parse_into_fragments(const char *input);
    void get_command(char *command);
    void get_db_name(char *db_name);
    void set_key(char *value);

    void classify();
    void get_output();
    void get_prefix();
    void create_db(HMap *init);
    void insert_into_db(Zset* db);
    void get_from_db(Zset* db);
    void del_from_db(Zset* val);
    void zset_query_from_db(Zset* db);
    void zset_query_by_score(Zset* db);
    void zset_rank(Zset* db);
};
// template<class T>
// class EntryParser{
//     public:
//     std::string key="";
//     int len =0;
//     long long score=0;
//     T value;
// };


void parser::insert_into_db(Zset* db){
    if(parser::total_input > MAX_VAL){
        if(!isSortedSetCommand){
            score = total_input;
            if(!chainedQuery){
                std::string total="";
                total+='[';
                for(auto x:parsedVal){
                    total+=x;
                    total+=" , ";
                }
                total+=']';

                zset_insert(db,keyName.c_str(),keyName.size(),score);
            }
            else{
                std::string total = "";
                for(auto x:parsedVal){
                    zset_insert(db,keyName.c_str(),);

                }
            }

        }
        




    }

}