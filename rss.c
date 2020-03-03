#include <stdio.h>
#include <stdlib.h>
#include <string.h>


void count_links(long *count, FILE *fp){
	char test[1000];

	while(fscanf(fp, "%s", test) == 1){
		if(test[0] == '<' && test[1] == 'l'){
			(*count)++;
		}
	}
}

int main(void) {
	
	FILE *fp;
	char links[10000];
	int i =0;
	int j = 0;
	long count = 0;

	char link[1000][1000];

	//open downloaded file
	fp = fopen("index.html", "r");

	//count number of lines with <link> at beginning
	count_links(&count, fp);
	
	char *plinks;
	plinks = (char *) malloc(count * sizeof(char));
	while(fscanf(fp, "%s", links) == 1){
		if(links[0] == '<' && links[1] == 'l' && links[2] == 'i'){
			links[0] = ' ';
			for(int i = 0; i < 295; i++){
				 /*for(int j = 5; j > 0; j--){
					*(*(link+i)+j) = ' ';
				}*/
			}

			strncat(link[i], links,1000);
			i++;
		}
		puts(*link);
	}

	printf("Links: %d\n", count);			
	return 0;
}