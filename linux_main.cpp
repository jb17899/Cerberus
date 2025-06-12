#include<unistd.h>
#include "./Networking/server.h"
int main(){
    char buf[25];
    int name_val = getlogin_r(buf, 24);
    assert(buf);
    int val = loads(buf);
    cout<<"exiting out"<<endl;
    return 0;
}
