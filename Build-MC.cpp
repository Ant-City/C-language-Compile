#include "MAP.h"

int main(int argc, char **argv)
{
	TiXmlDocument *myDocument = new TiXmlDocument("mytest.tree.xml");    //��һ��xml�ļ�
	myDocument->LoadFile();

	TiXmlElement *node = myDocument->FirstChildElement();      //���ص�ǰ�ڵ���ӽڵ�
	if (node == NULL)               //��Ϊ���򷵻�
	{
		return 0;
	}
     build(node);
	//readxml(node);
	return 0;
}