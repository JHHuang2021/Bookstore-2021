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

//数字，字母，下划线 -- 1
//除不可见字符以外 Ascii 字符 -- 2
//数字 -- 3
//除不可见字符和英文双引号以外 Ascii 字符 -- 4
//数字和 . -- 5
bool IfInvaild(const char *content, int index, int max_length) {
    if (strlen(content) > max_length) return true;
    if (strcmp(content, "-1") == 0) return false;
    if (index == 1) {
        for (int i = 0; i < strlen(content); i++)
            if (*(content + i) <= 47 ||
                (*(content + i) >= 58 && *(content + i) <= 64) ||
                (*(content + i) >= 91 && *(content + i) <= 94) ||
                *(content + i) == 96 || *(content + i) >= 123)
                return true;
        return false;
    } else if (index == 2) {
        return false;
    } else if (index == 3) {
        for (int i = 0; i < strlen(content); i++)
            if (*(content + i) <= 47 || *(content + i) >= 58) 
            return true;
        return false;
    } else if (index == 4) {
        for (int i = 0; i < strlen(content); i++)
            if (*(content + i) == '\"')
             return true;
        return false;
    } else if (index == 5) {
        int precision = -1;
        for (int i = 0; i < strlen(content); i++) {
            if (precision != -1) precision++;
            if (*(content + i) == '.') {
                if (precision == -1)
                    precision++;
                else
                    return true;
            } else if (*(content + i) <= 47 || *(content + i) >= 58)
                return true;
        }
        if (precision <= 2)
            return false;
        else
            return true;
    }
    return false;
}
