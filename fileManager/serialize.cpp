#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <vector>
#include <algorithm>
#include "serialize.h"

using namespace std;

vector<byte>* serializeVector(vector<string*>* v1, dataType t)     //empaquetar los datos en bytes (tipo, datos, tamanio)
{
    int tamFinal = 0;
    vector<byte>* vres = new vector<byte>();
    
    cout << "SIZE = " << vres->size() << endl;
    vres->reserve(sizeof(dataType) + sizeof(int));
    // vres->resize(sizeof(dataType) + sizeof(int));
    cout << "SIZE = " << vres->size() << endl;

    tamFinal += sizeof(dataType) + sizeof(int);

    //recibir tipo (int)
    byte* data = vres->data();
    vres->push_back(t);
    // memcpy(data, &t, sizeof(dataType));

    cout << "SIZE AFTER dataType = " << vres->size() << endl;
    cout << "DATA = " << vres->data() << endl;

    for(int i = 0; i < vres->size(); i++)
    {
        cout << (char)vres->data()[i] << " "; 
    }
    cout << endl;

    cout << "FFS = " << vres->data()[0] << endl;

    data += sizeof(dataType);
    cout<<"TIPO: "<<t<<endl;

    //recibir numero de cadenas (int)
    int numCadenas = v1->size();
    memcpy(data, &numCadenas, sizeof(int));
    data += sizeof(int);
    cout<<"NCADENAS: "<<numCadenas<<endl;
    cout << "DATA = " << data;
    

    for(unsigned int i = 0; i < numCadenas; ++i)
    {
        //tamanio de la cadena (int)
        int tam = v1->at(i)->length() + 1;
        //cadena (c_str)
        const char* str = v1->at(i)->c_str();

        //reservar tamanio para todo
        vres->reserve(tamFinal + sizeof(int) + tam * sizeof(byte));
        tamFinal += sizeof(int) + tam * sizeof(byte);

        memcpy(data, &tam, sizeof(int));
        data += sizeof(int);
        memcpy(data, str, tam * sizeof(byte));
        data += tam * sizeof(byte);
    }

    vres->resize(tamFinal);

    std::cout << "Holi";
    for (auto const i: *vres) {
        std::cout << i << " ";
    }
    std::cout << "Adios";


    return vres;
}

vector<string*>* deserializeVector(vector<byte>* v1, dataType t)
{
    int tamFinal = 0;
    vector<string*>* vres = new vector<string*>();

    dataType tIn = t_error;

    byte* data = v1->data();
    memcpy(&tIn, data, sizeof(dataType));
    data += sizeof(dataType);

    cout<<"TIPO: "<<tIn<<endl;

    if(tIn != t)
    {
        cout<<"Error en linea "<<__LINE__<<" fichero "<<__FILE__<<endl;
        return nullptr;
    }

    int size = 0;
    memcpy(&size, data, sizeof(int));
    data += sizeof(int);

    // for(unsigned int i = 0; i < size; ++i)
    // {

    // }

    vres->reserve(size);
    vres->resize(size);

    memcpy(vres->data(), data, v1->size() * sizeof(string*));

    return vres;
}