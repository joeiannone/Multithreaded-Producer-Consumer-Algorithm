#include <stdio.h>
#include <pthread.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <math.h>
#include <stdbool.h>

#define BUF_SIZE	20

char blue_queue[BUF_SIZE][60];
char red_queue[BUF_SIZE][60];
char green_queue[BUF_SIZE][60];

struct sigaction sa;
pthread_mutex_t lock;

int count = 0;

int eof=0;
int r=0,b=0,g=0;

void * producer_thread(void * arg);
void * blue_consumer_thread(void * arg);
void * red_consumer_thread(void * arg);
void * green_consumer_thread(void * arg);
bool is_square(int x);
bool is_fib(int n);
int is_prime(int num);
int Compare(const void * p1, const void * p2);
static void event_handler(int sig, siginfo_t *siginfo, void *context);

int main(int argc, char **argv) {

	sa.sa_sigaction = event_handler;
        sa.sa_flags = SA_RESTART;
	if (sigaction(SIGINT, &sa, NULL) < 0)
        {
                perror("error sigint");
                return 1;
        }

	pthread_mutex_init(&lock, NULL);	
	
	pthread_t producer;
	pthread_t blue_consumer;
	pthread_t red_consumer;
	pthread_t green_consumer;
	
	pthread_create(&producer, NULL, &producer_thread, NULL);
        pthread_create(&blue_consumer, NULL, &blue_consumer_thread, NULL);
        pthread_create(&red_consumer, NULL, &red_consumer_thread, NULL);
        pthread_create(&green_consumer, NULL, &green_consumer_thread, NULL);

	pthread_join(producer, NULL);
	pthread_join(blue_consumer, NULL);
	pthread_join(red_consumer, NULL);
	pthread_join(green_consumer, NULL);

	return 0;
}


void * producer_thread(void * arg) {
	
	char line[80];
	char color[12];
	int i;
	
	while(fgets(line, sizeof(line), stdin)) {
		sscanf(line, "%s", color);
		if (strcmp("red", color) == 0) {
			i = 0;
			while (strlen(red_queue[i]) != 0) {
				if (i == BUF_SIZE) {
					i = 0;
				} else {
					i++;
				}
			}
			strcpy(red_queue[i], line);
		}
		else if (strcmp("green", color) == 0) {
			i = 0;
			while (strlen(green_queue[i]) != 0) {
                      		if (i == BUF_SIZE) {
                       			i = 0;
                   		} else {
                       			i++;
                		}
              		}
     			strcpy(green_queue[i], line);
		}
		else if (strcmp("blue", color) == 0) {
			i = 0;	
			while (strlen(blue_queue[i]) != 0) {
        			if (i == BUF_SIZE) {
          				i = 0;
      				} else {
       					i++;
     				}
             		}
      			strcpy(blue_queue[i], line);
       		} 
		else {
			printf("invalid color.\n");
		}
	}
	eof = 1;
	pthread_exit(0);
}


void * blue_consumer_thread(void * arg) {
	int i, num;
	char color[12];
	char blue[120];
	printf("\x1b[4m\x1b[1m\x1b[34m3s & 5s\x1b[0m\n");
	while(eof == 0) {
		i = 0;
                while (strlen(blue_queue[i]) != 0) {
			pthread_mutex_lock(&lock);
			sscanf(blue_queue[i], "%s %d", color, &num);
			if (((num % 3) == 0) || ((num % 5) == 0)) {
                		snprintf(blue, sizeof(blue),"\x1b[34m%s\x1b[0m", blue_queue[i]);
				printf("%s", blue);
			}
			memset(blue_queue[i], 0, sizeof(blue_queue[i]));
			pthread_mutex_unlock(&lock);
			i++;
			if (eof == 0) {
                		sleep(1);
			}
		}
	}
	b = 1;
	pthread_exit(0);
}


void * red_consumer_thread(void * arg) {
	int i, num;
	char color[12];
	char red[120];
	printf("	\x1b[4m\x1b[1m\x1b[31mprime\x1b[0m\n");
	while(eof == 0) {
		i = 0;
                while (strlen(red_queue[i]) != 0) {
			pthread_mutex_lock(&lock);
			sscanf(red_queue[i], "%s %d", color, &num);
                        if (is_prime(num) == 1) {
				snprintf(red, sizeof(red),"\x1b[31m%s\x1b[0m", red_queue[i]);
				printf("	%s", red);
                        }
			memset(red_queue[i], 0, sizeof(red_queue[i]));
			pthread_mutex_unlock(&lock);
			i++;
			if (eof == 0) {
                		sleep(1);
			}
		}
	}
	r = 1;
	pthread_exit(0);
}


void * green_consumer_thread(void * arg) {
	int i, num;
	char color[12];
	char green[120];
	printf("		\x1b[4m\x1b[1m\x1b[32mfibonacci\x1b[0m\n");
	while(eof == 0) {
		i = 0;
                while (strlen(green_queue[i]) != 0) {
			pthread_mutex_lock(&lock);
			sscanf(green_queue[i], "%s %d", color, &num);
                        if (is_fib(num) == true) {
				snprintf(green, sizeof(green),"\x1b[32m%s\x1b[0m", green_queue[i]);
				printf("		%s", green);
			}
			memset(green_queue[i], 0, sizeof(green_queue[i]));
			pthread_mutex_unlock(&lock);
			i++;
			if (eof == 0) {
				sleep(1);
			}
                }
	}
	g = 1;
	pthread_exit(0);
}


bool is_square(int x) {
	int s = sqrt(x);
	return (s*s == x);
}


bool is_fib(int n) {
    	return is_square(5*n*n + 4) || is_square(5*n*n - 4);
}


int is_prime(int num) {
	int i;
	if (num <= 1) return 0;
	if (num % 2 == 0 && num > 2) return 0;
	for(i = 3; i < num / 2; i+= 2) {
		if (num % i == 0)
             		return 0;
     	}
     	return 1;
}


int Compare(const void * p1, const void * p2) {
        int ret;
        ret = strncmp(p1, p2, 18);
        return ret;
}


static void event_handler(int sig, siginfo_t *siginfo, void *context) {
	fclose(stdin);
	printf("\nSTDIN CLOSED.");
	printf("\nClearing queue...\n\n");
	
	while(r==0 || g==0 || b==0) {
	
	}
	printf("\nBye.\n");
}

