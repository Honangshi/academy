//서버에는 ip가 있음 -> 내부 네트워크에도 ip 알려줘야함 
#define SERVER_IP "10.182.0.2"
//동적 포트 사용
#define SERVER_PORT 61188
//너무 많은 양의 전송 하면 성능상 + 물리적인 네트워크 기기에도 한계 
#define BUFF_SIZE 1024
//제한 해야 하는 요소 많음
//접속 인원의 한계 미리 정해둠 
//이 한계를 넘는 인원 들어오는 경우 대기열 서버로 넘겨줌
//리눅스는 모든 것을 파일 형태로 관리 -> 소켓 = 파일
#define FD_NUMBER
//틱레이트 설정
#define TICK_RATE 16

#include <iostream>
//클라이언트가 직접 주소와 포트를 이용해서 들어오라고 소켓을 사용함
#include <sys/socket.h>
//ip를 사용하기 위해 inet 가져옴
#include <netinet/in.h>
#include <arpa/inet.h>
//플레이어들을 계속 순회하면서 자연스럽게 나에게 준 사용자가 있는 경우에만
#include <poll.h>

//얼마나 시간이 니났는지 체크를 해야 틱레이트 계산 가능
#include <sys/time.h>
#include <sys/types.h>
#include <string.h>
#include <unistd.h>


using namespace std;
int main() {
	cout << "Hello Linex" << endl;
	return 0;
}


