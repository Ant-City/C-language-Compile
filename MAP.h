#ifndef MAP
#define MAP
#include "write_xml\tinystr.h"
#include "write_xml\tinyxml.h"
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#define INT 0
#define CHAR 1
using namespace std;

struct tt//symbol list
{
	string name;//name in c
	string symbol;// name in X86
}maplist[255];

int numSYM = 0;
int flagFALSE[255] = { 0 };//flag of FALSE 2 - use without appearance 1 - appearance and use 0 - no use
int flagDONE[255] = { 0 };
int flagBEGIN[255] = { 0 };
int flagIF =  0 ;//flag of IF 
int flagWHILE = 0 ;//flag of WHILE
int flagELSE = 0;
int flagCONDITION = 0;
int numFALSE = 0;
int numDONE = 0;
int flagASS_STMT = 0;//true when ASS_STMT is used; false not used or quit from
int flagDECLARE = 0;
int flagEXPR = 0;
int flagINT = 0;//���ֿ����ڴ�Ĵ�С
int flagCHAR = 0;
int flagFirst = 0;//���ֱ��ʽ�е�����
int flagSecond = 0;
int flag_tmp[10];
int tmp_count;

int numBEGIN = 0;
char tagFALSE[20] = "FALSE";
char tagDONE[20] = "DONE";
char tagBEGIN[20] = "BEGIN";
char mc[255];
char x86Symbol[20] = "Sym";
char x86Tmp[20] = "temp";

