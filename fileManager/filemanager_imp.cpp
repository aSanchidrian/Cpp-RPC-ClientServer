#include "filemanager_imp.h"

void FileManager_imp::recibeListFiles()
{
	char* recvBuff = nullptr;
    char* path = nullptr;
	int recvBuffSize = 0;
	int numCadenas = 0;
	
	vector<string*>* res = new vector<string*>();
	
	//path
	recvMSG(clientID, (void**)&recvBuff, &recvBuffSize);
	path = new char[recvBuffSize];
	strcpy(path, recvBuff);
	delete[] recvBuff;

	this->fm = new FileManager(path);
	res = fm->listFiles();
	
	//numCadenas
	numCadenas = res->size();
	sendMSG(clientID, (const void*)&numCadenas, sizeof(int));
	
	for(int i = 0; i < res->size(); i++)
	{
		sendMSG(clientID, (const void*)res->at(i)->c_str(), res->at(i)->length());
	}
}

void FileManager_imp::recibeReadFiles()
{
	char* recvBuff = nullptr;
	int recvBuffSize = 0;
	char* fileName = nullptr;
	char* data = nullptr;
	unsigned long int dataLength = 0;
	
	recvMSG(clientID, (void**)&recvBuff, &recvBuffSize);
	fileName = new char[recvBuffSize];
	strcpy(fileName, recvBuff);
	delete[] recvBuff;
	
	fm->readFile(fileName, data, dataLength);
	
	sendMSG(clientID, (const void*)data, strlen(data)+1);
	sendMSG(clientID, (const void*)&dataLength, sizeof(int));
}

void FileManager_imp::recibeWriteFiles()
{
	int* dataLengthBuff = nullptr;
	char* recvBuff = nullptr;
	int recvBuffSize = 0;
	
	char* fileName = nullptr;
	char* data = nullptr;
	unsigned long int dataLength = 0;
	
	recvMSG(clientID, (void**)&recvBuff, &recvBuffSize);
	fileName = new char[recvBuffSize];
	strcpy(fileName, recvBuff);
	delete[] recvBuff;
	
	recvMSG(clientID, (void**)&recvBuff, &recvBuffSize);
	data = new char[recvBuffSize];
	strcpy(data, recvBuff);
	delete[] recvBuff;
	
	recvMSG(clientID, (void**)&dataLengthBuff, &recvBuffSize);
	dataLength = dataLengthBuff[0];
	delete[] dataLengthBuff;
	
	fm->writeFile(fileName, data, dataLength);
}

void FileManager_imp::recibeFreeFiles()
{
	int* buffNumCadenas = nullptr;
	char* buffRcvChar = nullptr;
	int resBuffLen = 0;
	int numCadenas = 0;
    
	vector<string*>* flist = new vector<string*>();
	
    //numCadenas
	recvMSG(clientID, (void**)&buffNumCadenas, &resBuffLen);
	numCadenas = buffNumCadenas[0];
	delete[] buffNumCadenas;
	
	for(int i = 0; i < numCadenas; i++)
	{
		recvMSG(clientID, (void**)&buffRcvChar, &resBuffLen);
		flist->push_back(new string(buffRcvChar));
		delete[] buffRcvChar;
	}
	
	fm->freeListedFiles(flist);
}

FileManager_imp::FileManager_imp(int clientID)
{
	this->clientID = clientID;
	
	if(clientID < 0) cout << "ERROR" << __FILE__ << ":" << __LINE__ << "\n";
}

FileManager_imp::~FileManager_imp()
{
	closeConnection(this->clientID);
    delete fm;
}

void FileManager_imp::recvOp()
{
    int* typeOp = nullptr;
    int typeOpSize = 0;

    do
    {
        if(typeOp != nullptr) delete[] typeOp;

        recvMSG(clientID, (void**)&typeOp, &typeOpSize);

        switch (typeOp[0])
        {
            case OP_LIST:
                recibeListFiles();
                break;

            case OP_READ:
                recibeReadFiles();
                break;

            case OP_WRITE:
                recibeWriteFiles();
                break;

            case OP_FREE:
                recibeFreeFiles();
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