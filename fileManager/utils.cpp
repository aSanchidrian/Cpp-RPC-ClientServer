#include "utils.h"

int idGenerator=0;
std::map<int, connection_t*>  clientList;
bool closeAndExit=false;
std::thread* waitForConnectionsThread;
std::list<int> pendingConnections;


int initServer(int port)
{
    closeAndExit=false;
    int sock_fd;
    sock_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (sock_fd < 0)
    {
       printf("Error creating socket\n");
    }
   struct sockaddr_in serv_addr;
   serv_addr.sin_family = AF_INET;
   serv_addr.sin_addr.s_addr = INADDR_ANY;
   serv_addr.sin_port = htons(port);

   int option = 1;
   setsockopt(sock_fd,SOL_SOCKET,
              (SO_REUSEPORT | SO_REUSEADDR),
              &option,sizeof(option));

   if (bind(sock_fd,(struct sockaddr *) &serv_addr,
         sizeof(serv_addr)) < 0)
         printf("ERROR on binding");
   listen(sock_fd,5);


   waitForConnectionsThread=new std::thread(waitForConnectionsAsync,sock_fd);
   return sock_fd;
}



int initClient(char* host, int port)
{
    int sock_out = 0; 
    struct sockaddr_in serv_addr; 
  
    if ((sock_out = socket(AF_INET, SOCK_STREAM, 0)) < 0) 
    { 
        printf("\n Socket creation error \n"); 
        return -1; 
    } 
   
    serv_addr.sin_family = AF_INET; 
    serv_addr.sin_port = htons(port); 
       
    // Convert IPv4 and IPv6 addresses from text to binary form 
    if(inet_pton(AF_INET, host, &serv_addr.sin_addr)<=0)  
    { 
        printf("\nInvalid address/ Address not supported \n"); 
        return -1; 
    } 
   
    if (connect(sock_out, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) 
    { 
        printf("\nConnection Failed \n"); 
        return -1; 
    } 
    
    connection_t* conn=new connection_t[1];

    read(sock_out,&conn->clientID,sizeof(int));
    conn->socket_fd=sock_out;

    conn->buffer=new std::list<dataPacket_t*>();

    clientList[conn->clientID]=conn;

    return conn->clientID;
}


void waitForConnectionsAsync(int sock_fd)
{
    //OJO: CERROJOS
    while(!closeAndExit){
        int newClient=waitForConnections(sock_fd);
        pendingConnections.push_back(newClient);
    }
}

void recvMSGAsync(int clientID)
{
    connection_t* conn= clientList[clientID];

    while (conn->alive) {
        dataPacket_t* packet=new dataPacket_t[1];
        recvMSG(clientID,(void**)&(packet->data),&(packet->dataSize));
        conn->buffer->push_back(packet);
    }
}


int getCountMessages(int clientID)
{
    connection_t* conn= clientList[clientID];
    return conn->buffer->size();
}
bool checkMessage(int clientID)
{
    return getCountMessages(clientID)>0;
}


void popMSG(int clientID,void** data, int* dataLen)
{

    if(!checkMessage(clientID))
    {
        *data=NULL;
        *dataLen=0;
        return ;
    }
    else
    {
        connection_t* conn= clientList[clientID];
        dataPacket_t * packet=conn->buffer->back();
        conn->buffer->pop_back();

        *data=packet->data;
        *dataLen=packet->dataSize;
        delete[] packet;
    }

}


void shutOff()
{
    closeAndExit=true;
}


int waitForConnections(int sock_fd){
    struct sockaddr_in cli_addr;
    socklen_t clilen = sizeof(cli_addr);
    int newsock_fd = accept(sock_fd,
                            (struct sockaddr *) &cli_addr,
                            &clilen);

    connection_t* conn=new connection_t[1];
    conn->socket_fd=newsock_fd;
    conn->clientID=idGenerator;
    idGenerator++;

    conn->buffer=new std::list<dataPacket_t*>();

    clientList[conn->clientID]=conn;

    write(newsock_fd,&conn->clientID,sizeof(int));


    return conn->clientID;

}

void sendMSG(int connID, const void* data, int dataLen){
	
    connection_t* conn=clientList[connID];
    int socket=conn->socket_fd;

    short int tag=SYNC;
    int crc=0;
    // Send sync
    write(socket,&tag,sizeof(short int));
    // Receive sync acknowledgement (if not, error & close connection)
    read(socket,&tag,sizeof(short int));
    //si !=SYNC_ACK
    if(tag!=SYNC_ACK)
    {
            //error
        printf("ERROR: sendMSG, linea %d \n",__LINE__);
            //exit
        closeConnection(socket);
        exit(0);
    }

            // Send ACK
    tag=ACK;
    write(socket,&tag,sizeof(short int));

            // Envía tamaño de los datos
            write(socket, &dataLen, sizeof(int));

            // Envía los datos
            write(socket, data, dataLen);

            //recv END

                //recv "crc"
            read(socket,&crc,sizeof(int));
            //comprobar crc
            if(crc!=dataLen)
            {    // !crc
                    //recuperacion de errores
                printf("ERROR: sendMSG, linea: %d\n",__LINE__);
                //error
                tag=ERROR;
                write(socket,&tag,sizeof(short int));
                closeConnection(socket);
                    //exit
                exit(0);
            }
                //else
            tag=ACK;
                    //send ACK

            write(socket,&tag,sizeof(short int));
                //recv END
            read(socket,&tag,sizeof(short int));
                //comprobar END

            if(tag!=END)
            {
                printf("ERROR: sendMSG, linea: %d\n",__LINE__);
                //error
                closeConnection(socket);
                    //exit
                exit(0);
            }


}

void recvMSG(int connID, void** data, int* dataLen){

    connection_t* conn=clientList[connID];
    int socket=conn->socket_fd;

    short int tag=0;
    //recv SYNC
        read(socket, &tag, sizeof(short int));
    if(tag!= SYNC)
    {
        printf("ERROR: recvMSG, linea: %d\n",__LINE__);
        //error
        closeConnection(socket);
         //exit
        exit(0);
    }

    tag=SYNC_ACK;
	//send SYNC_ACK
    write(socket,&tag,sizeof(short int));
	//recv ACK
    read(socket, &tag, sizeof(short int));

	//si !=ACK
		//error
		//exit	
    if(tag!= ACK)
    {
        printf("ERROR: recvMSG, linea: %d\n",__LINE__);
        //error
        closeConnection(socket);
         //exit
        exit(0);
    }



	int tamanioDato=0;
	char* datos=nullptr;
        int crc=0;
	//recibe tamaño
	read(socket,&tamanioDato,sizeof(int));
	//recibe datos
		//reservar buffer
		datos=new char[tamanioDato];
        crc=read(socket,datos,tamanioDato);
	
        //enviar END
            //envio "crc"
            write(socket,&crc,sizeof(int));
            //recv ACK
            read(socket,&tag,sizeof(short int));

            if(tag!=ACK)
            {
                printf("ERROR: recvMSG, linea: %d\n",__LINE__);
                //error
                closeConnection(socket);
                 //exit
                exit(0);
            }
            tag=END;
            write(socket,&tag,sizeof(short int));
            //envio END
	*data=datos;
	*dataLen=tamanioDato;

}

void closeConnection(int clientID){

    connection_t* conn=clientList[clientID];
    close(conn->socket_fd);
    clientList.erase(clientID);
    if(conn->buffer->size()>0)
    {
        printf("WARNING Unatended messages on close connection\n");
    }
    for(std::list<dataPacket_t*>::iterator it= conn->buffer->begin();
        it != conn->buffer->end();
        ++it)
    {
        delete[] (*it)->data;
        delete[] (*it);
    }

    delete  conn->buffer;
    conn->alive=false;
}

bool checkNewConnections()
{
    return (pendingConnections.size()>0);
}

int getNewConnection()
{
    int newId=pendingConnections.front();
    pendingConnections.pop_front();
    return newId;
}
