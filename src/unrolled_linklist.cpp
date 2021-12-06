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

Ull::Ull(const string &file_name, const string &file_free)
    : file_name(file_name), fio_name(file_free) {
    ifstream in_1(file_name, ifstream::in);

    if (!in_1) {
        // ffile.open(file_name, fstream::out);
        // ffile.close();
        ofstream out(file_name, ofstream::out);
        int block_num = 1;
        UllBlock tmp;
        ffile.open(file_name, fstream::in | fstream::binary | fstream::out);
        ffile.write(reinterpret_cast<char *>(&block_num), sizeof(int));
        ffile.write(reinterpret_cast<char *>(&tmp), sizeof(UllBlock));
        ffile.close();
    }

    ifstream in_2(file_free, ifstream::in);
    if (!in_2) {
        ofstream out_(file_free, ofstream::out);
        // file_spare.open(file_free, fstream::out);
        // file_spare.close();
        memset(spare_block_index, -1, sizeof(spare_block_index));
        spare_block_index[0] = 0;
        file_spare.open(file_free,
                        fstream::in | fstream::binary | fstream::out);
        file_spare.write(reinterpret_cast<char *>(spare_block_index),
                         sizeof(spare_block_index));
        file_spare.close();
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
        if (strcmp(tmp.end, book.str) >= 0)
            for (int i = 0; i < tmp.num; i++)
                if (tmp.array[i] == book) {
                    ffile.close();
                    return;
                }
    }
    ffile.seekg(sizeof(int));

    for (index = 0; index < block_num; index++) {  // find the block
        ffile.read(reinterpret_cast<char *>(&tmp), sizeof(UllBlock));
        if (strcmp(book.str, tmp.end) <= 0) break;
    }
    if (index == block_num) index--;
    i = tmp.binary_search(book.str);
    if (strcmp(book.str, tmp.array[i].str) > 0) i++;
    // for (i = 0; i < tmp.num; i++)  // find the position in the block
    // {
    //     if (strcmp(book.str, tmp.array[i].str) < 0) break;
    // }

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
    file_spare.open(fio_name, fstream::in | fstream::out | fstream::binary);
    file_spare.seekg(0);
    file_spare.read(reinterpret_cast<char *>(spare_block_index),
                    sizeof(spare_block_index));
    if (spare_block_index[0] == 0) {
        put_index = block_num;
    } else {
        put_index = spare_block_index[spare_block_index[0]];
        spare_block_index[spare_block_index[0]] = -1;
        spare_block_index[0]--;
    }
    block_num++;
    file_spare.seekp(0);
    file_spare.write(reinterpret_cast<char *>(spare_block_index),
                     sizeof(spare_block_index));
    file_spare.close();
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
    // obj.nxt = put_index;
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
            int index = tmp.binary_search(key);
            for (int i = index; i < tmp.num; i++)
                if (strcmp(tmp.array[i].str, key.c_str()) == 0) {
                    tp.insert(tmp.array[i].index);
                } else
                    break;
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
            int indexx = tmp.binary_search(node.str);
            for (i = indexx; i < tmp.num; i++) {
                // find the position in the block
                if (node == tmp.array[i]) break;
            }
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

int UllBlock::binary_search(const string &search) {
    int l = 0, r = this->num - 1;
    while (l < r) {
        int mid = (l + r) >> 1;
        if (strcmp(search.c_str(), this->array[mid].str) <= 0)
            r = mid;
        else
            l = mid + 1;
    }
    return l;
}