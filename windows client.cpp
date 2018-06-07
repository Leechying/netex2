// windows client.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"


#include<iostream>  
#include <winsock2.h>
#include<cstdio>
#define BUFSIZE 10000
using namespace std;

#pragma comment(lib, "ws2_32.lib")  
int main()
{
	//加载套接字库  
	WSADATA wsaData;
	int iRet = 0;
	iRet = WSAStartup(MAKEWORD(2, 2), &wsaData);//winsock版本,指向wsadata的指针
	if (iRet != 0)
	{
		cout << "WSAStartup(MAKEWORD(2, 2), &wsaData) execute failed!" << endl;
		return -1;
	}
	if (2 != LOBYTE(wsaData.wVersion) || 2 != HIBYTE(wsaData.wVersion))
	{//版本号不对，卸载
		WSACleanup();
		cout << "WSADATA version is not correct!" << endl;
		return -1;
	}

	//创建套接字  
	SOCKET clientSocket = socket(AF_INET, SOCK_STREAM, 0);//IPv4,TCP，内定
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
	char buf1[BUFSIZE],buf2[BUFSIZE];
	int len1,len2;
	//接收消息  
	len1= recv(clientSocket, buf1, BUFSIZ, 0);//接收服务器端信息  
	buf1[len1] = '\0';
	printf("%s\n", buf1); //打印服务器端信息  

	 /*循环的发送接收信息并打印接收信息（可以按需发送）--recv返回接收到的字节数，send返回发送的字节数*/
	while (1)
	{
		printf("Enter string to send:");
		scanf("%s", buf2);
		len2 = send(clientSocket, buf2, strlen(buf2), 0);
		len1 = recv(clientSocket, buf1, BUFSIZ, 0);
		buf1[len1] = '\0';
		if (!strcmp(buf1, "FIN"))
			break;
		printf("received:%s\n", buf1);
	}
	//清理  
	printf("server FIN, then close the socket\n");
	closesocket(clientSocket);
	WSACleanup();

	system("pause");
	return 0;
}