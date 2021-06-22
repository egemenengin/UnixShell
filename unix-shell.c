/////////////////////
// Egemen Engin  ///
// A Unix Shell ///
//////////////////

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <ctype.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>


char * removeExtra(char * , int);
int isQuit(char *);
void * threadCall(void*);
void wipe(char *);

int main(int argc, char * argv[])
{	
	FILE * fp;                                       //If batch mode is activeted filePointer will be needed.
	char * input = (char *)malloc(512*sizeof(char)); //To hold 512-char input which comes from file or command prompt.
        int exitChecker=0;                               //To control quit
	if(input==NULL){				   //If input is NULL it gives Error message.	
		perror("Error: Not enough memory.\n");
	}
	if(argc <=1){                                    //If there is no argument(input file), systems work in interactive mode	.	
	     fp = stdin;
	}
	else{						   //If there is an argument(input file), systems work in batch mode and open file.
	    fp = fopen(argv[1],"r");	
	}
	 
	 while(exitChecker==0){                          //while loop until exitChecker=1 which means quit is in input line.
	 	if(fp==NULL){	                          //If filePointer is null systems works in interactive mode.
	 		fp=stdin;
	 	}                                     
	 	if(fp==stdin){                           //In interactive mode, "prompt>" is printed before input.
	 		printf("prompt>");
	 	}
	 	
	 	fgets(input,512,fp);                     //Read an input from file or command prompt.
 	
		if(exitChecker== 0&& feof(fp)){	   // If input is in end of file and quit command is not taken, 
			fclose(fp);				   //systems turn into interactive mode and take input one more time to continue.			
			fp=stdin;
			printf("prompt>");
			fgets(input,512,fp);		
		}
	 	if(input[strlen(input)-1]!='\n'){        //If input is bigger than 512 char , program should read only 512 char of it and should take another input.
			 int c = fgetc(fp);
			while ((c = fgetc(fp)) != '\n' && c != feof(fp)) {
				
			}
			printf("SIZE IS TOO LONG TO READ\n");
			continue;
		}
			
		input = removeExtra(input,0);            //These remove extra repeated " " , "|" and ";" leaving only one.
		input = removeExtra(input,1);					
		input = removeExtra(input,2);
		wipe(input);
		
		if(fp!=stdin){	                         // If system works in batch mode, input printed firstly for user read.
	           printf("Current Line: %s\n",input);
		}
		int sizeOfArr = 0;                       
		int numberOfSemicolon=0;
		for(int k = 0 ; k < strlen(input); k++){ //It counts number of "|" because after few steps it will be needed for array.
			if(input[k]=='|'){
				sizeOfArr++;
			}			
		}
		char * endPtr;
		char ** seperatedArr=(char**)malloc(sizeOfArr+1); //It is array to hold splitted input from each "|" character.
		if(seperatedArr == NULL){			    //If memory cannot be allocated for seperatedArr.
			printf("There cannot allocate memory location for seperatedArr");
		        continue;
		}
		char * temp = strtok_r(input, "|",&endPtr);       //Starts to split and hold it in temp.
		
		int counter=0;
		
		while(temp!=NULL){
			
			seperatedArr[counter]=(char*)malloc(strlen(temp)+1);  // It open place for temp.
			if(seperatedArr[counter]==NULL){			//If memory cannot be allocated for seperatedArr.
			   break;
			}
			strcpy(seperatedArr[counter++],temp);                 // copy from temp to array
			 
			temp=strtok_r(NULL,"|",&endPtr);			//Split again input from current index.
			
		}
		if(temp!=NULL){
			printf("There cannot allocate memory location for seperatedArr");
		        continue;
		}
		
		//So input seperated from each "|" character and it holds in seperatedArr.
		//This loop looks every elements of this array.
		for(int l=0; l<sizeOfArr+1;l++)	
		{	
			
			
			int counterOfThread=0;
					
		        for(int m = 0; m < strlen(seperatedArr[l]);m++){ //It counts semicolon
		        	if(seperatedArr[l][m]==';'){
				numberOfSemicolon++;
				}					       
		        }
		        
		        pthread_t * thread = (pthread_t *)malloc(sizeof(pthread_t)*(numberOfSemicolon+1)); //Dynamic Thread array according to number of semicolon.
		        if(thread == NULL){										
		        	printf("There cannot allocate memory location for threads");
		        	continue;		        	
		        }
		        //According to number of semicolon		        		        
		        //If there is no semicolon it means there is one commend and it runs it.
		        
		        if(numberOfSemicolon==0 ){
				 if(isQuit(seperatedArr[l])){ //If it is equal quit, exitChecker becomes 1 
				 			       //which means end of outer for loop programs will exit.
		          		exitChecker=1;		  		          		
		          	}
		          	else                          //If it is not equal to quit, it creats a thread  commands
		          	{                             //and call threadCall with seperatedArr element function to run.
		        	  int rc = pthread_create(&thread[counterOfThread++], NULL, threadCall, (void *)seperatedArr[l]);
				}
				
		        }
		        
		        //If number of semicolon is greater than 0.
		        else if(numberOfSemicolon>0 ){
		        
	                    char * innerTemp = strtok(seperatedArr[l], ";"); // It splits seperatedArr element until ";"
	     
	                    while(innerTemp !=NULL){      //Loop until splitted value equals to NULL
	          	            if(isQuit(innerTemp)) // If there is any quit command, exit checker becomes 1.
	          	            {
	          		           exitChecker=1;		  	          		
	          	            }
	          	            else 		      //If it is not equal to quit, it creats a thread  commands
		          	    {                        //and call threadCall with seperatedArr element function to run.	          	            
	        	               int rc = pthread_create(&thread[counterOfThread++], NULL, threadCall, (void *)innerTemp);
	        	               if (rc) {
            					printf("ERROR; thread create is unsuccessful. Return value: %d\n", rc);
           					exit(-1);
     					}	        	      
	        	            }
	        	           innerTemp = strtok(NULL, ";"); // It splits from current index until ";"
	        	    }
		        }			
				
			  
			for(int c = 0; c < counterOfThread;c++){  //Waits until created threads to completion 
			
				pthread_join(thread[c],NULL);	
				 
			}			
				numberOfSemicolon=0;
			if(thread != NULL){		
				free(thread);
			}
							
		}
		//FREE every element of array and array directly.
		for(int p =0;p<sizeOfArr;p++){
		    if(seperatedArr[p]!=NULL){
			free(seperatedArr[p]);
		    }
		}
		if(seperatedArr!=NULL){		
		    free(seperatedArr);
		}
		
		//If there was any quit command in line, loops end and after that programs terminated. 
		if(exitChecker){
     		   printf("exit state\n");
		  
	 	   break;
		}
		
		}
		
	//FREE input.
	free(input);
	
	return 0;
}
void * threadCall(void* comment){
//It runs the command with the system and then exits the thread.

	char *command = (char *) comment;

  	system(command);  	
  	pthread_exit(NULL);
  	
}
char * removeExtra(char * inp,int deletion){  

// It removes extra repeated symbols which are ";" "|" and space " ".
// In the end it returns string which extra symbols are removed.
// 0 for "space" //  1 for "|" //   2 for ";"

     int i,x;
     char willDelete;
     if(deletion==0){
       willDelete=' ';
     }
     if(deletion==1){
       willDelete=';';
     }
     if(deletion==2){
       willDelete='|';
     }
     for(i=x=0;inp[i];++i){
        	if((inp[i]!=willDelete || (i > 0 && inp[i-1]!=willDelete)) )	
     		     inp[x++] = inp[i];
     	}
     	inp[x]='\0';    
	return inp;
}
int isQuit(char *inp) {
//It  search for "quit" in inp parameter. 
//If there is, it returns 1, otherwise it returns 0.
//Also if there is an echo it doesn't return 1 because echo means print.
    char quit [] = "quit";
    char echo[]="echo";
    int x = 0;
    int  y = 0;
    for (int i=0; inp[i]; i++){
        if (quit[x] == inp[i]){ // For each same char, x increases by 1.
            x++;
        } else if(x !=4) {      //If there is not same character until x is equal 4, x becomes 0.
            x=0;
        }
    }
    for (int i=0; inp[i]; i++){
        if (echo[y] == inp[i]){ // For each same char, x increases by 1.
            y++;
        } else if(y !=4) {      //If there is not same character until x is equal 4, x becomes 0.
            y=0;
        }
    }
    if (x == 4 && y!= 4) { //If there is quit, it returns 1.
        return 1;
    } else {
        return 0; //If there is not, it returns 0.
    }
}
void wipe(char * str){
//If there is a "\n" or "\r" char in end of input it deletes them.
int i=0;
for( ; str[i]!='\0';i++){
	if(str[i]=='\n' || str[i]=='\r'){
	str[i]='\0';
	break;
	}
	
}


}

