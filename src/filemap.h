#ifndef ULL_H
#define ULL_H

#include <bits/stdc++.h>
using namespace std;
#define BLOCK_SIZE 1000
#define BLOCK_SPLIT_THRESHOLD 980
#define BLOCK_SPLIT_LEFT 490
// #define BLOCK_MERGE_THRESHOLD 20

class UllNode {
   public:
    int index_ = 0;      // value
    char str_[65] = "";  // key

    bool operator==(const UllNode &obj) const;

    UllNode(){};

    UllNode(const string &isbn, const int &index);

    UllNode &operator=(const UllNode &rhs);

    static bool Cmp(const UllNode &lhs, const UllNode &rhs);
};

class UllBlock {  // 0-base
   private:
   public:
    bool if_occupied_ = false;
    int num_ = 0;
    char start_[65] = "", end_[65] = "";
    UllNode array_[BLOCK_SIZE];  // 0-base

    UllBlock(){};

    UllBlock &operator=(const UllBlock &rhs);
};

class Ull {
   private:
    const string file_name_;
    fstream ffile_;  // note that a digit is stored in ffile

    inline void DelBlock(const int &index);

    void SplitBlock(UllBlock &obj, const int &index);

   public:
    Ull(){};

    Ull(const string &file_name);

    ~Ull(){};

    void FindNode(const string &key, set<int> &array);
    //may not use set

    void AddNode(const UllNode &node);

    void DeleteNode(const UllNode &node);
};

template <class T>
class MainInfo {
   private:
    string file_name_;

   public:
    MainInfo(string file_name);
    ~MainInfo();
    void WriteInfo(T full_info, string &key);
    void DeleteInfo(T &full_info, string &key);
    T FindInfo(string &key);
};

#endif

// ull->ullblock->ullnode