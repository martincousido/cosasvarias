/*
 * socketLibrary.c
 *
 *  Created on: 05/08/2013
 *      Author: utnso
 */


#include "socketLibrary.h"

t_socket_client *socket_crearCliente(char* ip, int port){
		int descriptorSocket;
		struct sockaddr_in estructuraCliente;
		t_socket_client* Cliente;
		t_socket socketCliente;

		Cliente =(t_socket_client*) malloc(sizeof(t_socket_client));
		descriptorSocket= socket(AF_INET,SOCK_STREAM,0);

		if (descriptorSocket==-1) ;/*todo loggear error o comunicar error*/

		estructuraCliente.sin_family = AF_INET;
		estructuraCliente.sin_port= htons(port);
		estructuraCliente.sin_addr.s_addr = inet_addr(ip);
		memset(&(estructuraCliente),'\0',8);

		socketCliente.desc = descriptorSocket;
		socketCliente.mode = SOCKETMODE_BLOCK;
		socketCliente.my_addr = (struct sockaddr*) &estructuraCliente;

		Cliente->socket = &socketCliente;
		Cliente->state = SOCKETSTATE_DISCONNECTED;

		return Cliente;


}


int socket_estaConectado(t_socket_client* cliente){
	return socket_getEstado(cliente) == SOCKETSTATE_CONNECTED;

}


e_socket_state socket_getEstado(t_socket_client* cliente){
	return cliente->state;
}


void socket_setEstado(t_socket_client* cliente, e_socket_state estado){
	cliente->state = estado;
}


int socket_conectar(t_socket_client* cliente, char* ipServidor, int puertoServidor){
	struct sockaddr_in estructuraServidor;
	int resultado;

	estructuraServidor.sin_family = AF_INET;
	estructuraServidor.sin_port= htons(puertoServidor);
	estructuraServidor.sin_addr.s_addr = inet_addr(ipServidor);
	memset(&(estructuraServidor),'\0',8);

	cliente->serv_socket->my_addr = (struct sockaddr*) &estructuraServidor;
	cliente->serv_socket->mode = SOCKETMODE_BLOCK;

	resultado = connect(cliente->socket->desc,cliente->serv_socket->my_addr,sizeof(struct sockaddr));
	if (resultado == 0){
		socket_setEstado(cliente,SOCKETSTATE_CONNECTED);
		return resultado;
	}
	else{
		return resultado;
	}



}



int socket_enviar(t_socket_client* cliente,void* datos,int largoDatos){
	return send(cliente->socket->desc,datos,largoDatos,MSG_WAITALL);

}


int socket_enviarBuffer(t_socket_client* cliente,t_socket_buffer* buffer){
	return send(cliente->socket->desc,(void*)&buffer->data,buffer->size,MSG_WAITALL);
}

int socket_enviarString(t_socket_client* cliente,char* string){
	return send(cliente->socket->desc,(void*)string,strlen(string),MSG_WAITALL);
}

int socket_enviarChar(t_socket_client* cliente,char unChar){
	return send(cliente->socket->desc,(void*)&unChar,sizeof(char),MSG_WAITALL);
}

t_socket_buffer* socket_recv(t_socket_client* cliente){
		t_socket_buffer* unBuffer;
		unBuffer = (t_socket_buffer*) malloc(sizeof(t_socket_buffer));
		unBuffer->size = recv(cliente->socket->desc,(void*)unBuffer->data,DEFAULT_BUFFER_SIZE,MSG_WAITALL);
		return unBuffer;

}

int socket_recvEnBuffer(t_socket_client* cliente, t_socket_buffer* buffer){
		 buffer = socket_recv(cliente);
		 return buffer->size;
}

int socket_recvChar(t_socket_client* cliente,char* unChar){
		return recv(cliente->socket->desc,(void*)unChar,sizeof(char),MSG_WAITALL);
}

t_socket_server* socket_crearServer(char* ip, int port){
	int descriptorSocket;
	struct sockaddr_in estructuraServidor;
	t_socket_server* Servidor;
	t_socket socketServidor;
	int yes = 1;

	Servidor =(t_socket_server*) malloc(sizeof(t_socket_server));
	descriptorSocket= socket(AF_INET,SOCK_STREAM,0);

	if (descriptorSocket==-1) ;/*todo loggear error o comunicar error*/

	if(setsockopt(descriptorSocket,SOL_SOCKET,SO_REUSEADDR,&yes,sizeof(int)) ==-1); //todo loguear error

	if (bind(descriptorSocket, (struct sockaddr *)&estructuraServidor, sizeof(struct sockaddr)) == -1) {
	perror("bind");
	exit(1);
	}

	estructuraServidor.sin_family = AF_INET;
	estructuraServidor.sin_port= htons(port);
	estructuraServidor.sin_addr.s_addr = inet_addr(ip);
	memset(&(estructuraServidor),'\0',8);

	socketServidor.desc = descriptorSocket;
	socketServidor.mode = SOCKETMODE_BLOCK;
	socketServidor.my_addr = (struct sockaddr*) &estructuraServidor;

	Servidor->socket = &socketServidor;

	return Servidor;


}


void socket_setMaxConexiones(t_socket_server* server,int conexiones){
		server->maxconexions = conexiones;

}


int socket_getMaxConexiones(t_socket_server* server){
	return server->maxconexions;

}


int socket_escuchar(t_socket_server* server){
	return listen(server->socket->desc,server->maxconexions);

}

t_socket_client* socket_aceptar(t_socket_server* server){
		t_socket_client* cliente;
		unsigned int i = sizeof(struct sockaddr_in);

		cliente = (t_socket_client*) malloc(sizeof(t_socket_client));
		cliente->socket->mode = SOCKETMODE_BLOCK;
		cliente->serv_socket = server->socket;
		cliente->socket->desc = accept(server->socket->desc,cliente->socket->my_addr,&i);
		if (cliente->socket->desc == -1){
			//todo bum
			return cliente;
		}
		else return cliente;

}
