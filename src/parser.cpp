#include "parser.h"

#include <sstream>

TokenScanner::TokenScanner() {}

TokenScanner::~TokenScanner() {}

TokenScanner::TokenScanner(const string &str) { buffer.str(str); }
void TokenScanner::setInput(const string &str) { buffer.str(str); }

string TokenScanner::nextToken() {
    string token;
    buffer >> token;
    if (token != "")
        return token;
    else
        return "-1";
}
