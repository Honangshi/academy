#include "/usr/include/mysql/mysql.h"
#include <iostream>

using namespace std;

//����� SQL ����
MYSQL* connectedDatebase;

//DB�� Ŀ���� �Ѵ� = ������ �ܾ��
MYSQL_RES* queryResult;

//�ܾ�� ������ 1��
MYSQL_ROW queryRow;

bool MySQLInitialize() {
	if (!(connectedDatebase = mysql_init((MYSQL*)nullptr))) {
		cout << "Cannot Initialize MySQL" << endl;
		return false;
	}
	cout << "MySQL successfully Initialize" << endl;

	//						mysql ������ ��ġ	mysql�� �ּ�
	if (!(mysql_real_connect(connectedDatebase, "localhost", "root", "1234", nullptr, 3306, nullptr, 0))) {
		cout << "Failed to connect MySQL" << endl;
		return false;
	}
	cout << "Successfully Connected to MySQL" << endl;


	//mysql�� ���ǹ� ����
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
