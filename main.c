#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <math.h>
#include <ctype.h>
#include <assert.h>
#include <sys/types.h>
#include <sys/wait.h>

#define MAX 4000
#define MAX_STRING 200
#define PI 3.14159265358979323846

typedef struct struct_t {
	char elements[MAX][MAX_STRING];
	int top;
} stack;

int isEmpty(stack s) {
	return s.top <= -1;
}

int isFull(stack s) {
	return s.top >= MAX - 1;
}

void create(stack *s) {
	s->top = -1;
}

void push(stack *s, char *a) {
	if (!isFull(*s)) {
		strcpy(s->elements[++s->top], a);
	}
}

char * pop(stack *s) {
	if (!isEmpty(*s)) {
		return s->elements[s->top--];
	}
	return 0;
}

char * peek(stack *s) {
	if (!isEmpty(*s)) {
		return s->elements[s->top];
	}
	return 0;
}

void printStack(stack s) {
	if (isEmpty(s)) {
		printf("Stack is empty!\n");
		return;
	}
	int n = s.top;
	for (int i = 0; i <= n; i++) {
		printf("%s ", s.elements[i]);
	}
	printf("\n");
}

char * charToString(char ch) {
	char * str = (char *)malloc(2 * sizeof(char));
	str[0] = ch;
	str[1] = '\0';
	return str;
}
int isOperator(char *ch) {
	if (strcmp(ch, "+") == 0 || strcmp(ch, "-") == 0 
	|| strcmp(ch, "*") == 0 || strcmp(ch, "/") == 0 
	|| strcmp(ch, "sin") == 0 || strcmp(ch, "cos") == 0 || strcmp(ch, "tan") == 0
	|| strcmp(ch, "asin") == 0 || strcmp(ch, "acos") == 0 
	|| strcmp(ch, "^") == 0 || strcmp(ch, "sqrt") == 0) {
		return 1;
	}
	return 0;
}
int getPriority(char * ch) {
	int p = 0;
	if (strcmp(ch, "+") == 0 || strcmp(ch, "-") == 0) {
		return p;
	}
	p++;
	if (strcmp(ch, "*") == 0 || strcmp(ch, "/") == 0) {
		return p;
	}
	p++;
	if (strcmp(ch, "sin") == 0 || strcmp(ch, "cos") == 0 
		|| strcmp(ch, "tan") == 0 || strcmp(ch, "asin") == 0
		|| strcmp(ch, "acos") == 0 
		|| strcmp(ch, "^") == 0 || strcmp(ch, "sqrt") == 0) {
		return p;
	}
	return 0;
}
int isNumber(char * ch) {
	int n = strlen(ch);
	for (int i = 0; i < n; i++) {
		if ((ch[i] < '0' || ch[i] > '9') && ch[i] != '.') {
			return 0;
		}
	}
  return 1;
}
char *trim(char *s) {
    while (isspace((unsigned char) *s))
        s++;
    if (*s) {
        char *p = s;
        while (*p)
            p++;
        while (isspace((unsigned char) *(--p)));
        p[1] = '\0';
    }
    return s;
}
float balan(char *a) {
	stack k;
	create(&k);
	int lenA = strlen(a);
	char elements[MAX][MAX_STRING];
	int idx = 0;
	//printf("xx%sxx", a);
	for (int i = 0; i < lenA;) {
		
		char str[100];
		str[0] = '\0';
		
		//la so
		if ((a[i] >= '0' && a[i] <= '9') || a[i] == '.') {
			do {
				strcat(str, charToString(a[i]));
				i++;
	        } while ((a[i] >= '0' && a[i] <= '9') || a[i] == '.');
		}
		//la toan tu sin cos asin acos sqrt
		else if (a[i] >= 'a' && a[i] <= 'z') {
			do {
				strcat(str, charToString(a[i]));
				i++;
	        } while (a[i] >= 'a' && a[i] <= 'z');
		} else if (a[i] == '+' || a[i] == '-' || a[i] == '*' 
			|| a[i] == '/' || a[i] == '(' || a[i] == ')'
			|| a[i] == '^') {
			//la toan tu + - * / ( ) ^
			strcpy(str, charToString(a[i]));
			i++;
		}
		//printf("str=%s i = %d lenA = %d\n", str, i, lenA);
		
		if (isNumber(str)) {
			strcpy(elements[idx++], str);
		}
		
		if (strcmp(str, "(") == 0) {
			push(&k, str);
		}
		
      	if (isOperator(str)) {
	      if (!isEmpty(k)) {
	      	 // cau lenh nay` kiem tra do uu tien giua cac toan tu
	        if (isOperator(peek(&k)) //top cua stack la toan tu thi so sanh do uu tien toan tu
	            && (getPriority(str) <= getPriority(peek(&k)))) {
	       		do {
	       			char * m;
			        m = pop(&k);
			        strcpy(elements[idx++], m);
				} while (!isEmpty(k) && isOperator(peek(&k)) && getPriority(str) <= getPriority(peek(&k)));
				push(&k, str);
	        } else {
	          push(&k, str);
	        }
	      } else {
	    	push(&k, str);
		  }
    	}
    	if (strcmp(str, ")") == 0) {
			char * d;
			while (1) {
				d = pop(&k);
				//pop luon ca dau (
				if (strcmp(d, "(") == 0) 
					break;
				strcpy(elements[idx++], d);
	    	}
	    }
	    
	    //in ra stack
	    // printStack(k);
	    //in ra hau ho
	  	// for (int i = 0; i < idx; i++) {
	  	// 	printf("%s ", elements[i]);
		// }
		// printf("\n====================\n");
		
		
		
	}
  	while (!isEmpty(k)) {
		char * m;
		m = pop(&k);
		strcpy(elements[idx++], m);
  	}
  	//in ra hau ho
	printf("\nHau to: ");
  	for (int i = 0; i < idx; i++) {
  		printf("%s ", elements[i]);
	}
	printf("\n-------------------------");
	
	
  	float data[1000];
  	int idxData = 0;
	for (int i = 0; i < idx; i++) {
		
		if (isNumber(elements[i])) {
			data[idxData++] = atof(elements[i]);
		}
		
	    if (isOperator(elements[i])) {
	      	if (strcmp(elements[i], "+") == 0) {
	      		float temp = data[idxData - 1] + data[idxData - 2];
		        data[idxData - 2] = temp;
		        idxData--;
			} else if (strcmp(elements[i], "-") == 0) {
				float temp = data[idxData - 2] - data[idxData - 1];
		        data[idxData - 2] = temp;
		        idxData--;
			} else if (strcmp(elements[i], "*") == 0) {
				float temp = data[idxData - 1] * data[idxData - 2];
		        data[idxData - 2] = temp;
		        idxData--;
			} else if (strcmp(elements[i], "/") == 0) {
				float temp = data[idxData - 2] / data[idxData - 1];
		        data[idxData - 2] = temp;
		        idxData--;
			} else if (strcmp(elements[i], "^") == 0) {
				float temp = pow(data[idxData - 2], data[idxData - 1]);
				data[idxData - 2] = temp;
				idxData--;
			} else if (strcmp(elements[i], "sqrt") == 0) {
				float temp = data[idxData - 1];
		        data[idxData - 1] = sqrt(temp);
			} else if (strcmp(elements[i], "sin") == 0) {
				float temp = data[idxData - 1];
		        data[idxData - 1] = sin(temp * PI / 180);
			} else if (strcmp(elements[i], "cos") == 0) {
				float temp = data[idxData - 1];
		        data[idxData - 1] = cos(temp * PI / 180);
			} else if (strcmp(elements[i], "asin") == 0) {
				float temp = data[idxData - 1];
		        data[idxData - 1] = asin(temp) * 180 / PI;
			} else if (strcmp(elements[i], "acos") == 0) {
				float temp = data[idxData - 1];
		        data[idxData - 1] = acos(temp) * 180 / PI;
			}
	    }
	    // for (int i = 0; i < idxData; i++) {
	    // 	printf("data[%d] = %f ", i, data[i]);
		// }
		// printf("\n==================\n");
	}
	
	return data[0];	
} 

