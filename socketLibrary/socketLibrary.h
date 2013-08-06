/*
 * socketLibrary.h
 *
 *  Created on: 05/08/2013
 *      Author: utnso
 */

#ifndef SOCKETLIBRARY_H_
#define SOCKETLIBRARY_H_

	#include <sys/select.h>
	#include <stdio.h>
	#include <stdlib.h>
	#include <errno.h>
	#include <netdb.h>
	#include <sys/types.h>
	#include <sys/socket.h>
	#include <netinet/in.h>
	#include <arpa/inet.h>
	#include <string.h>

	#define DEFAULT_BUFFER_SIZE 	16384
	#define DEFAULT_MAX_CONEXIONS 	100
	#define SELECT_USEC_TIMEOUT 	500

	typedef enum {
		SOCKETSTATE_CONNECTED,
		SOCKETSTATE_DISCONNECTED
	}e_socket_state;

	typedef enum {
		SOCKETMODE_NONBLOCK		= 1,
		SOCKETMODE_BLOCK		= 2
	}e_socket_mode;


	typedef struct {
		int desc;
		struct sockaddr *my_addr;
		e_socket_mode mode;
	} t_socket ;

	typedef struct {
		t_socket* socket;
		t_socket* serv_socket;
		e_socket_state state;
	} t_socket_client ;

	typedef struct {
		t_socket    *socket;
		int			maxconexions;
	} t_socket_server ;

	typedef struct {
		char data[DEFAULT_BUFFER_SIZE];
		int size;
	} t_socket_buffer ;

	typedef struct {
		void *serializated_data;
		int size;
	} t_socket_sbuffer ;

	t_socket_client *socket_crearCliente(char* ip, int port);
	int socket_estaConectado(t_socket_client* cliente);
	e_socket_state socket_getEstado(t_socket_client* cliente);
	void socket_setEstado(t_socket_client* cliente, e_socket_state estado);
	int socket_conectar(t_socket_client* cliente, char* ipServidor, int puertoServidor);
	int socket_enviar(t_socket_client* cliente,void* datos,int largoDatos);
	int socket_enviarBuffer(t_socket_client* cliente,t_socket_buffer* buffer);
	int socket_enviarString(t_socket_client* cliente,char* string);
	t_socket_buffer* socket_recv(t_socket_client* cliente);
	int socket_recvEnBuffer(t_socket_client* cliente, t_socket_buffer* buffer);
	int socket_recvChar(t_socket_client* cliente,char* unChar);
	int socket_enviarChar(t_socket_client* cliente,char unChar);
	t_socket_server* socket_crearServer(char* ip, int port);
	int socket_escuchar(t_socket_server* server);
	t_socket_client* socket_aceptar(t_socket_server* server);





#endif /* SOCKETLIBRARY_H_ */
