#pragma once
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <map>
#include <vector>
#include <dirent.h>
#include "utils.h"

#define OP_END 0
#define OP_LIST 1
#define OP_READ 2
#define OP_WRITE 3
#define OP_FREE 4

using namespace std;

class FileManager
{
    string dirPath;
    map<string, string* > files;
    int serverID = -1;

public:
    FileManager(string path);
    vector<string*>* listFiles();
    void readFile(char* fileName, char* &data, unsigned long int & dataLength);
    void writeFile(char* fileName, char* data, unsigned long int dataLength);
    void freeListedFiles(vector<string*>* fileList);
    ~FileManager();
};

