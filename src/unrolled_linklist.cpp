#include "unrolled_linklist.hpp"

#include <algorithm>
#include <cstdint>
#include <cstring>
#include <fstream>
#include <ostream>
UllNode::UllNode(const string &isbn, const int &index) {
    strcpy(str, isbn.c_str());
    this->index = index;
}
bool UllNode::cmp(const UllNode &lhs, const UllNode &rhs) {
    if (strcmp(lhs.str, rhs.str) < 0)
        return true;
    else
        return false;
}
bool UllNode::operator==(const UllNode &obj) const {
    if (strcmp(this->str, obj.str) == 0 && this->index == obj.index)
        return true;
    return false;
}

UllNode &UllNode::operator=(const UllNode &rhs) {
    this->index = rhs.index;
    strcpy(this->str, rhs.str);
    return *this;
}

UllBlock &UllBlock::operator=(const UllBlock &rhs) {
    // this->nxt = rhs.nxt;
    // this->pre = rhs.pre;
    this->num = rhs.num;
    for (int i = 0; i < num; i++) this->array[i] = rhs.array[i];
    return *this;
}

Ull::Ull(const string &file_name) : file_name(file_name) {
    ifstream in_1(file_name, ifstream::in);

    if (!in_1) {
        ofstream out(file_name, ofstream::out);
        int block_num = 1;
        UllBlock tmp;
        ffile.open(file_name, fstream::in | fstream::binary | fstream::out);
        ffile.write(reinterpret_cast<char *>(&block_num), sizeof(int));
        ffile.write(reinterpret_cast<char *>(&tmp), sizeof(UllBlock));
        ffile.close();
    }
}

void Ull::addNode(const UllNode &book) {
    int block_num;
    ffile.close();
    ffile.open(file_name, fstream::out | fstream::binary | fstream::in);
    ffile.seekg(0);
    ffile.read(reinterpret_cast<char *>(&block_num), sizeof(int));
    UllBlock tmp;
    int i = 0, index = 0;

    for (index = 0; index < block_num; index++) {
        ffile.read(reinterpret_cast<char *>(&tmp), sizeof(UllBlock));
        if (strcmp(tmp.end, book.str) >= 0) {
            auto binary_find =
                lower_bound(tmp.array, tmp.array + tmp.num, book, UllNode::cmp);
            int indexx = binary_find - tmp.array;
            for (int i = indexx; i < tmp.num; i++)  // binary_search
                if (strcmp(tmp.array[i].str, book.str) == 0) {
                    if (tmp.array[i].index == book.index) {
                        ffile.close();
                        return;
                    }
                } else
                    break;
        }
    }
    ffile.seekg(sizeof(int));

    for (index = 0; index < block_num; index++) {  // find the block
        ffile.read(reinterpret_cast<char *>(&tmp), sizeof(UllBlock));
        if (strcmp(book.str, tmp.end) <= 0) break;
    }
    if (index == block_num) index--;
    auto binary_find =
        lower_bound(tmp.array, tmp.array + tmp.num, book, UllNode::cmp);
    i = binary_find - tmp.array;
    // update the bound
    for (int j = tmp.num - 1; j >= i; j--)
        tmp.array[j + 1] = tmp.array[j];  // move and copy
    tmp.array[i] = book;
    tmp.num++;

    // operations that might make the program slower
    strcpy(tmp.start, tmp.array[0].str);
    strcpy(tmp.end, tmp.array[tmp.num - 1].str);
    if (tmp.num <= BLOCK_SPLIT_THRESHOLD) {
        ffile.seekp(sizeof(int) + index * sizeof(UllBlock));
        ffile.write(reinterpret_cast<char *>(&tmp), sizeof(UllBlock));
    } else {  // split the block and rewrite
        splitBlock(tmp, index);
        block_num++;
        ffile.seekp(0);
        ffile.write(reinterpret_cast<char *>(&block_num), sizeof(int));
    }
    ffile.close();
}

