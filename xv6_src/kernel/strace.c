#include "kernel/strace.h"


int tracing;
int syscall_flag = 0; 
int flag_f = 0;
int flag_e = 0;
int flag_s = 0;
int flag_o = 0;

int callcount = 0;
char trace_1[1000];
char trace_2[1000];
char trace_3[1000];
char trace_4[1000];
char trace_5[1000];
char trace_6[1000];
char trace_7[1000];
char trace_8[1000];
char trace_9[1000];
char trace_10[1000];
//char trace_messages[1000][N];
char* sysbuf[N];


char* strcat(char* dest, char* src){
    // make `ptr` point to the end of the destination string
    int i;
    for (i = 0; dest[i]; i++)
        ;

    char* ptr = dest + i;
 
    // appends characters of the source to the destination string
    while (*src != '\0') {
        *ptr++ = *src++;
    }
 
    // null terminate destination string
    *ptr = '\0';
 
    // the destination is returned by standard `strcat()`
    return dest;
}

int streq(char * string1, char * string2){
    int stringlength = 0;
    while(string2[stringlength]!= 0){
        stringlength++;
    }
    for(int i = 0; i < stringlength; i++){
        if(*string1 != *string2){
            return 0;
        }
        if(*string1 == '\n'){
            return 1;
        }
        string1++;
        string2++;
    }
    return 1;
}

void reverse(char str[], int length)
{
    int start = 0;
    int end = length - 1;
    char temp;
    while (start < end)
    {
        temp = *(str+start);
        *(str+start) = *(str+end);
        *(str+end) = temp;
        start++;
        end--;
    }
}

char* itoa(int num, char* str, int base)
{
    int i = 0;
    int isNegative = 0;
 
    /* Handle 0 explicitly, otherwise empty string is printed for 0 */
    if (num == 0)
    {
        str[i++] = '0';
        str[i] = '\0';
        return str;
    }
 
    // In standard itoa(), negative numbers are handled only with
    // base 10. Otherwise numbers are considered unsigned.
    if (num < 0 && base == 10)
    {
        isNegative = 1;
        num = -num;
    }
 
    // Process individual digits
    while (num != 0)
    {
        int rem = num % base;
        str[i++] = (rem > 9)? (rem-10) + 'a' : rem + '0';
        num = num/base;
    }
 
    // If number is negative, append '-'
    if (isNegative)
        str[i++] = '-';
 
    str[i] = '\0'; // Append string terminator
 
    // Reverse the string
    reverse(str, i);
 
    return str;
}