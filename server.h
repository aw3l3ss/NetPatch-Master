#ifndef SERVER_H
#define SERVER_H

#include "libssh/libssh.h"

class Server {
    private:
        const char *hostname;
        const char *username;
        const char *password;
        int ssh_port; 
        ssh_session session;
        
    public:
        Server(const char *hostname = "127.0.0.1", const char *username = "root", const char *password = "root", int ssh_port = 22);
        bool connect();
        void disconnect();
        bool execute_command(const char* command, const char* filename);
};

#endif 