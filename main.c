#include<stdio.h>
#include"dfa.h"
#define MAX_TOKEN_CLASS 50 //no of regeular expressions

int tcCount;
int check(char* input,int first, int last,int class);
int first =0;
struct token_class{
	char regex[max_regex];
	char class_id[20];
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
			printf(" %c",tcArray[z].Dtrans[i][j]);
		}	
		printf("\n");
		
	}
	printf("initial State :%c\n", tcArray[z].initialstate+'A');
	printf("reject State :%c\n", tcArray[z].rejectstate+'A');
	printf("final State :%c\n", tcArray[z].finalstate+'A');
	
}
int yylen;
char yytext[20];
char* yyclass;
void lex(char* s){
	
	s = s + first;
	while(s[0]== ' '){
	 s++;
	 first++;
	}
	int  last = 0;
	//printf("first = %d,last = %d, s = {%s}\n",first,last,  s);
	
	memset(yytext, '\0',sizeof(yytext));
	yyclass= NULL;
	yylen =0;
	
	//int len = strlen(s);
	int last_token_found_start =-1;
	int last_token_found_end =-1;
	int last_token_found_class=-1;
	while(s[last] != '\0' && s[last] != ' '){
		int j;
		for(j=0;j<tcCount; j++){
			//printf("first = %d\tlast = %d\tcheck = %d\n",0, last, check(s, 0,last,j));
			if(check(s, 0,last,j )){
			
			 //last_token_found_start = first;
			 last_token_found_end = last;
			 last_token_found_class = j;
			 break;
			}
		}
		last++;
		if(s[last] == '\0' || s[last] == ' ') {
			if(last_token_found_end ==-1) printerror();
			//last = first;
			yylen = last_token_found_end+1;
			first = first+last_token_found_end+1;			
			strncpy(yytext,s, yylen);
		}
	}
	yyclass =  tcArray[last_token_found_class].class_id;
	//printf("tokens:\n");
}
int check(char* input,int first, int last,int class){
	int i;
	int state = tcArray[class].initialstate;
	for( i =first; i<=last; i++)
	{
		//printf("class is %d state is %d\n",class, state+1);
		state = tcArray[class].Dtrans[state+1][input[i]-first_char+1]-'A';
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
		if(!isreserved(input[c]))//(input[c]>='a'&& input[c]<='z') || (input[c]>='A' && input[c]<='Z'))
		{
			
			if(isalpha)
			{	
				regex[pt] = '.';
				pt++;
				
			}
			if(input[c] == '\\' ){
					regex[pt] = input[c];
					pt++;
					c++;
			}
			isalpha = 1;
			regex[pt] = input[c];
			pt++;
		}
		else if(input[c]=='|')
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
		memset(followPos[i],0,sizeof(followPos[i]));
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
int symbol_present(char *symtable, char * sym){
	if (strstr(symtable,sym)) return 0;
	else return 1;
}
main(){
	tcCount = 0;
	//char regex[max_regex];
	char regex_in[max_regex];                           
	FILE *fp;
	fp=fopen("rules.txt", "r");
	if(!fp){
		printf("file not found");
		printerror();
	
	}
	while(!feof(fp))
	{
		fscanf(fp,"%s %s\n", tcArray[tcCount].class_id,regex_in);
		//printf("regex_in= %s\n",regex_in);
		preprocess(regex_in, tcArray[tcCount].regex);
		printf("regex= %s\n",tcArray[tcCount].regex);
		recycle();
		//printf("regex= %s\n",tcArray[tcCount].regex);
		create_transition_table(tcArray[tcCount].regex);/*resets global Dtates and Dtrans before usage function  init();*/
		construct_token_classes();
		print_dfatrans(tcCount);
		tcCount++;		                 
	}
	fclose(fp);
	fp = fopen("input.c","r");
	if(!fp){
		printf("file not found");
		printerror();
	
	}
	char *input =(char *)malloc(100*sizeof(char));
	while(!feof(fp))
	{
		char buf[20];
		fscanf(fp,"%s",buf);
		if(feof(fp)) break;
		strcpy(input+strlen(input),buf);
		strcpy(input+strlen(input)," ");
		
	}
	//printf("input = %s\n", input);
	fclose(fp);
	fopen("output.txt","w");
	if(!fp){
		printf("file not found");
		printerror();
	
	}//printf("check = %d ",check("brbit",0,0,0));
	int j=0;
	int i=0;
	char output[100];
	memset(output, '\0',strlen(output));
	while(input[first+1] != '\0'){
		lex(input);
		//if(!strcmp(yyclass,"ID")) 
		//	if(!symbol_present(output,yytext))
		//		printf(output+strlen(output), "%d\t%s\t%s\n",i++,yyclass, yytext);
		//else
		 printf("%d\t%s\t%s\n",j++,yyclass, yytext);
	}
	fprintf(fp," %s",output);
	fclose(fp);
}