void Ull::splitBlock(UllBlock &obj, const int &index) {  // to be checked
    int block_num, put_index = 0;
    // ffile.open(file_name, fstream::out | fstream::binary | fstream::in);
    ffile.seekg(0);
    ffile.read(reinterpret_cast<char *>(&block_num), sizeof(int));
    block_num++;
    UllBlock tmp;
    for (int i = BLOCK_SPLIT_LEFT; i < obj.num; i++)
        tmp.array[i - BLOCK_SPLIT_LEFT] = obj.array[i];
    tmp.num = obj.num - BLOCK_SPLIT_LEFT;
    obj.num = BLOCK_SPLIT_LEFT;
    strcpy(obj.end, obj.array[obj.num - 1].str);
    strcpy(tmp.start, tmp.array[0].str);
    strcpy(tmp.end, tmp.array[tmp.num - 1].str);
    tmp.nxt = obj.nxt;
    obj.nxt = put_index;
    ffile.seekp(0);
    ffile.write(reinterpret_cast<char *>(&block_num), sizeof(int));
    ffile.seekp(sizeof(int) + index * sizeof(UllBlock));
    ffile.write(reinterpret_cast<char *>(&obj), sizeof(UllBlock));
    ffile.seekp(sizeof(int) + put_index * sizeof(UllBlock));
    ffile.write(reinterpret_cast<char *>(&tmp), sizeof(UllBlock));
}

void Ull::findNode(const string &key, set<int> &tp) {
    int block_num;
    tp.clear();
    ffile.close();
    ffile.open(file_name, fstream::out | fstream::binary | fstream::in);
    ffile.seekg(0);
    ffile.read(reinterpret_cast<char *>(&block_num), sizeof(int));
    UllBlock tmp;
    int index = 0;

    for (index = 0; index < block_num; index++) {
        ffile.read(reinterpret_cast<char *>(&tmp), sizeof(UllBlock));
        if (strcmp(tmp.end, key.c_str()) >= 0) {
            auto binary_find = lower_bound(tmp.array, tmp.array + tmp.num,
                                           UllNode(key, 0), UllNode::cmp);
            int indexx = binary_find - tmp.array;
            for (int i = indexx; i < tmp.num; i++)
                if (strcmp(tmp.array[i].str, key.c_str()) != 0)
                    break;
                else
                    tp.insert(tmp.array[i].index);
        }
    }
    ffile.close();
}

void Ull::deleteNode(const UllNode &node) {
    int block_num;
    ffile.close();
    ffile.open(file_name, fstream::out | fstream::binary | fstream::in);
    ffile.seekg(0);
    ffile.read(reinterpret_cast<char *>(&block_num), sizeof(int));
    UllBlock tmp;
    int i = 0, index = 0;
    for (index = 0; index < block_num; index++) {  // find the block
        ffile.read(reinterpret_cast<char *>(&tmp), sizeof(UllBlock));
        if (strcmp(node.str, tmp.end) <= 0) {
            for (i = 0; i < tmp.num; i++)
                if (node == tmp.array[i]) break;
            if (i == tmp.num) continue;

            strcpy(tmp.array[i].str, "");
            tmp.array[i].index = 0;

            for (int j = i + 1; j < tmp.num; j++)  //
                tmp.array[j - 1] = tmp.array[j];
            tmp.array[tmp.num - 1].index = 0;
            strcpy(tmp.array[tmp.num - 1].str, "");
            tmp.num--;

            strcpy(tmp.start, tmp.array[0].str);
            strcpy(tmp.end, tmp.array[tmp.num - 1].str);
            ffile.seekp(sizeof(int) + index * sizeof(UllBlock));
            ffile.write(reinterpret_cast<char *>(&tmp), sizeof(UllBlock));
            ffile.close();
            return;
        }
    }
    ffile.close();
}

void Ull::show() {
    ffile.open(file_name, fstream::out | fstream::binary | fstream::in);
    int block_num;
    ffile.seekg(0);
    ffile.read(reinterpret_cast<char *>(&block_num), sizeof(int));
    for (int i = 1; i <= block_num; i++) {
        UllBlock tmp;
        ffile.read(reinterpret_cast<char *>(&tmp), sizeof(UllBlock));
        cout << "block" << i << " ";
        for (int i = 0; i < tmp.num; i++) cout << tmp.array[i].index << " ";
        cout << endl;
    }
    ffile.close();
}