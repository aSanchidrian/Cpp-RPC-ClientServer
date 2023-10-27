#pragma once
#include "filemanager.h"
#include "utils.h"
#include "serialize.h"

#define OP_END 0
// #define OP_CONSTRUCTOR 1
#define OP_LIST 1
#define OP_READ 2
#define OP_WRITE 3
#define OP_FREE 4

using namespace std;

class FileManager_imp
{

    private:
        FileManager* fm = nullptr;
        int clientID = -1;
        // void recibeFileManager(); //mandar por listFiles
        void recibeListFiles();
        void recibeReadFiles();
        void recibeWriteFiles();
        void recibeFreeFiles();
        

    public:
        FileManager_imp(int clientID);
        ~FileManager_imp();
        void recvOp();
};