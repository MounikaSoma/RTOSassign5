#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

int mathematicalEngine(int, char, int);

struct msg_buffer { //datastructure of the message.
    long msg_type;
    char mtext[200];
};

int main(void)
{
    struct msg_buffer buf;
   // The output data will be written into a file.
    FILE *outputFile; 
    int num1, num2;
    char operator;
    int msg_queue_id; //message queue id that is used to access it.
    char *token;
    key_t key; //unique key to identify particular queue using msgget()
    int result;
    outputFile = fopen("answer.txt", "w+");
    fclose(outputFile);
    if ((key = ftok("client.c", 'C')) == -1) { 
        perror("ftok");
        exit(1);
    }
    if ((msg_queue_id = msgget(key, 0644 | IPC_CREAT)) == -1) { 
        perror("msgget");
        exit(1);
    }
    
    printf("Server is on!\n");
    printf("<ClientID>: <Request> = <Result>\n");

    for(;;) { 
        if (msgrcv(msg_queue_id, &buf, sizeof buf.mtext, 0, 0) == -1) 
        {
            perror("msgrcv");
            exit(1);
        }
        token = strtok(buf.mtext, " ");
        if(token==NULL) 
        {
          printf("%li: Empty string recieved\n", buf.msg_type);
          outputFile = fopen("answer.txt", "a+");
          fprintf(outputFile, "%li: Empty string recieved\n", buf.msg_type);
          fclose(outputFile);
        }
        else
        {
        	num1 = atoi(token); 
        	{
        		token = strtok(NULL, " ");
        		if(token==NULL) 
        		{
        			printf("%li: Invalid input\n", buf.msg_type);
              outputFile = fopen("answer.txt", "a+"); 
              fprintf(outputFile, "%li: Invalid input\n", buf.msg_type);
              fclose(outputFile);
        		}
        		else
        		{
        			operator=token[0];
        			token = strtok(NULL, " ");
        			if(token==NULL)
	        		{
	        			printf("%li: Invalid input\n", buf.msg_type);
                outputFile = fopen("answer.txt", "a+");
                fprintf(outputFile, "%li: Invalid input.\n", buf.msg_type);
                fclose(outputFile);
	        		}
        			else
        			{
        				num2 = atoi(token);
        				result = mathematicalEngine(num1, operator, num2); 
				        if(result!=-9999)
				        {
				        	printf("%li: %d %c %d = %d\n", buf.msg_type, num1, operator, num2, result);			        
                  outputFile = fopen("answer.txt", "a+");
                  fprintf(outputFile, "%li: %d %c %d = %d\n", buf.msg_type, num1, operator, num2, result);
                  fclose(outputFile);
                }
        			}
        		}
        	}
        }
       	

    }
    return 0;
}

int mathematicalEngine(int num1, char operator, int num2)
{
   
   int result, i;
   if(operator=='+')
   {
      return num1 + num2;
   }
   else if(operator=='-')
   {
      return num1 - num2;
   }
   else if(operator=='*')
   {
      return num1 * num2;
   }
   else if(operator=='/')
   {
      return num1 / num2;
   }
   else if(operator=='^')
   {
      result = 1;
      for(i=num2;i>0;i--)
      {
        result=result*num1;
      }
      return result;
   }
   else
   {
      printf("Please use one among +,-,*,/ and ^ only as the operator\n");
      return -9999;
   }
   
}
