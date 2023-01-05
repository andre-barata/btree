#include <stdlib.h>
#include <stdio.h>

#define types \
Type(Object, ) \
Type(ListNode, Object obj; uint id; ListNode subNode0; ListNode subNode1; ) \
Type(List, ListNode rootNode; uint idNextNode; ) \
Type(String, char* data; )

#include "types.h"

void List_addWithId(List list, Object object, uint id) {
	
}

int main() {
	List list = new(List);
	String string1 = new(String);
	
	list->idNextNode = 0;
	string1->data = "Hello World";

	List_addWithId(list, (Object)string1, 1);
	
	return 0;
}



