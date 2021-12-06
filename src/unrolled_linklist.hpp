#ifndef ULL_H
#define ULL_H

#include <bits/stdc++.h>
using namespace std;
#define BLOCK_SIZE 600
#define BLOCK_SPLIT_THRESHOLD 580
#define BLOCK_SPLIT_LEFT 290
// #define BLOCK_MERGE_THRESHOLD 20

class UllNode {
   public:
    int index = 0;      // value
    char str[65] = "";  // key

    bool operator==(const UllNode &obj) const;

    UllNode(){};

    UllNode(const string &isbn, const int &index);

    UllNode &operator=(const UllNode &rhs);

    static bool cmp(const UllNode &lhs, const UllNode &rhs);
};

class UllBlock {  // 0-base
   private:
   public:
    int nxt = 0;
    int pre = 0;
    bool if_occupied = false;
    int num = 0;
    char start[65] = "", end[65] = "";
    UllNode array[BLOCK_SIZE];  // 0-base

    UllBlock(){};

    UllBlock &operator=(const UllBlock &rhs);

    int binary_search(const string &search);
};

class Ull {
   private:
    int spare_block_index[600];

   private:
    const string file_name, fio_name;
    fstream ffile, file_spare;  // note that a digit is stored in ffile

    inline int nextBlock(const int &index);

    inline void delBlock(const int &index);

    // void mergeBlock(UllBlock &tmp1, UllBlock &tmp2, const int &index1,
    // const int &index2);

    void splitBlock(UllBlock &obj, const int &index);

   public:
    Ull(){};

    Ull(const string &file_name, const string &file_free);

    ~Ull(){};

    void findNode(const string &key, set<int> &array);

    void addNode(const UllNode &node);

    void deleteNode(const UllNode &node);

    void show();
};

#endif

// ull->ullblock->ullnode