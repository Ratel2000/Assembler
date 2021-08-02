#include <stdio.h>
#include <string.h>

int split3(char *str, char *delim, char arr[2][80]) {
    char *tok;
    char strCopy[80];
    char temp2[80],temp3[80];
    int i;

    strcpy(strCopy, str);

    if(!strcmp(strCopy,"")) { /* if the string is empty */
        strcpy(arr[0],"");
        strcpy(arr[1],"");
        strcpy(arr[2],"");
        return -1;
    }

    tok = strtok(strCopy, delim); /* look for the first token */
    if (tok != NULL && strlen(tok) == strlen(str)) { /* if there is no delim */
        strcpy(arr[0], str);
        strcpy(arr[1], "");
        strcpy(arr[2], "");
        return -1;
    }   
	
    i = 0;
    
    strcpy(arr[0], tok);
    tok = strtok (NULL, delim);
    
    int d = strcpy(arr[1], tok);
    
    strcpy(arr[2], (strCopy+( strlen(arr[0]) + strlen(arr[1])+2)));

    
    return 0;
}


int main()
{
	char arr[2][80];
	arr[2][80]='\0';
	char *str="$3,$5,loop";
	char *del=",";
	split3(str,del,arr);
	printf("%s",arr[0]);
	printf("\n");
	printf("%s",arr[1]);
	printf("\n");
	printf("%s",arr[2]);
	printf("\n");

	return 0;
}
