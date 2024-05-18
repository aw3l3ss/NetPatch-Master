#include <iostream>
#include <stdio.h>
#include "server.h"

Server::Server(const char *hostname, const char *username, const char *password, int ssh_port) {
    this->hostname = hostname;
    this->username = username;
    this->password = password;
    this->ssh_port = ssh_port;

    session = ssh_new();

    ssh_options_set(session, SSH_OPTIONS_HOST, hostname);
    ssh_options_set(session, SSH_OPTIONS_USER, username);
    ssh_options_set(session, SSH_OPTIONS_PORT, &ssh_port);
}

bool Server::connect() {
    int rc;

    rc = ssh_connect(session);
    if (rc != SSH_OK) {
        std::cerr << "Error connecting to " << hostname << ": " << ssh_get_error(session) << std::endl;
        ssh_free(session);
        return false;
    }

    rc = ssh_userauth_password(session, nullptr, password);
    if (rc != SSH_AUTH_SUCCESS) {
        std::cerr << "Error authenticating with password: " << ssh_get_error(session) << std::endl;
        ssh_disconnect(session);
        ssh_free(session);
        return false;
    }

    return true;
}

void Server::disconnect() {
    ssh_disconnect(session);
    ssh_free(session);
}

bool Server::execute_command(const char* command, const char* filename) {
    ssh_channel channel;
    int rc;

    channel = ssh_channel_new(session);
    if (channel == nullptr) {
        return false;
    }
    
    rc = ssh_channel_open_session(channel);
    if (rc != SSH_OK) {
        ssh_channel_free(channel);
        std::cerr << "Error executing command: " << ssh_get_error(session) << std::endl;
        return false;
    }

    rc = ssh_channel_request_exec(channel, command);
    if (rc != SSH_OK) {
        ssh_channel_close(channel);
        ssh_channel_free(channel);
        std::cerr << "Error executing command: " << ssh_get_error(session) << std::endl;
        return false;
    }

    FILE *file = fopen(filename, "wb");

    char buffer[256];
    int nbytes;
    while ((nbytes = ssh_channel_read(channel, buffer, sizeof(buffer), 0)) > 0) {
        if (fwrite(buffer, 1, nbytes, file) != (unsigned int)nbytes) {
            ssh_channel_close(channel);
            ssh_channel_free(channel);
            std::cerr << "Error executing command: " << ssh_get_error(session) << std::endl;
            return false;
        }
    }

    fclose(file);

    if (nbytes < 0) {
        return false;
    }

    ssh_channel_send_eof(channel);
    ssh_channel_close(channel);
    ssh_channel_free(channel);

    return true;
}