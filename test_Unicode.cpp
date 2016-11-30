/* Copyright(c) 2016 Jonathan D. Lettvin, All Rights Reserved. */
#include <iostream>
#include <iomanip>
#include <limits>
#include <string>
#include <cstring>
#include <string.h>
#include <map>

#include "Unicode.h"

int main(int argc, char **argv) {
    using std::endl;
    using std::cout;
    using std::hex;
    using std::setw;
    using std::setfill;
    using std::string;
    //using std::strlen;

    using jlettvin::UTF8_to_char32_t;

    while (--argc) {
        char *arg = *++argv;
        const unsigned char* buffer =
            reinterpret_cast<const unsigned char*>(arg);
        size_t head = 0, tail = strlen(arg);
        char twixt = '[';
        cout << buffer << ": ";
        while (head < tail) {
            cout <<
                twixt << "U+" << hex << UTF8_to_char32_t(buffer, head, tail);
            twixt = ' ';
        }
        cout << "]" << endl;
    }

    //static string failpass[2] = {"[FAIL]", "[PASS]"};
    //static string equality[2] = {" != ", " == "};

}