void doChild(int data_pipes[]) {
	char input[MAX_STRING];
	char output[MAX_STRING];
	int rc;

	// NOTE: Read from other process with PIPES_READ
	memset(input, 0, sizeof(input));
	rc = read(data_pipes[0], &input, sizeof(input)); //nhan bieu thuc tu parent
	printf("\n>> Dang tinh toan bieu thuc: %s", trim(input));
	// puts(input);

	// NOTE: Write to other process with PIPES_WRITE
	memset(output, 0, sizeof(output));
	double res = balan(input);
	snprintf(output, 200, "%f", res);
	rc = write(data_pipes[1], &output, sizeof(output)); //goi ket qua sang parent

	// NOTE: Close all pipes
	close(data_pipes[1]);
	close(data_pipes[0]);
	exit(0);
}

void doParent(int data_pipes[]) {
	int c, rc;
	char input[MAX_STRING];
	char output[MAX_STRING];

	// NOTE: Write to other process with PIPES_WRITE
	memset(input, 0, sizeof(input));
	printf("\n>> Nhap bieu thuc tinh toan: ");
	fgets(input, sizeof input, stdin);
	rc = write(data_pipes[1], &input, sizeof(input)); //goi bieu thuc den child
	
	// NOTE: Wait for other process send data
	wait(NULL);

	// NOTE: Read from other process with PIPES_READ
	memset(output, 0, sizeof(output));
	rc = read(data_pipes[0], &output, sizeof(output)); //nhan ket qua tu child
	printf("\n>> Ket qua = %s", trim(output));

	// NOTE: Close all pipes
	close(data_pipes[1]);
	close(data_pipes[0]);
	exit(0);
}

int main() {
	int data_pipes[2];
	int pid;
	int rc;
	rc = pipe(data_pipes);
	if(rc == -1) {
		perror("Pipe not created!");
		exit(1);
	}
	pid = fork();
	switch(pid) {
		case -1: {
			perror("Child process not created!");
			exit(1);
		}
		case 0: {
			doChild(data_pipes);
		}
		default: {
			doParent(data_pipes);
		}
	}
	return 0;
}