#include "/usr/include/mysql/mysql.h"
#include <iostream>

using namespace std;

//연결된 SQL 저장
MYSQL* connectedDatebase;

//DB에 커리를 한다 = 정보를 긁어옴
MYSQL_RES* queryResult;

//긁어온 정보의 1줄
MYSQL_ROW queryRow;

bool MySQLInitialize() {
	if (!(connectedDatebase = mysql_init((MYSQL*)nullptr))) {
		cout << "Cannot Initialize MySQL" << endl;
		return false;
	}
	cout << "MySQL successfully Initialize" << endl;

	//						mysql 저장할 위치	mysql의 주소
	if (!(mysql_real_connect(connectedDatebase, "localhost", "root", "1234", nullptr, 3306, nullptr, 0))) {
		cout << "Failed to connect MySQL" << endl;
		return false;
	}
	cout << "Successfully Connected to MySQL" << endl;


	//mysql에 질의문 던짐
	if (mysql_query(connectedDatebase, "USE serverdata") != 0) {
		cout << "Database is invalid" << endl;
		return false;
	}

	cout << "Database has open" << endl;
	return true;
}

void MySQLClose() {
	mysql_close(connectedDatebase);
}

void SaveUser(string *id, string color) {
	string query "INSERT INTO userdata(ID, Color) VALUES (";
	query += id;
	query += ", ";
	query += color;
	query += ");";

	if (mysql_query(connectedDatebase, query.c_str()) != 0) {
		cout << "Cannot Save Data" << endl;
		return;
	}
	
	cout << "Save Succeed" << endl;
}
