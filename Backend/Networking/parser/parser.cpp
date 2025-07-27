#include "parser.h"

void parser::create_db(HMap* init){
    Zset newDB;
    newDB.root = (AVLNode*)malloc(sizeof(AVLNode));
    Avl_init(newDB.root);
    prefix+=dbName;
    //CODE WILL BE PORTED FROM COMMABS.
    //STORE VALUE OF NEW DB
}

parser::parser(/* args */)
{
    memset(user_db, 0, 256 * sizeof(char));
    memset(pass_db, 0, 256 * sizeof(char));
    initial = new init();
    isSortedSetCommand = false;
    toLog = true;
    chainedQuery = false;
    command = -1;
}
std::string parser::parse_into_fragments(const char *input)
{
    std::string val = "";
    std::string key = "";
    int len_remaining = sizeof(input) / sizeof(char);

    const char *ptr = input;
    int consumed = 0;
    char value[256];
    memset(value, 0, sizeof(value));

    while (*ptr == ' ' && len_remaining != 0)
    {
        ptr++;
        len_remaining--;
    }
    if (len_remaining > 0)
    {
        sscanf(ptr, "%255s%n", value, &consumed);
        len_remaining -= consumed;
        ptr += consumed;
    }
    // Now pass function and get the command.
    get_command(value);
    if (parser::success == 15)
    {
        return output;
    }
    memset(value, 0, sizeof(value));
    while (*ptr == ' ' && len_remaining != 0)
    {
        ptr++;
        len_remaining--;
    }
    if (len_remaining > 0)
    {
        sscanf(ptr, "%255s%n", value, &consumed);
        len_remaining -= consumed;
        ptr += consumed;
    }
    get_db_name(value);
    if (success == 15)
    {
        return output;
    }
    memset(value, 0, sizeof(value));
    while (*ptr == ' ' && len_remaining != 0)
    {
        ptr++;
        len_remaining--;
    }
    if (len_remaining > 0)
    {
        sscanf(ptr, "%255s%n", value, &consumed);
        len_remaining -= consumed;
        ptr += consumed;
    }
    set_key(value);
    if (success == 15)
    {
        return output;
    }
    while (len_remaining != 0)
    {
        while (*ptr == ' ')
        {
            len_remaining--;
        }
        memset(value, 0, sizeof(value));
        if (len_remaining > 0)
        {
            sscanf(ptr, "%255s%n", value, &consumed);
            len_remaining -= consumed;
            ptr += consumed;
        }
        std::string(value);
        parsedVal.push_back(value);
    }
    get_output();
}
void parser::get_prefix()
{
    prefix = initial->get_prefix();
    if(prefix == "")
    {
        success = 15;
        output = "Prefix not set! Please set the Log out and try again.\n";
        errorHandler.set_type(1);
        errorHandler.write_warning(output.c_str());
        return;
    }
}

void parser::set_key(char *value)
{
    size_t size = sizeof(value), count = 0;
    if (size == 0 || size > 256)
    {
        success = 15;
        output = "Did not give right value of key!Give the Value of key";
        return;
    }
    while (count < size)
    {
        keyName.push_back(value[count]);
        count++;
    }
}

void parser::get_db_name(char *value)
{
    if (sizeof(value) == 0)
    {
        success = 15;
        output = "Some error happened";
    }
    size_t size = sizeof(value);
    int count = 0;
    while (count < size)
    {
        dbName.push_back(value[count]);
        count++;
    }
}

void parser::get_cred(const char *user, const char *password)
{
    size_t users = sizeof(user) / sizeof(char);
    size_t pass = sizeof(password) / sizeof(char);
    int count = 0;
    while (count < min(users, pass))
    {
        user_db[count] = user[count];
        pass_db[count] = password[count];
        count++;
    }
    while (count < users)
    {
        user_db[count] = user[count];
        count++;
    }
    while (count < pass)
    {
        pass_db[count] = password[count];
        count++;
    }
    get_prefix();
}

void parser::get_command(char *commandm)
{
    int count = 0;
    std::string commands = "";
    size_t commSize = sizeof(commandm) / sizeof(commandm[0]);
    while (count < commSize)
    {
        if (commandm[count] >= 65 && commandm[count] <= 90)
        {
            commandm[count] += 32;
            commands.push_back(commandm[count]);
            continue;
        }
        commands.push_back(commandm[count]);
    }
    if (commands == "create")
    {
        command = 0;
    }
    else if (commands == "put")
    {
        command = 1;
    }
    else if (commands == "putc")
    {
        command = 2;
    }
    else if (commands == "get")
    {
        command = 3;
    }
    else if (commands == "getc")
    {
        command = 4;
    }
    else if (commands == "del")
    {
        command = 5;
    }
    else if (commands == "delc")
    {
        command = 6;
    }
    else if (commands == "zadd")
    {
        command = 7;
    }
    else if (commands == "zscore")
    {
        command = 8;
    }
    else if (commands == "zrem")
    {
        command = 9;
    }
    else if(commands == "zquery"){
        command=10;
    }
    else if(commands == "zscan"){
        command = 11;
    }
    else if(commands == "zrangebysc"){
        command = 12;
    }
    else if(commands == "zrev"){
        commands = 13;
    }
    else
    {
        success = 15;
        output = "Wrong Query!!! Take A look at Docs and Try Again.\n";
    }
    if (success == 15)
    {
        return;
    }
}
void parser::classify(){
    if(command == 2||command == 4||command == 6){
        chainedQuery = true;
    }
}














parser::~parser()
{

}
