#pragma once
#include "raylib.h"
#include <vector>
#include <iostream>
#include <string>
#include <iomanip>
#include <mariadb/mysql.h>
#include <string.h>

namespace helper {

    // Base template for print (Python-style)
    template <typename T>
    void print(const T& value) {
        std::cout << value << std::endl;
    }

    template <typename T, typename... Args>
    void print(const T& first, const Args&... args) {
        std::cout << first << " " << std::endl;
        print(args...);
    }

    void println() {
        std::cout << std::endl;
    }

    template <typename T, typename... Args>
    void println(const T& first, const Args&... args) {
        print(first, args...);
        std::cout << std::endl;
    }

    // Print vector
    template <typename T>
    void print_vector(const std::vector<T>& vectorObject, const std::string& label = "Vector") {
        std::cout << label << ": [";
        for (size_t i = 0; i < vectorObject.size(); ++i) {
            std::cout << vectorObject[i];
            if (i < vectorObject.size() - 1) {
                std::cout << ", " ;
            }
        }
        std::cout << "]" << std::endl;
    }

    // Print array (generic)
    template <typename T>
    void print_array(const T& arr) {
        std::cout << "[";
        for (size_t i = 0; i < arr.size(); ++i) {
            std::cout << arr[i];
            if (i < arr.size() - 1) {
                std::cout << ", ";
            }
        }
        std::cout << "]" << std::endl;
    }

    // Print array (fixed-size)
    template <typename T, size_t N>
    void print_array(const T (&arr)[N]) {
        std::cout << "[";
        for (size_t i = 0; i < N; ++i) {
            std::cout << arr[i];
            if (i < N - 1) {
                std::cout << ", ";
            }
        }
        std::cout << "]" << std::endl;
    }

    // Print ASCII table for MySQL results
    void print_ascii_table(MYSQL_RES* result) {
        if (!result) {
            std::cerr << "No result set returned." << std::endl;
            return;
        }

        // Get metadata about the result
        MYSQL_FIELD* fields = mysql_fetch_fields(result);
        int num_fields = mysql_num_fields(result);

        // Calculate column widths
        std::vector<size_t> column_widths(num_fields, 0);
        for (int i = 0; i < num_fields; ++i) {
            column_widths[i] = std::max<size_t>(strlen(fields[i].name), 10); // Minimum width
        }

        MYSQL_ROW row;
        while ((row = mysql_fetch_row(result))) {
            for (int i = 0; i < num_fields; ++i) {
                if (row[i]) {
                    column_widths[i] = std::max<size_t>(column_widths[i], strlen(row[i]));
                }
            }
        }

        // Reset the result pointer to the beginning
        mysql_data_seek(result, 0);

        // Print header
        for (int i = 0; i < num_fields; ++i) {
            std::cout << std::setw(column_widths[i] + 2) << std::left << fields[i].name;
        }
        std::cout << std::endl;

        for (int i = 0; i < num_fields; ++i) {
            std::cout << std::string(column_widths[i] + 2, '-');
        }
        std::cout << std::endl;

        // Print rows
        while ((row = mysql_fetch_row(result))) {
            for (int i = 0; i < num_fields; ++i) {
                std::cout << std::setw(column_widths[i] + 2) << std::left << (row[i] ? row[i] : "NULL");
            }
            std::cout << std::endl;
        }
    }
}
