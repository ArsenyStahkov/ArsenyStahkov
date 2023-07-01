#include <iostream>
#include <vector>
#include <WinSock2.h>
#include <WS2tcpip.h>

#pragma comment(lib, "Ws2_32.lib")

using namespace std;

int WorkWithClient();
string MorseDecode(string str);
string MorseLetterDecode(string letter);

int main()
{
	return WorkWithClient();
}

int WorkWithClient()
{
	const char IP_ADDRESS[] = "172.20.10.4";
	const int SERVER_PORT = 52590;

	// Win32API socket interfaces initialization
	WSADATA wsData;

	int errorStat = WSAStartup(MAKEWORD(2, 2), &wsData);

	if (errorStat != 0)
	{
		cout << "Error Windows socket version " << WSAGetLastError() << '\n';

		return 1;
	}
	else
		cout << "Windows socket initialization is fine\n";


	// Socket initialization
	SOCKET ClientSocket = socket(AF_INET, SOCK_STREAM, 0);

	if (ClientSocket == INVALID_SOCKET)
	{
		cout << "Error initialization socket " << WSAGetLastError() << '\n';
		closesocket(ClientSocket);
		WSACleanup();

		return 1;
	}
	else
		cout << "Server socket initialization is fine\n";

	// Establishing a connection to server
	in_addr ipToNum;
	inet_pton(AF_INET, IP_ADDRESS, &ipToNum);

	sockaddr_in servInfo;
	ZeroMemory(&servInfo, sizeof(servInfo));

	servInfo.sin_family = AF_INET;
	servInfo.sin_addr = ipToNum;
	servInfo.sin_port = htons(SERVER_PORT);

	errorStat = connect(ClientSocket, (sockaddr*)&servInfo, sizeof(servInfo));

	if (errorStat != 0)
	{
		cout << "Connection to server is failed. Error " << WSAGetLastError() << '\n';
		closesocket(ClientSocket);
		WSACleanup();

		return 1;
	}
	else
		cout << "Connection established successfully\n";

	// Message exchange
	vector<char> servBuff(1024), clientBuff(1024);
	short packSize = 0;

	while (true)
	{
		packSize = recv(ClientSocket, servBuff.data(), servBuff.size(), 0);

		if (packSize == SOCKET_ERROR)
		{
			cout << "Cannot receive message from server. Error " << WSAGetLastError() << '\n';
			closesocket(ClientSocket);
			WSACleanup();

			return 1;
		}
		else
			cout << "Server message: " << servBuff.data() << '\n';
			string str = MorseDecode(servBuff.data());
			cout << "Decoded message: " << str << '\n';
	}

	closesocket(ClientSocket);
	WSACleanup();

	return 0;
}

string MorseLetterDecode(string letter)
{
	string res;

	if (letter == ".-")
		res += 'a';
	else if (letter == "-...")
		res += 'b';
	else if (letter == "-.-.")
		res += 'c';
	else if (letter == "-..")
		res += 'd';
	else if (letter == ".")
		res += 'e';
	else if (letter == "..-.")
		res += 'f';
	else if (letter == "--.")
		res += 'g';
	else if (letter == "....")
		res += 'h';
	else if (letter == "..")
		res += 'i';
	else if (letter == ".---")
		res += 'j';
	else if (letter == "-.-")
		res += 'k';
	else if (letter == ".-..")
		res += 'l';
	else if (letter == "--")
		res += 'm';
	else if (letter == "-.")
		res += 'n';
	else if (letter == "---")
		res += 'o';
	else if (letter == ".--.")
		res += 'p';
	else if (letter == "--.-")
		res += 'q';
	else if (letter == ".-.")
		res += 'r';
	else if (letter == "...")
		res += 's';
	else if (letter == "-")
		res += 't';
	else if (letter == "..-")
		res += 'u';
	else if (letter == "...-")
		res += 'v';
	else if (letter == ".--")
		res += 'w';
	else if (letter == "-..-")
		res += 'x';
	else if (letter == "-.--")
		res += 'y';
	else if (letter == "--..")
		res += 'z';
	else
		res += ' ';

	return res;
}

string MorseDecode(string str)
{
	int len = str.size();
	string letter;
	string res;
	
	for (int i = 0; i < len; i++)
	{
		if (str[i] == ' ')
		{
			res += MorseLetterDecode(letter);
			letter = "";
		}
		else
			letter += str[i];
	}
	
	return res;
}