// wclient file.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include<iostream>  
#include<cstdlib>
#include<cstring>
#include <winsock2.h>
#include<cstdio>
#define FILE_NAME_MAX_SIZE 100
#define BUFSIZE 10000
using namespace std;
#pragma comment(lib, "ws2_32.lib")  

char recvBuf[BUFSIZE];
char file_name[FILE_NAME_MAX_SIZE + 1];
char buffer[BUFSIZE];
void func(SOCKET);
void func1(SOCKET);
int main()
{
	WSADATA wsaData;
	
	//加载套接字库  
	
	int iRet = 0;
	iRet = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iRet != 0)
	{
		cout << "WSAStartup(MAKEWORD(2, 2), &wsaData) execute failed!" << endl;
		return -1;
	}
	if (2 != LOBYTE(wsaData.wVersion) || 2 != HIBYTE(wsaData.wVersion))
	{
		WSACleanup();
		cout << "WSADATA version is not correct!" << endl;
		return -1;
	}

	//创建套接字  
	SOCKET clientSocket = socket(AF_INET, SOCK_STREAM, 0);
	if (clientSocket == INVALID_SOCKET)
	{
		cout << "clientSocket = socket(AF_INET, SOCK_STREAM, 0) execute failed!" << endl;
		return -1;
	}

	//初始化服务器端地址族变量  
	SOCKADDR_IN srvAddr;
	srvAddr.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");
	srvAddr.sin_family = AF_INET;
	srvAddr.sin_port = htons(6000);

	//连接服务器  
	iRet = connect(clientSocket, (SOCKADDR*)&srvAddr, sizeof(SOCKADDR));
	if (0 != iRet)
	{
		cout << "connect(clientSocket, (SOCKADDR*)&srvAddr, sizeof(SOCKADDR)) execute failed!" << endl;
		return -1;
	}
	
	recv(clientSocket, recvBuf, 100, 0);
	printf("%s\n", recvBuf);
	
	int op;
	while (1)
	{
		
		printf("(1)向server上传文件\n");
		printf("(2)向server下载文件\n");
		printf("(0)退出\n");
		cin >> op;
		
		switch (op)
		{
		case 1:memset(buffer, 0, BUFSIZE); strcpy(buffer, "UP"); send(clientSocket, buffer, BUFSIZE, 0); func1(clientSocket); break;
		case 2:memset(buffer, 0, BUFSIZE); strcpy(buffer, "DOWN"); send(clientSocket, buffer, BUFSIZE, 0); func(clientSocket); break;
		case 0:memset(buffer, 0, BUFSIZE); strcpy(buffer ,"FIN"); send(clientSocket, buffer, BUFSIZE, 0); break;
		default:printf("wrong!byebye~\n"); break;
		}
		if (op == 0)
			break;
		printf("\n");
	}
	//清理  
	printf("server FIN, then close the socket\n");
	closesocket(clientSocket);
	WSACleanup();

	system("pause");
	return 0;
}
//下载文件
void func(SOCKET clientSocket)
{
	//输入文件名 
	memset(file_name, 0, FILE_NAME_MAX_SIZE + 1);
	printf("Please Input File Name On Server: ");
	scanf("%s", &file_name);
	memset(buffer, 0, BUFSIZE);
	strncpy(buffer, file_name, strlen(file_name) > BUFSIZE ? BUFSIZE : strlen(file_name));

	//向服务器发送文件名 
	if (send(clientSocket, buffer, BUFSIZE, 0) < 0)
	{
		printf("Send File Name Failed\n");
		system("pause");
		exit(1);
	}
	if (!strcmp(buffer, "FIN"))
		return;
	//打开文件，准备写入 
	FILE * fp = fopen(file_name, "w");
	if (NULL == fp)
	{
		printf("File: %s Can Not Open To Write\n", file_name);
		system("pause");
		exit(1);
	}
	else
	{
		memset(buffer, 0, BUFSIZE);
		int length = 0;
		length = recv(clientSocket, buffer, BUFSIZE, 0);
		fputs(buffer, fp);
		printf("Receive File: %s From Server Successful!\n", file_name);
	}

	fclose(fp);
}
//上传文件
void func1(SOCKET clientSocket)
{
	//输入文件名 
	memset(file_name, 0, FILE_NAME_MAX_SIZE + 1);
	printf("Please Input File Name On Server: ");
	scanf("%s", &file_name);
	memset(buffer, 0, BUFSIZE);
	strncpy(buffer, file_name, strlen(file_name) > BUFSIZE ? BUFSIZE : strlen(file_name));

	//向服务器发送文件名 
	if (send(clientSocket, buffer, BUFSIZE, 0) < 0)
	{
		printf("Send File Name Failed\n");
		system("pause");
		exit(1);
	}
	if (!strcmp(buffer, "FIN"))
		return;
	FILE * fp = fopen(file_name, "r"); //打开文本文件 
	if (NULL == fp)
	{
		printf("File: %s Not Found\n", file_name);
	}
	else
	{
		memset(buffer, 0, BUFSIZE);
		int length = 0;

		while ((length = fread(buffer, sizeof(char), BUFSIZE, fp)) > 0)
		{
			if (send(clientSocket, buffer, length, 0) < 0)
			{
				printf("Send File: %s Failed\n", file_name);
				break;
			}
			memset(buffer, 0, BUFSIZE);
		}
		printf("Receive File: %s To Server Successful!\n", file_name);
		fclose(fp);
	}
}