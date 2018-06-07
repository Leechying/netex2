// windows server.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"

#include<iostream>  
#include<WinSock2.h>  
#pragma comment(lib, "ws2_32.lib")  
#define CONNECT_NUM_MAX 10  

using namespace std;

//�׽��־�����ⲿ����
//������������winsock�汾������wisock�⡢��ʼ���������׽��֡������׽���ѡ��ر��׽��֡�ж�ؿ⡢�ͷ���Դ
int main()
{
	//�����׽��ֿ�  
	WSADATA wsaData;
	int iRet = 0;
	iRet = WSAStartup(MAKEWORD(2, 2), &wsaData);//�ô˺�����ʼ��Womsock��̬�⣬winsock�汾�ţ�ָ��WSADATA��ָ��
	if (iRet != 0)//���ز��ɹ�
	{
		cout << "WSAStartup(MAKEWORD(2, 2), &wsaData) execute failed!" << endl;;
		return -1;
	}
	if (2 != LOBYTE(wsaData.wVersion) || 2 != HIBYTE(wsaData.wVersion))
	{
		WSACleanup();//ж�ض�̬��
		cout << "WSADATA version is not correct!" << endl;
		return -1;
	}

	//�����׽���  
	SOCKET serverSocket = socket(AF_INET, SOCK_STREAM, 0);//IPV4��TCP���ڶ�
	if (serverSocket == INVALID_SOCKET)
	{
		cout << "serverSocket = socket(AF_INET, SOCK_STREAM, 0) execute failed!" << endl;
		return -1;
	}

	//��ʼ����������ַ�����  
	SOCKADDR_IN addrSrv;
	addrSrv.sin_addr.S_un.S_addr = htonl(INADDR_ANY);//IP��ַ���κ����ӱ������ĵ�ַ
	addrSrv.sin_family = AF_INET;//IPv4
	addrSrv.sin_port = htons(6000);//�˿ں�

	//���׽��ֺͷ�������ַ  
	iRet = bind(serverSocket, (SOCKADDR*)&addrSrv, sizeof(SOCKADDR));
	if (iRet == SOCKET_ERROR)
	{
		cout << "bind(serverSocket, (SOCKADDR*)&addrSrv, sizeof(SOCKADDR)) execute failed!" << endl;
		return -1;
	}
	//����  
	iRet = listen(serverSocket, CONNECT_NUM_MAX);
	if (iRet == SOCKET_ERROR)
	{
		cout << "listen(serverSocket, 10) execute failed!" << endl;
		return -1;
	}

	

	//�ȴ�����_����_����  
	SOCKADDR_IN clientAddr;
	int len = sizeof(SOCKADDR);
	int  len2;//recvbuf�ַ��ĳ���
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