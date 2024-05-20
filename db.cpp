#include "db.h"

#include <iostream>

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

    rc = sqlite3_exec(db, "CREATE TABLE IF NOT EXISTS vulns (id INTEGER PRIMARY KEY AUTOINCREMENT, server_id INTEGER NOT NULL, cve_id TEXT NOT NULL, cve_summary TEXT NOT NULL, cvss4 TEXT NOT NULL, cvss3 TEXT NOT NULL, cvss2 TEXT NOT NULL);", 0, 0, &zErrMsg);
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

    std::string name(name_b);
    std::string hostname(hostname_b);
    std::string username(username_b);
    std::string password(password_b);

    std::string expr = "INSERT INTO servers (name, hostname, username, password, port) VALUES ('" + name + "','" + hostname + "','" + username + "','" + password + "'," + std::to_string(port) + ");";

    rc = sqlite3_exec(db, expr.c_str(), 0, 0, &zErrMsg);
    if (rc!= SQLITE_OK) {
        sqlite3_free(zErrMsg);
        sqlite3_close(db);
        return false;
    }

    sqlite3_close(db);

    return true;
}

std::vector<Server> get_servers(const char *filename) {
    std::vector<Server> servers;

    char *zErrMsg = 0;
    int rc;
    sqlite3 *db;
    sqlite3_stmt *stmt;

    rc = sqlite3_open(filename, &db);
    if (rc) {
        sqlite3_close(db);
        return servers;
    }

    rc = sqlite3_prepare_v2(db, "SELECT name, hostname, username, password, port FROM servers;", -1, &stmt, nullptr);

    if (rc!= SQLITE_OK) {
        sqlite3_close(db);
        return servers;
    }

    while ((rc = sqlite3_step(stmt)) == SQLITE_ROW) {
        const unsigned char *name = sqlite3_column_text(stmt, 0);
        const unsigned char *hostname = sqlite3_column_text(stmt, 1);
        const unsigned char *username = sqlite3_column_text(stmt, 2);
        const unsigned char *password = sqlite3_column_text(stmt, 3);
        const unsigned char *port_b = sqlite3_column_text(stmt, 4);

        int i = 0;
        int port = 0;

        while (port_b[i] != '\0') {
            port = (port + (port_b[i] - '0')) * 10;
            ++i;
        }

        port /= 10;

        Server server((char *) name, (char *) hostname, (char *) username, (char *) password, port);
        servers.push_back(server);
    }

    if (rc != SQLITE_DONE) {
        sqlite3_finalize(stmt);
        sqlite3_close(db);

        return servers;
    }

    sqlite3_finalize(stmt);
    sqlite3_close(db);

    return servers;
}

bool delete_server(const char *filename, Server server) {
    char *zErrMsg = 0;
    int rc;
    sqlite3 *db;
    
    rc = sqlite3_open(filename, &db);
    if (rc) {
        sqlite3_close(db);
        return false;
    }

    std::string name = server.get_name();
    std::string hostname = server.get_hostname();

    std::string expr = "DELETE FROM servers WHERE name = '" + name + "' AND hostname = '" + hostname + "';";
    rc = sqlite3_exec(db, expr.c_str(), 0, 0, &zErrMsg);
    if (rc!= SQLITE_OK) {
        sqlite3_free(zErrMsg);
        sqlite3_close(db);
        return false;
    }

    sqlite3_close(db);

    return true;
}

