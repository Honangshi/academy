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
#define USER_MAXIMUM 100
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
#include "ServerEnum.h"

using namespace std;
//전방선언//

//전방선언//

class UserData {
public:
	//본인이 타고 있는  소켓의 번호 저장
	int FDNumber = 0;

	UserData() {
		cout << "유저데이터가 생성되었습니다." << endl;
	}

	~UserData() {
		cout << "유저 연결이 종료되었습니다." << endl;
	}

};



//전역변수 선언란//
struct pollfd pollFDArray[USER_MAXIMUM];
UserData* userFDArray[USER_MAXIMUM];

//전연변수 선언란//

//currentFD는 현재 리슨 소켓으로 사용할 녀석을 넣어줌
bool StartServer(int* currentFD) {
	//위에서 못 만들어 왔을때
	if (*currentFD == -1) {
		//오류 발생
		perror("socket()");
		//혹시 모르니 close도 놓고
		close(*currentFD);

		return true;
	}
	//소켓에는 IP와 포트가 있음
	sockaddr_in address;

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

void CheckMessage(char receive[], int length) {

	switch (receive[0])
	{
	case Chat:
		//맨 앞 1바이트는 메세지 구분용
		char* value = new char[length - 1];
			//			맨 앞 1 바이트
		memcpy(value, receive + 1, length - 1);
		//이 아래쪽은 받는 버퍼의 내용을 가져왔을 때에만 여기 있겠죠!
		cout << value << endl;
		for (int i = 0; i < USER_MAXIMUM; i++) {
			if (pollFDArray[i].fd != -1) {
				//유저한테 채팅내용을 전달해줌
				write(pollFDArray[i].fd, receive, length);
			}
		}


		break;
	}

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

	if (StartServer(&listenFD)) return -4;

	cout << "서버가 정상적으로 실행되었습니다" << endl;

	//pollFDArray = 내가 연락을 기다리고 있는 대상들
	//처음에는 연락할 대상이 없다는 것 확인
	for (int i = 0; i < USER_MAXIMUM; i++) {
		//-1 이 없다는 뜻
		pollFDArray[i].fd = -1;
	}

	pollFDArray[0].fd = listenFD;
	//읽기 대기중
	pollFDArray[0].events = POLLIN;
	pollFDArray[0].revents = 0;

	//무한 반복!
	for (;;)
	{
		//기다려요! 만약에 누군가가 저한테 메세지를 건네준다면! 그 때에서야 제가 움직이는 거에요!
		//메세지가 있는지 없는지를 확인하는 방법!
		result = poll(pollFDArray, USER_MAXIMUM, -1);

		//메세지가 있어야만 뭔가 할 거에요!
		if (result > 0)
		{
			//0번이 리슨 소켓이었습니다!
			//0번에 들어오려고 하는 애들을 체크해주긴 해야 해요!
			//                           누가 왔어?
			if (pollFDArray[0].revents == POLLIN)
			{
				//들어오세요^^
				connectFD = accept(listenFD, (struct sockaddr*)&connectSocket, &addressSize);

				//어디보자... 자리가 있나..
				//0번은 리슨 소켓이니까! 1번 부터 찾아봅시다!
				for (int i = 1; i < USER_MAXIMUM; i++)
				{
					//여기있네!
					if (pollFDArray[i].fd == -1)
					{
						pollFDArray[i].fd = connectFD;
						pollFDArray[i].events = POLLIN;
						pollFDArray[i].revents = 0;

						//새로운 유저 정보를 생성합니다!
						userFDArray[i] = new UserData();
						//너가 이 자리에 있는 거야!
						userFDArray[i]->FDNumber = i;



						break;
					};
				};
			};

			//0번은 리슨 소켓이니까! 위에서 처리했으니까!
			//1번부터 돌아주도록 하겠습니다!
			for (int i = 1; i < USER_MAXIMUM; i++)
			{
				//이녀석이 저한테 무슨 내용을 전달을 해줬는지 보러갑시다!
				switch (pollFDArray[i].revents)
				{
					//암말도 안했어요! 그럼 무시!
				case 0: break;
					//뭔가 말할 때가 있겠죠!
				case POLLIN:
					//보낼 때는 write였는데, 받아올 때에는 read가 되겠죠!
					//받는 용도의 버퍼를 사용해서 읽어주도록 합시다!
					//버퍼를 읽어봤는데.. 세상에나! 아무것도 들어있지 않아요!
					//굉장히 소름돋죠! 클라이언트가 뭔가 말을 했는데!
					//열어봤더니 빈 봉투다...?
					//이 상황은 클라이언트가 "연결을 끊겠다" 라는 의미입니다!
					if (read(pollFDArray[i].fd, buffRecv, BUFF_SIZE) < 1)
					{
						delete userFDArray[i];
						pollFDArray[i].fd = -1;
						break;
					};

					//메세지 해석
					//숫자 같은 의미 해석하기 힘든 내용 처리
					CheckMessage(buffRecv, BUFF_SIZE);
					break;
				};
			}
		};
	};


	return -4;
}

