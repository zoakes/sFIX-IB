#include "Socket.h"


#include <winsock2.h>
#include <winsock.h>
#include <ws2tcpip.h>
#include <iphlpapi.h>
#include <stdio.h>


using namespace std;

extern int iRes;
extern int out;
extern struct addrinfo* 
			result,
			*ptr, 
	hints;

extern Socket ConnectSocket;
char recvbuff[1024];

//extern Socket::Socket();


/*

Socket::Socket() {
	iRes = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iRes != 0) {
		printf("WSAStartup failed: %d\n", iRes);
		out = -1;
	}
	else {
		printf("WSAStartup Succeeded %d", iRes);
		out = 0;
	}

	//Set up default values for family, type, and protocol via STRUCT 
	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_UNSPEC; //Can specify IPv6 or IPv4 -- not specified here.
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;

	// Resolve the server address and port
	iRes = getaddrinfo("www.google.com", DEFAULT_PORT, &hints, &result);
	if (iRes != 0) {
		printf("getaddrinfo failed: %d\n", iRes);
		WSACleanup();
	}
	//Attempt to connect to first address called by ptr to getaddrinfo
	ptr = result; //This may need to be iRes ?

	//Create socket !! 
	ConnectSocket = socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol);
	if (ConnectSocket == INVALID_SOCKET) {
		printf("Socket Error %ld \n", WSAGetLastError());
		freeaddrinfo(result); //Clear addrinfo 
		WSACleanup();
		out = 0;
	}
	//Connect Socket -- Finally...
	iRes = connect(ConnectSocket, ptr->ai_addr, (int)ptr->ai_addrlen);
	if (iRes == SOCKET_ERROR) {
		closesocket(ConnectSocket);
		ConnectSocket = INVALID_SOCKET;
		//Could also just try next addr in struct... but this is simple for now. + freeddrinfo
		printf("Socket %d Invalid", ConnectSocket);
		out = 0;
	}
	if (ConnectSocket == INVALID_SOCKET) {
		printf("Unable to connect to server \n");
		WSACleanup();
		freeaddrinfo(result);
		out = -1;
	}
	else {
		printf("Socket %p Connected ... ", ptr->ai_addr);
		out = 1;
	}
}



Socket::~Socket() {
	iRes = shutdown(ConnectSocket, SD_SEND);
	if (iRes == SOCKET_ERROR)
		closesocket(ConnectSocket);
	WSACleanup();
}

int Socket::sendMessage(const char* const& msg) { //Jesus.  Ptr pointing straight to hell.
	const char* sendbuf = msg; //"This is test msg";  FUCK yes -- solved ! 
	iRes = send(ConnectSocket, sendbuf, (int)strlen(sendbuf), 0);
	if (iRes == SOCKET_ERROR) {
		printf("Send Failed %d \n", WSAGetLastError());
		//closesocket(ConnectSocket); Don't think I want to close the socket?
		WSACleanup();
		return -1;
	}
	printf("Message Sent: %s", msg);
	return 1;
}


*/