// windows client.cpp : �������̨Ӧ�ó������ڵ㡣
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
	//�����׽��ֿ�  
	WSADATA wsaData;
	int iRet = 0;
	iRet = WSAStartup(MAKEWORD(2, 2), &wsaData);//winsock�汾,ָ��wsadata��ָ��
	if (iRet != 0)
	{
		cout << "WSAStartup(MAKEWORD(2, 2), &wsaData) execute failed!" << endl;
		return -1;
	}
	if (2 != LOBYTE(wsaData.wVersion) || 2 != HIBYTE(wsaData.wVersion))
	{//�汾�Ų��ԣ�ж��
		WSACleanup();
		cout << "WSADATA version is not correct!" << endl;
		return -1;
	}

	//�����׽���  
	SOCKET clientSocket = socket(AF_INET, SOCK_STREAM, 0);//IPv4,TCP���ڶ�
	if (clientSocket == INVALID_SOCKET)
	{
		cout << "clientSocket = socket(AF_INET, SOCK_STREAM, 0) execute failed!" << endl;
		return -1;
	}

	//��ʼ���������˵�ַ�����  
	SOCKADDR_IN srvAddr;
	srvAddr.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");
	srvAddr.sin_family = AF_INET;
	srvAddr.sin_port = htons(6000);

	//���ӷ�����  
	iRet = connect(clientSocket, (SOCKADDR*)&srvAddr, sizeof(SOCKADDR));
	if (0 != iRet)
	{
		cout << "connect(clientSocket, (SOCKADDR*)&srvAddr, sizeof(SOCKADDR)) execute failed!" << endl;
		return -1;
	}
	char buf1[BUFSIZE],buf2[BUFSIZE];
	int len1,len2;
	//������Ϣ  
	len1= recv(clientSocket, buf1, BUFSIZ, 0);//���շ���������Ϣ  
	buf1[len1] = '\0';
	printf("%s\n", buf1); //��ӡ����������Ϣ  

	 /*ѭ���ķ��ͽ�����Ϣ����ӡ������Ϣ�����԰��跢�ͣ�--recv���ؽ��յ����ֽ�����send���ط��͵��ֽ���*/
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
	//����  
	printf("server FIN, then close the socket\n");
	closesocket(clientSocket);
	WSACleanup();

	system("pause");
	return 0;
}