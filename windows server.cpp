// windows server.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"

#include<iostream>  
#include<WinSock2.h>  
#pragma comment(lib, "ws2_32.lib")  
#define CONNECT_NUM_MAX 10  

using namespace std;

//套接字句柄与外部交互
//定义变量、获得winsock版本、加载wisock库、初始化、创建套接字、设置套接字选项、关闭套接字、卸载库、释放资源
int main()
{
	//加载套接字库  
	WSADATA wsaData;
	int iRet = 0;
	iRet = WSAStartup(MAKEWORD(2, 2), &wsaData);//用此函数初始化Womsock动态库，winsock版本号，指向WSADATA的指针
	if (iRet != 0)//加载不成功
	{
		cout << "WSAStartup(MAKEWORD(2, 2), &wsaData) execute failed!" << endl;;
		return -1;
	}
	if (2 != LOBYTE(wsaData.wVersion) || 2 != HIBYTE(wsaData.wVersion))
	{
		WSACleanup();//卸载动态库
		cout << "WSADATA version is not correct!" << endl;
		return -1;
	}

	//创建套接字  
	SOCKET serverSocket = socket(AF_INET, SOCK_STREAM, 0);//IPV4，TCP，内定
	if (serverSocket == INVALID_SOCKET)
	{
		cout << "serverSocket = socket(AF_INET, SOCK_STREAM, 0) execute failed!" << endl;
		return -1;
	}

	//初始化服务器地址族变量  
	SOCKADDR_IN addrSrv;
	addrSrv.sin_addr.S_un.S_addr = htonl(INADDR_ANY);//IP地址，任何连接本主机的地址
	addrSrv.sin_family = AF_INET;//IPv4
	addrSrv.sin_port = htons(6000);//端口号

	//绑定套接字和服务器地址  
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
	SOCKADDR_IN clientAddr;
	int len = sizeof(SOCKADDR);
	int  len2;//recvbuf字符的长度
	while (1)
	{
		
		SOCKET connSocket = accept(serverSocket, (SOCKADDR*)&clientAddr, &len);
		if (connSocket == INVALID_SOCKET)
		{
			cout << "accept(serverSocket, (SOCKADDR*)&clientAddr, &len) execute failed!" << endl;
			return -1;
		}

		char sendBuf[1000];
		sprintf_s(sendBuf, "Welcome %s", inet_ntoa(clientAddr.sin_addr));
		send(connSocket, sendBuf, strlen(sendBuf) + 1, 0);
		char recvBuf[1000];
		while (1)
		{
			len2=recv(connSocket, recvBuf, 1000, 0);
			recvBuf[len2] = '\0';
			printf("receive:%s\n", recvBuf);
			printf("YOU want to send: ");
			scanf("%s", sendBuf);
			send(connSocket, sendBuf, strlen(sendBuf) + 1, 0);
			if (!strcmp(sendBuf, "FIN"))
				break;
			
		}
		printf("cli FIN and server FIN, then close the socket\n");
       closesocket(connSocket);
	   printf("\n\n");
	}
	WSACleanup();
	system("pause");
	return 0;
}