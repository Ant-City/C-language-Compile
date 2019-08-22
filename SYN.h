#ifndef LEX
#define LEX
#include "write_xml\tinystr.h"
#include "write_xml\tinyxml.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <cstring>
#include <iostream>
using namespace std;
//�������Σ���ǰλ�õ�ֵ����ǰλ��
//�ṩ�ʷ������õ��ĵ�ǰֵ
//ʹ��ǰ�����ʷ������Ľ�β + #��ʾ����
int point = 0;//��ǰָ�� - ������ʱ��ָ���λ�ò���# ˵�����м�λ�ó����˴���
struct ww
{
	char value[255];
	char type[20];
	int line;
}word[200];
//ע����������ַ�->��������
//validֵ������Ҫ ���ֱ�ӷ���
void write(TiXmlElement * pNew)
{
	TiXmlElement *pTxt = new TiXmlElement(word[point].type);
	pNew->LinkEndChild(pTxt);
	TiXmlText * Content = new TiXmlText(word[point].value);
	pTxt->LinkEndChild(Content);
}
void read(char target[])
{
	int num = 0;
	TiXmlDocument mydoc(target);//xml�ĵ�����  
	bool loadOk = mydoc.LoadFile();//�����ĵ�  
	if (!loadOk)
	{
		cout << "could not load the test file.Error:" << mydoc.ErrorDesc() << endl;
		exit(1);
	}

	TiXmlElement *RootElement = mydoc.RootElement();  //��Ԫ��, Info  
	TiXmlElement *pEle = RootElement;
	TiXmlElement *pTokens = pEle->FirstChildElement();
	//�����ý��  
	for (TiXmlElement *tokenElement = pTokens->FirstChildElement();//��һ����Ԫ��  
		tokenElement != NULL;
		tokenElement = tokenElement->NextSiblingElement())//��һ���ֵ�Ԫ��  
	{
		TiXmlAttribute *pAttr = tokenElement->FirstAttribute();//��һ������
		int j = 0;
		for (TiXmlElement *sonElement = tokenElement->FirstChildElement();
			sonElement != NULL;
			sonElement = sonElement->NextSiblingElement(), j++)
		{
			j %= 4;
			if (j == 0)
				strcpy(word[num].value, sonElement->FirstChild()->Value());
			else if (j == 1)
				strcpy(word[num].type, sonElement->FirstChild()->Value());
			else if (j == 2)
			{
				char tmp_line[20];
				strcpy(tmp_line, sonElement->FirstChild()->Value());
				//ת�����ֺ��ַ�
				int len = strlen(tmp_line);
				word[num].line = 0;
				for (int k = len - 1; k >= 0; k--)
				{
					word[num].line += pow(10, len - k - 1)*(tmp_line[k] - '0');
				}
			}
			else
				num++;
		}
	}
}
void CMPL_UNIT(TiXmlElement * pEnd);
void FUNC_LIST(TiXmlElement * pEnd);
void FUNC_DEF(TiXmlElement * pEnd);
void TYPE_SPEC(TiXmlElement * pEnd);
void PARA_LIST(TiXmlElement * pEnd);
void ARGUMENT(TiXmlElement * pEnd);
void CODE_BLOCK(TiXmlElement * pEnd);
void STMT_LIST(TiXmlElement * pEnd);
void STMT(TiXmlElement * pEnd);
void RTN_STMT(TiXmlElement * pEnd);
void ASSIGN_STMT(TiXmlElement * pEnd);
void EXPR(TiXmlElement * pEnd);
void EXPR2(TiXmlElement * pEnd);
void TERM(TiXmlElement * pEnd);
void TERM2(TiXmlElement * pEnd);
void FACTOR(TiXmlElement * pEnd);
void WHILE(TiXmlElement * pEnd);
void CONDITION(TiXmlElement * pEnd);
void IF(TiXmlElement * pEnd);
void ELSEIF(TiXmlElement * pEnd);
void RELATION(TiXmlElement * pEnd);
void DECLARE(TiXmlElement * pEnd);
void DECLARE_STMT(TiXmlElement * pEnd);


