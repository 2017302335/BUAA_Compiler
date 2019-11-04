#include "lexical.h"
#include <iostream>
#include <fstream>
#include <cstdio>
#include <cstring>
#include <string>
#include "main.h"
using namespace std;

extern char ch;
extern char token[100000];
extern int tokenI;
extern int num;   //��¼���γ���
extern char con_ch;  //��¼�ַ��ͳ���
extern char s[100000];  //��¼�ַ�������
extern enum typeId symbol;
extern int len_reservedWord;
extern char reservedWord[20][10];
extern string filecontent;  //�ļ�������
extern ifstream inputfile;
extern ofstream outputfile;
extern ofstream errorfile;
extern int indexs;  //�ļ�������
extern int oldIndex;    //�������ָ�
extern int line;  //�к�

bool isSpace()
{
	return (ch == ' ');
}

bool isNewline()
{
	return (ch == '\n');
}

bool isBlank()
{
	return (ch == ' ' || ch == '\n' || ch == '\t' || ch == '\r' || ch == '\f' || ch == '\v');
}

bool isLetter()
{
	return ((ch >= 'A' && ch <= 'Z') || (ch >= 'a' && ch <= 'z') || (ch == '_'));
}

bool isDigit()
{
	return (ch >= '0' && ch <= '9');
}

bool isPlus()
{
	return (ch == '+');
}

bool isMinu()
{
	return (ch == '-');
}

bool isMult()
{
	return (ch == '*');
}

bool isDiv()
{
	return (ch == '/');
}

bool isChar() {
	return (isLetter() || isDigit() || isPlus() || isMinu() || isMult() || isDiv());
}

bool isLss()
{
	return (ch == '<');
}

bool isGre()
{
	return (ch == '>');
}

bool isExcla()
{
	return (ch == '!');
}

bool isAssign()
{
	return (ch == '=');
}

bool isSemicn()
{
	return (ch == ';');
}

bool isComma()
{
	return (ch == ',');
}

bool isLparent()
{
	return (ch == '(');
}

bool isRparent()
{
	return (ch == ')');
}

bool isLbrack()
{
	return (ch == '[');
}

bool isRbrack()
{
	return (ch == ']');
}

bool isLbrace()
{
	return (ch == '{');
}

bool isRbrace()
{
	return (ch == '}');
}

bool isSquo()
{
	return (ch == '\'');
}

bool isDquo()
{
	return (ch == '\"');
}

bool isEOF()
{
	//return ch == EOF;
	return indexs >= filecontent.size();
}

bool isStringChar()
{
	return ((ch >= 35 && ch <= 126) || ch == 32 || ch == 33);
}

bool isFactorFellow()
{
	return (isLss() || isGre() || isAssign() || isExcla()
		|| isPlus() || isMinu() || isMult() || isDiv()
		|| isRparent() || isRbrack() || isComma() || isSemicn());
}

void clearToken()
{  //���token
	tokenI = 0;
}

void catToken()
{  //�ѵ�ǰ�ַ�chƴ��token��
	token[tokenI++] = ch;
}

void get_ch()
{  //��һ���ַ�
	ch = filecontent[indexs++];
	if (ch == '\n') {
		line++;
	}
}

void retract()
{  //���ַ�д�ص�������
	if (ch == '\n') {
		line--;
	}
	indexs--;
}

void retractString(int old) {
	for (int i = old; i < indexs; i++) {
		if (filecontent[i] == '\n') {
			line--;
		}
	}
	indexs = old;
}

int reserver()
{  //���ұ����� ����-1�����Ǳ�ʶ�� ���Ǳ����� ���򷵻ر����ֱ���
	for (int i = 0; i < len_reservedWord; i++) {
		if (strcmp(reservedWord[i], token) == 0) {
			return i;
		}
	}
	return -1;
}

int transNum()
{
	int res = 0;
	for (int i = 0; i < tokenI; i++) {
		res = res * 10 + (token[i] - '0');
	}
	return res;
}

