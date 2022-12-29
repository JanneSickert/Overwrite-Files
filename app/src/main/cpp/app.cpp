#pragma once
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <stdio.h>
#include <cstdio>

#define STRING_END '\0'
#define NEW_LINE 0x5c

using namespace std;

const char PATH_TXT[] = "Paths.txt";
vector<string> paths;
unsigned int nr = 0;
vector<string> logg;

void parseFile();
void start();

int main()
{
    cout << "Overwrite-Files started" << endl;
    start();
    cout << "Overwrite-Files ende" << endl;
    return 0;
}

class Operations
{
public:
    string path;
    string name;
    string folder;
    int var_k;

    virtual void calc(int k) = 0;
    virtual void printVars() = 0;
    virtual void overwrite() = 0;
    virtual void renaimFile() = 0;
    virtual void deleteFile() = 0;
};

class Test : public Operations
{
public:
    void calc(int k) override
    {
        ifstream file(paths[k]);
        if (!file.good())// exists
        {
            logg.push_back(path + " do not exists");
        }
        if (!((folder + "/" + name) == path))
        {
            logg.push_back(path + " is not " + (folder + "/" + name));
        }
    }

    void printVars() override
    {}

    void overwrite() override
    {
        ifstream file(path, ios::binary);
        if (file.good()) {
            file.seekg(0, ios::end);
            std::streampos fileSize = file.tellg();
            file.seekg(0, ios::beg);
            char* buffer = new char[fileSize];
            file.read(buffer, fileSize);
            while (*buffer != STRING_END) {
                if (*buffer != 0x0) {
                    logg.push_back(path + " fehlerhalft überschrieben ");
                }
                ++buffer;
            }
            delete[] buffer;
        }
        else {
            logg.push_back("Failed to open file " + path);
        }
    }

    void renaimFile() override
    {}

    void deleteFile() override
    {
        ifstream file(path, ios::binary);
        if (file.good()) {
            logg.push_back(path + " wurde nicht gelöscht");
        }
    }
};

class File : public Operations
{
private:
    int getFileLength()
    {
        ifstream in_file(path, ios::binary);
        in_file.seekg(0, std::ios::end);
        int file_size = in_file.tellg();
        return file_size;
    }

public:
    void calc(int k) override
    {
        path = paths[k];
        var_k = k;
        int lastSlash = -1;
        for (int i = path.size() - 1; i >= 0; --i) {
            if (path[i] == 0x2F || path[i] == 0x5C) {
                lastSlash = i;
                break;
            }
        }
        name = path.substr(lastSlash + 1, string::npos);
        folder = path.substr(0, lastSlash);
    }

    void overwrite() override
    {
        fstream file(path, ios::in | ios::out);
        if (file.is_open()) {
            cout << "Die Datei wurde geöffnet." << endl;
        }
        else {
            cout << "Die Datei wurde nicht geöffnet." << endl;
        }
        file.seekp(0);
        int size = getFileLength();
        for (int i = 0; i < size; ++i) {
            file.put(0x0);
        }
        file.close();
    }

    void renaimFile() override
    {
        string newName = folder + "/" + to_string(var_k);
        int result = rename(path.c_str(), newName.c_str());
        if (result == 0) {
            cout << "File was renamed successfully." << endl;
        }
        else {
            cout << "Error renaming file." << endl;
        }
    }

    void printVars() override
    {
        cout << path << endl;
        cout << name << endl;
        cout << folder << endl;
        cout << endl << endl;
    }

    void deleteFile() override
    {
        remove(path.c_str());
    }
};

template <typename T>
class Stack : public File, public Test
{
public:
    void calc(int k) override
    {
        T::calc(k);
    }

    void printVars() override
    {
        T::printVars();
    }

    void overwrite() override
    {
        T::overwrite();
    }

    void renaimFile() override
    {
        T::renaimFile();
    }

    void deleteFile() override
    {
        T::deleteFile();
    }

    void action(int k)
    {
        calc(k);
        printVars();
        overwrite();
        renaimFile();
    }
};

void start()
{
    parseFile();
    Stack<File> f;
    Stack<Test> t;
    for (int i = 0; i < nr; ++i) {
        f.action(i);
        t.action(i);
    }
    f.deleteFile();
    t.deleteFile();
    cout << endl << endl;
    for (int i = 0; i < logg.size(); ++i) {
        cout << "ERROR: " << logg[i] << endl;
    }
}

void parseFile()
{
    fstream pathFile;
    pathFile.open(PATH_TXT, std::ios::in);
    if (pathFile.is_open()) {
        string buffer;
        while (getline(pathFile, buffer)) {
            paths.push_back(buffer);
            ++nr;
        }
        pathFile.close();
    }
    else {
        cout << "ERROR: " << PATH_TXT << " is not open.";
        exit(1);
    }
}