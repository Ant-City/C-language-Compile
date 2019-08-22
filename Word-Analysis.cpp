#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <fstream>
#include "MyJudge.h"
#include "write_xml\tinystr.h"
#include "write_xml\tinyxml.h"
#define MAX_FILE 100000
using namespace std;
//����xml�ļ�
char output[_MAX_PATH]; //·�����ƶ�����ȫ��
int * bit = NULL;

struct word
{
	char valid[10];//�Ƿ���ȷ
	int row;//�ڼ���
	char type[260];//��������
	char value[260];//����ȡֵ
	char tips[10];
};
//��Ŀ���ļ���д����Ϣ
void WriteIn(struct word now, TiXmlElement *Root)
{
	TiXmlElement *tokenElement = new TiXmlElement("token");//Stu  
														 //��������  
	Root->LinkEndChild(tokenElement);//���ڵ�д���ĵ�  
  
	TiXmlElement *valueElement = new TiXmlElement("value");
	tokenElement->LinkEndChild(valueElement);

	TiXmlText *valueContent = new TiXmlText(now.value);
	valueElement->LinkEndChild(valueContent);

	TiXmlElement *typeElement = new TiXmlElement("type");
	tokenElement->LinkEndChild(typeElement);

	TiXmlText *typeContent = new TiXmlText(now.type);
	typeElement->LinkEndChild(typeContent);
  
	TiXmlElement *lineElement = new TiXmlElement("line");
	tokenElement->LinkEndChild(lineElement);

	char line[10];
	sprintf(line, "%d", now.row);
	TiXmlText *lineContent = new TiXmlText(line);
	lineElement->LinkEndChild(lineContent);

	TiXmlElement *validElement = new TiXmlElement("valid");
	tokenElement->LinkEndChild(validElement);

	TiXmlText *validContent = new TiXmlText(now.valid);
	validElement->LinkEndChild(validContent);

//TiXmlElement *tipsElement = new TiXmlElement("tips");
//tokenElement->LinkEndChild(tipsElement);
//
//TiXmlText *tipsContent = new TiXmlText(now.tips);
//tipsElement->LinkEndChild(tipsContent);
}

