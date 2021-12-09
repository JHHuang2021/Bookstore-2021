#include "filemap.h"

#include <algorithm>
#include <cstdint>
#include <cstring>
#include <exception>
#include <fstream>
#include <ostream>

#include "error.h"

UllNode::UllNode(const string &isbn, const int &index) {
    strcpy(str_, isbn.c_str());
    this->index_ = index;
}
bool UllNode::Cmp(const UllNode &lhs, const UllNode &rhs) {
    if (strcmp(lhs.str_, rhs.str_) < 0)
        return true;
    else
        return false;
}
bool UllNode::operator==(const UllNode &obj) const {
    if (strcmp(this->str_, obj.str_) == 0 && this->index_ == obj.index_)
        return true;
    return false;
}

UllNode &UllNode::operator=(const UllNode &rhs) {
    this->index_ = rhs.index_;
    strcpy(this->str_, rhs.str_);
    return *this;
}

UllBlock &UllBlock::operator=(const UllBlock &rhs) {
    this->num_ = rhs.num_;
    for (int i = 0; i < num_; i++) this->array_[i] = rhs.array_[i];
    return *this;
}

Ull::Ull(const string &file_name) : file_name_(file_name) {
    ifstream in_1(file_name, ifstream::in);

    if (!in_1) {
        ofstream out(file_name, ofstream::out);
        int block_num = 1;
        UllBlock tmp;
        ffile_.open(file_name, fstream::in | fstream::binary | fstream::out);
        ffile_.write(reinterpret_cast<char *>(&block_num), sizeof(int));
        ffile_.write(reinterpret_cast<char *>(&tmp), sizeof(UllBlock));
        ffile_.close();
    }
}

void Ull::AddNode(const UllNode &book) {
    int block_num;
    ffile_.close();
    ffile_.open(file_name_, fstream::out | fstream::binary | fstream::in);
    ffile_.seekg(0);
    ffile_.read(reinterpret_cast<char *>(&block_num), sizeof(int));
    UllBlock tmp;
    int i = 0, index = 0;

    for (index = 0; index < block_num; index++) {
        ffile_.read(reinterpret_cast<char *>(&tmp), sizeof(UllBlock));
        if (strcmp(tmp.end_, book.str_) >= 0) {
            auto binary_find = lower_bound(tmp.array_, tmp.array_ + tmp.num_,
                                           book, UllNode::Cmp);
            int indexx = binary_find - tmp.array_;
            for (int i = indexx; i < tmp.num_; i++)  // binary_search
                if (strcmp(tmp.array_[i].str_, book.str_) == 0) {
                    if (tmp.array_[i].index_ == book.index_) {
                        ffile_.close();
                        return;
                    }
                } else
                    break;
        }
    }
    ffile_.seekg(sizeof(int));

    for (index = 0; index < block_num; index++) {  // find the block
        ffile_.read(reinterpret_cast<char *>(&tmp), sizeof(UllBlock));
        if (strcmp(book.str_, tmp.end_) <= 0) break;
    }
    if (index == block_num) index--;
    auto binary_find =
        lower_bound(tmp.array_, tmp.array_ + tmp.num_, book, UllNode::Cmp);
    i = binary_find - tmp.array_;
    // update the bound
    for (int j = tmp.num_ - 1; j >= i; j--)
        tmp.array_[j + 1] = tmp.array_[j];  // move and copy
    tmp.array_[i] = book;
    tmp.num_++;

    // operations that might make the program slower
    strcpy(tmp.start_, tmp.array_[0].str_);
    strcpy(tmp.end_, tmp.array_[tmp.num_ - 1].str_);
    if (tmp.num_ <= BLOCK_SPLIT_THRESHOLD) {
        ffile_.seekp(sizeof(int) + index * sizeof(UllBlock));
        ffile_.write(reinterpret_cast<char *>(&tmp), sizeof(UllBlock));
    } else {  // split the block and rewrite
        SplitBlock(tmp, index);
        block_num++;
        ffile_.seekp(0);
        ffile_.write(reinterpret_cast<char *>(&block_num), sizeof(int));
    }
    ffile_.close();
}

void Ull::SplitBlock(UllBlock &obj, const int &index) {  // to be checked
    int block_num, put_index = 0;
    // ffile.open(file_name, fstream::out | fstream::binary | fstream::in);
    ffile_.seekg(0);
    ffile_.read(reinterpret_cast<char *>(&block_num), sizeof(int));
    put_index = block_num;
    block_num++;
    UllBlock tmp;
    for (int i = BLOCK_SPLIT_LEFT; i < obj.num_; i++)
        tmp.array_[i - BLOCK_SPLIT_LEFT] = obj.array_[i];
    tmp.num_ = obj.num_ - BLOCK_SPLIT_LEFT;
    obj.num_ = BLOCK_SPLIT_LEFT;
    strcpy(obj.end_, obj.array_[obj.num_ - 1].str_);
    strcpy(tmp.start_, tmp.array_[0].str_);
    strcpy(tmp.end_, tmp.array_[tmp.num_ - 1].str_);
    ffile_.seekp(0);
    ffile_.write(reinterpret_cast<char *>(&block_num), sizeof(int));
    ffile_.seekp(sizeof(int) + index * sizeof(UllBlock));
    ffile_.write(reinterpret_cast<char *>(&obj), sizeof(UllBlock));
    ffile_.seekp(sizeof(int) + put_index * sizeof(UllBlock));
    ffile_.write(reinterpret_cast<char *>(&tmp), sizeof(UllBlock));
}

