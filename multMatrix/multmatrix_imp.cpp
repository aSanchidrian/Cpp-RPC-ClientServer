#include "multmatrix_imp.h"

void multMatrix_imp::recibeReadMatrix()
{
    char* recvBuff = nullptr;
    int recvBuffSize = 0;

    recvMSG(clientID, (void**)&recvBuff, &recvBuffSize);

    matrix_t* res = m->readMatrix(recvBuff);

    delete[] recvBuff;

    sendMSG(clientID, (const void*)&res->rows, sizeof(int));
    sendMSG(clientID, (const void*)&res->cols, sizeof(int));
    sendMSG(clientID, (const void*)res->data, (sizeof(int) * res->rows * res->cols));
}

void multMatrix_imp::recibeMultMatrix()
{
    matrix_t* m1 = new matrix_t;
    matrix_t* m2 = new matrix_t;
    int* bufferA = nullptr; //buffer matriz 1
    int* bufferB = nullptr; //buffer matriz 2
    int recvBuffSize = 0;

    //recibir primera matriz
    recvMSG(clientID, (void**)&bufferA, &recvBuffSize);
    m1->rows = bufferA[0];
    delete[] bufferA;
    recvMSG(clientID, (void**)&bufferA, &recvBuffSize);
    m1->cols = bufferA[0];
    delete[] bufferA;
    recvMSG(clientID, (void**)&bufferA, &recvBuffSize);
    m1->data = bufferA;

    //segunda matriz
    recvMSG(clientID, (void**)&bufferB, &recvBuffSize);
    m2->rows = bufferB[0];
    delete[] bufferB;
    recvMSG(clientID, (void**)&bufferB, &recvBuffSize);
    m2->cols = bufferB[0];
    delete[] bufferB;
    recvMSG(clientID, (void**)&bufferB, &recvBuffSize);
    m2->data = bufferB;

    matrix_t* res = m->multMatrices(m1, m2);

    sendMSG(clientID, (const void*)&res->rows, sizeof(int));
    sendMSG(clientID, (const void*)&res->cols, sizeof(int));
    sendMSG(clientID, (const void*)res->data, (sizeof(int) * res->rows * res->cols));
}

void multMatrix_imp::recibeWriteMatrix()
{
    matrix_t* m1 = new matrix_t;    //recibe matriz
    char* recvBuff = nullptr;   //recibe filename
    int* buffer = nullptr;  //aux para matriz
    int recvBuffSize = 0;   

    //matriz
    recvMSG(clientID, (void**)&buffer, &recvBuffSize);
    m1->rows = buffer[0];
    delete[] buffer;
    recvMSG(clientID, (void**)&buffer, &recvBuffSize);
    m1->cols = buffer[0];
    delete[] buffer;
    recvMSG(clientID, (void**)&buffer, &recvBuffSize);
    m1->data = buffer;

    //filename
    recvMSG(clientID, (void**)&recvBuff, &recvBuffSize);

    m->writeMatrix(m1, recvBuff);
}

void multMatrix_imp::recibeIdentity()
{
    int* recvBuffA = nullptr;
    int* recvBuffB = nullptr;
    int recvBuffSize = 0;

    recvMSG(clientID, (void**)&recvBuffA, &recvBuffSize);
    recvMSG(clientID, (void**)&recvBuffB, &recvBuffSize);

    matrix_t* res = m->createIdentity(recvBuffA[0], recvBuffB[0]);

    sendMSG(clientID, (const void*)&res->rows, sizeof(int));
    sendMSG(clientID, (const void*)&res->cols, sizeof(int));
    sendMSG(clientID, (const void*)res->data, (sizeof(int) * res->rows * res->cols));
}

void multMatrix_imp::recibeRandom()
{
    int* recvBuffA = nullptr;
    int* recvBuffB = nullptr;
    int recvBuffSize = 0;

    recvMSG(clientID, (void**)&recvBuffA, &recvBuffSize);
    recvMSG(clientID, (void**)&recvBuffB, &recvBuffSize);

    matrix_t* res = m->createRandMatrix(recvBuffA[0], recvBuffB[0]);

    sendMSG(clientID, (const void*)&res->rows, sizeof(int));
    sendMSG(clientID, (const void*)&res->cols, sizeof(int));
    sendMSG(clientID, (const void*)res->data, (sizeof(int) * res->rows * res->cols));
}

multMatrix_imp::multMatrix_imp(int clientID)
{
    m = new multMatrix();
    this->clientID = clientID;

    if(this->clientID < 0) cout<<"ERROR "<<__FILE__<<":"<<__LINE__<<endl;
}

multMatrix_imp::~multMatrix_imp()
{
    delete m;
    closeConnection(this->clientID);
}

void multMatrix_imp::recvOp()
{
    int* typeOp = nullptr;
    int typeOpSize = 0;

    do
    {
        if(typeOp != nullptr) delete[] typeOp;

        recvMSG(clientID, (void**)&typeOp, &typeOpSize);

        switch (typeOp[0])
        {
            case OP_READ:
                recibeReadMatrix();
                break;

            case OP_MULT:
                recibeMultMatrix();
                break;

            case OP_WRITE:
                recibeWriteMatrix();
                break;

            case OP_IDENTITY:
                recibeIdentity();
                break;

            case OP_RAND:
                recibeRandom();
                break;
            
            case OP_END:
                cout<<"DESCONEXION CLIENTE "<<clientID<<endl;
                break;

            default:
                cout<<"ERROR: operacion no valida "<<__FILE__<<":"<<__LINE__<<endl;
                break;
        }
    }while(typeOp[0] != OP_END);

    if(typeOp != nullptr) delete[] typeOp;
}