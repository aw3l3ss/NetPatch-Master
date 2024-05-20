#include "db.h"

bool create_db(const char *filename) {
    char *zErrMsg = 0;
    int rc;
    sqlite3 *db;

    rc = sqlite3_open(filename, &db);
    if (rc) {
        sqlite3_close(db);
        return false;
    }

    rc = sqlite3_exec(db, "CREATE TABLE IF NOT EXISTS servers (id INTEGER PRIMARY KEY AUTOINCREMENT, name TEXT NOT NULL, hostname TEXT NOT NULL, username TEXT NOT NULL, password TEXT NOT NULL, port INTEGER NOT NULL);", 0, 0, &zErrMsg);
    if (rc!= SQLITE_OK) {
        sqlite3_free(zErrMsg);
        sqlite3_close(db);
        return false;
    }

    rc = sqlite3_exec(db, "CREATE TABLE IF NOT EXISTS vulns (id INTEGER PRIMARY KEY AUTOINCREMENT, server_id INTEGER NOT NULL, cves TEXT NOT NULL);", 0, 0, &zErrMsg);
    if (rc!= SQLITE_OK) {
        sqlite3_free(zErrMsg);
        sqlite3_close(db);
        return false;
    }

    sqlite3_close(db);

    return true;
}

bool add_server(const char *filename, const char *name_b, const char *hostname_b, const char *username_b, const char *password_b, int port) {
    char *zErrMsg = 0;
    int rc;
    sqlite3 *db;

    rc = sqlite3_open(filename, &db);
    if (rc) {
        sqlite3_close(db);
        return false;
    }

    std::string name = name_b;
    std::string hostname = hostname_b;
    std::string username = username_b;
    std::string password =  password_b;

    std::string expr = "INSERT INTO servers (name, hostname, username, password, port) VALUES (" + name + "," + hostname + "," + username + "," + password + "," + std::to_string(port) + ");";

    rc = sqlite3_exec(db, expr.c_str(), 0, 0, &zErrMsg);
    if (rc!= SQLITE_OK) {
        sqlite3_free(zErrMsg);
        sqlite3_close(db);
        return false;
    }

    sqlite3_close(db);

    return true;
}

bool add_vuln(const char *filename, int server_id, std::string cves) {
    char *zErrMsg = 0;
    int rc;
    sqlite3 *db;

    rc = sqlite3_open(filename, &db);
    if (rc) {
        sqlite3_close(db);
        return false;
    }

    rc = sqlite3_exec(db, "INSERT INTO vulns (server_id, cves) VALUES (?,?);", 0, 0, &zErrMsg);
    if (rc!= SQLITE_OK) {
        sqlite3_free(zErrMsg);
        sqlite3_close(db);
        return false;
    }

    sqlite3_close(db);

    return true;
}

int callback(void *server, int colCount, char **columns, char **colNames) {
    int port = 0;

    int i = 0;
    while (columns[4][i] != '\0') {
        port = (port + (columns[4][i] - '0')) * 10;
        ++i;
    }
    port /= 10;

    Server server_n(columns[0], columns[1], columns[2], columns[3], port);
    (Server) *server = server_n;
    
    return 0;
}

std::vector<Server> get_servers(const char *filename) {
    std::vector<Server> servers;

    char *zErrMsg = 0;
    int rc;
    sqlite3 *db;

    rc = sqlite3_open(filename, &db);
    if (rc) {
        sqlite3_close(db);
        return servers;
    }

    Server server(NULL, NULL, NULL, NULL, 0);

    rc = sqlite3_exec(db, "SELECT name, hostname, username, password, port FROM servers;", callback, (void *) &server, &zErrMsg);

    if (rc!= SQLITE_OK) {
        sqlite3_free(zErrMsg);
        sqlite3_close(db);
        return servers;
    }

    sqlite3_close(db);

    return servers;
}