bool add_cve(const char *filename, Server server, Cve cve) {
    char *zErrMsg = 0;
    int rc;
    sqlite3 *db;

    rc = sqlite3_open(filename, &db);
    if (rc) {
        sqlite3_close(db);
        return false;
    }
    
    int server_id = 0;

    std::string name = server.get_name();
    std::string hostname = server.get_hostname();

    std::string expr = "SELECT id FROM servers WHERE hostname = '" + hostname + "' AND name = '" + name + "' LIMIT 1;";

    sqlite3_stmt *stmt;

    rc = sqlite3_prepare_v2(db, expr.c_str(), -1, &stmt, nullptr);

    if (rc!= SQLITE_OK) {
        sqlite3_close(db);
        return false;
    }

    rc = sqlite3_step(stmt);
    if (rc!= SQLITE_ROW) {
        sqlite3_finalize(stmt);
        sqlite3_close(db);
        return false;
    }

    server_id = sqlite3_column_int(stmt, 0);

    sqlite3_finalize(stmt);

    sqlite3_close(db);

    sqlite3_open(filename, &db);

    std::string expr_1 = "INSERT INTO vulns (server_id, cve_id, cve_summary, cvss4, cvss3, cvss2) VALUES (" + std::to_string(server_id) + ",'" + cve.get_id() + "','" + cve.get_summary() + "','" + cve.get_cvss4() + "','" + cve.get_cvss3() + "','" + cve.get_cvss2() + "');";
    
    rc = sqlite3_exec(db, expr_1.c_str(), 0, 0, &zErrMsg);
    if (rc != SQLITE_OK) {
        sqlite3_free(zErrMsg);
        sqlite3_close(db);
        return false;
    }

    sqlite3_close(db);

    return true;
}

bool clear_cves(const char *filename) {
    char *zErrMsg = 0;
    int rc;
    sqlite3 *db;

    rc = sqlite3_open(filename, &db);
    if (rc) {
        sqlite3_close(db);
        return false;
    }

    std::string expr = "DELETE FROM vulns;";
    rc = sqlite3_exec(db, expr.c_str(), 0, 0, &zErrMsg);
    if (rc!= SQLITE_OK) {
        sqlite3_free(zErrMsg);
        sqlite3_close(db);
        return false;
    }

    sqlite3_close(db);

    return true;
}

std::vector<Cve> get_cves(const char *filename, Server server) {
    std::vector<Cve> cves;

    int server_id;

    char *zErrMsg = 0;
    int rc;
    sqlite3 *db;
    sqlite3_stmt *stmt;

    rc = sqlite3_open(filename, &db);
    if (rc) {
        sqlite3_close(db);
        return cves;
    }

    std::string name = server.get_name();
    std::string hostname = server.get_hostname();

    std::string expr = "SELECT id FROM servers WHERE hostname = '" + hostname + "' AND name = '" + name + "' LIMIT 1;";

    rc = sqlite3_prepare_v2(db, expr.c_str(), -1, &stmt, nullptr);

    if (rc!= SQLITE_OK) {
        sqlite3_close(db);
        return cves;
    }

    rc = sqlite3_step(stmt);
    if (rc!= SQLITE_ROW) {
        sqlite3_finalize(stmt);
        sqlite3_close(db);
        return cves;
    }

    server_id = sqlite3_column_int(stmt, 0);

    sqlite3_finalize(stmt);

    sqlite3_close(db);

    rc = sqlite3_open(filename, &db);
    if (rc) {
        sqlite3_close(db);
        return cves;
    }

    std::string expr_1 = "SELECT cve_id, cve_summary, cvss4, cvss3, cvss2 FROM vulns WHERE server_id = " + std::to_string(server_id) + ";";

    rc = sqlite3_prepare_v2(db, expr_1.c_str(), -1, &stmt, nullptr);

    if (rc!= SQLITE_OK) {
        sqlite3_close(db);
        return cves;
    }

    while ((rc = sqlite3_step(stmt)) == SQLITE_ROW) {
        const unsigned char *cve_id = sqlite3_column_text(stmt, 0);
        const unsigned char *cve_summary = sqlite3_column_text(stmt, 1);
        const unsigned char *cvss4 = sqlite3_column_text(stmt, 2);
        const unsigned char *cvss3 = sqlite3_column_text(stmt, 3);
        const unsigned char *cvss2 = sqlite3_column_text(stmt, 4);

        Cve cve((char *) cve_id, (char *) cve_summary, (char *) cvss4, (char *) cvss3, (char *) cvss2);
        cves.push_back(cve);
    }

    if (rc != SQLITE_DONE) {
        sqlite3_finalize(stmt);
        sqlite3_close(db);

        return cves;
    }

    sqlite3_finalize(stmt);
    sqlite3_close(db);

    return cves;
}