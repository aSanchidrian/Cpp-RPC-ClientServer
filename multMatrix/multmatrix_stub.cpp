#include "multmatrix_stub.h"
#include "utils.h"

using namespace std;

#define IP_SERVER "3.221.116.174"  //server
#define PORT 40000

multMatrix::multMatrix()
{
    serverID = initClient((char*)IP_SERVER, PORT);
    if(serverID < 0) cout<<"STUB ERROR "<<__FILE__<<":"<<__LINE__<<endl;
}

matrix_t* multMatrix::readMatrix(const char* fileName)
{
    int typeOp = OP_READ;
    sendMSG(serverID, (const void*)&typeOp, sizeof(int));

    //madar filename
    sendMSG(serverID, (const void*)fileName, (sizeof(char) * strlen(fileName)+1));

    matrix_t* res = new matrix_t;
    int* recvBuff=nullptr;
    int recvBuffSize = 0;

    recvMSG(serverID, (void**)&recvBuff, &recvBuffSize);
    res->rows=recvBuff[0];
    delete[] recvBuff;
    recvMSG(serverID, (void**)&recvBuff, &recvBuffSize);
    res->cols=recvBuff[0];
    delete[] recvBuff;
    recvMSG(serverID, (void**)&recvBuff, &recvBuffSize);
    res->data=recvBuff;

    return res;
}

matrix_t* multMatrix::multMatrices(matrix_t* m1, matrix_t *m2)
{
    int typeOp = OP_MULT;
    sendMSG(serverID, (const void*)&typeOp, sizeof(int));

    //enviar matrices
    //primera matriz
    sendMSG(serverID, (const void*)&m1->rows, sizeof(int)); //filas
    sendMSG(serverID, (const void*)&m1->cols, sizeof(int)); //columnas
    sendMSG(serverID, (const void*)m1->data, (sizeof(int) * m1->rows * m1->cols));

    //segunda matriz
    sendMSG(serverID, (const void*)&m2->rows, sizeof(int)); //filas
    sendMSG(serverID, (const void*)&m2->cols, sizeof(int)); //columnas
    sendMSG(serverID, (const void*)m2->data, (sizeof(int) * m2->rows * m2->cols));

    matrix_t* res = new matrix_t;
    int* recvBuff=nullptr;
    int recvBuffSize = 0;

    recvMSG(serverID, (void**)&recvBuff, &recvBuffSize);
    res->rows=recvBuff[0];
    delete[] recvBuff;
    recvMSG(serverID, (void**)&recvBuff, &recvBuffSize);
    res->cols=recvBuff[0];
    delete[] recvBuff;
    recvMSG(serverID, (void**)&recvBuff, &recvBuffSize);
    res->data=recvBuff;

    return res;
}

void multMatrix::writeMatrix(matrix_t* m, const char *fileName)
{
    int typeOp = OP_WRITE;
    sendMSG(serverID, (const void*)&typeOp, sizeof(int));

    //matriz
    sendMSG(serverID, (const void*)&m->rows, sizeof(int)); //filas
    sendMSG(serverID, (const void*)&m->cols, sizeof(int)); //columnas
    sendMSG(serverID, (const void*)m->data, (sizeof(int) * m->rows * m->cols));

    //filename
    sendMSG(serverID, (const void*)fileName, sizeof(char) * strlen(fileName));
}

multMatrix::~multMatrix()
{
    int typeOp = OP_END;

    sendMSG(serverID, (const void*)&typeOp, sizeof(int));
    closeConnection(serverID);
}

matrix_t* multMatrix::createIdentity(int rows, int cols)
{
    int typeOp = OP_IDENTITY;
    sendMSG(serverID, (const void*)&typeOp, sizeof(int));

    //mandar filas y columnas
    sendMSG(serverID, (const void*)&rows, sizeof(int));
    sendMSG(serverID, (const void*)&cols, sizeof(int));

    matrix_t* res = new matrix_t;
    int* recvBuff=nullptr;
    int recvBuffSize = 0;

    recvMSG(serverID, (void**)&recvBuff, &recvBuffSize);
    res->rows=recvBuff[0];
    delete[] recvBuff;
    recvMSG(serverID, (void**)&recvBuff, &recvBuffSize);
    res->cols=recvBuff[0];
    delete[] recvBuff;
    recvMSG(serverID, (void**)&recvBuff, &recvBuffSize);
    res->data=recvBuff;

    return res;
}

matrix_t* multMatrix::createRandMatrix(int rows, int cols)
{
    int typeOp = OP_RAND;
    sendMSG(serverID, (const void*)&typeOp, sizeof(int));
    
    //mandar filas y columnas
    sendMSG(serverID, (const void*)&rows, sizeof(int));
    sendMSG(serverID, (const void*)&cols, sizeof(int));

    matrix_t* res = new matrix_t;
    int* recvBuff=nullptr;
    int recvBuffSize = 0;

    recvMSG(serverID, (void**)&recvBuff, &recvBuffSize);
    res->rows=recvBuff[0];
    delete[] recvBuff;
    recvMSG(serverID, (void**)&recvBuff, &recvBuffSize);
    res->cols=recvBuff[0];
    delete[] recvBuff;
    recvMSG(serverID, (void**)&recvBuff, &recvBuffSize);
    res->data=recvBuff;

    return res;
}
