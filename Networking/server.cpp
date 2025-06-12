#include "server.h"
#define MAX_VAL 4096
bool nvalue = true;
bool cvalue = true;
std::string queryDB_for_main;
struct timeVal get_time(){
    struct timeVal t;
    time_t now = time(NULL);
    struct tm *tm = localtime(&now);
    t.year = tm->tm_year + 1900;
    t.month = tm->tm_mon + 1;
    t.day = tm->tm_mday;
    t.hour = tm->tm_hour;
    t.minute = tm->tm_min;
    t.sec = tm->tm_sec;
    return t;
}
// static void heap_upsert(std::vector<HeapItem> &a, size_t pos, HeapItem item) {
//     if(pos<a.size()){
//         a[pos] = item;
//     }
//     else{
//         pos = a.size();
//         *item.entry = pos;
//         a.push_back(item);
//     }
//     heap_update(a.data(),pos,a.size());
// }
// static void entry_set_ttl(Entry* ent,u_int64_t ttl_ms){
//     if(ttl_ms<0&&ent->heap_idx!=(-1)){
//         heap_delete(g_data.heap,ent->heap_idx);
//         ent->heap_idx = -1;
//     }
//     else if(ttl_ms>0){
//         u_int64_t expire_at = getCurrentTimeMillis()+ttl_ms;
//         HeapItem item = {expire_at,&ent->heap_idx};
//         heap_upsert(g_data.heap,ent->heap_idx,item);
//     }
// }
// Read function to ensure complete data transfer

int write_to_socket(int s2,const char* text){
    int len = strlen(text);
    char wbuf[4 + MAX_VAL];
    memcpy(wbuf, &len, 4);  // little endian
    memcpy(&wbuf[4], text, len);
    if (write_full(s2, wbuf, 4 + len) < 0) {
        perror("write_full failed");
        return -1;
    }
    return 1;
}


int32_t read_full(int fd, char* buf, int n) {
    while (n > 0) {
        ssize_t readValue = read(fd, buf, n);
        if (readValue <= 0) {
            return -1;
        }
        assert((size_t)readValue <= (size_t)n);
        n -= (size_t)readValue;
        buf += readValue;
    }
    return 0;
}

// Write function to ensure full data transmission
int32_t write_full(int fd, char* buf, int n) {
    while (n > 0) {
        ssize_t writeValue = write(fd, buf, n);
        if (writeValue <= 0) {
            return -1;
        }
        assert((size_t)writeValue <= (size_t)n);
        n -= (size_t)writeValue;
        buf += writeValue;
    }
    return 0;
}

