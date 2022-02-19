#include "/usr/include/mysql/mysql.h"
#include <iostream>

using namespace std;

//����� SQL ����
MySQL* connectedDatebase;

//DB�� Ŀ���� �Ѵ� = ������ �ܾ��
MYSQL_RES* queryResult;

//�ܾ�� ������ 1��
MySQL_ROW queryRow;

bool MySQLInitialize() {
	if (connectedDatebase = mysql_init((MYSQL*)nullptr)) {
		cout << "MySQL �ʱ�ȭ ����" << endl;
		return false;
	}
	cout << "MySQL �ʱ�ȭ ����" << endl;

	//						mysql ������ ��ġ	mysql�� �ּ�
	if (!(mysql_real_connect(connectedDatebase, "localhost", "root", "1234", nullptr, 3306, nullptr, 0))) {
		cout << "MySQL ���� ����" << endl;
		return false;
	}
	cout << "MySQL ���� ����" << endl;
}

void MySQLClose() {
	mysql_close(connectedDatebase);
}