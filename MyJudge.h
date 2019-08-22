#ifndef MyJudge
#define MyJudge

#include <string.h>
#include <ctype.h>

#define NUM_KEY 42 //�ؼ���
#define NUM_OP 39
#define NUM_BORDER 8

#define KEY 1 //�ؼ���
#define OP 2  //�����
#define BORDER 3 //���޷�
#define NAME 4  //��ʶ��
#define CONST_I 51 //��������
#define CONST_S 52  //�ַ�������
#define CONST_C 53  //�ַ�����
#define CONST_F 54 //����������
#define ERROR 6 //���� - NAME ��������

#define STR 7   //λͼ�ַ������
#define CHAR 8  //�ַ����
#define CONV 9  //λͼת����
#define NORMAL 10 //�������
char keyword[NUM_KEY][20]
{
	"auto" ,"break","case","char","const","continue","default","do",
	"double","else","enum","extern","float","for","goto","if",
	"int","long","register","return","short","signed","sizeof","static",
	"struct","switch","typedef","unsigned","union","void","volatile","while"
	,"_Alignas","_Alignas","_Atomic","_Bool","_Complex","_Generic","_Imaginary","_Noreturn"
	,"_Static_assert","_Thread_local" };
char op[39][10] = {
	"[","]","+","++","+=","-","--","-=",
	"*","*=","/","//=","=","==","<","<=",
	">",">=","<<","<<=",">>=",">>","%","%=",
	"!","!=","&","&&","|","||","|=","&=",
	"~","~=","^","^=","?:","->","."
};
char border[8][10] = {
	"{","}","(",")",",","\"","\'",";"
};

int JudgeKey(char word[])
{
	for (int i = 0; i < NUM_KEY; i++)
	{
		if (strcmp(word, keyword[i]) == 0) return 1;
	}
	return 0;
}

int JudgeOp(char word[])
{
	for (int i = 0; i < NUM_OP; i++)
	{
		if (strcmp(word, op[i]) == 0) return 1;
	}
	return 0;
}

int JudgeBorder(char word[])
{
	for (int i = 0; i < NUM_BORDER; i++)
	{
		if (strcmp(word, border[i]) == 0) return 1;
	}
	return 0;
}

int JudgeType(int flag_str, int flag_conv)
{//�����ֱ��ʾ��ǰָ��λ�ã�������ǣ����λͼ
 //�ж����� �������ַ�����ǵ�ʱ��ת�������Ч ˵��ת�����Ӧ������ȥ ������ַ���Ϊ���ַ� ���Ƿ���
 //���������ַ������ʱ�����ܹ�����ת�������Ч�����
	if (flag_str == 0)//��������
		return  NORMAL;
	else
	{
		if (flag_conv == 1)
		{
			return CONV;//����CONV��ʾ��ǰ�ַ���Ӧ�ö����Ϊ����
		}
		else
		{
			return STR; //��ʾ���ַ����ַ����еĲ��֣����ý��д���
		}
	}
}
//�����ַ��������ͱ������ķ��� - �������ʱ����������˫���� 
int JudgeWord(char word[])
{
	if (JudgeKey(word) == 1) return KEY;
	if (JudgeOp(word) == 1) return OP;
	if (JudgeBorder(word) == 1) return BORDER;
	if (word[0] == 'u' || word[0] == 'U' || word[0] == 'L')
		if (word[1] == '\'') return CONST_C;
		else if (word[1] == '\"') return CONST_S;
		else if (word[0] == 'u'&&word[1] == '8'&&word[2] == '\"') return CONST_S;
		else if (word[0] == 'u'&&word[1] == '8'&&word[2] == '\'') return CONST_C;
	if (word[0] == '\"') return CONST_S;
	else if (word[0] == '\'') return CONST_C; //˵�����ַ���
	else if (isdigit(word[0])||word[0] == '-'||word[0] == '+')//�����������ôȫ�������� ����ֻ�ܰ���һ��С����
	{
		int p = 1; int flag_point = 0, flag_E = 0, flag_pn = 0;
		while (word[p] != '\0')
		{
			if (word[p] == '.')
			{
				if (flag_point == 0) flag_point = 1;
				else if (flag_point == 1) return ERROR; //�Ȳ����ַ���Ҳ��������������
			}
			else if (word[p] == 'e' || word[p] == 'E')
			{
				if (flag_E == 0) flag_E = 1;
				else if (flag_E == 1) return ERROR;
			}
			else if (word[p] == '+' )
			{
				if (flag_pn == 0) flag_pn = 1;
				else if (flag_pn != 0) return ERROR;
			}
			else if (word[p] == '-')
			{
				if (flag_pn == 0) flag_pn = -1;
				else if (flag_pn != 0) return ERROR;
			}
			p++;
		}
		if (flag_E==1&&(flag_pn == -1)) return CONST_F;
		else if (flag_point == 1 && flag_E == 0) return CONST_F;
		else return CONST_I;
	}//
	else//�������ֿ�ͷ�ĳ��� Ҳ�����ַ��� - ��ôӦ���Ǳ�ʶ��
	{
		int p = 0;
		while (word[p] != '\0')
		{
			//һ�����ֲ�����ʾ���ַ������ж�Ϊ�Ƿ�
			//isctrl && ! isspace ��ֱ����isprint
			if (!isalpha(word[p]) && !isdigit(word[p]) && word[p] != '_') return ERROR;
			p++;
		}
	}
	return NAME;
}


#endif // Judge.h