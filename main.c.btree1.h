#include <stdlib.h>
#include <stdio.h>
#include <limits.h>

#define types \
Type(Object, ) \
Type(List, Object root; uint idNextNode; ) \
Type(ListLeaf, void* ref; uint id; ListLeaf nextLeaf; ) \
Type(ListBranch, uint bitmask; Object subNode0; Object subNode1; ) \
Type(String, char* data; )

#include "types.h"

// Util functions
// return an int with the most significant bit set to 1 (in the input int) active
inln uint highBit(register uint v) {
	v |= v >> 1; v |= v >> 2; v |= v >> 4; v |= v >> 8; v |= v >> 16;
#if UINT_MAX > 0xffffffff
	v |= v >> 32;
#endif
	v++; v >>= 1;
	return v;
}
// convert int to string of bits (has memory usage bugs)
char *itob(int x) { static char buff[8 + 1];
	int i; int j =8-1; buff[j] = 0;
	for(i=0;i<1 * 8; i++) { if(x & (1 << i)) buff[j] = '1'; else buff[j] = '0'; j--; }
	return buff;
}
// print error if condition is false
void ifnok(bool success, char* string) { if (!success) printf("ERROR: %s\n", string); }

// Constructors
ListLeaf ListLeaf_new(void* ref, uint id, ListLeaf nextLeaf) {
	ListLeaf ll = new(ListLeaf); ll->ref = ref; ll->id = id; ll->nextLeaf = nextLeaf; return ll;
}
ListBranch ListBranch_new(uint bitmask, Object subNode0, Object subNode1) {
	ListBranch lb = new(ListBranch); lb->bitmask = bitmask; lb->subNode0 = subNode0; lb->subNode1 = subNode1; return lb;
}

// List Methods
void* List_get(List list, uint id) {
	Object iNode = list->root;
	while(is(iNode,ListBranch)) 
		iNode = (id & as(iNode,ListBranch)->bitmask) ? as(iNode,ListBranch)->subNode1 : as(iNode,ListBranch)->subNode0;
	if (is(iNode,ListLeaf) && id == as(iNode,ListLeaf)->id) return as(iNode,ListLeaf)->ref;
	return null;
}
bool List_addWithId(List list, void* ref, uint id) {
	Object iNode = list->root;
	Object* piNode;
	Object newLeaf;
	uint iMask = 0;
	uint diff;
	
	if (List_get(list,id)) return false;

	newLeaf = (Object)ListLeaf_new(ref, id, null);
	if (list->root == null) { list->root = newLeaf; return true; }

	if(is(iNode,ListLeaf)) {
		diff = as(iNode,ListLeaf)->id ^ id;
		iMask = highBit(diff);
		if (id & iMask) list->root = (Object)ListBranch_new(iMask, iNode, newLeaf);
		else list->root = (Object)ListBranch_new(iMask, newLeaf, iNode);
		return true;
	}

	if(is(iNode,ListBranch)) {
		piNode = (id & as(iNode,ListBranch)->bitmask) ? &(as(iNode,ListBranch)->subNode1) : &(as(iNode,ListBranch)->subNode0);
	}
	
	if(is(*piNode,ListLeaf)) {
		diff = as(*piNode,ListLeaf)->id ^ id;
		iMask = highBit(diff);
		if (id & iMask) *piNode = (Object)ListBranch_new(iMask, *piNode, newLeaf);
		else *piNode = (Object)ListBranch_new(iMask, newLeaf, *piNode);
		return true;
	}
	/*
	// xor da mask do branch com o id a adicionar, se o resultado for menor que a mask do filho, segue para o filho
	while(is(iNode,ListBranch) && ((iMask ^ id) > as(iNode,ListBranch)->bitmask)) {
		iMask |= as(iNode,ListBranch)->bitmask;
		iNode = (id & as(iNode,ListBranch)->bitmask) ? as(iNode,ListBranch)->subNode1 : as(iNode,ListBranch)->subNode0;
	}*/
	
	return true;
}
void List_printNode(Object node, uint level) { 
	if (is(node,ListBranch)) {
		printf("[branch] [mask %s] [level %d]\n", itob(as(node,ListBranch)->bitmask), level);
		printf("======= Subnode Left =======\n");
		List_printNode(as(node,ListBranch)->subNode0,  level +1);
		printf("======= Subnode Right =======\n");
		List_printNode(as(node,ListBranch)->subNode1,  level +1);
	} else if (is(node,ListLeaf))
		printf("[Leaf] [ID %d (%s)] [Data pointer %d] [level %d]\n", as(node,ListLeaf)->id, itob(as(node,ListLeaf)->id), (uint)as(node,ListLeaf)->ref, level);
}
	
void List_print(List list) {
	printf("____________________________ Root ____________________________\n");
	List_printNode(list->root, 0);
}


int main() {
	List list = new(List);
	String string0 = new(String);
	String string1 = new(String);
	String string2 = new(String);
	String string3 = new(String);
	
	string0->data = "Hello World 0";
	string1->data = "Hello World 1";
	string2->data = "Hello World 2";
	string3->data = "Hello World 3";

	ifnok(List_addWithId(list, string0, 0), "error adding list item 0");
	ifnok(List_addWithId(list, string1, 1), "error adding list item 1");
	ifnok(List_addWithId(list, string3, 3), "error adding list item 3");
	ifnok(List_addWithId(list, string2, 2), "error adding list item 2");
	List_print(list);
	
	printf("%s\n", as(List_get(list, 0),String)->data);
	printf("%s\n", as(List_get(list, 1),String)->data);
	printf("%s\n", as(List_get(list, 2),String)->data);
	printf("%s\n", as(List_get(list, 3),String)->data);
	
	return 0;
}