void Ull::FindNode(const string &key, set<int> &tp) {
    int block_num;
    tp.clear();
    ffile_.close();
    ffile_.open(file_name_, fstream::out | fstream::binary | fstream::in);
    ffile_.seekg(0);
    ffile_.read(reinterpret_cast<char *>(&block_num), sizeof(int));
    UllBlock tmp;
    int index = 0;

    for (index = 0; index < block_num; index++) {
        ffile_.read(reinterpret_cast<char *>(&tmp), sizeof(UllBlock));
        if (strcmp(tmp.end_, key.c_str()) >= 0) {
            auto binary_find = lower_bound(tmp.array_, tmp.array_ + tmp.num_,
                                           UllNode(key, 0), UllNode::Cmp);
            int indexx = binary_find - tmp.array_;
            for (int i = indexx; i < tmp.num_; i++)
                if (strcmp(tmp.array_[i].str_, key.c_str()) != 0)
                    break;
                else
                    tp.insert(tmp.array_[i].index_);
        }
    }
    ffile_.close();
}

void Ull::DeleteNode(const UllNode &node) {
    int block_num;
    ffile_.close();
    ffile_.open(file_name_, fstream::out | fstream::binary | fstream::in);
    ffile_.seekg(0);
    ffile_.read(reinterpret_cast<char *>(&block_num), sizeof(int));
    UllBlock tmp;
    int i = 0, index = 0;
    for (index = 0; index < block_num; index++) {  // find the block
        ffile_.read(reinterpret_cast<char *>(&tmp), sizeof(UllBlock));
        if (strcmp(node.str_, tmp.end_) <= 0) {
            // int indexx = tmp.binary_search(node.str) + 1;
            // if (node == tmp.array[0]) indexx = 0;
            for (i = 0; i < tmp.num_; i++)
                // find the position in the block
                if (node == tmp.array_[i]) break;
            if (i == tmp.num_) continue;

            strcpy(tmp.array_[i].str_, "");
            tmp.array_[i].index_ = 0;

            for (int j = i + 1; j < tmp.num_; j++)
                tmp.array_[j - 1] = tmp.array_[j];
            tmp.array_[tmp.num_ - 1].index_ = 0;
            strcpy(tmp.array_[tmp.num_ - 1].str_, "");
            tmp.num_--;

            strcpy(tmp.start_, tmp.array_[0].str_);
            strcpy(tmp.end_, tmp.array_[tmp.num_ - 1].str_);
            ffile_.seekp(sizeof(int) + index * sizeof(UllBlock));
            ffile_.write(reinterpret_cast<char *>(&tmp), sizeof(UllBlock));
            ffile_.close();
            return;
        }
    }
    ffile_.close();
}

template <class T>
MainInfo<T>::MainInfo(string file_name) {
    this->file_name_ = file_name;
    ifstream in_1(file_name, ifstream::in);

    if (!in_1) {
        ofstream out(file_name, ofstream::out);
        out.close();
    }

    Ull key_index(file_name + "_ull");
}

template <class T>
MainInfo<T>::~MainInfo<T>() {}

template <class T>
void MainInfo<T>::WriteInfo(T info, string &key) {
    Ull key_index(file_name_ + "_ull");
    set<int> find;
    key_index.FindNode(key, find);
    if (!find.empty()) throw Error();  // todo
    fstream ffile(file_name_, fstream::in | fstream::binary | fstream::out);
    ffile.seekp(0, ios::end);
    ffile.write(reinterpret_cast<char *>(&info), sizeof(T));

    int index = (ffile.end - ffile.beg) / sizeof(T) - 1;
    ffile.close();
    key_index.AddNode(UllNode(key, index));
}

template <class T>
void MainInfo<T>::DeleteInfo(T &info, string &key) {
    Ull key_index(file_name_ + "_ull");
    set<int> find;
    key_index.FindNode(key, find);
    if (find.empty()) throw Error();  // todo
    key_index.DeleteNode(UllNode(key, *find.begin()));
}
template <class T>
T MainInfo<T>::FindInfo(string &key) {
    Ull key_index(file_name_ + "_ull");
    set<int> find;
    key_index.FindNode(key, find);
    if (find.empty()) throw Error();
    T tmp;
    fstream ffile(file_name_, fstream::in | fstream::binary | fstream::out);
    ffile.seekg(*find.begin() * sizeof(T));
    ffile.read(reinterpret_cast<char *>(&tmp), sizeof(T));
    ffile.close();
    return tmp;
}