int getsym(int out)
{
	oldIndex = indexs;  //��¼û�ж�ȡ֮ǰ��indexsΪoldIndex
	clearToken();
	get_ch();
	while (isBlank()) {
		get_ch();
	}// һֱ�� ֱ��ch���ǿհ׷�
	if (isEOF()) {
		return -1;
	}
	if (isLetter()) {  //  'a'-'z' 'A'-'Z' '_'
		while (isLetter() || isDigit()) {  //ƴ��ʶ�� ����ʶ����::=����ĸ��������ĸ���������֣���
			catToken();  //��ch���ӵ�token��
			get_ch();  //��ch
		}
		//��ʱch��������ĸ���������� ˵����ʶ��������
		retract();  //����ch
		token[tokenI] = '\0';
		int resultValue = reserver();  //���ұ����ֱ�
		if (resultValue == -1) {  //��ʶ��
			symbol = IDENFR;
		}
		else {  //������
			symbol = (typeId)resultValue;
		}
		return 1;
	}
	else if (isDigit()) {  // '0'-'9'  ����
		while (isDigit()) {
			catToken();
			get_ch();
		}
		//��ʱch������������
		retract();
		token[tokenI] = '\0';
		num = transNum();
		symbol = INTCON;  //���ͳ���
		return 1;
	}
	else if (isSquo()) {  // '
		get_ch();
		if (isChar()) {
			char tmp = ch;  //��ʱ��¼
			get_ch();
			if (isSquo()) {
				con_ch = tmp;
				symbol = CHARCON;  //�ַ�����
			}
			else {
				//wrong! retract(); ȱ���ҵ����� �����ϴʷ�
				if (out) {
					errorfile << line << " a\n";  //�����ϴʷ�
				}
				int old = indexs;
				while (1) {
					if (isSquo()) {
						break;
					}
					if (isNewline()) {
						retractString(old-1);  //�ص���������˵� ������'��λ��
						break;
					}
					if (isFactorFellow()) {  //, ; + - * / > < ! =
						indexs--;  //��Ҫ��,;�˻�ȥ
						break;
					}
					get_ch();
				}
				con_ch = tmp;
				symbol = CHARCON;  //�ַ�����
			}
		}
		else {
			//wrong! retract();  �ַ�����a-z A-Z _+-*/
			if (out) {
				errorfile << line << " a\n";  //�����ϴʷ�
			}
			char tmp = ch;
			get_ch();
			if (isSquo()) {
				con_ch = tmp;
				symbol = CHARCON;  //�ַ�����
			}
			else {
				//wrong! retract(); ȱ���ҵ����� �����ϴʷ�
				int old = indexs;
				while (1) {
					if (isSquo()) {
						break;
					}
					if (isNewline()) {
						retractString(old - 1);  //�ص���������˵� ������'��λ��
						break;
					}
					if (isFactorFellow()) {  //, ; + - * / > < ! =
						indexs--;  //��Ҫ��,;�˻�ȥ
						break;
					}
					get_ch();
				}
				con_ch = tmp;
				symbol = CHARCON;  //�ַ�����
			}
		}
		return 1;
	}
	else if (isDquo()) {  // "
		get_ch();
		while (isStringChar()) {
			catToken();
			get_ch();
		}
		//��ʱch�����ǿ�������ַ������ַ�
		if (isDquo()) {  //�����˽�����˫����
			symbol = STRCON;  //�ַ�������
			token[tokenI] = '\0';
			strcpy(s, token);  //�浽s��
		}
		else {  //wrong! retract(); ȱ����˫���� �����ϴʷ�
			if (isNewline()) {
				line--;
				if (out) {
					errorfile << line << " a\n";  //�����ϴʷ�
				}
				while (1) {
					indexs--;
					if (filecontent[indexs] == ')') {
						break;
					}
					tokenI--;
				}
				symbol = STRCON;  //�ַ�������
				token[tokenI] = '\0';
				strcpy(s, token);  //�浽s��
			}
		}
		return 1;
	}
	else if (isPlus()) {  // +
		symbol = PLUS;
		return 1;
	}
	else if (isMinu()) {  // -
		symbol = MINU;
		return 1;
	}
	else if (isMult()) {  // *
		symbol = MULT;
		return 1;
	}
	else if (isDiv()) {  // /
		symbol = DIV;
		return 1;
	}
	else if (isLss()) {  // <
		get_ch();
		if (isAssign()) {  //<=
			symbol = LEQ;
		}
		else {
			symbol = LSS;  //<
			retract();  //����
		}
		return 1;
	}
	else if (isGre()) {  // >
		get_ch();
		if (isAssign()) {  //>=
			symbol = GEQ;
		}
		else {
			symbol = GRE;  //>
			retract();  //����
		}
		return 1;
	}
	else if (isExcla()) {  // !
		get_ch();
		if (isAssign()) {  // !=
			symbol = NEQ;
		}
		else {
			//wrong! retract();  �����ȱ��=
			retract();
			if (out) {
				errorfile << line << " a\n";  //�����ϴʷ�
			}
			symbol = NEQ;
		}
		return 1;
	}
	else if (isAssign()) {  // =
		get_ch();
		if (isAssign()) {  //==
			symbol = EQL;
		}
		else {
			symbol = ASSIGN;
			retract();  //����
		}
		return 1;
	}
	else if (isSemicn()) {  // ;
		symbol = SEMICN;
		return 1;
	}
	else if (isComma()) {  // ,
		symbol = COMMA;
		return 1;
	}
	else if (isLparent()) {  // (
		symbol = LPARENT;
		return 1;
	}
	else if (isRparent()) {  // )
		symbol = RPARENT;
		return 1;
	}
	else if (isLbrack()) {  // [
		symbol = LBRACK;
		return 1;
	}
	else if (isRbrack()) {  // ]
		symbol = RBRACK;
		return 1;
	}
	else if (isLbrace()) {  // {
		symbol = LBRACE;
		return 1;
	}
	else if (isRbrace()) {  // }
		symbol = RBRACE;
		return 1;
	}
	else {
		//wrong! retract();
		if (out) {
			errorfile << line << " a\n";  //�����ϴʷ�
		}
		return getsym(out);
	}
}

