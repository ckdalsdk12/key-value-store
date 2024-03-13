#include <stdio.h> // 기본 stdio.h 헤더 
#include <strings.h> // strlen과 strcpy과 strcmp등을 사용하기 위한 strings.h 헤더 
#include <stdlib.h> // malloc과 free를 사용하기 위한 stdlib.h 헤더 

#define GET_ERROR -1 // 에러코드 정의 
#define PUT_ERROR -2 // 에러코드 정의 
#define DEL_ERROR 0 // 에러코드 정의 
#define OPEN_ERROR -4 // 에러코드 정의 
#define CLOSE_ERROR -5 // 에러코드 정의 

// 각 모든 함수는 성공시 0, 실패시 각자 정의한 에러코드를 리턴한다.

struct hashNode{ // 구조체 hashNode 선언 
	struct hashNode *prev; // 앞 노드를 나타낼 prev 구조체 포인터. 
	struct hashNode *next; // 뒷 노드를 나타낼 next 구조체 포인터. 
	char *nodekey; // 노드내의 key 
	char *nodevalue; // 노드내의 value 
};

struct hashNode *hashTable[520193] = {NULL}; // hashTable이라는 이름을 가진 hashNode형 구조체 포인터 배열 생성 및 NULL로 초기화 

int hashFunction(char *key) // int 리턴값과 매개변수 key를 가지는 해시 함수 선언 
{
	int hash = 0; // int형 hash 변수 선언 및 0으로 초기화 
	int i; // for문에 사용할 int형 i 선언
	int length = strlen(key); // for문에 사용할 int형 length 선언 및 key의 길이로 초기화. 
	for (i = 0; i < length; i++) // i가 0부터 시작하여 length보다 작을때까지 반복문. i는 1씩 증가 
	{
		hash += key[i]; // hash 변수에 key가 가진 문자 하나하나의 아스키 코드값을 더한다. 
	}
	hash = hash % 520193; // hash 변수를 hashTable의 배열 크기로 나눈다. (소수) 
	return hash; // hash 변수를 리턴한다. 
}

// 아래의 PrintAllHashData함수는 디버깅 테스트용 - 추후 삭제
/* 
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
                printf("%s ", node -> nodevalue);
                node = node -> next;
            }
        }
        printf("\n");
    }
}
*/
// 위의 PrintAllHashData함수는 디버깅 테스트용 - 추후 삭제

// 주어진 key를 이용하여 value를 찾고, buf에 저장하여 리턴시켜 준다.
// 존재하지 않는 key인 경우 null을 리턴한다.
int kvget(char *key, char *buf) // int형을 리턴하고 key와 buf를 받는 kvget 함수. 
{
	int hash = hashFunction(key); // int형 hash변수에 hash함수를 통한 해시값을 넣음. 
	struct hashNode* temp = hashTable[hash]; // 임시 노드에 첫번째 노드 복사 
	while(1) // 노드가 비어 있을 때 까지 탐색 
	{
		if (temp == NULL) // 노드가 비어 있으면 
		{
			strcpy(buf, "\0"); // 널문자를 buf에 넣는다.
			return GET_ERROR; // 에러코드(-1)를 리턴하고 함수 종료. 
		}
		
		else if (strcmp(temp -> nodekey, key) == 0) // 만약 임시 노드에 있는 key가 받아온 key와 같다면
		{
			strcpy(buf, temp -> nodevalue); // 밸류값을 buf에 넣는다.
			return 0; // 0을 리턴하고 함수 종료. 
		}
        temp = temp -> next; // 임시 노드를 다음 노드로 넘김.
    }
 	return 0; // 0을 리턴하고 함수 종료. 
}

