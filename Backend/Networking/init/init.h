#pragma once
#include <iostream>
#include <vector>
#include <cstring>
#include <string>
#include <functional>
#include "../CommandImp/CommAbs.h"
#include <fstream>
#include <time.h>

#define MAX_VAL 1000000

class Error{
    private:
    int type = 0;
    LOG* logHandler = nullptr;
    public:
    Error(){
        logHandler = new LOG();
    }
    void set_type(int type){
        this->type = type;
    }
    void write_warning(const char* value){
        if(type == 1){
            logHandler->set_file_name("Error.txt");
            logHandler->write_to_file(value);
            throw "System error! Please restart again";
        }
        else{
            logHandler->set_file_name("syslogs.txt");
            logHandler->write_to_file(value);
            std::cout<<value;

        }
    }
};
class LOG{
    private:
    std::string fileName = "syslogs.txt";
    std::fstream MyFile;
    int last_checked;
    int max_limit = 10000;
    Error errorHandler;
    public:
    void set_file_name(const char* fileName){
        this->fileName = fileName;
    }
    void write_to_file(const char* value);
    void read_from_file();
};
void LOG::write_to_file(const char* value){
    MyFile.open(fileName, std::ios::app);
    if(!MyFile.is_open()){
        errorHandler.write_warning("Error opening log file. Please check permissions or file path.");
        return;
    }
    MyFile<<value<<"\n";
    MyFile.close();
}
void LOG::read_from_file(){
    MyFile.open(fileName, std::ios::in);
    if(!MyFile.is_open()){
        errorHandler.write_warning("Error opening log file for reading. Please check permissions or file path.");
        return;
    }
    std::string line;
    while(std::getline(MyFile, line)){
        if(line.substr(0, 4) == "Error"){
            std::cout << line << std::endl;
        }
        else{
            std::cout << line << std::endl;
        }
    }
    MyFile.close();
}
class Entry_Init:public Entry<HMap*>{
    public:
    Entry_Init(const char* key){
        this->key  = key;
        this->node.hash = hashFun(key, strlen(key));
    }
    Entry_Init(const char* key, HMap* value){
        this->key = key;
        this->value = value;
        this->node.hash = hashFun(key, strlen(key));
    }
    ~Entry_Init(){
        delete value;
    }
};
class Hmap_init{
    private:
    HMap* hmap;
    Error errorHandler;
    public:
    Hmap_init(){
        hmap = new HMap();
        hm_init(hmap);
    }
    ~Hmap_init(){
        delete hmap;
    }
    HMap* get_hmap(){
        return hmap;
    }
    void insert_hmap_init(){
        if (hmap == NULL) {
            errorHandler.set_type(1);
            errorHandler.write_warning("HMap is not initialized.");
            return;
        }
        Entry_Init* entry = new Entry_Init("init",hmap);
        if(get_hmap_init(hmap) != NULL){
            errorHandler.write_warning("Init entry already exists in the database.Not modifying or Adding it again.");
            return;
        }
        hm_insert(hmap, &entry->node);
    }
    HMap* get_hmap_init(HMap* db){
        Entry_Init* entry = new Entry_Init("init");
        HNode* node = hm_lookup(db, &entry->node, entry_eq);
        if (!node) {
            errorHandler.set_type(1);
            errorHandler.write_warning("Init entry not found in the database.");
            return NULL;
        }
        return (HMap*)node;
    }

};
class Server{
    private:
    int sockId =0;
};
class init{
    private:
    long long time =0;
    HMap* initial;
    LOG* handler;
    Hmap_init* only_init;
    std::string prefix = "";
    std::vector<Server*>vecs;
    clock_t start_time;
    clock_t present_time;
    Error errorHandler;
    public:
    init(){
        only_init = new Hmap_init();
        initial = new HMap();
        LOG logs;
        handler = &logs;
        start_timer();
        present_time = clock();
        handler->write_to_file(("Started the init table.All running correctly.Hashmap assigned a memory block in area.\nHave Started Logging" + std::to_string(present_time) + "\n").c_str());
    }
    void start_timer(){
        start_time = clock();
        start_log(start_time);    
    }
    void create_init(const char* user, const char* password);
    void start_log(clock_t start_time);
    void create_prefix(const char* user,const char* password);
    std::string get_prefix(){
        return prefix;
    }
    void start_server();
};
void init::start_log(clock_t start_time){
    handler->write_to_file(("Started logging at: " + std::to_string(start_time) + "\n").c_str());
}
void init::start_server(){
    init_server();
}   
void init::create_prefix(const char* user,const char* password){
    for(size_t c=0;user[c]!='/0';c++){
        init::prefix.push_back(user[c]);
    }
    for(size_t c=0;password[c]!='/0';c++){
        init::prefix.push_back(password[c]);
    }
}
void init::create_init(const char* user, const char* password){
    Hmap_init* hmap = only_init;
    create_prefix(user, password);
    handler->write_to_file("Init table created with user and password.");
    handler->write_to_file(("Created prefix: " + get_prefix()).c_str());
    hmap->insert_hmap_init();
    initial = hmap->get_hmap_init(hmap->get_hmap());
    insert(initial,std::string(user),prefix);
    start_server();
}