// Handle a single request from the client
int32_t one_request(int s2,char* buf) {
    printf("1\n");
    char rbuf[4 + MAX_MESSAGE_LEN];
    memset(rbuf,0,sizeof(rbuf)*sizeof(char));
    int32_t err = read_full(s2, rbuf, 4);
    if (err) {
        return err;
    }

    int32_t len = 0;
    memcpy(&len, rbuf, 4);
    printf("%d\n",len);
    if (len > MAX_MESSAGE_LEN) {
        printf("Message too long\n");
        return -1;
    }

    err = read_full(s2, &rbuf[4], len);
    if (err) {
        printf("Read error\n");
        return err;
    }

    printf("Client says: %.*s\n", len, &rbuf[4]);

    // Extract command, folder, and value
char command[256], folder[256], value[256], vals[256] ,score[256],offset[256];


char* ptr = rbuf + 4;
int len_remaining = len;

int consumed = 0;

// Always clear buffers before parsing
memset(command, 0, sizeof(command));
memset(folder, 0, sizeof(folder));
memset(value, 0, sizeof(value));

if (len_remaining > 0) {
    sscanf(ptr, "%255s%n", command, &consumed);
    ptr += consumed;
    len_remaining -= consumed;
}

while (*ptr == ' ' && len_remaining > 0) { ptr++; len_remaining--; }


if(strncmp(command,"zset",4) != 0){

    if (len_remaining > 0) {
    sscanf(ptr, "%255s%n", folder, &consumed);
    ptr += consumed;
    len_remaining -= consumed;
    }

    while (*ptr == ' ' && len_remaining > 0) { ptr++; len_remaining--; }

    if (len_remaining > 0) {
     sscanf(ptr, "%255s%n", value, &consumed);
     ptr += consumed;
     len_remaining -= consumed;
    }

    while (*ptr == ' ' && len_remaining > 0) { ptr++; len_remaining--; }
    memset(vals, 0, sizeof(vals)); 
    if (len_remaining > 0) {
       strncpy(vals, ptr, sizeof(vals) - 1);
       vals[sizeof(vals) - 1] = '\0';
    } else {
       vals[0] = '\0';
    }
    printf("Parsed: command='%s', folder='%s', value='%s', vals='%s'\n", command, folder, value, vals);
}
else{
        if (len_remaining > 0) {
    sscanf(ptr, "%255s%n", folder, &consumed);
    ptr += consumed;
    len_remaining -= consumed;
    }

    while (*ptr == ' ' && len_remaining > 0) { ptr++; len_remaining--; }

    if (len_remaining > 0) {
     sscanf(ptr, "%255s%n", value, &consumed);
     ptr += consumed;
     len_remaining -= consumed;
    }
    while (*ptr == ' ' && len_remaining > 0) { ptr++; len_remaining--; }
    memset(score,0,sizeof(score));

    if (len_remaining > 0) {
     sscanf(ptr, "%255s%n", score, &consumed);
     ptr += consumed;
     len_remaining -= consumed;
    }
    while (*ptr == ' ' && len_remaining > 0) { ptr++; len_remaining--; }
    memset(offset, 0, sizeof(vals)); 
    if (len_remaining > 0) {
       strncpy(offset, ptr, sizeof(offset) - 1);
       offset[sizeof(offset) - 1] = '\0';
    } else {
       offset[0] = '\0';
    }
    printf("Parsed: command='%s', folder='%s', value='%s', score='%s', offset='%s'\n", command, folder, value, score, offset);


    
}

    struct timeVal t = get_time();
FILE* fp = fopen("./Logging/log.txt","a+");
if (fp == NULL) {
    int result = system("mkdir Logging");
    result = system("touch Logging/log.txt");
    fp = fopen("./Logging/log.txt", "a+");
}
    
    if(fp == NULL){
        perror("Error opening file");
        return -1;
    }
    std::string queryDB = folder + std::to_string(s2)+std::string(buf);

    if(strncmp(command,"get",3) == 0){
        if(!maps.count(queryDB)){
            write_to_socket(s2,"db does not exist");
            perror("empty db");
        }
        HMap db = maps[queryDB];
        std::string ans;
        get(&db,value,ans);
        fprintf(fp,"get command at time:%d-%d-%d %d:%d:%d\n",t.year,t.month,t.day,t.hour,t.minute,t.sec);
        write_to_socket(s2,ans.c_str());
        
    }
    else if(strncmp(command,"del",3) == 0){
        if(!maps.count(queryDB)){
            write_to_socket(s2,"empty database!fill it up first");
            perror("empty db");
        }
        HMap &db = maps[queryDB];
        std::string ans;
        if(!deletesm(&db,value)){
            ans = "val deleted";
        }
        else{
            ans = "val not found!error";
        }
        fprintf(fp,"del command at time:%d-%d-%d %d:%d:%d\n",t.year,t.month,t.day,t.hour,t.minute,t.sec);
        write_to_socket(s2,ans.c_str());
    }
    else if(strncmp(command,"new",3) == 0){
        if(maps.count(queryDB)){
            write_to_socket(s2,"database exists already!");
            perror("db exists");
        }
        HMap val;
        maps[queryDB] = val;
        const char* ans = "built successfully";
        fprintf(fp,"new command at time:%d-%d-%d %d:%d:%d\n",t.year,t.month,t.day,t.hour,t.minute,t.sec);
        write_to_socket(s2,ans);
    }
    else if(strncmp(command,"put",3)==0){
        if(!maps.count(queryDB)){
            write_to_socket(s2,"database does not exist already!Made a");
            HMap val;
            maps[queryDB] = val;
        }
        HMap& db = maps[queryDB];
        insert(&db,value,vals);
        fprintf(fp,"put command at time:%d-%d-%d %d:%d:%d\n",t.year,t.month,t.day,t.hour,t.minute,t.sec);
        const char* val = "inserted Successfully";
        write_to_socket(s2,val);
}
else if(strncmp(command,"zsetPut",7) == 0){
    if(!mapZset.count(queryDB)){
    write_to_socket(s2,"DB does not exists...\nmaking a new one\n");
        Zset val;
        Avl_init(val.root);
        mapZset[queryDB] = val;
    }

    Zset &val = mapZset[queryDB];
    int len = strlen(value);
    double x = atof(score);
    zset_insert(&val,value,len,x);
    write_to_socket(s2,"[1]");
}
else if(strncmp(command,"zsetCreate",10)==0){
    if(mapZset.count(queryDB)){
        write_to_socket(s2,"DB already exists...");
        perror("already exists");
    }
    Zset val;
    val.root = (AVLNode*)malloc(sizeof(AVLNode));
    Avl_init(val.root);
    mapZset[queryDB] = val;
    write_to_socket(s2,"[1]");
}
else if(strncmp(command,"zsetQuery",9) == 0){
        if(!mapZset.count(queryDB)){
          write_to_socket(s2,"DB does not exists...\nmake a new one,then run commands\n");
          perror("does not exist");
        }
        Zset val = mapZset[queryDB];
        int len = strlen(value);
    double x = atof(score);
    double y = atof(offset);
        string ans = do_zquery(&val,(string)value,x,y,10);
        cout<<ans<<endl;
        write_to_socket(s2,ans.c_str());
}
else if(strncmp(command,"zsetDel",7) == 0){
    if(!mapZset.count(queryDB)){
          write_to_socket(s2,"DB does not exists...\nmake a new one,then run commands\n");
          perror("does not exist");
    }
    Zset val = mapZset[queryDB];
    int len = strlen(value);
    ZNode* valm = zset_lookup(&val,value,len);
    zset_delete(&val,valm);
    write_to_socket(s2,"[1]");
}
else{
    write_to_socket(s2,"unknown query!!!!!!!!TRY AGAIN properly you imbecile...");
}
fclose(fp);
    return 0;
}

