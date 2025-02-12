extern int tracing;
# define N 10
extern int syscall_flag;
extern int flag_f;
extern int flag_e;
extern int flag_s;
extern int flag_o;

// trace message strings
extern char trace_1[1000];
extern char trace_2[1000];
extern char trace_3[1000];
extern char trace_4[1000];
extern char trace_5[1000];
extern char trace_6[1000];
extern char trace_7[1000];
extern char trace_8[1000];
extern char trace_9[1000];
extern char trace_10[1000];
//extern char trace_messages[1000][N]; // alternative character[] array implementation that we ran out of time to test

// char** pointer to cycle access the N string's storing the trace messages
extern char* sysbuf[N];       

// Keeps track of our spot in the ring buffer
extern int callcount;
// standard lib function for string concatenation. Not standard in xv6
extern char* strcat(char*, char*);
// Function to check string equivalence.
extern int streq(char *, char *);
// Reverse string used when converting integers to strings
void reverse(char*, int);
// Function to convert integer to character
extern char* itoa(int, char*, int);