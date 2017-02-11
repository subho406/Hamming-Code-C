

/*
Written by Subhojeet Pramanik
The program uses Little Endian representation for storing the bits of the character
Hamming code is also calculated treating leftmost bit as 1st bit
*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>



void asciitobin(char *,int);
char* addparitybits(char *);
void paritycheck(char *,char *);



int main(int argc, char *argv[])
{
	int i,j;
	char *encoded;
	if(argc==1)
		printf("Hamming Code: Calculates Hamming code for a given word, string or file");
	else{
		encoded=addparitybits(argv[1]);
//Encode a Word
		printf("\n\n_________Hamming Code Generation_______\n");
		for(i=0;i<strlen(argv[1]);i++){
//Print the bytes after adding parity bits. We now need 12 bit for each byte
			printf("Parity encoding for '%c' : ", argv[1][i]);
			for(j=0;j<12;j++)
				printf("%c", encoded[i*12+j]);
			printf("\n");
}//Modify some bits
	printf("\n\nModifying 5th bit of first character.\n");
	
	if(encoded[4]=='0')
		encoded[4]='1';
	else
		encoded[4]='0';
	//Check error.
	printf("\n\n_________Error Detection and Correction_______\n");
	paritycheck(encoded,argv[1]);
}
}
void paritycheck(char *encodedmsg,char *str){
	int i,j;
int charcount=strlen(str); //Each byte uses 12 bits using parity bits
char * recvmsg=(char *)malloc(strlen(encodedmsg)); //Create a copy of the encoded message
strcpy(recvmsg,encodedmsg);
for(i=0;i<strlen(str);i++){ //We need to reset the parity bits to dummy values again
		int msgbit=0;
		for(j=1;j<=8;j*=2){
	recvmsg[i*12+j-1]='p'; //For now let the parity bits be p.
	}
}
for(i=0;i<charcount;i++){
	int badbitfound=0;
	int badbitpos=0;
	for(j=1;j<=8;j*=2){
		int count=0,offset=0,bit;
		for(offset=0;offset<=12;offset+=j*2){
			for(bit=0;bit<j;bit++)
			{
				if(recvmsg[i*12+(j-1)+offset+bit]=='1')
					count++;
			}
		}
		char result;
		if(count%2==0){
			result='0';
			recvmsg[i*12+j-1]='0';
		}
		else{
			result='1';
			recvmsg[i*12+j-1]='1';}
			if(result!=encodedmsg[i*12+j-1]){
				badbitfound=1;
				badbitpos+=j;
			}
		}
		badbitpos=badbitpos-1;
		if(badbitfound==1){
			printf("Bad bit found for char '%c'\n", str[i]);
			printf("Bad bit at location %d\n",badbitpos+1); 
			printf("Bit found is %c\n",encodedmsg[i*12+badbitpos] );
			if(encodedmsg[i*12+badbitpos]=='0')
				printf("Changing bad bit to 1\n");
			else
				printf("Changing bad bit to 0\n");
		}
		else{
			printf("No Bad bit found for char '%c'\n", str[i]);
		}
	}
	free(recvmsg);
}
void asciitobin(char *bin,int asc)
{
	int i;
	for (i=0;i<8;i++){
		int r=asc%2;
		asc=asc/2;
		if(r==0)
			bin[8-i-1]='0';
		else
			bin[8-i-1]='1';
	}
}
char* addparitybits(char *str) {
	int i,j;
	char *message;
	message=(char *)malloc(strlen(str)*8+1);
	message[strlen(str)*8]='\0';
	for (i=0;i<strlen(str);i++){
		asciitobin(message+(i*8),(int)str[i]);
	}
	printf("Binary values in little endian representation:\n" );
	for(i=0;i<strlen(str)*8;i++){
		printf("%c", message[i]);
		if((i+1)%8==0)
			printf("\n");
	}
	char *encodedmsg=(char *)malloc(12*strlen(str)+1);
encodedmsg[12*strlen(str)]='\0';//Insert all the bits and set parity bits to -1
for(i=0;i<strlen(str);i++){ //For each character
	int msgbit=0;
	for(j=0;j<12;j++){
		if(8%(j+1)!=0) {
			encodedmsg[i*12+j]=message[i*8+msgbit];
			msgbit++;
		}
		else{
encodedmsg[i*12+j]='p'; //For now let the parity bits be p.
}
}
}
for(i=0;i<strlen(str);i++){
	for(j=1;j<=8;j*=2){
		int count=0,offset=0,bit;
		for(offset=0;offset<=12;offset+=j*2){
			for(bit=0;bit<j;bit++)
			{
				if(encodedmsg[i*12+(j-1)+offset+bit]=='1')
					count++;
			}
		}
		if(count%2==0)
			encodedmsg[i*12+j-1]='0';
		else
			encodedmsg[i*12+j-1]='1';
	}
}
return encodedmsg;
}