void CMPL_UNIT(TiXmlElement * pEnd)
{
	TiXmlElement *pNew = new TiXmlElement("CMPL_UNIT");
	pEnd->LinkEndChild(pNew);
	FUNC_LIST(pNew);
}
//��FOLLOW����
void FUNC_LIST(TiXmlElement * pEnd)
{   //��ȡ�յ����
	if (strcmp(word[point].value, "#") != 0)
	{
		TiXmlElement *pNew = new TiXmlElement("FUNC_LIST");
		pEnd->LinkEndChild(pNew);
		FUNC_DEF(pNew);
		FUNC_LIST(pNew);
	}
}
void FUNC_DEF(TiXmlElement * pEnd)
{
	TiXmlElement *pNew = new TiXmlElement("FUNC_DEF");
	pEnd->LinkEndChild(pNew);
	TYPE_SPEC(pNew);
	if (strcmp(word[point].type, "identifer") == 0)//˵���Ǳ���
	{
		write(pNew);
		point++;
		if (strcmp(word[point].value, "(") == 0)
		{
			write(pNew);
			point++;
			PARA_LIST(pNew);
			if (strcmp(word[point].value, ")") == 0)
			{
				write(pNew);
				point++;
				CODE_BLOCK(pNew);
			}
		}
	}
}
void TYPE_SPEC(TiXmlElement * pEnd)
{
	TiXmlElement *pNew = new TiXmlElement("TYPE_SPEC");
	pEnd->LinkEndChild(pNew);
	if (strcmp(word[point].value, "int") == 0 ||
		strcmp(word[point].value, "void") == 0)
	{
		write(pNew);
		point++;
	}
}
//FOLLOW����
void PARA_LIST(TiXmlElement * pEnd)
{
	if (strcmp(word[point].value, ")") != 0)
	{
		TiXmlElement *pNew = new TiXmlElement("PARA_LIST");
		pEnd->LinkEndChild(pNew);
		ARGUMENT(pNew);
		if (strcmp(word[point].value, ",") == 0)
		{
			write(pNew);
			point++;
			PARA_LIST(pNew);
		}
	}
}
void ARGUMENT(TiXmlElement * pEnd)
{
	TiXmlElement *pNew = new TiXmlElement("ARGUMENT");
	pEnd->LinkEndChild(pNew);
	TYPE_SPEC(pNew);
	if (strcmp(word[point].type, "identifer") == 0)
	{
		write(pNew);
		point++;
	}
}
void CODE_BLOCK(TiXmlElement * pEnd)
{
	TiXmlElement *pNew = new TiXmlElement("CODE_BLOCK");
	pEnd->LinkEndChild(pNew);
	if (strcmp(word[point].value, "{") == 0)
	{
		write(pNew);
		point++;
		STMT_LIST(pNew);
		if (strcmp(word[point].value, "}") == 0)
		{
			write(pNew);
			point++;
		}
	}
}
//FOLLOW����
void STMT_LIST(TiXmlElement * pEnd)
{
	TiXmlElement *pNew = new TiXmlElement("STMT_LIST");
	pEnd->LinkEndChild(pNew);
	if (strcmp(word[point].value, "}") != 0)
	{
		STMT(pNew);
		STMT_LIST(pNew);
	}
}
void STMT(TiXmlElement * pEnd)
{
	TiXmlElement *pNew = new TiXmlElement("STMT");
	pEnd->LinkEndChild(pNew);
	if (strcmp(word[point].value, "return") == 0)
	{
		RTN_STMT(pNew);
		if (strcmp(word[point].value, ";") == 0)
		{
			write(pNew);
			point++;
		}
	}
	//�۲�first���ϵĲ�ͬ��ѡ��ֵ    
	else if (strcmp(word[point].type, "identifer") == 0)
	{
		ASSIGN_STMT(pNew);
		if (strcmp(word[point].value, ";") == 0)
		{
			write(pNew);
			point++;
		}
	}
	else if (strcmp(word[point].value, "int") == 0)
	{
		DECLARE(pNew);
		if (strcmp(word[point].value, ";") == 0)
		{
			write(pNew);
			point++;
		}
	}
	else if (strcmp(word[point].value, "if") == 0) IF(pNew);
	else if (strcmp(word[point].value, "while") == 0) WHILE(pNew);
}
void RTN_STMT(TiXmlElement * pEnd)
{
	TiXmlElement *pNew = new TiXmlElement("RTN_STMT");
	pEnd->LinkEndChild(pNew);
	if (strcmp(word[point].value, "return") == 0)
	{
		write(pNew);
		point++;
		EXPR(pNew);
		/*if (strcmp(word[point].value, ";") == 0)
		{
			write(pNew);
			point++;			
		}*/
	}
}
void ASSIGN_STMT(TiXmlElement * pEnd)
{
	TiXmlElement *pNew = new TiXmlElement("ASSIGN_STMT");
	pEnd->LinkEndChild(pNew);
	if (strcmp(word[point].type, "identifer") == 0)
	{
		write(pNew);
		point++;
		if (strcmp(word[point].value, "=") == 0)
		{
			write(pNew);
			point++;
			EXPR(pNew);
			/*if (strcmp(word[point].value, ";") == 0)
			{
				write(pNew);
				point++;
			}*/
		}
	}
}
void EXPR(TiXmlElement * pEnd)
{
	TiXmlElement *pNew = new TiXmlElement("EXPR");
	pEnd->LinkEndChild(pNew);
	TERM(pNew);
	EXPR2(pNew);
}
//FOLLOW����
void EXPR2(TiXmlElement * pEnd)
{
	if (strcmp(word[point].value, ";") != 0 &&
		strcmp(word[point].value, ")") != 0 && 
		strcmp(word[point].value, ",") != 0 &&
		strcmp(word[point].value, "<") != 0 &&
		strcmp(word[point].value, ">") != 0 &&
		strcmp(word[point].value, "<=") != 0 &&
		strcmp(word[point].value, ">=") != 0 &&
		strcmp(word[point].value, "!=") != 0 &&
		strcmp(word[point].value, "==") != 0)
	{
		TiXmlElement *pNew = new TiXmlElement("EXPR2");
		pEnd->LinkEndChild(pNew);
		if (strcmp(word[point].value, "+") == 0)
		{
			write(pNew);
			point++;
			TERM(pNew);
			EXPR2(pNew);
		}
		else if (strcmp(word[point].value, "-") == 0)
		{
			point++;
			TERM(pNew);
			EXPR2(pNew);
		}
	}
}
void TERM(TiXmlElement * pEnd)
{
	TiXmlElement *pNew = new TiXmlElement("TERM");
	pEnd->LinkEndChild(pNew);
	FACTOR(pNew);
	TERM2(pNew);
}
//FOLLOW����;
void TERM2(TiXmlElement * pEnd)
{
	if (strcmp(word[point].value, ";") != 0 &&
		strcmp(word[point].value, ")") != 0 &&
		strcmp(word[point].value, ",") != 0 &&
		strcmp(word[point].value, "+") != 0 &&
		strcmp(word[point].value, "-") != 0 &&
		strcmp(word[point].value, "<") != 0 && 
		strcmp(word[point].value, ">") != 0 &&
		strcmp(word[point].value, "<=") != 0 &&
		strcmp(word[point].value, ">=") != 0 &&
		strcmp(word[point].value, "!=") != 0 &&
		strcmp(word[point].value, "==") != 0 
		)
	{
		TiXmlElement *pNew = new TiXmlElement("TERM2");
		pEnd->LinkEndChild(pNew);
		if (strcmp(word[point].value, "*") == 0)
		{
			write(pNew);
			point++;
			FACTOR(pNew);
			EXPR2(pNew);
		}
		else if (strcmp(word[point].value, "/") == 0)
		{
			write(pNew);
			point++;
			FACTOR(pNew);
			EXPR2(pNew);
		}
	}
}
void FACTOR(TiXmlElement * pEnd)
{
	TiXmlElement *pNew = new TiXmlElement("FACTOR");
	pEnd->LinkEndChild(pNew);
	if (strcmp(word[point].type, "identifer") == 0)//���Ǳ�ʶ��ʱ
	{
		write(pNew);
		point++;
	}
	else if (strcmp(word[point].type, "const_i") == 0 ||
		     strcmp(word[point].type, "const_f") == 0 ||
		     strcmp(word[point].type, "stringLiteral") == 0 ||
		     strcmp(word[point].type, "characterConstant") == 0)//���ǳ���ʱ
	{
		write(pNew);
		point++;
	}
	else if (strcmp(word[point].value, "(") == 0)
	{
		write(pNew);
		point++;
		EXPR(pNew);
		if (strcmp(word[point].value, ")") == 0)
		{
			write(pNew);
			point++;
		}
	}
}
void IF(TiXmlElement * pEnd)
{
	TiXmlElement *pNew = new TiXmlElement("IF");
	pEnd->LinkEndChild(pNew);
	if (strcmp(word[point].value, "if") == 0)
	{
		write(pNew);
		point++;
		if (strcmp(word[point].value, "(") == 0)
		{
			write(pNew);
			point++;
			CONDITION(pNew);
			if (strcmp(word[point].value, ")") == 0)
			{
				write(pNew);
				point++;
				CODE_BLOCK(pNew);
				ELSEIF(pNew);
			}
		}
	}
    
}
void ELSEIF(TiXmlElement * pEnd)
{
	TiXmlElement *pNew = new TiXmlElement("ELSEIF");
	pEnd->LinkEndChild(pNew);
	if (strcmp(word[point].value, "if") != 0 &&
		strcmp(word[point].value, "return") != 0 &&
		strcmp(word[point].value, "while") != 0 &&
		strcmp(word[point].type, "identifer") != 0)
	{
		if (strcmp(word[point].value, "else") == 0)
		{
			write(pNew);
			point++;
			CODE_BLOCK(pNew);
		}
	}
}
void WHILE(TiXmlElement * pEnd)
{
	TiXmlElement *pNew = new TiXmlElement("WHILE");
	pEnd->LinkEndChild(pNew);
	if (strcmp(word[point].value, "while") == 0)
	{
		write(pNew);
		point++;
		if (strcmp(word[point].value, "(") == 0)
		{
			write(pNew);
			point++;
			CONDITION(pNew);
			if (strcmp(word[point].value, ")") == 0)
			{
				write(pNew);
				point++;
				CODE_BLOCK(pNew);
			}
		}
	}
}
void CONDITION(TiXmlElement * pEnd)
{
	TiXmlElement *pNew = new TiXmlElement("CONDITION");
	pEnd->LinkEndChild(pNew);
	EXPR(pNew);
	RELATION(pNew);
	EXPR(pNew);
}
void RELATION(TiXmlElement * pEnd)
{
	TiXmlElement *pNew = new TiXmlElement("RELATION");
	pEnd->LinkEndChild(pNew);
	int flag = 0;
	
	if (strcmp(word[point].value, ">") == 0) {flag = 1; }
	else if (strcmp(word[point].value, ">=") == 0) {flag = 1;}
	else if (strcmp(word[point].value, "==") == 0) {flag = 1;}
	else if (strcmp(word[point].value, "<") == 0) {flag = 1; }
	else if (strcmp(word[point].value, "<=") == 0) {flag = 1;}
	else if (strcmp(word[point].value, "!=") == 0) {flag = 1;}
	if (flag == 1)
	{
		write(pNew);
		point++;
	}
}
void DECLARE(TiXmlElement * pEnd)
{
	TiXmlElement *pNew = new TiXmlElement("DECLARE");
	pEnd->LinkEndChild(pNew);
	TYPE_SPEC(pNew);
	DECLARE_STMT(pNew);
}
void DECLARE_STMT(TiXmlElement * pEnd)
{
	TiXmlElement *pNew = new TiXmlElement("DECLARE_STMT");
	pEnd->LinkEndChild(pNew);

	if (strcmp(word[point].type, "identifer") == 0 &&
		strcmp(word[point + 1].value, "=") != 0)
	{
		write(pNew);
		point++;
		/*if (strcmp(word[point].value, ";") == 0)
		{   write(pNew);
			point++;
		}*/
		if (strcmp(word[point].value, ",") == 0)
		{
			write(pNew);
			point++;
			DECLARE_STMT(pNew);
		}
	}
	else if (strcmp(word[point].type, "identifer") == 0&&
			 strcmp(word[point+1].value, "=") == 0)
	{
		ASSIGN_STMT(pNew);
		if (strcmp(word[point].value, ",") == 0)
		{
			write(pNew);
			point++;
			DECLARE_STMT(pNew);
		}
	}
}
#endif // LEX.h
