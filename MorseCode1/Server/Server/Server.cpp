#include <iostream>
#include <vector>
#include <string>
#include <WinSock2.h>
#include <WS2tcpip.h>

#pragma comment(lib, "Ws2_32.lib")

using namespace std;

int WorkWithServer();
string MorseCode(string str);

int main()
{
	return WorkWithServer();
}

int WorkWithServer()
{
	const char IP_ADDRESS[] = "172.20.10.4";
	const int SERVER_PORT = 52590;

	// Check IP
	in_addr ipToNum;
	int errorStat = inet_pton(AF_INET, IP_ADDRESS, &ipToNum);

	if (errorStat <= 0)
	{
		cout << "Error in IP translation\n";

		return 1;
	}

	// Win32API socket interfaces initialization
	WSADATA wsData;

	errorStat = WSAStartup(MAKEWORD(2, 2), &wsData);

	if (errorStat != 0)
	{
		cout << "Error Windows socket version " << WSAGetLastError() << '\n';

		return 1;
	}
	else
		cout << "Windows socket initialization is fine\n";

	SOCKET ServSocket = socket(AF_INET, SOCK_STREAM, 0);

	if (ServSocket == INVALID_SOCKET)
	{
		cout << "Error initialization socket " << WSAGetLastError() << '\n';
		closesocket(ServSocket);
		WSACleanup();

		return 1;
	}
	else
		cout << "Server socket initialization is fine\n";

	// Server socket binding
	sockaddr_in servInfo;
	ZeroMemory(&servInfo, sizeof(servInfo));

	servInfo.sin_family = AF_INET;
	servInfo.sin_addr = ipToNum;
	servInfo.sin_port = htons(SERVER_PORT);

	errorStat = bind(ServSocket, (sockaddr*)&servInfo, sizeof(servInfo));
	if (errorStat != 0)
	{
		cout << "Error socket binding to server. Error " << WSAGetLastError() << '\n';
		closesocket(ServSocket);
		WSACleanup();

		return 1;
	}
	else
		cout << "Binding socket to the server info is fine\n";

	// Listening to clients
	errorStat = listen(ServSocket, SOMAXCONN);

	if (errorStat != 0)
	{
		cout << "Cannot start listening to. Error " << WSAGetLastError() << '\n';
		closesocket(ServSocket);
		WSACleanup();

		return 1;
	}
	else {
		cout << "Listening turned on\n";
	}

	// Creation socket
	sockaddr_in clientInfo;

	ZeroMemory(&clientInfo, sizeof(clientInfo));
	int clientInfoSize = sizeof(clientInfo);
	SOCKET ClientConnect = accept(ServSocket, (sockaddr*)&clientInfo, &clientInfoSize);

	if (ClientConnect == INVALID_SOCKET)
	{
		cout << "Client detected, but cannot connect to a client. Error " << WSAGetLastError() << '\n';
		closesocket(ServSocket);
		closesocket(ClientConnect);
		WSACleanup();

		return 1;
	}
	else
	{
		cout << "Connection to a client established successfully\n";
		char clientIP[22];
		inet_ntop(AF_INET, &clientInfo.sin_addr, clientIP, INET_ADDRSTRLEN);
		cout << "Client connected with IP address " << clientIP << '\n';
	}

	// Message exchange
	vector<char> servBuff(1024), clientBuff(1024);
	short packetSize = 0;

	while (true)
	{	
		cout << "Type message: ";
		fgets(clientBuff.data(), clientBuff.size(), stdin);

		if (   clientBuff[0] == 'e'
			&& clientBuff[1] == 'n'
			&& clientBuff[2] == 'd')
		{
			shutdown(ServSocket, SD_BOTH);
			closesocket(ServSocket);
			WSACleanup();

			return 0;
		}

		string str = MorseCode(clientBuff.data());
		packetSize = send(ClientConnect, str.data(), str.size(), 0);

		if (packetSize == SOCKET_ERROR)
		{
			cout << "Cannot send message to client. Error " << WSAGetLastError() << '\n';
			closesocket(ServSocket);
			closesocket(ClientConnect);
			WSACleanup();

			return 1;
		}
	}

	packetSize = recv(ClientConnect, servBuff.data(), servBuff.size(), 0);
	cout << "Client message: " << servBuff.data() << '\n';

	closesocket(ServSocket);
	closesocket(ClientConnect);
	WSACleanup();

	return 0;
}

string MorseCode(string str)
{
	string res;

	for (char c : str)
	{
		switch (c)
		{
		case 'A':
		case 'a':
			res += ".- ";
			break;
		case 'B':
		case 'b':
			res += "-... ";
			break;
		case 'C':
		case 'c':
			res += "-.-. ";
			break;
		case 'D':
		case 'd':
			res += "-.. ";
			break;
		case 'E':
		case 'e':
			res += ". ";
			break;
		case 'F':
		case 'f':
			res += "..-. ";
			break;
		case 'G':
		case 'g':
			res += "--. ";
			break;
		case 'H':
		case 'h':
			res += ".... ";
			break;
		case 'I':
		case 'i':
			res += ".. ";
			break;
		case 'J':
		case 'j':
			res += ".--- ";
			break;
		case 'K':
		case 'k':
			res += "-.- ";
			break;
		case 'L':
		case 'l':
			res += ".-.. ";
			break;
		case 'M':
		case 'm':
			res += "-- ";
			break;
		case 'N':
		case 'n':
			res += "-. ";
			break;
		case 'O':
		case 'o':
			res += "--- ";
			break;
		case 'P':
		case 'p':
			res += ".--. ";
			break;
		case 'Q':
		case 'q':
			res += "--.- ";
			break;
		case 'R':
		case 'r':
			res += ".-. ";
			break;
		case 'S':
		case 's':
			res += "... ";
			break;
		case 'T':
		case 't':
			res += "- ";
			break;
		case 'U':
		case 'u':
			res += "..- ";
			break;
		case 'V':
		case 'v':
			res += "...- ";
			break;
		case 'W':
		case 'w':
			res += ".-- ";
			break;
		case 'X':
		case 'x':
			res += "-..- ";
			break;
		case 'Y':
		case 'y':
			res += "-.-- ";
			break;
		case 'Z':
		case 'z':
			res += "--.. ";
			break;
		default:
			res += " ";
			break;
		}
	}

	return res;
}