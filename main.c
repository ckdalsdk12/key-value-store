#include <stdio.h>
#include <stdlib.h>
#include <time.h> 
#include <string.h>
#include <unistd.h>



// This code is an example to test
int main(int argc, char *argv[]) {
	
	int rs;
	int i;
	int iteration=4095;
	
	char value[8192]="";
	char key[4096]="";
	char buf[8192]; // 예제 크기임. 실제로는 다양한 크기를 처리할 수 있어야 함.
	char *k, *v;
	
	struct timeval tv1,tv2;

		
	

	
	// 4095 byte
	strncpy(key, "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", 4095);
//	strncpy(key, "abcdefgh", 8);
	//printf("%d\n", strlen(key));

	strncpy(value, key, 4096);
	strncat(value, value, 4096);
	//printf("%d\n", strlen(value));




	gettimeofday(&tv1, NULL);
	rs=kvopen();
	gettimeofday(&tv2, NULL);
	printf("kvopen ok %d\n", rs);
	printf("time : %f\n",((tv2.tv_sec+tv2.tv_usec*0.000001)-(tv1.tv_sec+tv1.tv_usec*0.000001)));



	k=key;
	v=value;
	gettimeofday(&tv1, NULL);
	for (i=0; i<iteration; i++)
	{
		rs=kvput(k, v);
		k++;
		v++;
	}
	gettimeofday(&tv2, NULL);
	printf("kvput ok %d\n", rs);
	printf("time : %f\n",((tv2.tv_sec+tv2.tv_usec*0.000001)-(tv1.tv_sec+tv1.tv_usec*0.000001)));


	k=key;
	v=value;
	gettimeofday(&tv1, NULL);
	for (i=0; i<iteration; i++)
	{
		rs=kvget(k, buf);
		//printf("%s\n", buf);
		k++;
	}
	gettimeofday(&tv2, NULL);
	printf("kvget ok %d\n", rs);
	printf("time : %f\n",((tv2.tv_sec+tv2.tv_usec*0.000001)-(tv1.tv_sec+tv1.tv_usec*0.000001)));


	k=key;
	v=value;
	gettimeofday(&tv1, NULL);
	for (i=0; i<iteration/2; i++)
	{
		rs=kvdel(k);
		k++;
	}
	gettimeofday(&tv2, NULL);
	printf("kvdel ok %d\n", rs);
	printf("time : %f\n",((tv2.tv_sec+tv2.tv_usec*0.000001)-(tv1.tv_sec+tv1.tv_usec*0.000001)));


	gettimeofday(&tv1, NULL);
	rs=kvclose();
	gettimeofday(&tv2, NULL);
	printf("kvclose ok %d\n", rs);
	printf("time : %f\n",((tv2.tv_sec+tv2.tv_usec*0.000001)-(tv1.tv_sec+tv1.tv_usec*0.000001)));








	gettimeofday(&tv1, NULL);
	rs=kvopen();
	rs=kvclose();
	rs=kvopen();
	rs=kvclose();
	rs=kvopen();
	gettimeofday(&tv2, NULL);
	printf("kvopen ok %d\n", rs);
	printf("time : %f\n",((tv2.tv_sec+tv2.tv_usec*0.000001)-(tv1.tv_sec+tv1.tv_usec*0.000001)));


	k=key;
	v=value;
	gettimeofday(&tv1, NULL);
	for (i=0; i<iteration; i++)
	{
		rs=kvget(k, buf);
		//printf("%s\n", buf);
		k++;
	}
	gettimeofday(&tv2, NULL);
	printf("kvget ok %d\n", rs);
	printf("time : %f\n",((tv2.tv_sec+tv2.tv_usec*0.000001)-(tv1.tv_sec+tv1.tv_usec*0.000001)));

	k=key;
	v=value;
	gettimeofday(&tv1, NULL);
	for (i=0; i<iteration; i++)
	{
		//rs=kvdel(k);
		k++;
	}
	gettimeofday(&tv2, NULL);
	printf("kvdel ok %d\n", rs);
	printf("time : %f\n",((tv2.tv_sec+tv2.tv_usec*0.000001)-(tv1.tv_sec+tv1.tv_usec*0.000001)));


	gettimeofday(&tv1, NULL);
	rs=kvclose();
	gettimeofday(&tv2, NULL);
	printf("kvclose ok %d\n", rs);
	printf("time : %f\n",((tv2.tv_sec+tv2.tv_usec*0.000001)-(tv1.tv_sec+tv1.tv_usec*0.000001)));






	return 0;
}


