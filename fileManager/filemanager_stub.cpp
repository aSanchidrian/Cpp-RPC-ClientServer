#include "filemanager_stub.h"
#include <iostream>
#include <cstdio>
#include <cstdlib>

#define IP_SERVER "3.221.116.174"  //server
#define PORT 40000

using namespace std;

FileManager::FileManager(string path)
{
	serverID = initClient((char*)IP_SERVER, PORT);
	//guardar path enviarlo despues
	this->dirPath = path;
	
	if(serverID < 0) cout << "ERROR" << __FILE__ << ":" << __LINE__ << "\n";
}

vector<string*>* FileManager::listFiles()
{
	int typeOp = OP_LIST;
	sendMSG(serverID, (const void*)&typeOp, sizeof(int));
	
	int numCadenas = 0;
	char* recvBuff = nullptr;
    int* buffNumCadenas = nullptr;
	int recvBuffSize = 0;
	
	//path
	sendMSG(serverID, (const void*)dirPath.c_str(), dirPath.length());
	
	//numCadenas
	recvMSG(serverID, (void**)&buffNumCadenas, &recvBuffSize);
	numCadenas = buffNumCadenas[0];
	delete[] buffNumCadenas;
	
	vector<string*>* res = new vector<string*>();
	
	for(int i = 0; i < numCadenas; i++)
	{
		recvMSG(serverID, (void**)&recvBuff, &recvBuffSize);
		res->push_back(new string(recvBuff));
		delete[] recvBuff;
	}
	
	return res;
}

void FileManager::readFile(char* fileName, char* &data, unsigned long int & dataLength)
{
	int typeOp = OP_READ;
	sendMSG(serverID, (const void*)&typeOp, sizeof(int));
	
	char* recvBuff = nullptr;
	int* buffInteger = nullptr;
	int recvBuffSize = 0;
	
	//fileName
	sendMSG(serverID, (const void*)fileName, sizeof(char)*(strlen(fileName)+1));

	//recibe data
	recvMSG(serverID, (void**)&recvBuff, &recvBuffSize);
	data = new char[recvBuffSize];
	// memcpy(data, recvBuff, recvBuffSize);
	strcpy(data, recvBuff);
	delete[] recvBuff;
	
	//recibe dataLength
	recvMSG(serverID, (void**)&buffInteger, &recvBuffSize);
	dataLength = buffInteger[0];
	delete[] buffInteger;
}

void FileManager::writeFile(char* fileName, char* data, unsigned long int dataLength)
{
	int typeOp = OP_WRITE;
	sendMSG(serverID, (const void*)&typeOp, sizeof(int));
	
    //fileName
	sendMSG(serverID, (const void*)fileName, sizeof(char) * (strlen(fileName)+1));
    //data
	sendMSG(serverID, (const void*)data, sizeof(char)*dataLength);
    //dataLength
	sendMSG(serverID, (const void*)&dataLength, sizeof(unsigned long int));
}

void FileManager::freeListedFiles(vector<string*>* fileList)
{
	int typeOp = OP_FREE;
	sendMSG(serverID, (const void*)&typeOp, sizeof(int));

	int numCadenas = 0;
    //numCadenas
	numCadenas = fileList->size();
	sendMSG(serverID, (const void*)&numCadenas, sizeof(int));
	
	for(int i = 0; i < fileList->size(); i++)
	{
		sendMSG(serverID, (const void*)fileList->at(i)->c_str(), fileList->at(i)->length());
	}
}

FileManager::~FileManager()
{
	int typeOp = OP_END;
	sendMSG(serverID, (const void*)&typeOp, sizeof(int));
	
	closeConnection(serverID);
}