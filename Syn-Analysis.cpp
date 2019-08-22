#include "LEX.h"
using namespace std;

int main(int argc, char **argv)
{
	char target[255]; 
	char output[255];
	scanf("%s", target);
	//strcpy(target, argv[1]);//��Ŀ�긳ֵ
	read(target);
	//Ԥ�������
	//����ݹ��½��������д���
	int p = 0;
	while (target[p] != '.')
	{
		p++;
	}
	target[p] = '\0';//����Ŀ�괮����
					   //��ʼ
	sprintf(output, "%s.tree.xml", target);
	//д��xml�ļ��ĸ��ڵ�
	TiXmlDocument *writeDoc = new TiXmlDocument; //xml�ĵ�ָ��  
	TiXmlDeclaration *decl = new TiXmlDeclaration("1.0", "UTF-8", "yes");//�ĵ���ʽ����  
	writeDoc->LinkEndChild(decl); //д���ĵ� 

	TiXmlElement *RootElement = new TiXmlElement("Project");//��Ԫ��  
	RootElement->SetAttribute("name", output); //����  
	writeDoc->LinkEndChild(RootElement);


	CMPL_UNIT(RootElement);

	writeDoc->SaveFile(output);
	delete writeDoc;

	cout << "Lex-Analysis Done" << endl;
	system("pause");
	return 0;
}