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
inln uint highBit(/*register */uint v) {
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
// swap values macro
#define swap(a, b) (((a) ^= (b)), ((b) ^= (a)), ((a) ^= (b)))
// print error if condition is false
void ifnok(bool success, char* string) { if (!success) printf("ERROR: %s\n", string); }

// Constructors
inln ListLeaf ListLeaf_new(void* ref, uint id, ListLeaf nextLeaf) {
	ListLeaf ll = new(ListLeaf); ll->ref = ref; ll->id = id; ll->nextLeaf = nextLeaf; return ll;
}
inln ListBranch ListBranch_new(uint bitmask, Object subNode0, Object subNode1) {
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
	Object* pNode = &list->root;
	Object newLeaf;
	uint iMask = 0; uint iId = 0; uint testId; 
	//uint diff;
	
	//if (List_get(list,id)) return false;

	newLeaf = (Object)ListLeaf_new(ref, id, null);
	if (list->root == null) { list->root = newLeaf; return true; }

	if(is(iNode,ListLeaf)) {
		iMask = highBit( as(iNode,ListLeaf)->id ^ id );
		list->root = (id & iMask) ? (Object)ListBranch_new(iMask, iNode, newLeaf) : (Object)ListBranch_new(iMask, newLeaf, iNode);
		return true;
	}

	
/*	testId = (id & as(*pNode,ListBranch)->bitmask);
	iId |= testId; 
	iMask = highBit( iId ^ id );
	printf("1### testId [%s]    ", itob(testId));
	printf("### id [%s]    ", itob(id));
	printf("### iId [%s]    \n\n", itob(iId));
	printf("1### pNode mask [%s]    ", itob(as(*pNode,ListBranch)->bitmask));
	printf("### iMask mask [%s]\n\n", itob(iMask));
	
	while(is(*pNode,ListBranch) && as(*pNode,ListBranch)->bitmask > iMask) {
		testId = (id & as(*pNode,ListBranch)->bitmask);
		iId |= testId; 
		iMask = highBit( iId ^ id );
	printf("2### testId [%s]    ", itob(testId));
	printf("### id [%s]    ", itob(id));
	printf("### iId [%s]    \n\n", itob(iId));
	printf("2### pNode mask [%s]    ", itob(as(*pNode,ListBranch)->bitmask));
	printf("### iMask mask [%s]\n\n", itob(iMask));
		pNode = (testId) ? &as(*pNode,ListBranch)->subNode1 : &as(*pNode,ListBranch)->subNode0;
		//printf("### iMask2 mask [%s]\n\n", itob(iMask));
	}
*/
	while(is(*pNode,ListBranch) && (id & as(*pNode,ListBranch)->bitmask)) {

		pNode = (id & as(*pNode,ListBranch)->bitmask) ? &as(*pNode,ListBranch)->subNode1 : &as(*pNode,ListBranch)->subNode0;
	}

	*pNode = (id & iMask) ? (Object)ListBranch_new(iMask, *pNode, newLeaf) : (Object)ListBranch_new(iMask, newLeaf, *pNode);
	
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
		printf("[Leaf] [ID %4d (%s)] [Data pointer %d] [level %d]\n", as(node,ListLeaf)->id, itob(as(node,ListLeaf)->id), (uint)as(node,ListLeaf)->ref, level);
}
	
void List_print(List list) {
	printf("____________________________ Root ____________________________\n");
	List_printNode(list->root, 0);
}


int main() {
	int iters = 3; // list:6.000.000 vs int*[]:20.000.000 vs int[]100.000.000
	List list = new(List);
	int* intp;
	int i;
	list->root = null;

	//printf("[list] adding\n");
	for (i = 0; i < iters; i++) {
		intp = malloc(sizeof(int));
		*intp = i+100;//rand();
		if (!List_addWithId(list, intp, rand() % 128)) printf("error adding list item\n");
	List_print(list);
	}
	
	//printf("[list] getting\n");
/*
	for (i = 0; i < iters; i++) {
		intp = (int*)List_get(list, i);
		//printf("[%12d] %12d\n", i, *intp);
		if (i != *intp - 100) printf("error adding list item\n");
		free(intp);
	}*/
	free(list);

	//printf("[list] done\n");
	
	return 0;
}



