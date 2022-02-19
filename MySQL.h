#include "/usr/include/mysql/mysql.h"
#include <iostream>

using namespace std;

//연결된 SQL 저장
MYSQL* connectedDatebase;

//DB에 커리를 한다 = 정보를 긁어옴
MYSQL_RES* queryResult;

//긁어온 정보의 1줄
MYSQL_ROW queryRow;

string targetTable = "Userdata";

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

bool LoadUser(string id) {
	string query = "SELECT * FROM ";
	query += targetTable;
	query += "where ID = \"";
	query += id;
	query += "\";";
	if (mysql_query(connectedDatebase, query.c_str()) != 0) {
		return false;
	}
	//select문은 물어보고 가져오는 것 실제로 어딘가에 저장해야 함
	queryResult = mysql_store_result(connectedDatebase);

	//한줄로 받아와야함
	queryRow = mysql_fetch_row(queryResult);

	return true;
}

void SaveUser(string id, string color) {
	string query;

	if (LoadUser(id)) {
		//이미 유저가 있다면 값만 바꿔주면 됨
		query = "UPDATE ";
		query += targetTable;
		query += " SET COLOR=\"";
		query += color;
		query += "\" WHERE ID = \"";
		query += id;
		query += "\";;

	}
	else {
		query = "INSERT INTO";
		query += targetTable;
		query += " (ID, Color) VALUES(\"";
		query += id;
		query += "\", \"";
		query += color;
		query += "\");";
	}

	if (mysql_query(connectedDatebase, query.c_str()) != 0) {
		cout << "Cannot Save Data" << endl;
		return;
	}
	
	cout << "Save Succeed" << endl;
}
