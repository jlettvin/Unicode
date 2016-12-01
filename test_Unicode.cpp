/* Copyright(c) 2016 Jonathan D. Lettvin, All Rights Reserved. */
#include <iostream>
#include <fstream>
#include <iomanip>
#include <limits>
#include <string>
#include <cstring>
#include <string.h>
#include <map>

#include "Unicode.h"

using std::endl;
using std::ifstream;
using std::cout;

using std::hex;
using std::dec;
using std::setw;
using std::setfill;

using std::string;

using jlettvin::ubyte_t;
using jlettvin::UTF8_to_char32_t;
using jlettvin::char32_t_to_UTF8;

void xlat(const char* arg, const size_t tail) {

    //_________________________________________________________________________
    static const char* boilerplate_head = R"UTF8(
#include <iostream>
#include <iomanip>
#include "Unicode.h"

using std::cout;
using std::endl;

using jlettvin::ubyte_t;
using jlettvin::UTF8_to_char32_t;
using jlettvin::char32_t_to_UTF8;

void show(const char32_t* text) {
    ubyte_t buf[5];
    char32_t c32;
    for (size_t i = 0; 0 != ( c32 = text[i]); ++i) {
        char32_t_to_UTF8(c32, buf);
        cout << reinterpret_cast<const char*>(buf);
    }
    cout << endl;
}

int main(int argc, char **argv) {
)UTF8";

    //_________________________________________________________________________
    static const char* boilerplate_tail = R"UTF8(
    return(0);
}
)UTF8";

    //_________________________________________________________________________
    static const char* begin = "    show(U\"";
    static const size_t width = 8;

    const unsigned char* buffer = reinterpret_cast<const unsigned char*>(arg);
    size_t head = 0;
    cout << boilerplate_head;
    cout << begin;
    while (head < tail) {
        char32_t c32 = UTF8_to_char32_t(buffer, head, tail);
        if (c32 == 0) {
            break;
        } else if (c32 == '\x0a') {
            cout << "\");" << endl << begin;
        } else {
            cout << "\\U" << hex << setw(width) << setfill('0') << c32;
        }
    }
    cout << "\");" << endl;
    cout << boilerplate_tail;
}

int main(int argc, char **argv) {
    if (argc == 1) {
        ifstream source;
        source.open ("haiku.utf8.txt", std::ios::in | std::ios::binary);
        source.seekg (0, std::ios::end);
        size_t length = source.tellg();
        source.seekg (0, std::ios::beg);
        char* buffer = new char [length];
        source.read(buffer, length);
        xlat(buffer, length);
        source.close();
    } else {
        while (--argc) {
            char* arg = *++argv;
            xlat(arg, strlen(arg));
        }
    }
}
