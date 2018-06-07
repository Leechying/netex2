// wserver file.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"


#include<iostream>  
#include<WinSock2.h>  
#pragma comment(lib, "ws2_32.lib")  
#define CONNECT_NUM_MAX 10  
#define BUFSIZE   10000
#define FILE_NAME_MAX_SIZE 100
using namespace std;

char sendBuf[BUFSIZE];
char recvBuf[BUFSIZE];
char file_name[FILE_NAME_MAX_SIZE + 1];
void func1(SOCKET);
void func2(SOCKET);
int main()
{
	WSADATA wsaData;
	
	SOCKADDR_IN addrSrv;
	SOCKADDR_IN clientAddr;
	//加载套接字库  

	int iRet = 0;
	iRet = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iRet != 0)
	{
		cout << "WSAStartup(MAKEWORD(2, 2), &wsaData) execute failed!" << endl;;
		return -1;
	}
	if (2 != LOBYTE(wsaData.wVersion) || 2 != HIBYTE(wsaData.wVersion))
	{
		WSACleanup();
		cout << "WSADATA version is not correct!" << endl;
		return -1;
	}

	//创建套接字 
	SOCKET serverSocket = socket(AF_INET, SOCK_STREAM, 0);
	if (serverSocket == INVALID_SOCKET)
	{
		cout << "serverSocket = socket(AF_INET, SOCK_STREAM, 0) execute failed!" << endl;
		return -1;
	}

	//初始化服务器地址族变量  
	
	addrSrv.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
	addrSrv.sin_family = AF_INET;
	addrSrv.sin_port = htons(6000);

	//绑定  
	iRet = bind(serverSocket, (SOCKADDR*)&addrSrv, sizeof(SOCKADDR));
	if (iRet == SOCKET_ERROR)
	{
		cout << "bind(serverSocket, (SOCKADDR*)&addrSrv, sizeof(SOCKADDR)) execute failed!" << endl;
		return -1;
	}


	//监听  
	iRet = listen(serverSocket, CONNECT_NUM_MAX);
	if (iRet == SOCKET_ERROR)
	{
		cout << "listen(serverSocket, 10) execute failed!" << endl;
		return -1;
	}

	//等待连接_接收_发送  
	
	int len = sizeof(SOCKADDR);
	
	while (1)
	{
		SOCKET connSocket = accept(serverSocket, (SOCKADDR*)&clientAddr, &len);
		if (connSocket == INVALID_SOCKET)
		{
			cout << "accept(serverSocket, (SOCKADDR*)&clientAddr, &len) execute failed!" << endl;
			return -1;
		}
		
		sprintf_s(sendBuf, "Welcome %s", inet_ntoa(clientAddr.sin_addr));
		send(connSocket, sendBuf, strlen(sendBuf) + 1, 0);

		while (1)
		{
			memset(recvBuf, 0, BUFSIZE);
			if (recv(connSocket, recvBuf, BUFSIZE, 0) < 0)
			{
				printf("Server Receive Data Failed!");
				break;
			}
			if (!strcmp(recvBuf, "FIN"))
			{
				sprintf_s(sendBuf, "FIN");
				send(connSocket, sendBuf, strlen(sendBuf) + 1, 0);
				break;
			}
			if (!strcmp(recvBuf, "UP"))
			{
				memset(recvBuf, 0, BUFSIZE);
				func2(connSocket);
			}
			if (!strcmp(recvBuf, "DOWN"))
			{
				memset(recvBuf, 0, BUFSIZE);
				func1(connSocket);
			}
		}
		printf("cli  is close\n");
		printf("\n");
		closesocket(connSocket);
	}
	//清理  
	closesocket(serverSocket);
	WSACleanup();
	system("pause");
	return 0;
}
void func1(SOCKET connSocket)
{
	recv(connSocket, recvBuf, BUFSIZE, 0);
	memset(file_name, 0, FILE_NAME_MAX_SIZE + 1);
	strncpy(file_name, recvBuf, strlen(recvBuf) > FILE_NAME_MAX_SIZE ? FILE_NAME_MAX_SIZE : strlen(recvBuf));
	printf("%s\n", file_name);
	FILE * fp = fopen(file_name, "r"); //打开文本文件 
	if (NULL == fp)
	{
		printf("File: %s Not Found\n", file_name);
	}
	else
	{
		memset(sendBuf, 0, BUFSIZE);
		int length = 0;

		while ((length = fread(sendBuf, sizeof(char), BUFSIZE, fp)) > 0)
		{
			if (send(connSocket, sendBuf, length, 0) < 0)
			{
				printf("Send File: %s Failed\n", file_name);
				break;
			}
			memset(sendBuf, 0, BUFSIZE);
		}
		fclose(fp);
		printf("File: %s Transfer Successful!\n", file_name);
	}
}
void func2(SOCKET connSocket)
{
	recv(connSocket, recvBuf, BUFSIZE, 0);
	memset(file_name, 0, FILE_NAME_MAX_SIZE + 1);
	strncpy(file_name, recvBuf, strlen(recvBuf) > FILE_NAME_MAX_SIZE ? FILE_NAME_MAX_SIZE : strlen(recvBuf));
	printf("%s\n", file_name);
	FILE * fp = fopen(file_name, "w");
	if (NULL == fp)
	{
		printf("File: %s Can Not Open To Write\n", file_name);
		system("pause");
		exit(1);
	}
	else
	{
		memset(recvBuf, 0, BUFSIZE);
		int length = 0;
		length = recv(connSocket, recvBuf, BUFSIZE, 0);
		fputs(recvBuf, fp);
		printf("Receive File: %s From Client Successful!\n", file_name);
	}

	fclose(fp);

}