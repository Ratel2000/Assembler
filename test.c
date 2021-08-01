#include "defaults.h"
#include "firstPass.h"

int split(char *str, char *delim, char arr[3][LINE_LEN]) {
    char *tok;
    char strCopy[LINE_LEN];
    char temp[LINE_LEN];
    int i;

    strcpy(strCopy, str);

    

    /* check if the delimeter on the first char */
    for(i=0;i<strlen(delim);i++) {
        if(str[0] == delim[i]) {
            strcpy(arr[IMPORTANT],"");
            strcpy(arr[REST],strCopy+1);
            return DELIM_EXIST;
        }
    }

    tok = strtok(strCopy, delim); /* look for the first token */
    
    
    

    tok = strtok(NULL, delim); /* look for the next token */
    strcpy(arr[IMPORTANT], strCopy);
    strcpy(temp, (str + strlen(strCopy) + 1)); /* we want the rest of the string, and not until the next token */
    strcpy(arr[REST],temp);
    
    strcpy(temp, (str + strlen(strCopy) + 3));
    strcpy(arr[2],temp);
    
    
    
    
    
    return DELIM_EXIST;
}



int main()
{
	
	char arr[3][LINE_LEN];
	char str[82]="$2,$5,$7";
	char delim[3]=",";
	split(str,delim,arr);
	printf(arr[0]);
	printf("%c",'\n');
	printf(arr[1]);
	printf("%c",'\n');
	printf(arr[2]);
	printf("%c",'\n');
	printf(str);
	
	
	
	return 0;
	
		

}
