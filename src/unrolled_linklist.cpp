#include "unrolled_linklist.hpp"

// #include <cstdint>
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
        tmp.if_occupied = true;
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
    ffile.open(file_name, fstream::out | fstream::binary | fstream::in);
    ffile.seekg(0);
    ffile.read(reinterpret_cast<char *>(&block_num), sizeof(int));
    UllBlock tmp;
    int i = 0, index = 0;

    for (index = 0; index < block_num; index++) {
        ffile.read(reinterpret_cast<char *>(&tmp), sizeof(UllBlock));
        if (!tmp.if_occupied) continue;
        if (strcmp(tmp.end, book.str) >= 0) {
            for (int i = 0; i < tmp.num; i++)
                if (tmp.array[i] == book) {
                    ffile.close();
                    return;
                }
        }
    }
    ffile.seekg(sizeof(int));

    for (index = 0; index < block_num; index++) {  // find the block
        ffile.read(reinterpret_cast<char *>(&tmp), sizeof(UllBlock));
        if (!tmp.if_occupied) continue;
        if (strcmp(book.str, tmp.end) <= 0) break;
    }
    if (index == block_num) index--;
    for (i = 0; i < tmp.num; i++)  // find the position in the block
        if (strcmp(book.str, tmp.array[i].str) == -1) break;

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
    tmp.if_occupied = true;
    for (int i = BLOCK_SPLIT_LEFT; i < obj.num; i++)
        tmp.array[i - BLOCK_SPLIT_LEFT] = obj.array[i];
    tmp.num = obj.num - BLOCK_SPLIT_LEFT;
    obj.num = BLOCK_SPLIT_LEFT;
    strcpy(obj.end, obj.array[obj.num - 1].str);
    strcpy(tmp.start, tmp.array[0].str);
    strcpy(tmp.end, tmp.array[tmp.num - 1].str);
    // obj.nxt = put_index;
    ffile.seekp(0);
    ffile.write(reinterpret_cast<char *>(&block_num), sizeof(int));
    ffile.seekp(sizeof(int) + index * sizeof(UllBlock));
    ffile.write(reinterpret_cast<char *>(&obj), sizeof(UllBlock));
    ffile.seekp(sizeof(int) + put_index * sizeof(UllBlock));
    ffile.write(reinterpret_cast<char *>(&tmp), sizeof(UllBlock));
}

void Ull::findNode(const string &key, set<int> &array) {
    int block_num;
    ffile.open(file_name, fstream::out | fstream::binary | fstream::in);
    ffile.seekg(0);
    ffile.read(reinterpret_cast<char *>(&block_num), sizeof(int));
    UllBlock tmp;
    int index = 0;

    /* only for test*/
    // for (index = 0; index < block_num; index++) {
    //     ffile.read(reinterpret_cast<char *>(&tmp), sizeof(UllBlock));
    //     cout << "block_index"
    //          << " " << index << endl;
    //     for (int i = 0; i < tmp.num; i++)
    //         cout << tmp.array[i].str << " " << tmp.array[i].index << endl;
    // }
    // ffile.seekg(sizeof(int));

    for (index = 0; index < block_num; index++) {
        ffile.read(reinterpret_cast<char *>(&tmp), sizeof(UllBlock));
        if (!tmp.if_occupied) continue;
        if (strcmp(tmp.end, key.c_str()) >= 0) {
            for (int i = 0; i < tmp.num; i++)
                if (strcmp(tmp.array[i].str, key.c_str()) == 0)
                // to be modified
                {
                    // cout << i << endl;
                    array.insert(tmp.array[i].index);
                }
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
        if (!tmp.if_occupied) continue;
        if (strcmp(node.str, tmp.end) <= 0) {
            for (i = 0; i < tmp.num; i++)  // find the position in the block
                if (node == tmp.array[i]) break;
            if (i == tmp.num) continue;

            for (int j = i + 1; j < tmp.num; j++)  //
                tmp.array[j - 1] = tmp.array[j];
            tmp.num--;
            strcpy(tmp.start, tmp.array[0].str);
            strcpy(tmp.end, tmp.array[tmp.num - 1].str);
            // if (tmp.num > BLOCK_MERGE_THRESHOLD || block_num == 1) {
            ffile.seekp(sizeof(int) + index * sizeof(UllBlock));
            ffile.write(reinterpret_cast<char *>(&tmp), sizeof(UllBlock));
            ffile.close();
            return;
        }
    }
    ffile.close();
}

// types of args can be modified if the program is too slow
void Ull::mergeBlock(UllBlock &tmp1, UllBlock &tmp2, const int &index1,
                     const int &index2) {
    // index1 <- index2
    int block_num, put_index = 0;
    ffile.seekg(0);
    ffile.read(reinterpret_cast<char *>(&block_num), sizeof(int));
    file_spare.open(fio_name, fstream::in | fstream::out | fstream::binary);
    file_spare.seekg(0);
    file_spare.read(reinterpret_cast<char *>(spare_block_index),
                    sizeof(spare_block_index));
    spare_block_index[0]++;
    spare_block_index[spare_block_index[0]] = index2;

    file_spare.seekp(0);
    file_spare.write(reinterpret_cast<char *>(spare_block_index),
                     sizeof(spare_block_index));
    file_spare.close();
    // copy
    for (int i = 0; i < tmp2.num; i++) tmp1.array[tmp1.num + i] = tmp2.array[i];
    // tmp1.nxt = tmp2.nxt;
    tmp1.num += tmp2.num;
    strcpy(tmp1.end, tmp2.end);

    tmp2.if_occupied = false;

    // note: split if exceed the thershold
    ffile.seekp(sizeof(int) + index2 * sizeof(UllBlock));
    ffile.write(reinterpret_cast<char *>(&tmp2), sizeof(UllBlock));
    if (tmp1.num > BLOCK_SPLIT_THRESHOLD)
        splitBlock(tmp1, index1);
    else {
        // write
        block_num--;
        ffile.seekp(0);
        ffile.write(reinterpret_cast<char *>(&block_num), sizeof(int));
        ffile.seekp(sizeof(int) + index1 * sizeof(UllBlock));
        ffile.write(reinterpret_cast<char *>(&tmp1), sizeof(UllBlock));
    }
}

void Ull::show() {
    ffile.open(file_name, fstream::out | fstream::binary | fstream::in);
    int block_num;
    ffile.seekg(0);
    ffile.read(reinterpret_cast<char *>(&block_num), sizeof(int));
    for (int i = 1; i <= block_num; i++) {
        UllBlock tmp;
        ffile.read(reinterpret_cast<char *>(&tmp), sizeof(UllBlock));
        cout << "block" << i << " if occupied " << tmp.if_occupied << " ";
        for (int i = 0; i < tmp.num; i++) cout << tmp.array[i].index << " ";
        cout << endl;
    }
    ffile.close();
}