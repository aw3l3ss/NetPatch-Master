#ifndef DB_H
#define DB_H

#include "sqlite3/sqlite3.h"
#include <string>
#include <vector>
#include "server.h"

bool create_db(const char *filename);
bool add_server(const char *filename, const char *name_b, const char *hostname_b, const char *username_b, const char *password_b, int port);
bool add_vuln(const char *filename, int server_id, std::string cves);
std::vector<Server> get_servers(const char *filename);

#endif 