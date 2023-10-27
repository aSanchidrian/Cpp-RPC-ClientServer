#ifndef  _SERIALIZE_H_
#define  _SERIALIZE_H_

#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <vector>
#include <string.h>

using namespace std;

#define byte char   //cualquier cosa

//identificar bloques de datos
typedef enum dataType
{
    t_error = -1,
    t_char = 0,
    t_float = 1,
    t_int = 2,
    t_string = 3
}dataType;

vector<byte>* serializeVector(vector<string*>* v1, dataType t);
vector<string*>* deserializeVector(vector<byte>* v1, dataType t);

#endif