// 주어진 key와 data를 이용하여 kv store에 저장한다. 
// 이미 key가 존재하는 경우 덮어쓴다. 
int kvput(char *key, char *data) // int형을 리턴하고 key와 data(밸류)를 받는 kvput 함수
{
	int hash = hashFunction(key); // hash변수에 hash함수를 통한 해시값을 넣음. 
	struct hashNode* newNode = (struct hashNode*)malloc(sizeof(struct hashNode)); // newNode란 이름의 hashNode 동적할당 
	newNode -> nodekey = key; // 새로운 노드에 키를 집어 넣는다. 
	newNode -> nodevalue = data; // 새로운 노드에 밸류를 집어 넣는다. 
    newNode -> prev = NULL; // 새로운 노드의 전 노드를 가리키는 값에 NULL을 넣는다.
    newNode -> next = NULL; // 새로운 노드의 다음 노드를 가리키는 값에 NULL을 넣는다.
    
  	if (hashTable[hash] == NULL) // 첫번재 노드가 비어 있을 경우 
	{
    	hashTable[hash] = newNode; // 새로운 노드가 첫번째 노드가 된다. 
    	return 0; // 0을 반환하여 함수를 종료한다. 
  	}
  	
  	else // 첫번째 노드가 비어 있지 않을 경우 
	{
		struct hashNode* temp = hashTable[hash]; // 임시 노드에 첫번째 노드 복사 
		while(temp != NULL) // 노드가 비어 있을 때 까지 탐색 
		{
			if (strcmp(temp -> nodekey, key) == 0) // 만약 임시 노드에 있는 key가 받아온 key와 같다면
			{
        		temp -> nodevalue = data; // 임시 노드에 받은 value값 저장 (덮어쓰기) 
				return PUT_ERROR; // 에러코드 -2를 반환하여 함수를 종료한다. 
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
int kvdel(char *key) // int형을 리턴하고 key를 받는 kvdel 함수 
{
	int hash = hashFunction(key); // hash변수에 hash함수를 통한 해시값을 넣음. 
	struct hashNode* temp = hashTable[hash]; // 임시 노드에 첫번째 노드 복사 
	if (hashTable[hash] == NULL) // 해시테이블이 NULL일 경우 
	{
		return DEL_ERROR; // 검색할 키를 가진 노드가 없을테니 에러코드 0을 리턴하고 함수 종료. 
	}
	else if (strcmp(hashTable[hash] -> nodekey, key) == 0) // 만약 첫 번째 노드의 키가 삭제하여야 할 노드라면
	{
        hashTable[hash] = hashTable[hash] -> next; // 다음 노드를 첫 번째 노드로 설정한다. 
        return 0; // 0을 리턴하고 함수 종료.
	}
	else // 두 상황 모두 아니라면 
	{
		while(1) // 노드가 비어 있을 때 까지 탐색 
		{
			temp = temp -> next; // 임시 노드를 다음 노드로 넘김.
			if (temp == NULL) // 노드가 비어 있으면 
			{
				return DEL_ERROR; // 에러코드 0을 리턴하고 함수 종료. 
			}
			else if (strcmp(temp -> nodekey, key) == 0) // 만약 임시 노드에 있는 key가 받아온 key와 같다면
			{
				temp = temp -> prev; // 임시 노드를 이전 노드로 변경하고 
    			temp -> next = temp -> next -> next; // 임시노드의 next값을 임시노드의 다다음 노드로 연결해 삭제할 키와 밸류가 들어있는 노드를 연결에서 해제한다. 
    			return 0; // 0을 리턴하고 함수 종료. 
			}
    	}
	}
 	return 0; // 0을 리턴하고 함수 종료. 
}

// kv store를 open하여 기존의 key-value를 불러온다.
// 기존의 kv store가 없다면 생성한다. 
int kvopen() // int형을 리턴하는 kvopen 함수 
{
	FILE *fp; // fp란 이름의 파일포인터 생성 
	fp = fopen("kv-store.txt", "r"); // kv-store.txt 파일을 읽기 모드로 오픈. 파일이 없을 경우 NULL 리턴 
	if (fp == NULL) //  kv-store.txt 파일이 없어서 NULL이 파일포인터에 리턴되었다면 
	{
		fp = fopen("kv-store.txt", "w"); // kv-store.txt 파일을 쓰기 모드로 오픈. kv-store.txt 파일이 생성 됨. 
		fclose(fp); //  fclose로 파일을 닫는다. 
		return OPEN_ERROR; // 에러코드 -4를 리턴하고 함수 종료.  
	}
	else // kv-store.txt 파일이 있어서 제대로 열기가 되었으면 (파일포인터가 NULL이 아니라면) 
	{
		while(!feof(fp)) // 파일의 끝에 다다를 때 까지 반복 (feof 함수가 0을 반환할 때 까지) 
		{
			int hash; // 해시 값을 받아올 정수형 변수 선언 
			char *buf1 = malloc((sizeof(char) * 4096) + 1); // 키 값이 들어 갈 4097바이트의 buf1을 동적 할당한다.
			char *buf2 = malloc(sizeof(char) * 1000000); // 밸류 값이 들어갈 1MB의 buf2를 동적 할당한다. 문제 존재. 밸류 값의 크기를 확정할 수 없으므로 크기 조정 필요.
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
  				struct hashNode* temp = hashTable[hash]; // 임시 노드에 첫번째 노드 복사 
				while(temp != NULL) // 노드가 비어 있을 때 까지 탐색 
				{
					if (strcmp(temp -> nodekey, buf1) == 0) // 만약 임시 노드에 있는 key가 받아온 buf1과 같다면
					{
						temp -> nodekey = buf1; // 임시 노드에 받은 key값 저장 (덮어쓰기) 
        				temp -> nodevalue = buf2; // 임시 노드에 받은 value값 저장 (덮어쓰기) 
						return 0; // 0을 리턴하고 함수 종료.
					}
					temp = temp -> next; // 임시 노드를 다음 노드로 넘김.
    			}
  				hashTable[hash] -> prev = newNode; // 새로운 노드를 제일 앞에 넣고 
    			newNode -> next = hashTable[hash]; // 새로운 노드의 다음 노드를 기존 첫 번째 노드로 연결하고 
    			hashTable[hash] = newNode; // 새로운 노드를 첫번째 노드로 연결한다. 기존 첫 번째 노드는 두번째 노드가 된다.
			}
		}
		fclose(fp); // fclose로 파일을 닫는다.
		return 0; // 0을 리턴하고 함수 종료.
	}
	return 0; // 0을 리턴하고 함수 종료.
}

// kv store를 close한다. 
int kvclose() /// int형을 리턴하는 kvclose 함수 
{
	FILE *fp; // fp란 이름의 파일포인터 생성 
	fp = fopen("kv-store.txt", "w"); // kv-store.txt 파일을 쓰기 모드로 오픈. kv-store.txt의 기존 기록은 삭제되고 새롭게 내용이 작성 됨. 
	int i; // 반복문에 사용할 정수 형 변수 i 선언 
	int emptyCheck = 0; // 에러체크를 위해 변수 선언 및 0으로 초기화. 
	for (i = 0; i < 520193; i++) // 해시 테이블의 인덱스로 i를 사용하기 위해 i를 0부터 520192까지 늘리며 반복함 
	{
		struct hashNode* temp = hashTable[i]; // 임시노드에 i번째 해시테이블 복사 
		while(1) // 나중에 break가 나올때까지 무한 반복 
		{
			if (i == 520192) // i가 520192이면 (마지막 해시테이블이 되면) 
			{
				if (temp == NULL) // 마지막 해시테이블을 복사한 임시노드가 비어 있다면 
				{
					if(emptyCheck == 0) // 지금까지 파일에 써넣은 데이터가 한개도 없다면 
					{
						return CLOSE_ERROR; // 에러코드 -5를 리턴하고 함수 종료. 
					}
				}
			}
			
			if (temp == NULL) // i번째 해시테이블을 복사한 임시노드가 비어 있다면 
			{
				break; // break 하여 while문 밖으로 나가 for문에 의해 i++번째의 해시테이블을 탐색한다. 
			}
			
			else // i번째 해시테이블을 복사한 임시노드에 데이터가 있다면 
			{
				fprintf(fp, "%d %s %s\n", i, temp -> nodekey, temp -> nodevalue);
				// 파일에 작성할 내용을 공백과 개행으로 구분하여
				// i(해시테이블의 인덱스이자 해시값)를 첫번째로, 임시노드의 nodekey를 두번째로 , nodevalue를 세번째로 파일에 저장.
				emptyCheck++; // emptyCheck 변수에 1을 더함 
			}
			temp = temp -> next; // 임시 노드를 다음 노드로 넘김.
		}
	}
	fclose(fp); // fclose로 파일을 닫는다.
 	return 0; // 0을 리턴하고 함수 종료.
}
