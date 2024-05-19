#ifndef SERVER_H
#define SERVER_H

#include "libssh/libssh.h"

class Server {
    private:
        char name[128];
        char hostname[128];
        char username[128];
        char password[128];
        int ssh_port; 
        ssh_session session;
        
    public:
        Server(char name[128], char hostname[128], char username[128], char password[128], int ssh_port);
        bool connect();
        void disconnect();
        bool execute_command(const char* command, const char* filename);
        const char* get_name();
        const char* get_hostname();
};

#endif 