// // Main loop to handle multiple client requests
// void mainloop(int s) {   
//         // Handle client requests
//         int32_t err = one_request(s);
//             if (err) {
//                 perror("error in program.............\n");
//                 exit(0);
//             }
//         // Close client connection
//         close(s);
//     }

// Initialize server and start listening
int initserver(int16_t port) {
    int sockid;
    struct sockaddr_in sock;

    sock.sin_family = AF_INET;
    sock.sin_port = htons(port);
    sock.sin_addr.s_addr = inet_addr(HOST);

    sockid = socket(AF_INET, SOCK_STREAM, 0);
    assert(sockid > 0);

    if (bind(sockid, (struct sockaddr*)&sock, sizeof(sock)) != 0) {
        perror("Bind failed");
        exit(EXIT_FAILURE);
    }

    listen(sockid, 20);
    printf("Server listening on %s:%d\n", HOST, port);
    fflush(stdout);

    return sockid;
}
int loads(char* buf) {
    char* sport = PORT;
    int16_t port;

    port = (int16_t)atoi(sport);

    int s = initserver(port);
    struct pollfd fds[200];
    fds[0].fd = s;
    fds[0].events = POLLIN;
    int nfds = 1;
    int timeout = 3 * 60 * 1000; // 3 minutes timeout

    while (true) {
        printf("Waiting on poll()...\n");
        fflush(stdout);
        
        int rc = poll(fds, nfds, timeout);
        if (rc < 0) {
            perror("poll failed");
            break;
        }
        if (rc == 0) {
            printf("poll timed out. Ending program.\n");
            break;
        }

        for (int i = 0; i < nfds; i++) {
            if (fds[i].revents == 0) {
                continue;
            }
            
            if (!(fds[i].revents & POLLIN)) {
                printf("Error on socket %d\n", fds[i].fd);
                close(fds[i].fd);
                fds[i] = fds[nfds-1];
                nfds--;
                i--;
                continue;
            }

            if (fds[i].fd == s) {
                // New connection
                printf("Listening socket is readable\n");
                int s2 = accept(s, NULL, NULL);
                if (s2 < 0) {
                    perror("Accept failed");
                    continue;
                }

                printf("New incoming connection - %d\n", s2);
                if (nfds >= 200) {
                    printf("Too many connections\n");
                    close(s2);
                    continue;
                }

                fds[nfds].fd = s2;
                fds[nfds].events = POLLIN;
                nfds++;
            } else {
                // Existing connection
                int32_t err = one_request(fds[i].fd,buf);
                if (err) {
                    printf("Closing connection %d due to error\n", fds[i].fd);

                    close(fds[i].fd);
                    fds[i] = fds[nfds-1];
                    nfds--;
                    i--;
                }
            }
        }
    }

    // Clean up all connections
    for (int i = 0; i < nfds; i++) {
        close(fds[i].fd);
    }

    printf("Server shutting down\n");
    return 0;
}
