#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#define charstack_SIZE 50
#define set_size 10
#define MAX 10
#define MAX_STATES 10
#define CHAR_COUNT 26
void printerror(void );
void add(int *a, int *b);
struct node{
	char data;
	int isnull;
	int pos;
	struct node * left;
	struct node * right;
	int fp[set_size];
	int lp[set_size];
};
struct state{
	int marked ;
	char name;
	int pos[set_size];
};
int finalstate =-1 ;
int initialstate = 0;
int rejectstate = 1; 

char Dtrans[MAX_STATES][CHAR_COUNT];
struct state Dstates[MAX_STATES];

char charstack[charstack_SIZE];
struct node *structstack[10];
int structstacktop = -1;
int charstacktop = -1;
int followPos [MAX][set_size];

void structstackpush(struct node *a){
	if(structstacktop >= 9) printerror();
	structstack[++structstacktop] = a;
	 	
}
int structstackpop(){
	if(structstacktop== -1) return -1;
	else return structstacktop--;	
}
int structstackempty(){
	return structstacktop ==-1;
} 
void charstackpush(char a ){
	if(charstacktop ==charstack_SIZE) {
	fprintf(stderr,"printerror: charstack full!\n");
		return;
	}
	charstack[++charstacktop]= a;
	return; 
}
char charstackpop(){
	if(charstacktop==-1) return '\0';
	return charstack[charstacktop--];
}
int charstackempty(){
	if (charstacktop==-1) return 1;
	else return 0;
}
void printerror(){
	fprintf(stderr, "printerror!\n");
	exit(0);
}
/*void createtree();*/
char * infixcharstacktopostfix(char *regex){
	/*
		priority(high to low)
		()
		*, +, ?
		.
		|
		
	*/
	char *out = (char *)malloc(sizeof(regex));
	int count =-1;
	int i=0;
	for(;i<strlen(regex); i++){
		switch(regex[i]){
			case '(':
				charstackpush('(');
				break;
			case ')':
				//char c;
				while('('!=charstack[charstacktop]){
					out[++count] = charstackpop();
					if(charstackempty()) printerror();
				}
				if(!charstackempty()) charstackpop();
				break;
			case '|':
				while( !charstackempty() && ('*' ==charstack[charstacktop] || '|' == charstack[charstacktop] || '.' == charstack[charstacktop]) )
					out[++count] = charstackpop();
					charstackpush('|');
				break;
			case '.':
				while( !charstackempty() && ('*' ==charstack[charstacktop] || '.' == charstack[charstacktop]) )
					out[++count] = charstackpop();
					charstackpush('.');
				break;	
			case '*':		 
				while( !charstackempty() && ('*' ==charstack[charstacktop] || '+' ==charstack[charstacktop] || '?' ==charstack[charstacktop]) )
					out[++count] = charstackpop();
				charstackpush('*');
				break;
			case '+':		 
				while( !charstackempty() && ('*' ==charstack[charstacktop] || '+' ==charstack[charstacktop]|| '?' ==charstack[charstacktop]))
					out[++count] = charstackpop();
				charstackpush('+');
				break;
			case '?':		 
				while( !charstackempty() && ('*' ==charstack[charstacktop] || '+' ==charstack[charstacktop]|| '?' ==charstack[charstacktop]))
					out[++count] = charstackpop();
				charstackpush('?');
				break;
			default:
				out[++count]  = regex[i];
				 
		}
	}
	while(!charstackempty()){
        	if(charstack[charstacktop] =='(') printerror();	
        	out[++count]=charstackpop();
   	}
	out[++count] = '$';
	out[++count] = '.';
	out[++count] = '\0';
	printf("regex = %s \nout = %s\n",regex, out); 
	return out;
	
}

