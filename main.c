#include<stdio.h>
#include"dfa.h"
int check(char* input){
	int i;
	int state = initialstate;
	for( i =0; input[i] !='\0'; i++)
	{
		printf("state is %d\n",state+1);
		state = Dtrans[state+1][input[i]-'a'+1]-'A';
		//state = newstate;
	}
	return state == finalstate;
}
main(){
	char regex[20];
	printf("enter regular expressions:");
	scanf(" %s", regex);
	create_transition_table(regex);
	char input[20];
	printf("enter input:");
	scanf(" %s",input);
	
	printf("%d",check(input));
}
