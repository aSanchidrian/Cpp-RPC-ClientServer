#pragma once
#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <vector>

using namespace std;

#define byte char   //cualquier cosa

//identificar bloques de datos
typedef enum dataType
{
    t_error = -1,
    t_char = 0,
    t_float = 1,
    t_int = 2
}dataType;

template<typename T>    //puedo pasar cualquier tipo
vector<byte>* serializeVector(vector<T>* v1, dataType t)     //empaquetar los datos en bytes (tipo, datos, tamanio)
{
    vector<byte>* vres = new vector<byte>();
    vres->reserve(sizeof(dataType) + sizeof(int) + v1->size() * sizeof(T));
    vres->resize(sizeof(dataType) + sizeof(int) + v1->size() * sizeof(T));

    //datos
    byte* data = vres->data();  //array de bytes para guardar los datos del vector
    memcpy(data, &t, sizeof(dataType));
    data += sizeof(dataType);

    //num elementos
    int size = v1->size();
    memcpy(data, &size, sizeof(int));
    data += sizeof(int);

    memcpy(data, v1->data(), v1->size() * sizeof(T));

    return vres;
}

template<typename T>
vector<T>* deserializeVector(vector<byte>* v1, dataType t)
{
    vector<T>* vres = new vector<T>();

    dataType tIn = t_error;

    byte* data = v1->data();
    memcpy(&tIn, data, sizeof(dataType));
    data += sizeof(dataType);

    if(tIn != t)
    {
        cout<<"Error en linea "<<__LINE__<<" fichero "<<__FILE__<<endl;
        return nullptr;
    }

    int size = 0;
    memcpy(&size, data, sizeof(int));
    data += sizeof(int);

    vres->reserve(size);
    vres->resize(size);

    memcpy(vres->data(), data, v1->size() * sizeof(T));

    return vres;
}