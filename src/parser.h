#ifndef PARSER_H
#define PARSER_H
#include <sstream>
using namespace std;
class TokenScanner {
   public:
    TokenScanner();

    TokenScanner(const string &str);

    ~TokenScanner();

    void setInput(const string &str);

    string nextToken();

   private:
    istringstream buffer;
};
#endif

bool IfKeywordRepeated(const char *keywords);

bool IfInvaild(const char *content, int index, int length);