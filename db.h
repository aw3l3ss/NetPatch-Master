#ifndef DB_H
#define DB_H

#include "sqlite3/sqlite3.h"
#include <string>
#include <vector>
#include "cve.h"
#include "server.h"

bool create_db(const char *filename);
bool add_server(const char *filename, const char *name_b, const char *hostname_b, const char *username_b, const char *password_b, int port);
std::vector<Server> get_servers(const char *filename);
bool delete_server(const char *filename, Server server);
bool add_cve(const char *filename, Server server, Cve cve);
bool clear_cves(const char *filename);
std::vector<Cve> get_cves(const char *filename, Server server);

#endif 