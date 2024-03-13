#include <stdio.h>
#include <strings.h>
#include <stdlib.h>

// 각 모든 함수는 성공시 0, 실패시 각자 정의한 에러코드를 리턴한다.

struct hashNode{
	struct hashNode *prev;
	struct hashNode *next;
	char *nodekey;
	char *nodevalue;
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
	FILE *fp; // fp란 이름의 파일포인터 생성 
	fp = fopen("kv-store.txt", "r"); // kv-store.txt 파일을 읽기 모드로 오픈. 파일이 없을 경우 NULL 리턴 
	if (fp == NULL) //  kv-store.txt 파일이 없어서 NULL이 파일포인터에 리턴되었다면 
	{
		fp = fopen("kv-store.txt", "w"); // kv-store.txt 파일을 쓰기 모드로 오픈. kv-store.txt 파일이 생성 됨. 
		fclose(fp); //  fclose로 파일을 닫는다. 
		return 0; // 0을 리턴하고 함수 종료.  
	}
	else // kv-store.txt 파일이 있어서 제대로 열기가 되었으면 (파일포인터가 NULL이 아니라면) 
	{
		while(!feof(fp)) // 파일의 끝에 다다를 때 까지 반복 (feof 함수가 0을 반환할 때 까지) 
		{
			int hash; // 해시 값을 받아올 정수형 변수 선언 
			char *buf1 = malloc((sizeof(char) * 4096) + 1); // 키 값이 들어 갈 4097바이트의 buf1을 동적 할당한다.
			char *buf2 = malloc(sizeof(char) * 1000000); // 밸류 값이 들어갈 1MB의 buf2를 동적 할당한다. 문제 존재. 밸류 값의 크기를 확정할 수 없으므로 크기 조정 필요. 
			//printf("%d", i);
			struct hashNode* newNode = (struct hashNode*)malloc(sizeof(struct hashNode)); // newNode란 이름의 hashNode 동적할당  
    		newNode -> prev = NULL; // 새로운 노드의 전 노드를 가리키는 값에 NULL을 넣는다.
    		newNode -> next = NULL; // 새로운 노드의 다음 노드를 가리키는 값에 NULL을 넣는다.
    		fscanf(fp, "%d %s %s\n", &hash, buf1, buf2);
			// 파일안에 있는 내용을 공백과 개행으로 구분하여 첫번째 내용을 정수형으로 hash에 저장, 두번째 내용을 buf1에, 세번째 내용을 buf2에 저장.
    		newNode -> nodekey = buf1; // newNode의 nodekey에 buf1의 주소 전달.
    		newNode -> nodevalue = buf2; // newNode의 nodevalue에 buf2의 주소 전달. 
    		if (hashTable[hash] == NULL) // 해시 테이블이 비어 있을 경우 
			{
    			hashTable[hash] = newNode; // 새로운 노드가 첫번째 노드가 된다. 
  			}
  			else // 해시 테이블이 비어 있지 않을 경우 (새로운 노드를 연결해야 할 경우) 
  			{
  				hashTable[hash] -> prev = newNode; // 새로운 노드를 제일 앞에 넣고 
    			newNode -> next = hashTable[hash]; // 새로운 노드의 다음 노드를 기존 첫 번째 노드로 연결하고 
    			hashTable[hash] = newNode; // 새로운 노드를 첫번째 노드로 연결한다. 기존 첫 번째 노드는 두번째 노드가 된다.
			}
		}
		fclose(fp); // fclose로 파일을 닫는다.
	}
	return 0;
}

// kv store를 close한다. 
int kvclose()
{
	FILE *fp; // fp란 이름의 파일포인터 생성 
	fp = fopen("kv-store.txt", "w"); // kv-store.txt 파일을 쓰기 모드로 오픈. kv-store.txt의 기존 기록은 삭제되고 새롭게 내용이 작성 됨. 
	int i; // 반복문에 사용할 정수 형 변수 i 선언 
	for (i = 0; i < 10007; i++) // 해시 테이블의 인덱스로 i를 사용하기 위해 i를 0부터 10006까지 늘리며 반복함 
	{
		struct hashNode* temp = hashTable[i]; // 임시노드에 i번째 해시테이블 복사 
		while(1) // 나중에 break가 나올때까지 무한 반복 
		{
			if (temp == NULL) // i번째 해시테이블을 복사한 임시노드가 비어 있다면 
			{
				break; // break 하여 while문 밖으로 나가 for문에 의해 i++번째의 해시테이블을 탐색한다. 
			}
			else // i번째 해시테이블을 복사한 임시노드에 데이터가 있다면 
			{
				fprintf(fp, "%d %s %s\n", i, temp -> nodekey, temp -> nodevalue);
				// 파일에 작성할 내용을 공백과 개행으로 구분하여
				// i(해시테이블의 인덱스이자 해시값)를 첫번째로, 임시노드의 nodekey를 두번째로 , nodevalue를 세번째로 파일에 저장. 
			}
			temp = temp -> next; // 임시 노드를 다음 노드로 넘김.
		}
	}
	fclose(fp); // fclose로 파일을 닫는다.
 	return 0;
}
