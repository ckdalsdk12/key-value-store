#include <stdio.h>
#include <strings.h>
#include <stdlib.h>

// 각 모든 함수는 성공시 0, 실패시 각자 정의한 에러코드를 리턴한다.

struct hashNode{
	char *nodekey;
	char *nodevalue;
	struct hashNode *prev;
	struct hashNode *next;
};

struct hashNode *hashTable[10007] = {NULL};

int hashFunction(char *key)
{
	int hash = 0;
	int i;
	for (i = 0; i < strlen(key); i++)
	{
		hash += key[i];
	}
	hash = hash % 10007;
	return hash;
}

// 아래의 PrintAllHashData함수는 디버깅 테스트용 - 추후 삭제 
void PrintAllHashData()
{
	int i;
    for (i = 40; i < 100; i++)
    {
        printf("%d번째 해시테이블 : ", i);
        if (hashTable[i] != NULL)
        {
            struct hashNode* node = hashTable[i];
            while (node != NULL)
            {
                printf("%s ", node -> nodekey);
                printf("%s\n", node -> nodevalue);
                node = node -> next;
            }
        }
    }
}
// 위의 PrintAllHashData함수는 디버깅 테스트용 - 추후 삭제

// 주어진 key를 이용하여 value를 찾고, buf에 저장하여 리턴시켜 준다.
// 존재하지 않는 key인 경우 null을 리턴한다.
int kvget(char *key, char *buf)
{
	int hash = hashFunction(key); // hash변수에 hash함수를 통한 해시값을 넣음. 
	struct hashNode* temp = hashTable[hash]; // 임시 노드에 첫번째 노드 복사 
	while(1) // 해시 테이블이 비어 있을 때 까지 탐색 
	{
		if (temp == NULL) // 해시 테이블이 비어 있으면 
		{
			return NULL; // NULL을 리턴하고 반복문 종료. 
		}
		
		else if (strcmp(temp -> nodekey, key) == 0) // 만약 임시 노드에 있는 key가 받아온 key와 같다면
		{
			printf("%s", temp -> nodevalue); // 노드에 있는 value 출력 // 디버깅 용도 - 추후 삭제 
			buf = temp -> nodevalue; // 밸류값을 buf에 넣는다.
			return buf; // buf를 리턴한다. 
		}
        temp = temp -> next; // 임시 노드를 다음 노드로 넘김.
    }
 	return 0; // 0을 리턴하고 함수 종료. 
}

// 주어진 key와 data를 이용하여 kv store에 저장한다. 
// 이미 key가 존재하는 경우 덮어쓴다. 
int kvput(char *key, char *data)
{
	int hash = hashFunction(key); // hash변수에 hash함수를 통한 해시값을 넣음. 
	struct hashNode* newNode = (struct hashNode*)malloc(sizeof(struct hashNode)); // newNode란 이름의 hashNode 동적할당 
	newNode -> nodekey = key; // 새로운 노드에 키를 집어 넣는다. 
	newNode -> nodevalue = data; // 새로운 노드에 밸류를 집어 넣는다. 
    newNode -> prev = NULL; // 새로운 노드의 전 노드를 가리키는 값에 NULL을 넣는다.
    newNode -> next = NULL; // 새로운 노드의 다음 노드를 가리키는 값에 NULL을 넣는다.
    
  	if (hashTable[hash] == NULL) // 해시 테이블이 비어 있을 경우 
	{
    	hashTable[hash] = newNode; // 새로운 노드가 첫번째 노드가 된다. 
    	return 0; // 0을 반환하여 함수를 종료한다. 
  	}
  	
  	else // 해시 테이블이 비어 있지 않을 경우 
	{
		struct hashNode* temp = hashTable[hash]; // 임시 노드에 첫번째 노드 복사 
		while(temp != NULL) // 해시 테이블이 비어 있을 때 까지 탐색 
		{
			if (strcmp(temp -> nodekey, key) == 0) // 만약 임시 노드에 있는 key가 받아온 key와 같다면
			{
        		temp -> nodevalue = data; // 임시 노드에 받은 value값 저장 (덮어쓰기) 
				return 0; // 0을 반환하여 함수를 종료한다. 
			}
			temp = temp -> next; // 임시 노드를 다음 노드로 넘김.
    	}
    	hashTable[hash] -> prev = newNode; // 새로운 노드를 제일 앞에 넣고 
    	newNode -> next = hashTable[hash]; // 새로운 노드의 다음 노드를 기존 첫 번째 노드로 연결하고 
    	hashTable[hash] = newNode; // 새로운 노드를 첫번째 노드로 연결한다. 기존 첫 번째 노드는 두번째 노드가 된다.
	}
 	return 0; // 0을 리턴하고 함수 종료. 
}

// 주어진 key를 이용하여 key와 value를 삭제한다. 
// 존재하지 않는 key를 삭제하려하면 그냥 0을 리턴한다.
int kvdel(char *key)
{
	int hash = hashFunction(key); // hash변수에 hash함수를 통한 해시값을 넣음. 
	struct hashNode* temp = hashTable[hash]; // 임시 노드에 첫번째 노드 복사
	//printf("%d", hash); // 디버깅 용도 - 추후 삭제 
	struct hashNode* delNode = hashTable[hash]; // 삭제 노드에 첫번째 노드 복사 
	if (hashTable[hash] -> nodekey == key) // 만약 첫 번째 노드의 키가 삭제하여야 할 노드라면 
	{
		delNode = hashTable[hash]; // delNode에 첫 번째 노드를 복사한다. 
        hashTable[hash] = hashTable[hash] -> next; // 다음 노드를 첫 번째 노드로 설정한다. 
	}
	else
	{
		while(1) // 해시 테이블이 비어 있을 때 까지 탐색 
		{
			if (temp == NULL) // 해시 테이블이 비어 있으면 
			{
				return 0; // 0을 리턴하고 함수 종료. 
			}
			else if (strcmp(temp -> nodekey, key) == 0) // 만약 임시 노드에 있는 key가 받아온 key와 같다면
			{
    			temp -> prev = temp -> next; // 임시노드 이전 노드와 다음노드를 연결해 현재 노드를 연결에서 해제한다. 
    			return 0; // 0을 리턴하고 함수 종료. 
			}
        temp = temp -> next; // 임시 노드를 다음 노드로 넘김.
    	}
	}
 	return 0; // 0을 리턴하고 함수 종료. 
}

// kv store를 open하여 기존의 key-value를 불러온다.
// 기존의 kv store가 없다면 생성한다. 
int kvopen()
{
 	return 0;
}

// kv store를 close한다. 
int kvclose()
{
 	return 0;
}
