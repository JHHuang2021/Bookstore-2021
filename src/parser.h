#ifndef PARSER_H
#define PARSER_H
#include <sstream>
using namespace std;
class TokenScanner {
   public:
    TokenScanner(){};

    TokenScanner(const string &str);

    virtual ~TokenScanner();

    void setInput(const string &str);

    string nextToken();

   private:
    istringstream buffer;
};
#endif