int tmpB;
int buffer_count = 0;
int c_buffer_count = 0;
int ass_buffer_count = 0;
int con_buffer_count = 0;
string const_buffer[2];
string buffer[2];
string opbuffer;
string con_buffer[2];
string ass_buffer[2];
string sizebuffer;
string tmpRelation;
ofstream ofile;
void readxml(TiXmlElement *node)                     //��ȡxml�ļ�
{
	while (node)
	{
		string tagValue;//get the tag and see what it is 
		string tagText;
		//take the content
		tagValue = node->Value();
		if (node->GetText() != NULL)
		{
			//cout << "=" << node->GetText() << endl;
			tagText = node->GetText();
		}
		else tagText = "\0";
		//����ʵ�ʵ��������ѡ��
		if (tagValue == "ASSIGN_STMT") {
			flagASS_STMT = 1;
			//�ݹ�
			if (node->FirstChildElement() != NULL)
			{
				readxml(node->FirstChildElement());    //�������ӽڵ㣬����еݹ����
			}
			flagASS_STMT = 0;
			if (flagDECLARE == 0)
			{
				ofile << "mov " << ass_buffer[0] << "," << ass_buffer[1] << endl;
				ass_buffer_count = 0;
			}
		}
		else if (tagValue == "DECLARE") {
			flagDECLARE = 1;//��ȡ��ǣ�������Ĳ�����ʾ����ǰ���������������
			if (node->FirstChildElement() != NULL)
			{
				readxml(node->FirstChildElement());    //�������ӽڵ㣬����еݹ����
			}
			flagDECLARE = 0;
		}
		else if (tagValue == "IF") {
			flagIF = flagWHILE + 1;
			//�ݹ�
			if (node->FirstChildElement() != NULL)
			{
				readxml(node->FirstChildElement());    //�������ӽڵ㣬����еݹ����
			}

			flagIF = 0;
			//д��������
			 
			for (int i = numDONE; i >= 0; i--)
			{
				if (flagDONE[i] == 2)//�ҵ����һ��ѹ��� д�����
				{
					ofile << tagDONE << i << ":" << endl;
					flagDONE[i] = 1;
				}
			}
		}
		else if (tagValue == "WHILE") {
			flagWHILE = flagIF + 1;
			 
			ofile << tagBEGIN << numBEGIN << ":" << endl;
			tmpB = numBEGIN++;
			//�ݹ�
			if (node->FirstChildElement() != NULL)
			{
				readxml(node->FirstChildElement());    //�������ӽڵ㣬����еݹ����
			}

			flagWHILE = 0;
			 
			for (int i = numFALSE; i >= 0; i--)
			{
				if (flagFALSE[i] == 2)//�ҵ����һ��ѹ��� д�����
				{
					ofile << tagFALSE << i << ":" << endl;
					flagFALSE[i] = 1;
					break;
				}
			}
		}
		else if (tagValue == "EXPR") 
		{
			flagEXPR = 1;
			if (node->FirstChildElement() != NULL)
			{
				readxml(node->FirstChildElement());    //�������ӽڵ㣬����еݹ����
			}
			//���ڲ�ͬ��������д���
			if (buffer_count == 2)
			{
				ofile << "mov eax," << buffer[0] << endl;
				ofile << opbuffer << buffer[1] << endl;
				buffer_count = 0;
				if (opbuffer == "div")//�������Ľ��λ�ò�ͬ
				{
					ofile << "mov " << x86Tmp << tmp_count << ",ax" << endl;
				}
				else
				{
					ofile << "mov " << x86Tmp << tmp_count <<",eax"<< endl;
				}
				char tmp2[255];
				sprintf(tmp2, "%s%d", x86Tmp, tmp_count++);
				if (flagASS_STMT == 1)
				{
					ass_buffer[ass_buffer_count] = tmp2;
					ass_buffer_count++;
				}
				else if (flagCONDITION == 1)
				{
					con_buffer[con_buffer_count] = tmp2;
					con_buffer_count++;
				}
			}
			else if (buffer_count == 1)
			{
				if (c_buffer_count == 1)
				{
					ofile << "mov eax," << buffer[0] << endl;
					ofile << opbuffer << const_buffer[0] << endl;
					buffer_count = 0;
					c_buffer_count = 0;
					if (opbuffer == "div")//�������Ľ��λ�ò�ͬ
					{
						ofile << "mov " << x86Tmp << tmp_count << ",ax" << endl;
					}
					else
					{
						ofile << "mov " << x86Tmp << tmp_count << ",eax" << endl;
					}
					char tmp2[255];
					sprintf(tmp2, "%s%d", x86Tmp, tmp_count++);
					if (flagASS_STMT == 1)
					{
						ass_buffer[ass_buffer_count] = tmp2;
						ass_buffer_count++;
					}
					else if (flagCONDITION == 1)
					{
						con_buffer[con_buffer_count] = tmp2;
						con_buffer_count++;
					}
				}
				else
				{
					//��ʱֻ��һ����ʶ�� ˵����ʱ����ֱ���÷��Ž��б�ʾ
					buffer_count = 0;
					
					if (flagASS_STMT == 1)
					{
						ass_buffer[ass_buffer_count] = buffer[0];
						ass_buffer_count++;
					}
					else if (flagCONDITION == 1)
					{
						con_buffer[con_buffer_count] = buffer[0];
						con_buffer_count++;
					}
				}
			}
			else if (c_buffer_count == 2)
			{
				ofile << "mov eax," << const_buffer[0] << endl;
				ofile << opbuffer << const_buffer[1] << endl;
				//buffer_count = 0;
				c_buffer_count = 0;
				if (opbuffer == "div")//�������Ľ��λ�ò�ͬ
				{
					ofile << "mov " << x86Tmp << tmp_count << ",ax" << endl;
				}
				else
				{
					ofile << "mov " << x86Tmp << tmp_count << ",eax" << endl;
				}
				//tmp_count++;
				c_buffer_count = 0;
				char tmp2[255];
				sprintf(tmp2, "%s%d", x86Tmp, tmp_count++);
				if (flagASS_STMT == 1)
				{
					ass_buffer[ass_buffer_count] = tmp2;
					ass_buffer_count++;
				}
				else if (flagCONDITION == 1)
				{
					con_buffer[con_buffer_count] = tmp2;
					con_buffer_count++;
				}
			}
			else if (c_buffer_count == 1)
			{
				c_buffer_count = 0;
				
				if (flagASS_STMT == 1)
				{
					ass_buffer[ass_buffer_count] = const_buffer[0];
					ass_buffer_count++;
				}
				else if (flagCONDITION == 1)
				{
					con_buffer[con_buffer_count] = const_buffer[0];
					con_buffer_count++;
				}
				c_buffer_count = 0;
			}
			flagEXPR = 0;
		}//in the EXPR, make sure the operator and do it
		else if (tagValue == "ELSEIF")
		{
			flagELSE = 1;
			for (int i = numFALSE; i >= 0; i--)
			{
				if (flagFALSE[i] == 2)
				{
					ofile << tagFALSE << i << ":" << endl;
					flagFALSE[i] = 1;
					break;//���֣���δ����
				}
			}
			if (node->FirstChildElement() != NULL)
			{
				readxml(node->FirstChildElement());    //�������ӽڵ㣬����еݹ����
			}
			flagELSE = 0;
		}
		else if (tagValue == "CODE_BLOCK") 
		{ //FALSE д�� WHILE ������ ��IF ������(�ڲ�CODEBLOCK��β��)��DONE д��WHILE��IF�ĳ���λ��
			if (node->FirstChildElement() != NULL)
			{
				readxml(node->FirstChildElement());    //�������ӽڵ㣬����еݹ����
			}
			if (flagIF >= 1) //��д��ת��� ��ʱ��if��ӵ�CODEBLOCK ����
			{ //����done
				if (flagELSE == 0)
				{
					char tmp4[255];
					flagDONE[numDONE] = 2;
					sprintf(tmp4, "%s%d", tagDONE, numDONE++);
					ofile << "jmp " << tmp4 << endl;
				}
				else
				{
					char tmp4[255];
					//flagDONE[numDONE] = 2;
			
					sprintf(tmp4, "%s%d", tagDONE, numDONE - 1);
					ofile << "jmp " << tmp4 << endl;
				}
			}
			if (flagWHILE >= 1 && flagELSE == 0 && flagIF == 0) //������ʲô��������CODEBLOCK�ĳ���дFALSE������ELSE֮��
			{
				ofile << "jmp " << tagBEGIN << tmpB << endl;
				for (int i = numFALSE; i >= 0; i--)
				{
					if (flagFALSE[i] == 2)
					{
						ofile << tagFALSE << i << ":" << endl;
						flagFALSE[i] = 1;
						break;//���֣���δ����
					}
				}					
			}
		}
		else if (tagValue == "identifer") 
		{//�ݹ�
			if (tagText == "main")
			{
				ofile << ".code" << endl << endl;
				ofile << "start :" << endl <<endl;
			}
			if (flagDECLARE == 1)//����������
			{
				//��ʱһ�����ڷ��ű���
				maplist[numSYM].name = tagText;
				maplist[numSYM].symbol = x86Symbol;
				char tmp[2];
				sprintf(tmp, "%d", numSYM);
				maplist[numSYM].symbol += tmp;
				numSYM++;

				ofile << maplist[numSYM - 1].symbol << " "<<sizebuffer;
			}
			else if(flagEXPR == 1)//����Ϊ0ʱ������ȡ��eax���б���
			{
				for (int i = 0; i < numSYM; i++)
				{
					if (tagText == maplist[i].name)
					{
						buffer[buffer_count] = maplist[i].symbol;
						buffer_count++;
						break;
					}
				}
			}
			else if (flagASS_STMT == 1)//˵���Ǳ���ֵ�ķ��� 
			{
				for (int i = 0; i < numSYM; i++)
				{
					if (tagText == maplist[i].name)
					{
						ass_buffer[ass_buffer_count] = maplist[i].symbol;
						ass_buffer_count++;
						break;
					}
				}
			}
		}
		else if (tagValue == "const_i")//����ʵ����ֻ���ǳ���
		{
			if (flagDECLARE == 1)//˵���ڶ�������ڲ�
			{

				ofile << " " << tagText << endl;
			}
			else if (flagEXPR == 1)//���ٶ�����䣬��Ӧ�ý������㣬������ʱ�������������
			{
				const_buffer[c_buffer_count] = tagText;
				c_buffer_count++;
			}
		}
		else if (tagValue == "seperator")
		{
			if (tagText == ";")//���ֱ������ ���ͱ�ǣ����ʽ���
			{
				flagINT = 0;
				flagCHAR = 0;
			}
		//�� �� ����,{}�ڱ��δ�����û��ʵ������	
		}
		else if (tagValue == "operator")
		{
			//���������
			if (tagText == "+") { opbuffer =  "add eax,"; }
			else if (tagText == "-") { opbuffer = "sub eax,"; }
			else if (tagText == "*") { opbuffer = "mul "; }
			else if (tagText == "//") { opbuffer ="div"; }
			//�Ƚϲ�����
			else if (tagText == "<") { tmpRelation = "jge "; }
			else if (tagText == ">") { tmpRelation = "jbe "; }
			else if (tagText == ">=") { tmpRelation = "jb "; }
			else if (tagText == "<=") { tmpRelation = "jg "; }
			else if (tagText == "==") { tmpRelation = "jnz "; }
			else if (tagText == "!=") { tmpRelation = "jz "; }
		}
		else if (tagValue == "CONDITION")
		{
			flagCONDITION = 1;
			if (node->FirstChildElement() != NULL)
			{
				readxml(node->FirstChildElement());    //�������ӽڵ㣬����еݹ����
			}
			ofile << "cmp " << con_buffer[0] << "," << con_buffer[1] << endl;
			ofile << tmpRelation;
			flagFALSE[numFALSE] = 2;
			char tmp3[255];
			sprintf(tmp3, "%s%d", tagFALSE, numFALSE++);
			ofile << tmp3 << endl;
			//�����ڴ˴������������ת
			flagCONDITION = 0;
			con_buffer_count = 0;
		}
		else if (tagValue == "keyword")//���﷨����ʱ����ƾ��� �ض�����ײ㣬��˲��ö�ȡ
		{
			if (tagText == "int") { sizebuffer = "dword"; }
			else if (tagText == "char") { sizebuffer = "byte"; }
		}
		else
		{//û����������ʱ Ӧ�������ݹ�
			if (node->FirstChildElement() != NULL)
			{
				readxml(node->FirstChildElement());    //�������ӽڵ㣬����еݹ����
			}
		}
		//����
		node = node->NextSiblingElement();
		if (!node)
		{
			break;
		}
	}
}
void build(TiXmlElement *node)
{
	//get the project name
	char projectname[255];
	TiXmlAttribute *attri = node->FirstAttribute();   //��ȡ�ڵ�ĵ�һ�����ԣ�������LastAttribute()��ȡ���һ�����Խڵ�
	while (attri != NULL)          //�����Բ�Ϊ�������
	{
		strcpy(projectname,attri->Value());
		attri = attri->Next();        //��ȡ��һ�����Խڵ�
	}
	for (int i = 0; projectname[i] != '\0'; i++)
	{
		if (projectname[i] == '.') {
			projectname[i] = '\0'; break;
		}
	}
	
	sprintf(mc, "C:\\Build-MC\\Build-MC\\%s.asm",projectname);
	ofile.open(mc);
	ofile << ".386\n.model flat, stdcall\noption casemap : none" << endl<<endl;
	ofile << "includelib user32.lib\nincludelib kernel32.lib\nincludelib msvcrt.lib" << endl<<endl;
	//������ʱ����
	ofile << ".data\n"
		  <<"temp0 dword ?\n"
		  <<"temp1 dword ?\n"
		  <<"temp2 dword ?\n"
		  <<"temp3 dword ?\n"
		  <<"temp4 dword ?\n"
		  <<"temp5 dword ?\n"
		  <<"temp6 dword ?\n"
		  <<"temp7 dword ?\n"
          <<"temp8 dword ?\n"
		  <<"temp9 dword ?\n";
	readxml(node);
	ofile << "end start" << endl;
}
#endif