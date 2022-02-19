#include "/usr/include/mysql/mysql.h"
#include <iostream>

using namespace std;

//����� SQL ����
MYSQL* connectedDatebase;

//DB�� Ŀ���� �Ѵ� = ������ �ܾ��
MYSQL_RES* queryResult;

//�ܾ�� ������ 1��
MYSQL_ROW queryRow;

string targetTable = "Userdata";

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

bool LoadUser(string id) {
	string query = "SELECT * FROM ";
	query += targetTable;
	query += "where ID = \"";
	query += id;
	query += "\";";
	if (mysql_query(connectedDatebase, query.c_str()) != 0) {
		return false;
	}
	//select���� ����� �������� �� ������ ��򰡿� �����ؾ� ��
	queryResult = mysql_store_result(connectedDatebase);

	//���ٷ� �޾ƿ;���
	queryRow = mysql_fetch_row(queryResult);

	return true;
}

void SaveUser(string id, string color) {
	string query;

	if (LoadUser(id)) {
		//�̹� ������ �ִٸ� ���� �ٲ��ָ� ��
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
