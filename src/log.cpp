#include "log.h"

#include <bits/stdc++.h>

#include <cstdlib>
#include <cstring>
#include <fstream>
#include <iomanip>

#include "error.h"
using namespace std;
Log::Log(string file_name) {
    this->file_name_ = file_name;
    ifstream in_1(file_name, ifstream::in);

    if (!in_1) {
        ofstream out(file_name, ofstream::out);
        int num = 0;
        fstream ffile(file_name, fstream::in | fstream::binary | fstream::out);
        ffile.write(reinterpret_cast<char *>(&num), sizeof(int));
        ffile.close();
    }
}

Log::~Log() {}

void Log::Record(string rec) {
    int num = 0;
    fstream ffile(file_name_, fstream::in | fstream::binary | fstream::out);
    ffile.seekg(0);
    ffile.read(reinterpret_cast<char *>(&num), sizeof(int));
    num++;
    ffile.seekp(0);
    ffile.write(reinterpret_cast<char *>(&num), sizeof(int));
    char rec_char[15];
    strcpy(rec_char, rec.c_str());
    ffile.seekp(sizeof(int) + (num - 1) * sizeof(rec_char));
    ffile.write(reinterpret_cast<char *>(rec_char), sizeof(rec_char));
    ffile.close();
}

void Log::ShowFinance(int times) {
    if (times == 0) {
        cout << "\n";
        return;
    }
    int num = 0;
    fstream ffile(file_name_, fstream::in | fstream::binary | fstream::out);
    ffile.seekg(0);
    ffile.read(reinterpret_cast<char *>(&num), sizeof(int));
    if (times > num)
        throw Error();
    else if (times == -1)
        times = num;
    double in = 0, out = 0;
    for (int i = num - 1; i >= num - times; i--) {
        char rec_char[15];
        ffile.seekg(sizeof(int) + i * sizeof(rec_char));
        ffile.read(reinterpret_cast<char *>(rec_char), sizeof(rec_char));
        double bill = atof(rec_char);
        if (bill < 0)
            out += bill;
        else
            in += bill;
    }
    ffile.close();
    if (out < 0)
        cout << "+ " << fixed << setprecision(2) << in << " "
             << "- " << fixed << setprecision(2) << (-out) << "\n";
    else
        cout << "+ " << fixed << setprecision(2) << in << " "
             << "- " << fixed << setprecision(2) << (out) << "\n";
}