struct node * make_tree(char *Po){
	int i,j,k;
	int pos = 1;
	struct node *N;
	for(i=0;Po[i]!='\0';i++){
       		N=(struct node *)malloc(sizeof(struct node));
       		N->data=Po[i]; N->left=NULL; N->right=NULL;
       		memset(N->fp,0,set_size);
       		memset(N->lp,0,set_size);
       		N->isnull = 0;
       		if(!(Po[i] == '*'|| Po[i] == '|'||Po[i] == '+'||Po[i] == '?'||Po[i] == '.')) N->pos = pos++;
       		else N->pos =0;
       		if(Po[i]=='.'||Po[i]=='|'){
          		N->right=structstack[structstackpop()];
          		N->left=structstack[structstackpop()];
	       		//printf("%c",N->left->data);
       		}
       		else if(Po[i]=='*' || Po[i] == '?'){
       			N->isnull = 1;
          		N->left=structstack[structstackpop()];
          		N->right=NULL;
       		
       		}
       		else if(Po[i] == '+' )
       		{
       			N->left=structstack[structstackpop()];
          		N->right=NULL;
       		
       		}
       		structstackpush(N);
   	}
   	return structstack[structstackpop()];
}

void print(struct node * n){
	if(n == NULL) return ;
	print(n->left);
	printf(" %c",n->data);
	print(n->right);
	
		
}
void first_last_pos(struct node * curr){
	if(curr->left == NULL && curr->right ==NULL) {
		curr->fp[0] = curr->pos;
		curr->lp[0] = curr->pos;
		return;
	}
	if(curr->left !=NULL)
		first_last_pos(curr->left);
	if(curr->right !=NULL)
		first_last_pos(curr->right);
		
	if (curr->data =='|'){
		if(curr->left == NULL || curr->right ==NULL) printerror();
		add(curr->fp,curr->left->fp);
		add(curr->fp,curr->right->fp);
		add(curr->lp,curr->left->lp);
		add(curr->lp,curr->right->lp);
		
		//return ;
	}
	if(curr->data == '.' || curr->data == '+'){
		if(curr->left->isnull) {
			add(curr->fp,curr->left->fp);
			add(curr->fp,curr->right->fp);		
		}
		else {
			add(curr->fp,curr->left->fp);
		}
		/*if(curr->right->isnull){
			add(curr->lp,curr->left->lp);
			add(curr->lp,curr->right->lp);
		}
		else {*/
			add(curr->lp,curr->right->lp);
		//}
	}
	if(curr->data == '*' || curr->data == '?'){
		add(curr->fp,curr->left->fp);
		add(curr->lp,curr->left->lp);
	}
}

