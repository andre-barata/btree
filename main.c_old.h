#include <stdlib.h>
#include <stdio.h>
//#include <windows.h>

//#define debugMem

#define uint unsigned int
#define intSize 6
//#define uint64 unsigned long
#define null NULL
#define bool int
#define true 1
#define false 0
#define dSz(a,b) uint a = sizeof(b);
//#define preallocBuffer 1024

#ifdef debugMem
uint allocBytes;
#define alloc(x) malloc(x); printf("DBG: Alocated bytes: %d / %d\n", x, allocBytes+=x);
#else
#define alloc(x) malloc(x);
#endif


/*typedef struct {
	void* ptr;
	void* ptr2;
} *Ref, tRef; dSz(szRef,tRef)
*/

typedef struct { void* ref; uint id; struct tListNode* subNode0; struct tListNode* subNode1; uint indexBitmask; }
	*ListNode, tListNode; dSz(szListNode, tListNode);

typedef struct { ListNode root; uint nextId; } 
	*List, tList; dSz(szList, tList);

List listNew(void) { List list = alloc(szList); if (!list) return null; list->root = null; list->nextId = 0; return list; }

void printBin(uint v) { int i; for (i = intSize -1; i >= 0; i--) printf("%d", (v&(1<<i))>0); printf("\n");}

bool listAddWithId(List list, void* ref, uint id) {
	ListNode iNode, newNode;
	uint iBitMask;
printf("---------------\nAdd: (%d) ",id);printBin(id);

	if (list->root == null) {
		newNode = alloc(szListNode);
		newNode->id = id;
		newNode->ref = ref;
		newNode->subNode0 = newNode->subNode1 = null;
		/*for(iBitMask = 1 << (intSize - 1); iBitMask >= 1; iBitMask >>= 1) if (id & iBitMask) {
			newNode->indexBitmask = iBitMask;
			break;
		}*/
		list->root = newNode;
		return true;
	}
	iNode = list->root;


	for(iBitMask = 1 << (intSize - 1); iBitMask >= 1; iBitMask >>= 1) {
printf("mask: ");printBin(iBitMask);
		if ((id ^ iNode->id) & iBitMask) { // diferent bit
printf("Found different bit\n");
			// new shift node to 1
			newNode = alloc(szListNode);
			newNode->id = id;
			newNode->ref = ref;
			newNode->indexBitmask = iBitMask;
			if (id & iBitMask) { iNode->subNode0 = ; iNode->subNode1 = (void*)newNode; }
			else { iNode->subNode0 = (void*)newNode; iNode->subNode1 = ; }
			return true;

		} else { //same bit
			if (iNode->indexBitmask == iBitMask) {
				iNode = (ListNode)((id & iBitMask) ? iNode->subNode1 : iNode->subNode0);
printf("Folowing subnode %d\n", (id & iNode->indexBitmask) > 0);
			}
		}

	}
	//if (list->nextId == id) list->nextId++;
	return false;
}
//bool listAdd(List list, void* ref) { return listAddWithId(list, ref, list->nextId); }

void* listGet(List list, uint id) {
	ListNode iNode = list->root;
	while(iNode != null) iNode = (ListNode)((id & iNode->indexBitmask) ? iNode->subNode1 : iNode->subNode0);
	if (id == iNode->id) return iNode->ref;
	return null;
}

void printNode(ListNode listNode) {
	if (listNode) {
		printf("Node Id:%d\n", listNode->id);
		if (listNode->subNode0) {printf("    (0)");printNode((ListNode)listNode->subNode0);}
		if (listNode->subNode1) {printf("    (1)");printNode((ListNode)listNode->subNode1);}
	}
}
void printList(List list) {
	printf("------ LIST ------\n");
	printNode(list->root);
}

int main() {
	char* data1 = "ola andre 1";
	char* data2 = "ola andre 2";
	char* data3 = "ola andre 3";
	char* data4 = "ola andre 4";
	char* data5 = "ola andre 5";

	List list = listNew();

	listAddWithId(list, data1, 1);
//	printList(list);
	listAddWithId(list, data2, 2);
//	printList(list);
	listAddWithId(list, data3, 3);
//	printList(list);
	listAddWithId(list, data4, 4);
//	printList(list);
	listAddWithId(list, data5, 5);
//	printList(list);


/*	printf("ID 1: %s\n", (char*)listGet(list, 1));
	printf("ID 2: %s\n", (char*)listGet(list, 2));
	printf("ID 3: %s\n", (char*)listGet(list, 3));
	printf("ID 4: %s\n", (char*)listGet(list, 4));
	printf("ID 5: %s\n", (char*)listGet(list, 5));
*/

	/*char* t;
	int i;
	FILE* file;
	int ret;
	size_t size;
	struct s1 var1, *pvar1;
	DWORD millis;

	pvar1 = calloc(2,sizeof(var1));
	pvar1[0].i = 5;
	pvar1[1].i = 9;
	(*(pvar1+1)).i = 8;
	printf("ola %d\n", (*(pvar1+1)).i);

	file = fopen("data.dat", "w+b");

	if (file == NULL) printf("Error creating data file\n");

	size = fwrite(pvar1, sizeof(var1), 2, file); 
	if (size <= 0) printf("Error writint to data file\n");
	ret = fclose(file);
	if (ret != 0) printf("Error closing file\n");


	// starting....
	printf("starting.... malloc 1GB\n");
	millis = timeGetTime();
	t = malloc(1000000001);
	if (t == NULL) printf("Error allocating memory\n");
	*(t+1000000000) = 1;

	for(i = 0; i < 2000000000; i++) ret = i;
	free(t);
	millis = timeGetTime() - millis;
	printf("finish %d\n", millis);*/

	return 0;
}

/*typedef struct {
	uint64 size;
	uint32 itemSize;
	void* ptrStart;
	uint32 prealloc;
} *Array, tArray; dSz(szArray,tArray)

Array arrayInit(Array array, uint32 itemSize) {
	array = alloc(szArray);
	array->itemSize = itemSize;
	array->prealloc = (preallocBuffer / itemSize) * itemSize;
	array->ptrStart = alloc(array->prealloc);
	printf("array: %d, ref: %d \n", szArray, itemSize);
	return array;
}*/