//����
//����һ�������Լ���ȫ������
//����һ������ �ж������ͼ�����
struct word Classify(char word[], int line, char tips[])
{
	int  type;
	struct word current;
	word[strlen(word)] = '\0';
	current.row = line;//д�벻������ı�Ĵ�
	strcpy(current.value, word);//������ַ� ���ַ�����������������
	strcpy(current.tips, tips);
	//�����ж����� - �õ����� 1 - 6 ��6��
	type = JudgeWord(word);
	if (type == ERROR)
	{
		cout << "in line " << line << " " << word << " is invalid" << endl;
		strcpy(current.type, "unknown");
		strcpy(current.valid, "false");
	}
	else
	{
		switch (type)
		{
		case KEY: strcpy(current.type, "keyword"); break;
		case OP: strcpy(current.type, "operator"); break;
		case BORDER: strcpy(current.type, "seperator"); break;
		case NAME: strcpy(current.type, "identifer"); break;
		case CONST_I: strcpy(current.type, "const_i"); break;
		case CONST_F: strcpy(current.type, "const_f"); break;
		case CONST_S: strcpy(current.type, "stringLiteral"); break;
		case CONST_C: strcpy(current.type, "characterConstant"); break;
		}
		strcpy(current.valid, "true");
	}
	return current;
}
//���� һ���ļ��͵�ǰ��дָ���λ��
//���� ���зִʺʹʵĴ���- û�н��� \���й��ܵĴ���
void CreateWord( char word[],int length, TiXmlElement *Root)
{
	int point = 0;     //����ɨ����	
	int line = 1;      //����
	int flag_str = 0;  //�ַ������
	int flag_conv = 0; //����ת���־ 
	int flag_three = 0;//��Ŀ��������
	int flag_num = 0;//���ֶ�����
	int end = 0;       //��ȡָ��
	char tmp[260] = { 0 };
	while (point < length)
	{
		char now = word[point];//ȡ�����㴦��
		if (isspace(now))
		{	if(now == '\n')line++;//����+1
			if (end != 0 )
			{
				if (flag_str == 0)//����¼������ �Ҳ��Ǳ����ڲ��Ŀո�ͻس���
				{
					tmp[end] = '\0';//����������
					WriteIn(Classify(tmp, line,""),Root);					
					end = 0;//��β�������
					memset(tmp, 0, sizeof(tmp));
				}
				else
				{
					tmp[end] = now; end++;//����ֵ��ָ�����
				}
			}
			//���Ϊ�� ��ô��ǰû�б������ֵ
		}//��ʱ������һ������ �����ַ����� - ���1
		 //���¶Դʷ�����û��Ӱ�� //ֱ�Ӵ��봦�� ����Ҫ�������� - С���ڶ�ȡʱ�ֳ���������
		else if (now == ';' || now == '.' ||now == '{' || now == '}' ||	now == '(' || now == ')'||now ==']'||now=='[' || now == ',')
		{
			if (now == '.' &&flag_num == 1)
			{
				tmp[end++] = now;//˵����С����ֱ�Ӷ���
			}
			else if (end != 0)
			{
				if (flag_str == 0)//����¼������ �Ҳ��Ǳ����ڲ��Ŀո�ͻس���
				{
					tmp[end] = '\0';//����������
					WriteIn(Classify(tmp, line,""),Root);
					end = 0;//��β�������
					tmp[0] = now; tmp[1] = '\0';//����ǰ�ַ�
					WriteIn(Classify(tmp, line,""), Root);
					flag_num = 0;//�������Żᵼ�����ֵĽ���
					memset(tmp, 0, sizeof(tmp));//����
				}
				else
				{
					tmp[end] = now; end++;//����ֵ��ָ�����
				}
			}
			else//tmp Ϊ��ʱֱ�Ӷ���ʹ���÷���
			{
				tmp[0] = now; tmp[1] = '\0';//����ǰ�ַ�
				WriteIn(Classify(tmp, line,""), Root);
				memset(tmp, 0, sizeof(tmp));//����}//���Ϊ�� ��ô��ǰû�б������ֵ
			}
		}//����һ������ �÷��űض������Ĵ�-  ; . { } ( ) [ ] , ��ǰ��Ĵʽ��� - ���� ������ô� - 
		 //����. ������������ʱ - ����Ϊ��������� ˵�����ǽ��޷�����С����
		else if (now == '\'' || now == '\"')
		{
			int type = 	JudgeType(flag_str, flag_conv);
			if (type == CONV)
			{
				tmp[end] = now; end++;//��ת�� ����ͨ���ַ�
				flag_conv = 0;//��ת��ķ����Ѿ�����
			}
			else if (type == STR)
			{
				flag_str = 0;//��־���ַ������ַ�ѹ�����
				tmp[end] = now; end++;
				tmp[end] = '\0';
				WriteIn(Classify(tmp, line,""), Root);
				end = 0;
				memset(tmp, 0, sizeof(tmp));
			}
			else
			{   //��ʱtmpӦ�ñ�ȻΪ�գ�������tmp��Ϊ�� �Һ���ַ������������ - ֻ��Ϊ�˱���
				//�����е����ݣ�
				if (end != 0)//�޸� ʹ����ǰ׺u l�����
				{
					tmp[end] = '\0';
					if (strcmp(tmp, "u8") == 0 || strcmp(tmp, "U") == 0|| strcmp(tmp,"u") == 0||  strcmp(tmp, "L") == 0 )
					{
						//ƥ��һ����˵��������Ŀǰ��ǰ׺
						flag_str = 1;
						tmp[end] = now;
						end++; point++; continue;
					}
					else if (flag_str == 0)
					{
						tmp[end] = '\0';//����������
						WriteIn(Classify(tmp, line,""), Root);
						end = 0;//��β�������
						tmp[0] = now; tmp[1] = '\0';//����ǰ�ַ�
						WriteIn(Classify(tmp, line,""), Root);
						memset(tmp, 0, sizeof(tmp));//����
					}
				}
				flag_str = 1;//��־һ���ַ��������ַ��Ŀ�ʼ ����tmp���е�����
				memset(tmp, 0, sizeof(tmp)); end = 0;
				tmp[end] = now; end++;	
			}
		}//ֻ�ܰ���һ���ַ� ʹ��λͼ��ǣ���û�б�ǵ�ʱ����Ҫ���ɨ������� ��������ѹ���ʾ�����ֵ���Ǳ�ʶ��
		//���԰���һ������ ���б�ǵ�ʱ�򣬲���Ҫ���� ֱ��ʶ��
		else if (now == '\\')//�����������ᵼ�½��봦��
		{
			int type = JudgeType(flag_str, flag_conv);//�鿴����ͨ�ַ�����ת�崦��
			if (type == CONV){
				//˵���Ǿ��е��ַ����з���
				tmp[end] = now; end++;
				flag_conv = 0;
			}
			else{
				flag_conv = 1;//˵����ת����� - ���Բ�������ָ�����
			}
		}
		else if(isalpha(now))//��ĸ
		{
			//�ַ�
			if (flag_conv == 1){
				char a[10];
				sprintf(a, "\\%c", now);//ֻ����ת������ĸ�����
				strcat(tmp, a);
				point += strlen(tmp)-1-end;//ָ���Ƶ�ĩβ
				end = strlen(tmp);//��ȡ�ô�
				flag_conv = 0;
				continue;
			}
			//ֱ��ƴ��
			else	tmp[end] = now; end++;
		}
		else if (isdigit(now))
		{
			if( end == 0 )//��ǰΪ��
				flag_num = 1;//�����ֱ��
			tmp[end++] = now;
		}
		else//ʣ����ŵ���� ���ȴ���tmp���е�����
		{   //����type
			if (flag_str == 1) {
				tmp[end++] = now; point++; continue;
			}
			char next = word[point + 1];//ȡ����һ���ַ�
			if (now == '+' || now == '-' || now == '&' || now == '|') //�������� ��ӵȺ�
			{
				if (flag_num == 1 && (now == '+' || now == '-')&&(word[point-1] == 'e'||word[point-1] == 'E'))
				{
					tmp[end++] = now; point++; continue;
				}//��ѧ�������ڲ���������
				else if (next == now || next == '=')
				{
					char a[10];
					sprintf(a, "%c%c\0", now, next);
					WriteIn(Classify(a, line,""), Root);
					point++;//�ַ������������ƶ�
					if (end != 0){
						WriteIn(Classify(tmp, line, ""), Root);
						memset(tmp, 0, sizeof(tmp)); end = 0;
					}
				}
				else
				{
					if (end != 0){
						WriteIn(Classify(tmp, line, ""), Root);
					}
					tmp[0] = now; tmp[1] = '\0';
					WriteIn(Classify(tmp, line,""), Root);
					memset(tmp, 0, sizeof(tmp)); end = 0;
				}
			}
			else if (now == '<' || now == '>')//�������� �ӵȺ� ���������� �ҽӵȺ�
			{   
				char a[10];
				if (next == now)
				{
					char last = word[point + 2];
					if (last == '=')
					{
						sprintf(a, "%c%c%c\0", now, next, last);
						point += 2;
					}
					else
					{
						sprintf(a, "%c%c\0", now, next);
						point += 1;
					}
					if (end != 0){
						WriteIn(Classify(tmp, line, ""), Root);
						memset(tmp, 0, sizeof(tmp)); end = 0;
					}
					WriteIn(Classify(a, line,""), Root);
				}
				else if (next == '=')
				{
					sprintf(a, "%c%c\0", now, next);
					WriteIn(Classify(a, line,""), Root);
					if (end != 0){
						WriteIn(Classify(tmp, line, ""), Root);
						memset(tmp, 0, sizeof(tmp)); end = 0;
					}
					point += 1;
				}
				else
				{//�ǵ����ķ���
					if (end != 0)
					{
						WriteIn(Classify(tmp, line, ""), Root);
						memset(tmp, 0, sizeof(tmp)); end = 0;
					}
					tmp[0] = now; tmp[1] = '\0';
					WriteIn(Classify(tmp, line, ""), Root);
					memset(tmp, 0, sizeof(tmp)); end = 0;
				}
			}
			else if (now == '*' || now == '/' || now == '!' || now == '~'||now =='='||now == '%') //�ӵȺ�
			{
				if (next == '=')
				{
					if (end != 0){
						WriteIn(Classify(tmp, line, ""), Root);
						memset(tmp, 0, sizeof(tmp)); end = 0;
					}
					char a[10];
					sprintf(a, "%c%c\0", now, next);
					WriteIn(Classify(a, line,""), Root);
					point++;//�ַ������������ƶ�
				}
				else
				{
					if (end != 0){
						WriteIn(Classify(tmp, line, ""), Root);
					}
					tmp[0] = now; tmp[1] = '\0';
					WriteIn(Classify(tmp, line,""), Root);
					memset(tmp, 0, sizeof(tmp)); end = 0;
				}
			}
			else if (now == '?')//��Ŀ����� �����Ŀ��������
			{
				flag_three++;
				if (end != 0){
					WriteIn(Classify(tmp, line, ""), Root);
				}
				tmp[0] = now; tmp[1] = '\0';
				WriteIn(Classify(tmp, line,""), Root);
				memset(tmp, 0, sizeof(tmp)); end = 0;
			}
			else if( now == ':')//��Ŀ����� - ������
			{
				char next = word[point + 1];
				if (flag_three > 1)//˵������Ŀ�����
				{
					flag_three -= 1;
					if (end != 0){
						WriteIn(Classify(tmp, line, ""), Root);
					}
					tmp[0] = now; tmp[1] = '\0';
					WriteIn(Classify(tmp, line,"three"), Root);
					memset(tmp, 0, sizeof(tmp)); end = 0;
				}
				else//����̽һλ
				{
					if (next == now)//˵���ǣ��������
					{
						if (end != 0){
							WriteIn(Classify(tmp, line, ""), Root);
							memset(tmp, 0, sizeof(tmp)); end = 0;
						}
						char a[10];
						sprintf(a, "%c%c\0", now, next);
						WriteIn(Classify(a, line, ""), Root);
						point++;//�ַ������������ƶ�
					}
					else
					{
						if (end != 0){
							WriteIn(Classify(tmp, line, ""), Root);
						}
						tmp[0] = now; tmp[1] = '\0';
						WriteIn(Classify(tmp, line, ""), Root);
						memset(tmp, 0, sizeof(tmp)); end = 0;
					}
				}
			}
			flag_num = 0;//������ĳ��ֻᵼ�½���
		}
		point++; 	    //����ָ��
	}
	struct word endd;
	endd.row = -1;
	strcpy(endd.value,"#");
	strcpy(endd.type, "#");
	strcpy(endd.valid, "true");
	WriteIn(endd, Root);
}

