#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <fstream>
#include "filemanager.h"

using namespace std;

void readFile(char* fileName, char* &data, unsigned long int & dataLength, string dirC) 
{
    string path = dirC + "/" + string(fileName); 
    
    FILE* f = fopen(path.c_str(),"r");

    fseek(f, 0L, SEEK_END);
    dataLength = ftell(f);
    fseek(f, 0L, SEEK_SET);
    data = new char[dataLength];

    fread(data,dataLength,1,f);
    fclose(f);
}

void writeFile(char* fileName, char* data, unsigned long dataLength, string dirC)
{
    string path = dirC + "/" + string(fileName);

    FILE* f = fopen(path.c_str(),"w");
    fwrite(data,dataLength,1,f);
    fclose(f);

}

bool existeFichCliente(const char *fileName)
{
    std::ifstream fichero(fileName);
    return fichero.good();
}

bool existeFichServer(FileManager* fm, string fileName)
{
    vector<string*>* fileList = fm->listFiles();
    
    for(unsigned int i=0;i<fileList->size();++i)
    {
        if (fileName == (*(fileList->at(i)))) //desreferencia
        {
            return true;
        }
    }

    fm->freeListedFiles(fileList); 

    return false;
}

void uploadCmd(FileManager* fm, string dirC)
{
    int success = 1;
    std::string entrada;
    
    while(success != 0)
    {
        std::cout<<"PLEASE INTRODUCE FILE NAME TO UPLOAD: ";
        std::cin>>entrada; 

        if(existeFichCliente((dirC + "/" + entrada).c_str()))  //si el fichero introducido existe en cliente
        { 
            char* data = nullptr;
            unsigned long int dataSize = 0;
            readFile((char*)(entrada.c_str()), data, dataSize, dirC);
            fm->writeFile((char*)(entrada.c_str()), data, dataSize);
            
            delete[] data;
            success = 0; 
        }        
        else if(entrada.compare("quit") == 0)
        {
            break;
        }
        else
        {
            std::cout<<"ERROR: FILE NOT FOUND/FILE DOES NOT EXIST"<<std::endl;
        }
    }
}

void downloadCmd(FileManager* fm, string dirC)
{
    int success = 1;
    std::string entrada;
    
    while(success != 0) 
    {
        std::cout<<"PLEASE INTRODUCE FILE NAME TO DOWNLOAD: ";       
        std::cin>>entrada; 

        if(existeFichServer(fm, entrada)) //si el fichero introducido existe en servidor
        {
            char* data = nullptr;
            unsigned long int dataSize = 0;
            fm->readFile((char *)(entrada.c_str()), data, dataSize);
            writeFile((char *)(entrada.c_str()), data, dataSize, dirC); 

            delete[] data;            
            success = 0;
        }
        else if(entrada.compare("quit") == 0)
        {
            break;
        }
        else
        {
            std::cout<<"ERROR: FILE NOT FOUND/FILE DOES NOT EXIST"<<std::endl;
        }
    }
}

void startInterface()
{   

    std::string dirC;
    std::string dirS;
    std::cout<<"PLEASE INTRODUCE CLIENT DIRECTORY PATH: ";
    std::cin>>dirC; 
    std::cout<<"PLEASE INTRODUCE SERVER DIRECTORY PATH: ";
    std::cin>>dirS; 
    
    FileManager *fm=new FileManager(dirS); //filemanager linkado server
    int success = 1;

    while(1){
        
        std::cout<<">>";
        std::string entrada;
        std::cin>>entrada; 

        if (entrada.compare("ls") == 0)
        {
            vector<string*>* fileList = fm->listFiles();   
            
            for(unsigned int i=0;i<fileList->size();++i)
            {
                cout<<fileList->at(i)->c_str()<<endl;
            }

            fm->freeListedFiles(fileList);     
        }
        else if(entrada.compare("upload") == 0)
        {
            uploadCmd(fm, dirC);
        }
        else if(entrada.compare("download") == 0)
        {
            downloadCmd(fm, dirC);
        }
        else if(entrada.compare("exit") == 0)
        {
            success = 0;
            delete fm;
            break;
        }
        else
        {
            std::cout<<"ERROR: COMMAND DOES NOT EXIST"<<std::endl;
        }
    }
}

int main(){

    startInterface();
    return 0;
}