void cal_follow_sets(struct node *curr ){
	if(curr == NULL) return;
	cal_follow_sets(curr->left);
	cal_follow_sets(curr->right);
	if(curr->left == NULL) return;
	int i;
	switch(curr->data){
		case '.':
			i=0;
			for(;curr->left->lp[i] !=0; i++)
				add(followPos[curr->left->lp[i]], curr->right->fp);
			break;
		case '*':
			i=0;
			for(;curr->left->lp[i] !=0; i++)
				add(followPos[curr->left->lp[i]], curr->left->fp);
			break;
		default:
			break;
	}

}
int isleaf(struct node * n){
	return (n->left ==NULL && n->right== NULL) ;
}
void add(int *a, int *b){
	//int i = 0 ;
	//for(i = 0 ; a[i]!=0; i++);
	int j =0;
	for(j = 0 ; b[j]!=0; j++){
		int i;
		int found = 0;
		for(i = 0 ; a[i]!=0; i++)
			if(a[i] == b[j] ) found = 1;
			
		if(!found) a[i] = b[j];
	}
	 
}
void print_fp(struct node * n){
	if(n == NULL) return ;
	print_fp(n->left);
	int i=0;
	printf("%d=> {",n->pos);
	for(; n->fp[i]!=0;i++) 
		printf(" %d,",n->fp[i]);
	printf("}\n");
	print_fp(n->right);
}
void print_lp(struct node * n){
	if(n == NULL) return ;
	print_lp(n->left);
	int i=0;
	printf("%d=> {",n->pos);
	for(; n->lp[i]!=0;i++) 
		printf(" %d,",n->lp[i]);
	printf("}\n");
	print_lp(n->right);
}
void print_follow(){
	int i=0;
	for(i=0;i<set_size; i++){
		printf("%d=>{",i);
		int j=0;
		for(;followPos[i][j] !=0; j++) printf("%d,",followPos[i][j]);
		printf("}\n");
	}
}
int unmarked(){
	int i=-1;
	for(i=0; i<MAX_STATES; i++){
		if( Dstates[i].marked == 0 && Dstates[i].name >= 'A')  
			return i;	
	}
	return -1;
}
int present(int *temppos){
	int i=-1;
	int testarray[255];	
	for(i=0; i<MAX_STATES; i++){
		memset(testarray,0,sizeof(testarray));
		int j=0;
		//printf("pos ={");
		for(j=0;Dstates[i].pos[j]!=0; j++)
			{
			 testarray[Dstates[i].pos[j]]++;
		//	 printf("%d,",Dstates[i].pos[j]);
			}
		//	printf("}\ntempos = {");
		for(j=0;temppos[j]!=0; j++)
		{
			testarray[temppos[j]] --;
			//printf("%d,",temppos[j]);
		}
		int found =1;
		//printf("}\n");
		
		/*printf("%d\n",i);
		for(j=0;j<255; j++)
			printf("%d ",testarray[j]);
		printf("\n");
		*/for(j=0;j<255; j++)
			if(testarray[j]!=0) found =0;   
		if( found ==1) return i;	
	}
	return -1;

}
void init(struct node *root){	
/*reset Dstates */
	memset(Dstates,'\0',sizeof(Dstates));
	int j=0;
	int i=0;
/*reset Dtrans*/
	for(i=1; i<MAX_STATES; i++)
		for(j=1; j<CHAR_COUNT; j++)
			Dtrans[i][j] = '\0';
/* fill states names and alphabet set in Dtrans*/
	for(j=1; j<CHAR_COUNT; j++)
		Dtrans[0][j] = 'a'+j-1;
	for(j=1; j<MAX_STATES; j++)
		Dtrans[j][0] =('A'+j-1);
	char name ='A'	 ;
	Dstates[0].name= name;
	add(Dstates[0].pos, root->fp);
	Dstates[0].marked = 0;
	name ='B'	 ;
	Dstates[1].name= name;
	Dstates[1].marked = 0;
	
	
}
void construct_dfa(char * postfix ){
	finalstate = initialstate;
	int j=0;
	char name ='B';
	int um =unmarked();
	printf("um  = %d\n",um);
	int statecount =1;
	while(um !=-1){
		
		Dstates[um].marked =1;
		for(j =1; j<CHAR_COUNT; j++){
			//printf("Dtrans[2][0] = %c\n",Dtrans[3][0]);
			int temppos[set_size];
			memset(temppos, 0, sizeof(temppos));
			int i=0;
			while(Dstates[um].pos[i]!=0 ){
				int k=0;
				int z=0;
				while(k != Dstates[um].pos[i]){
					if(isalnum(postfix[z])) k++;
					z++;
				}
				//printf("at position %d, char is %c\n",Dstates[um].pos[i], postfix[z-1]);
				if(postfix[z-1]== 'a'+ j-1)
					add(temppos,followPos[Dstates[um].pos[i]]);
				i++;
			} 
			/*printf("temppos1 = { ");
			for(i=0;temppos[i]!=0; i++)
				printf("%d,",temppos[i]);
			printf("}\n");
			*/
			int ispresent = present(temppos);
			//printf("ispresent= %d\n",ispresent);
			if( temppos[0] != 0){
				if(ispresent==-1){
				Dstates[statecount++].marked = 0;
				Dstates[statecount].name = ++name; 
				add(Dstates[statecount].pos, temppos);
				Dtrans[Dstates[um].name-'A'+1][j] = Dstates[statecount].name;
				}				
				else
				Dtrans[Dstates[um].name-'A'+1][j] = Dstates[ispresent].name;
				
			}
			else{
				Dtrans[Dstates[um].name-'A'+1][j] = 'B';
			}
		}
		um =unmarked();
	}
	if(name >'B') finalstate = name-'A';
	//printf("\nfinal state = %d\n",finalstate);
}

int create_transition_table(char * regex){
	/*
	char regex[20];
	printf("enter regular expressions:");
	scanf(" %s", regex);
	*/
	char * postfix = infixcharstacktopostfix(regex); 
	struct node * root  = make_tree(postfix);
	first_last_pos(root);
//	last_pos(root);
	print(root);
	printf("\nfirst sets \n");
	print_fp(root);
	printf("\nlast sets \n");
	print_lp(root);
	cal_follow_sets(root);
	printf("\nfollow sets \n");
	
	print_follow();	
	init(root);
	//print_dfatrans();
	construct_dfa(postfix);
	//print_dfatrans();
}
