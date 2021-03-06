// 시작을 위한 팁: 
//   1. [솔루션 탐색기] 창을 사용하여 파일을 추가/관리합니다.
//   2. [팀 탐색기] 창을 사용하여 소스 제어에 연결합니다.
//   3. [출력] 창을 사용하여 빌드 출력 및 기타 메시지를 확인합니다.
//   4. [오류 목록] 창을 사용하여 오류를 봅니다.
//   5. [프로젝트] > [새 항목 추가]로 이동하여 새 코드 파일을 만들거나, [프로젝트] > [기존 항목 추가]로 이동하여 기존 코드 파일을 프로젝트에 추가합니다.
//   6. 나중에 이 프로젝트를 다시 열려면 [파일] > [열기] > [프로젝트]로 이동하고 .sln 파일을 선택합니다.

#ifndef PCH_H
#define PCH_H

//표준 라이브러리
#include <iostream>

#include <string>
#include <vector>
#include <list>
#include <queue>
#include <set>

//네트워크 헤더파일 정의
#include <Winsock2.h>
#include <Ws2tcpip.h>
#include <Mswsock.h>

//네트워크 라이브러리 포함
#pragma comment(lib, "Ws2_32.lib")
#pragma comment(lib, "mswsock.lib")

// TODO: 여기에 미리 컴파일하려는 헤더 추가

#endif //PCH_H

namespace MHKLibrary
{
#define WSA_VERSION			MAKEWORD(2, 2)
#define PACKET_HADE			4U

#ifdef BUFFER_SIZE
#else
#define BUFFER_SIZE			512U
#endif // BUFFER_SIZE 가 정의되어있지 않다면 크기를 정의한다. 변경하고 싶으면 위에 정의 할것
}
