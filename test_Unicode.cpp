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
using std::ofstream;
using std::cout;

using std::hex;
using std::dec;
using std::setw;
using std::setfill;

using std::string;
using std::u32string;

using jlettvin::UTF8_to_char32_t;
using jlettvin::char32_t_to_UTF8;
using jlettvin::UTF8_to_u32string;
using jlettvin::u32string_to_UTF8;

void xlat(const char* arg, const size_t tail) {

    //_________________________________________________________________________
    static const char* boilerplate_head = R"UTF8(
#include <iostream>
#include <iomanip>
#include "Unicode.h"

using std::cout;
using std::endl;

using jlettvin::UTF8_to_char32_t;
using jlettvin::char32_t_to_UTF8;

void show(const char32_t* text) {
    char buf[5];
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

    const char* buffer = reinterpret_cast<const char*>(arg);
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

static string failpass[2] = {"[FAIL]", "[PASS]"};

void test_data(char*& buffer, size_t& length) {
    ifstream source;
    source.open ("haiku.utf8.txt", std::ios::in | std::ios::binary);
    source.seekg (0, std::ios::end);
    length = source.tellg();
    source.seekg (0, std::ios::beg);
    buffer = new char [length];
    source.read(buffer, length);
    source.close();
}

void test_1() {
    char *buffer;
    size_t length;
    test_data(buffer, length);
    xlat(buffer, length);
}

void test_2() {
    char* buffer;
    u32string target;
    size_t length;

    test_data(buffer, length);
    string before(buffer), after;

    UTF8_to_u32string(buffer, target);
    u32string_to_UTF8(target, after);
    after += "\n";  ///< TODO final endl missing in translation (BUG)

    ofstream store;
    store.open("haiku.utf8.err", std::ios::out | std::ios::binary);
    store.write(after.c_str(), length);
    store.close();

    cout <<
        "/* " <<
        failpass[static_cast<size_t>(length == after.length())] <<
        " string lengths should match after forward/reverse translation." <<
        " */" <<
        endl;
    cout <<
        "/* " <<
        failpass[static_cast<size_t>(0 == after.compare(before))] <<
        " string contents should match after forward/reverse translation." <<
        " */" <<
        endl;
}

void test() {
    test_1();
    test_2();
}

int main(int argc, char **argv) {
    if (argc == 1) {
        test();
    } else {
        while (--argc) {
            char* arg = *++argv;
            xlat(arg, strlen(arg));
        }
    }
}
