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

using jlettvin::UTF8_to_char32_t;

void xlat(const char* arg, const size_t tail) {
    const unsigned char* buffer = reinterpret_cast<const unsigned char*>(arg);
    size_t head = 0;
    size_t language = 0;
    const char *lead = "static const char32_t language";
    cout << lead << dec << language << "[] = U\"";
    while (head < tail) {
        char32_t c32 = UTF8_to_char32_t(buffer, head, tail);
        if (c32 == 0) break;
        if (c32 == '\x0a') {
            cout <<
                "\";" <<
                endl << endl <<
                lead << dec << ++language << "[] = U\"";
            continue;
        }
        cout << "\\U" << hex << setw(4) << setfill('0') << c32;
    }
    cout << "\";" << endl;
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

    //static string failpass[2] = {"[FAIL]", "[PASS]"};
    //static string equality[2] = {" != ", " == "};

}
