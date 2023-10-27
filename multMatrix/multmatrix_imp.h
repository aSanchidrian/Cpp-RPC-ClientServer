#pragma once
#include "multmatrix.h"
#include "utils.h"
#include "serialize.hpp"

#define OP_END 0
#define OP_READ 1
#define OP_MULT 2
#define OP_WRITE 3
#define OP_IDENTITY 4
#define OP_RAND 5

class multMatrix_imp
{
    private:
        multMatrix* m = nullptr;
        int clientID = -1;
        void recibeReadMatrix();
        void recibeMultMatrix();
        void recibeWriteMatrix();
        void recibeIdentity();
        void recibeRandom();

    public:
        multMatrix_imp(int clientID);
        ~multMatrix_imp();
        void recvOp();
};