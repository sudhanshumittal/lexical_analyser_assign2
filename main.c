#include<stdio.h>
#include"dfa.h"
#define max_tokens 10
#define MAX_TOKEN_CLASS 10
int tcCount;
int check(char* input,int first, int last,int class);
struct token_class{
	char regex[40];
	char class_id[10];
	//struct state Dstates[MAX_STATES];
	char Dtrans[MAX_STATES][CHAR_COUNT];
	int initialstate;
	int finalstate;
	int rejectstate;
};
struct token_class tcArray[MAX_TOKEN_CLASS];
void print_dfatrans(int z){
	int i,j;
	for(i=0;i<MAX_STATES; i++){
		
		for(j=0;j<CHAR_COUNT; j++){
			printf("\t%c",tcArray[z].Dtrans[i][j]);
		}	
		printf("\n");
		
	}
	printf("initial State :%d\n", tcArray[z].initialstate);
	printf("reject State :%d\n", tcArray[z].rejectstate);
	printf("final State :%d\n", tcArray[z].finalstate);
	
}
void tokenizer(char* s){
	int first=0, last =0;
	char token[max_tokens][10];
	int i;
	for(i=0; i<max_tokens;i++)
		memset(token[i], '\0',sizeof(token[i]));
	int tokencount = 0;
	int len = strlen(s);
	int last_token_found_start =-1;
	int last_token_found_end =-1;
	int last_token_found_class=-1;
	while(first < len){
		int j;
		for(j=0;j<tcCount; j++){
			//printf("first = %d\tlast = %d\tcheck = %d\n",first, last, check(s, first,last,j));
			if(check(s, first,last,j )){
			
			 last_token_found_start = first;
			 last_token_found_end = last;
			 last_token_found_class = j;
			 break;
			}
		}
		last++;
		if(last == len) {
			if(last_token_found_start ==-1) printerror();
			first = last_token_found_end+1;
			last = first;
			strncpy(token[tokencount],s+last_token_found_start, last_token_found_end-last_token_found_start+1);  
			tokencount++;
			last_token_found_start =-1;
			last_token_found_end =-1;
			last_token_found_class=-1;
		}
	}
	printf("tokens:\n");
	for(i=0; i<tokencount;i++)
		printf(" %s\n",token[i]);
}
int check(char* input,int first, int last,int class){
	int i;
	int state = tcArray[class].initialstate;
	for( i =first; i<=last; i++)
	{
		//printf("state is %d\n",state+1);
		state = tcArray[class].Dtrans[state+1][input[i]-'a'+1]-'A';
		//state = newstate;
	}
	return state == tcArray[class].finalstate;
}
void preprocess( char* input, char* regex)
{
	//printf("infn : %s\n",input);
	int c = 0;      // reading input[]
	int pt = 0; // to write in regex[]
	int isalpha = 0;
	while(input[c] != '\0')
	{
		//printf("%c\n",input[c]);
		if(isalnum(input[c]))//(input[c]>='a'&& input[c]<='z') || (input[c]>='A' && input[c]<='Z'))
		{
			
			if(isalpha)
			{	
				regex[pt] = '.';
				pt++;
			}
			isalpha = 1;
			regex[pt] = input[c];
			pt++;
		}else if(input[c]=='|')
		{
			isalpha = 0;
			regex[pt] = input[c];
			pt++;
		}else if(input[c]=='(')
		{
			if(isalpha)
			{	
				regex[pt] = '.';
				pt++;
			}
			isalpha = 0;
			regex[pt] = input[c];
			pt++;
		}else if(input[c]=='*'|| input[c]==')')
		{
			isalpha = 1;
			regex[pt] = input[c];
			pt++;
		}else
		{
			regex[pt] = input[c];
			pt++;
		}
		c++;
	}
	regex[pt] = '\0';
	//printf(" %s\n", regex);
}
void recycle(){
	/*intialstate, finalstate,rejectstate*/
	initialstate = 0;
	finalstate = -1;
	rejectstate = 1;
	/*emptying stacks*/
	structstacktop = -1;
	charstacktop = -1;
	/*resetting followpos*/
	int i;
	for(i=0;i<MAX; i++)
	{
		memset(followPos[i],0,sizeof(followPos));
	}
}

void construct_token_classes(){
	
//	strcpy(tcArray[tcCount].regex,regex);
//	strcpy(tcArray[tcCount].class_id,class_id);
	//construct_dfa(regex);
	int j=0;
	int i=0;
	/*copy global into local Dtrans of the token class*/
	for(i=0; i<MAX_STATES; i++)
		for(j=0; j<CHAR_COUNT; j++)
			tcArray[tcCount].Dtrans[i][j] = Dtrans[i][j];
	/*for(i=0; i<MAX_STATES; i++)
	{
		tcArray[tcCount-1].Dstates[i].marked = Dstates[i].marked;  
		tcArray[tcCount-1].Dstates[i].name = Dstates[i].name;
		for(j=0; j<10; j++){
			tcArray[tcCount-1].Dstates[i]
		}	
	}*/
	tcArray[tcCount].finalstate = finalstate;
	//printf("tcArray[tcCount].finalstate = %d",tcArray[tcCount].finalstate);
	tcArray[tcCount].initialstate = initialstate;
	tcArray[tcCount].rejectstate = rejectstate;	
	
}

main(){
	tcCount = 0;
	char regex[40];
	char regex_in[20];                           
	FILE *fp;
	fp=fopen("rules.txt", "r");
	//printf("enter regular expressions:");
	//scanf(" %s", regex_in);                    
	while(!feof(fp))
	{
		fscanf(fp,"%s %s\n", tcArray[tcCount].class_id,regex_in);
		preprocess(regex_in, tcArray[tcCount].regex);
		recycle();
		create_transition_table(tcArray[tcCount].regex);/*resets global Dtates and Dtrans before usage function  init();*/
		construct_token_classes();
		print_dfatrans(tcCount);
		tcCount++;		                 
	}
	fclose(fp);
	char input[20];
	printf("enter input:");
	scanf(" %s",input);
	tokenizer(input);
	//printf("%d",tokenizer(input));
}
