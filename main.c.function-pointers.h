#include <stdlib.h>
#include <stdio.h>

#define types																			\
Type(Obj, int id; int s; Obj child; uint (*inc)(Obj,uint); )													\
Type(ListNode, void* ref; uint id; ListNode subNode0; ListNode subNode1; )

#include "types.h"

uint incId(Obj o, uint inc) { o->id += inc; return o->id; }


/*typedef struct { void* ref; uint id; struct tListNode* subNode0; struct tListNode* subNode1; uint indexBitmask; }
	*ListNode, tListNode; dSz(szListNode, tListNode);
*/
int main() {
	Obj o = new(Obj);
	
	o->id = 11;
	
	o->inc = &incId;

	o->inc(o, 10);
	
	printf("obj id:%d, type:%d\n", o->id, o->type);

	printf("o is Obj: %d\n", is(o,Obj));
	
	return 0;
}


