#include <stdlib.h>
#include <stdio.h>

#define uint unsigned int
#define uchar unsigned char
#define null NULL
#define bool int
#define true 1
#define false 0
#define alloc(x) malloc(x)

uint hash(unsigned char *str) {
	uint hash = 5381; int c;
	while (c = *str++) hash = ((hash << 5) + hash) + c;
	return hash;
}

void* tpPtr;
#define Type(tp, ...) typedef struct st##tp *tp; struct st##tp { uint type; __VA_ARGS__ }; uchar* sst##tp = #tp;
#define newType(tp) tpPtr = alloc(sizeof(tp)); uint ist##tp = ((tp)tpPtr)->type = hash(sst##tp)
#define is(tp) (ist##tp == tp->type) 
//BUG: isto não funciona porque o ist##tp (hash) tá a ser declarado a acada instanciaçao

Type(Obj, int id; int s; Obj child; )
Type(Obj2, int id; int s; Obj child; )

/*typedef struct { void* ref; uint id; struct tListNode* subNode0; struct tListNode* subNode1; uint indexBitmask; }
	*ListNode, tListNode; dSz(szListNode, tListNode);
*/
int main() {
	Obj o = newType(Obj);
	Obj2 o2 = newType(Obj2);
	
	o->id = 3;
	o->child = o;

	o2->id = 3;
	o2->child = o;

	printf("obj id:%d, type:%d\n", o->id, o->type);
	printf("child obj id:%d, type:%d\n", o->child->id, o->child->type);

	printf("obj2 id:%d, type:%d\n", o2->id, o2->type);
	printf("child obj2 id:%d, type:%d\n", o2->child->id, o2->child->type);

	return 0;
}


