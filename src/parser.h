#ifndef PARSER_H
#define PARSER_H
#include <sstream>

class TokenScanner {
   public:
    TokenScanner();

    TokenScanner(const std::string &str);

    ~TokenScanner();

    void setInput(const std::string &str);

    std::string nextToken();

   private:
    std::istringstream buffer;
};
#endif

bool IfKeywordRepeated(const char *keywords);

bool IfInvaild(const char *content, int index, int length);