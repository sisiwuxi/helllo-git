
#include <stdio.h>
#include <stdlib.h>

int ulrand(void) 
{
	int ret = 0;
	ret = (((unsigned long)rand()<<24)&0xFF000000ul)
	|(((unsigned long)rand()<<12)&0x00FFF000ul)
	|(((unsigned long)rand()    )&0x00000FFFul);
	printf("\nRandom ret=0x%x\n", ret);    
	if(ret & 0xF0000000)
		return ret;
	else
	{
		ret = ret | (0x80000000 | ((unsigned long)rand()<<24)&0xFF000000ul);
		printf("\nRandom else ret=0x%x\n", ret); 
		return ret;
	}
}


int main(void) 
{
	int ul=0;
	char rand[33];
	char temp[33];
	memset(temp,  0,  33);
	srand(time(NULL));
	ul=ulrand();
	sprintf((char *)temp, "%02X\n", ul);
	strcpy(rand, temp);
	printf("\n[1]Random rand=[%s,%d]\n", rand, strlen(rand)); 
	ul=ulrand();
	sprintf((char *)temp, "%02X", ul);  
	strcat(rand, temp);
	printf("\n[2]Random rand=[%s,%d]\n", rand, strlen(rand)); 
	ul=ulrand();
	sprintf((char *)temp, "%02X", ul);  
	strcat(rand, temp);
	printf("\n[3]Random rand=[%s,%d]\n", rand, strlen(rand)); 
	ul=ulrand();
	sprintf((char *)temp, "%02X", ul);  
	strcat(rand, temp);
	printf("\n[4]Random rand=[%s,%d]\n", rand, strlen(rand)); 
}
