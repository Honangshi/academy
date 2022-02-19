#include "/usr/include/mysql/mysql.h"
#include <iostream>

using namespace std;

//연결된 SQL 저장
MySQL* connectedDatebase;

//DB에 커리를 한다 = 정보를 긁어옴
MYSQL_RES* queryResult;

//긁어온 정보의 1줄
MySQL_ROW queryRow;

bool MySQLInitialize() {
	if (connectedDatebase = mysql_init((MYSQL*)nullptr)) {
		cout << "MySQL 초기화 실패" << endl;
		return false;
	}
	cout << "MySQL 초기화 성공" << endl;

	//						mysql 저장할 위치	mysql의 주소
	if (!(mysql_real_connect(connectedDatebase, "localhost", "root", "1234", nullptr, 3306, nullptr, 0))) {
		cout << "MySQL 연결 실패" << endl;
		return false;
	}
	cout << "MySQL 연결 성공" << endl;
}

void MySQLClose() {
	mysql_close(connectedDatebase);
}