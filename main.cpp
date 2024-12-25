#include <iostream>
#include <iomanip>
#include <vector>
#include <string>
#include <mariadb/mysql.h>
#include <string.h>
#include "helper.h"

using namespace helper;

class ConnectionDetails {
public:
    const char *server, *user, *password, *database;
};

MYSQL* mysql_connection_setup(ConnectionDetails mysql_details) {
    MYSQL* connection = mysql_init(NULL);

    if (!mysql_real_connect(connection, mysql_details.server, mysql_details.user, mysql_details.password, mysql_details.database, 0, NULL, 0)) {
        print("Connection error: ");
        print(mysql_error(connection));
        exit(EXIT_FAILURE);
    }

    print("Connection successful!");
    return connection;
}

MYSQL_RES* mysql_execute_query(MYSQL* connection, const char* sql_query) {
    if (mysql_query(connection, sql_query)) {
        print("Query error: ");
        print(mysql_error(connection));
        exit(EXIT_FAILURE);
    }

    return mysql_store_result(connection);
}

int main() {
    MYSQL* connection = mysql_connection_setup({ "localhost", "root", "root", "game" });
    MYSQL_RES* result = mysql_execute_query(connection, "SELECT * FROM game_information");

    // Print the result 
    print_ascii_table(result);

    mysql_free_result(result);
    mysql_close(connection);
    return 0;
}