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
//전방선언//

//전방선언//

class UserData {
public:
	//본인이 타고 있는  소켓의 번호 저장
	int FD_NUMBER = 0;

	UserData() {
		cout << "유저데이터가 생성되었습니다." << endl;
	}

	~UserData() {
		cout << "유저 연결이 종료되었습니다." << endl;
	}

}



//전역변수 선언란//
struct poll pollFDArray[FD_NUMBER];
UserData* userFDArray[FD_NUMBER];

//전연변수 선언란//

//currentFD는 현재 리슨 소켓으로 사용할 녀석을 넣어줌
bool ServerStart(int* currentFD) {
	//위에서 못 만들어 왔을때
	if (*currentFD == -1) {
		//오류 발생
		perror("socket()");
		//혹시 모르니 close도 놓고
		close();

		return true;
	}
	//소켓에는 IP와 포트가 있음
	sockarr_in address;

	memset(&address, 0, sizeof(address));

	//IPv4의 IP를 넘겨줌
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = inet_addr(SERVER_IP);
	address.sin_port = htons(SERVER_PORT);

	//정보는 만들었다고 끝X -> 리슨 소켓에 연결해야 의미 있음
	if (bind(*currentFD, (struct sockaddr*)&address, sizeof(address)) == -1) {
		//오류 발생
		perror("bind()");
		//혹시 모르니 close도 놓고
		close(*currentFD);
		return true;
	}

	//바인드 하면 끝X -> 리슨소켓인지 확인 
	if (listen(*currentFD, 8) == -1) {
		//오류 발생
		perror("listen()");
		//혹시 모르니 close도 놓고
		close(*currentFD);
		return true;
	}
	//문제 X
	return false;

}

int main() {
	//소켓들은 전부 다 int로 관리될 것
	//컴퓨터가 내용 받기 위해선 상대방의 메세지를 받을 준비가 되어있어야 함
	//보통은 소켓이 닫혀 있지만 이미 그 주소로 메세지를 보냈다면 열림 아니면 무시
	//소켓을 열어주는 소켓 필요 => 소켓 하나를 리슨소켓으로 만듦
	//"접속 요청"만 받아주는 소켓을 여는 것. 누군가 접속 요청 하면 비어있는 소켓 하나에 연결 -> 하나의 창구
			//IPv4로 연결을 받음, 연결 지속시킴
	int listenFD = socket(AF_INET, SOCK_STREAM, 0);
	//연결할 FD
	int connectFD;
	int result = 0;

	struct sockaddr_in listenSocket, connectSocket;
	socklen_t addressSize;

	//받는 버퍼
	char buffRecv[BUFF_SIZE];
	//주는 버버
	char buffSend[BUFF_SIZE];

	//0으로 초기화
	memset(buffRecv, 0, sizeof(buffRecv));
	memset(buffSend, 0, sizeof(buffSend));

	if(StartServer(&listenFD)) return -4;

	cout << "Hello Linex" << endl;
	return -4;
}


