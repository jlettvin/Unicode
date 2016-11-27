/* Copyright(c) 2016 Jonathan D. Lettvin, All Rights Reserved. */
#include <iostream>
#include <iomanip>
#include <limits>
#include <string>

#include "Node.h"

int main(int argc, char **argv) {
    using std::endl;
    using std::cout;
    using std::hex;
    using std::setw;
    using std::setfill;
    using std::string;

    using jlettvin::Node;

    while (--argc) cout << *++argv;  ///< Gets rid of "unused" warning.
    cout << endl;

    static string failpass[2] = {"[FAIL]", "[PASS]"};
    static string equality[2] = {" != ", " == "};
    static size_t count = 0x110000;

    /** \brief instance a codepoint table */
    Node *cpt = new Node();
    Node &ref = *cpt;

    /** \brief set up some test values */
    const size_t values[] = {
        0x110000, 0x10FFFF, 0x10000, 0xFFFF, 0x61, 0x41, 0x31, 0x01, 0x00, 0x00
    };

    size_t source, target, results[2] = {0, 0};
    for (int i = 0; 0 != (source = values[i]); ++i) {
        ref.poke(source, source);
    }
    cout << "\tStandard suite (including intentionally bad values)" << endl;
    for (int i = 0; 0 != (source = values[i]); ++i) {
        target = ref.peek(source);
        int result = static_cast<int>(target == source);
        string& eq = equality[result];
        if (!result) result = static_cast<int>(source < 1 || source >= count);
        string& fp = failpass[result];
        cout << fp << ' ' << hex << setw(6) << source << eq << target << endl;
        ++results[result];
        if (i&1) ref.drop(source);  ///< drop every odd indexed value
    }
    cout << "\tEven suite (after dropping every odd indexed value)" << endl;
    for (int i = 0; 0 != (source = values[i]); ++i) {
        target = ref.peek(source);
        int result = static_cast<int>(target == source);
        string& eq = equality[result];
        if (!result)
            result = static_cast<int>(source < 1 || source > 0x10FFFF || i&1);
        string& fp = failpass[result];
        cout << fp << ' ' << hex << setw(6) << source << eq << target << endl;
        ++results[result];
    }
    size_t fails = results[0], passes = results[1];
    cout << "\t" << fails << " fails, " << passes << " passes" << endl;
}
