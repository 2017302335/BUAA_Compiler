#pragma once
#ifndef MAIN_H
#define MAIN_H
#include <iostream>
#include <string>
#include <vector>
#include <map>
using namespace std;

enum typeId {
	CONSTTK, INTTK, CHARTK, VOIDTK, MAINTK, IFTK, ELSETK, DOTK, WHILETK, FORTK, SCANFTK, PRINTFTK, RETURNTK,  //������
	IDENFR, INTCON, CHARCON, STRCON,
	PLUS, MINU, MULT, DIV, LSS, LEQ, GRE, GEQ, EQL, NEQ, ASSIGN, SEMICN, COMMA, LPARENT, RPARENT, LBRACK, RBRACK, LBRACE, RBRACE
};

enum operation {
	PLUSOP, //+
	MINUOP, //-
	MULTOP, //*
	DIVOP,  // /
	LSSOP,  //<
	LEQOP,  //<=
	GREOP,  //>
	GEQOP,  //>=
	EQLOP,  //==
	NEQOP,  //!=
	ASSIGNOP,  //=
	GOTO,  //��������ת
	BZ,    //������������ת
	BNZ,   //����������ת
	PUSH,  //��������ʱ��������
	CALL,  //��������
	RET,   //�����������
	RETVALUE, //�з���ֵ�������صĽ��
	SCAN,  //��
	PRINT, //д
	LABEL, //���
	CONST, //����
	ARRAY, //����
	VAR,   //����
	FUNC,  //��������
	PARAM, //��������
	GETARRAY,  //ȡ�����ֵ  t = a[]
	PUTARRAY,  //�����鸳ֵ  a[] = t
};

class symbolItem {
public:
	string name;
	int kind; //var const function array
	int type; //int char void
	int constInt;
	char constChar;
	int length;  //���鳤��  ���ں������ڼ�¼��������ж��ٱ���������+�ֲ�+��ʱ)
	vector<int> parameterTable;  //��������
	int addr;   //��ַ 
	symbolItem(string s, int add = 0, int k=0, int t=0, int ci=0, char cc=' ', int l=0) :
		name(s), kind(k), type(t), constInt(ci), constChar(cc), length(l), addr(add) {
		parameterTable = vector<int>();
	}
	symbolItem() {}
	void output() {
		cout << name << " ";
		switch (type) {
		case 1:
			cout << "int ";
			break;
		case 2:
			cout << "char ";
			break;
		case 3:
			cout << "void ";
			break;
		}
		switch (kind) {
		case 1:
			cout << "var ";
			break;
		case 2:
			cout << "const ";
			if (type == 1) {
				cout << constInt << " ";
			}
			else if (type == 2) {
				cout << constChar << " ";
			}
			break;
		case 3:
			cout << "func ";
			cout << "parameters: (";
			for (int i = 0; i < parameterTable.size(); i++) {
				if (parameterTable[i] == 1) {
					cout << "int ";
				}
				else {
					cout << "char ";
				}
			}
			cout << ")" << " ��������: " << length << " ";
			break;
		case 4:
			cout << "array ";
			cout << length << " ";
			break;
		}
		cout << "addr: " << addr;
		cout << "\n";
	}
	void insert(int t) {
		parameterTable.push_back(t);
	}
};

void showGlobal();

void showLocal();

void showAll();

class midCode {  //z = x op y
public:
	operation op; // ����
	string z;     // ���
	string x;     // �������
	string y;     // �Ҳ�����
	midCode(operation o, string zz, string xx, string yy) : op(o), z(zz), x(xx), y(yy) {}
};

string int2string(int t);

string genLabel();

string genTmp();

void outputMidCode();
#endif // !MAIN_H
