#include <stdio.h>
#include "headfile.h"


void InitList(List* qList) 
{
	qList->length = 0;
	qList->next   = NULL;
}

void PushList(List* qList,s_pqx s)
{
	List * newNode = (List *)malloc(sizeof(List));
	List * temp = qList;
	newNode->next = NULL;
	newNode->node = s;

	while(temp->next!=NULL)
		temp = temp->next;     //�ҵ���β
	temp->next = newNode;       //����β���µĽڵ�����
	qList->length++;
}

s_pqx GetList(List* qList,int index0)
 {
	 int i;
	 for(i=0;i<=index0;i++)
		 qList = qList->next;
	 return qList->node;
 }

void push_backList(List* qList)
{
	s_pqx s;
	memset(&s,0,sizeof(s_pqx));
	PushList(qList,s);
}