void doOutput()
{
	switch (symbol) {
	case IDENFR:
		outputfile << "IDENFR " << token << endl;
		break;
	case INTCON:
		outputfile << "INTCON " << token << endl;
		break;
	case CHARCON:
		outputfile << "CHARCON " << con_ch << endl;
		break;
	case STRCON:
		outputfile << "STRCON " << s << endl;
		break;
	case CONSTTK:
		outputfile << "CONSTTK const" << endl;
		break;
	case INTTK:
		outputfile << "INTTK int" << endl;
		break;
	case CHARTK:
		outputfile << "CHARTK char" << endl;
		break;
	case VOIDTK:
		outputfile << "VOIDTK void" << endl;
		break;
	case MAINTK:
		outputfile << "MAINTK main" << endl;
		break;
	case IFTK:
		outputfile << "IFTK if" << endl;
		break;
	case ELSETK:
		outputfile << "ELSETK else" << endl;
		break;
	case DOTK:
		outputfile << "DOTK do" << endl;
		break;
	case WHILETK:
		outputfile << "WHILETK while" << endl;
		break;
	case FORTK:
		outputfile << "FORTK for" << endl;
		break;
	case SCANFTK:
		outputfile << "SCANFTK scanf" << endl;
		break;
	case PRINTFTK:
		outputfile << "PRINTFTK printf" << endl;
		break;
	case RETURNTK:
		outputfile << "RETURNTK return" << endl;
		break;
	case PLUS:
		outputfile << "PLUS +" << endl;
		break;
	case MINU:
		outputfile << "MINU -" << endl;
		break;
	case MULT:
		outputfile << "MULT *" << endl;
		break;
	case DIV:
		outputfile << "DIV /" << endl;
		break;
	case LSS:
		outputfile << "LSS <" << endl;
		break;
	case LEQ:
		outputfile << "LEQ <=" << endl;
		break;
	case GRE:
		outputfile << "GRE >" << endl;
		break;
	case GEQ:
		outputfile << "GEQ >=" << endl;
		break;
	case EQL:
		outputfile << "EQL ==" << endl;
		break;
	case NEQ:
		outputfile << "NEQ !=" << endl;
		break;
	case ASSIGN:
		outputfile << "ASSIGN =" << endl;
		break;
	case SEMICN:
		outputfile << "SEMICN ;" << endl;
		break;
	case COMMA:
		outputfile << "COMMA ," << endl;
		break;
	case LPARENT:
		outputfile << "LPARENT (" << endl;
		break;
	case RPARENT:
		outputfile << "RPARENT )" << endl;
		break;
	case LBRACK:
		outputfile << "LBRACK [" << endl;
		break;
	case RBRACK:
		outputfile << "RBRACK ]" << endl;
		break;
	case LBRACE:
		outputfile << "LBRACE {" << endl;
		break;
	case RBRACE:
		outputfile << "RBRACE }" << endl;
		break;
	default:
		break;
	}
}