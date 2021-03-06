// ConsoleServer.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
//

#include "pch.h"

#include "cNetwork.h"
#include "cDataBuffer.h"

void ConnectEnter(MHKLibrary::cNetwork*, SOCKET);
void ConnectExit(MHKLibrary::cNetwork*, SOCKET);

struct CustomStruct
{
	byte data[16];
};

int main()
{
	CustomStruct customStruct;
	ZeroMemory(&customStruct, sizeof(CustomStruct));
	MHKLibrary::cDataBuffer dataBuffer;
	dataBuffer.TextToPacket("안녕");
	dataBuffer.TextToPacket("방가워");
	dataBuffer.DataToPacket(MHKLibrary::PACKET_DATA, &customStruct, sizeof(CustomStruct));
	dataBuffer.TextToPacket("여러개의 문자를 넣어봤어");
	dataBuffer.TextToPacket("잘되?");

	dataBuffer.PacketToData(dataBuffer.FrontPacket());
	dataBuffer.PopPacket();


	MHKLibrary::cNetwork TCPIpv4Server;								// 네트워크 클래스 사용
	TCPIpv4Server.AcceptSocket("127.0.0.1", 8600);					// 다른 클라이언트 연결요청 준비
	TCPIpv4Server.SetDelegateConnectEnter(ConnectEnter);
	TCPIpv4Server.SetDelegateConnectExit(ConnectExit);
	MHKLibrary::Message msg;
	bool isServerLive = true;
	while (isServerLive)
	{
		TCPIpv4Server.Confurm();									// 요청확인 및 메시지 받기
		while (TCPIpv4Server.IsRecvMessage())						// 받은 메시지를 연결된 모든 소켓에게 보냅니다.
		{
			TCPIpv4Server.GetRecvMessage(&msg);
			if (!strcmp("서버종료", msg.sText.c_str())) isServerLive = false;
			TCPIpv4Server.AllSend(msg.sText.c_str());
		}
	}
	// 연결해제
	TCPIpv4Server.AllCloseConnectSocket();							
	TCPIpv4Server.AcceptClose();
}

void ConnectEnter(MHKLibrary::cNetwork* pNetwork, SOCKET sock)
{
	char szText[512];
	sprintf_s(szText, 512, "접속확인(접속인원 총 : %d 명)", pNetwork->GetConnectSocketCount());
	pNetwork->AllSend(szText);
}

void ConnectExit(MHKLibrary::cNetwork* pNetwork, SOCKET sock)
{
	char szText[512];
	sprintf_s(szText, 512, "접속해제(접속인원 총 : %d 명)", pNetwork->GetConnectSocketCount());
	pNetwork->AllSend(szText);
}

// 프로그램 실행: <Ctrl+F5> 또는 [디버그] > [디버깅하지 않고 시작] 메뉴
// 프로그램 디버그: <F5> 키 또는 [디버그] > [디버깅 시작] 메뉴

// 시작을 위한 팁: 
//   1. [솔루션 탐색기] 창을 사용하여 파일을 추가/관리합니다.
//   2. [팀 탐색기] 창을 사용하여 소스 제어에 연결합니다.
//   3. [출력] 창을 사용하여 빌드 출력 및 기타 메시지를 확인합니다.
//   4. [오류 목록] 창을 사용하여 오류를 봅니다.
//   5. [프로젝트] > [새 항목 추가]로 이동하여 새 코드 파일을 만들거나, [프로젝트] > [기존 항목 추가]로 이동하여 기존 코드 파일을 프로젝트에 추가합니다.
//   6. 나중에 이 프로젝트를 다시 열려면 [파일] > [열기] > [프로젝트]로 이동하고 .sln 파일을 선택합니다.