int main(int argc, char * argv[])
{
	//�����ļ���������
	char textname[_MAX_PATH];
	scanf("%s", textname);
	//һ���Զ���ȫ�����ļ� ��buffer
	ifstream file;//���ļ�
	file.open(textname);//��������
	file.seekg(0, ios::end);
	int length = file.tellg();
	file.seekg(0, ios::beg);

	char buffer[MAX_FILE];//�����ڴ�ռ�
	
	memset(buffer, 0, sizeof(buffer));//��ս�Ҫʹ�õ��ڴ�

	file.read(buffer, length);
	file.close();
	int p = 0;
	while (textname[p] != '.')
	{		p++; 	}
	textname[p] = '\0';//����Ŀ�괮����
	//��ʼ����
	sprintf(output,"%s.token.xml",textname);
	//д��xml�ļ��ĸ��ڵ�
	TiXmlDocument *writeDoc = new TiXmlDocument; //xml�ĵ�ָ��  
    TiXmlDeclaration *decl = new TiXmlDeclaration("1.0", "UTF-8", "yes");//�ĵ���ʽ����  
	writeDoc->LinkEndChild(decl); //д���ĵ�  

	TiXmlElement *RootElement = new TiXmlElement("Project");//��Ԫ��  
	RootElement->SetAttribute("name", output); //����  
	writeDoc->LinkEndChild(RootElement);

	TiXmlElement *Root2Element = new TiXmlElement("tokens");//��Ԫ��  
	RootElement->LinkEndChild(Root2Element);
	//��ʼ�ִ�
	CreateWord(buffer, length,Root2Element);
	//�ִʽ��� д��
	writeDoc->SaveFile(output);
	delete writeDoc;

	cout << "Done" << endl;
	system("pause");
	return 0;
};