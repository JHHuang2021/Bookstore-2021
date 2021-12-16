#include "parser.h"

#include <cstring>
#include <sstream>
#include <vector>

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

bool IfKeywordRepeated(const char *keywords) {
    vector<string> key_word;
    char *src = new char[strlen(keywords) + 1];
    strcpy(src, keywords);
    char *token = strtok(src, "|");
    while (token != nullptr) {
        key_word.push_back(token);
        token = strtok(nullptr, "|");
    }
    delete[] src;
    while (!key_word.empty()) {
        for (auto iter = key_word.begin() + 1; iter != key_word.end(); iter++)
            if (*iter == *key_word.begin()) return true;
        key_word.erase(key_word.begin());
    }
    return false;
}

bool IfInvaild(const char *content, int index) {}
