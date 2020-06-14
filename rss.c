#include <stdio.h> 
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <curl/curl.h>
#include "parse.h" 

void clear_buffer(void)
{
    char c;
    while((c=getchar()) != EOF && c != '\n'){};
}

static size_t write_data(void *ptr, size_t size, size_t nmemb, void *stream)
{
  size_t written = fwrite(ptr, size, nmemb, (FILE *)stream);
  return written;
}

void print_list(struct rss_tags* n){
    while (n != NULL) {
        printf(" %s\n", n->title);
        
        n = n->next;
    }
}

void download_feed(char *save_file){
    
    size_t size=300; 
    char *rss_url;
    rss_url = malloc(size);
    memset(rss_url, 0, size);
    CURL *curl_handle;

    char *pagefilename = "index.html";
    FILE *pagefile;
    printf("URL? ");
    fscanf(stdin, "%s", rss_url);
    
    curl_global_init(CURL_GLOBAL_ALL);
 
    /* init the curl session */ 
    curl_handle = curl_easy_init();
    
    /* set URL to get here */ 
    curl_easy_setopt(curl_handle, CURLOPT_URL, rss_url);
 
    /* Switch on full protocol/debug output while testing */ 
    curl_easy_setopt(curl_handle, CURLOPT_VERBOSE, 1L);
 
    /* disable progress meter, set to 0L to enable it */ 
    curl_easy_setopt(curl_handle, CURLOPT_NOPROGRESS, 1L);
 
    /* send all data to this function  */ 
    curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION, write_data);
 
    /* open the file */ 
    pagefile = fopen(pagefilename, "wb");
    if(pagefile) 
    {
 
    /* write the page body to this file handle */ 
    curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA, pagefile);
 
    /* get it! */ 
    curl_easy_perform(curl_handle);
 
    /* close the header file */ 
    fclose(pagefile);
    }
 
  /* cleanup curl stuff */ 
        curl_easy_cleanup(curl_handle);
        curl_global_cleanup(); 
        free(rss_url);
}
    
    

void count_links(int *amount_of_links)
{
    FILE *fp;
    fp = fopen("index.html", "r");
   
    ssize_t read = 0;
    size_t len = 0;
    char * line = NULL;
    char *find_titles = NULL;

    while((read = getline(&line, &len, fp)) != -1) 
    {
        find_titles = strstr(line, "<title>");

        if(find_titles)
        {
            (*amount_of_links)++; 
        }  
    }
    free(find_titles);
    fclose(fp);
}

void append(struct rss_tags** head_ref, char *new_title){
    struct rss_tags* new_node = (struct rss_tags*) malloc(sizeof(struct rss_tags));
    struct rss_tags *last = *head_ref;
    new_node->title = new_title;
    
    new_node->next = NULL;

    if(*head_ref == NULL)
    {
        *head_ref = new_node;
        return;
    }    

    while (last->next != NULL)
        last = last->next;

    last->next = new_node;
    return;
}

int main(void)
{   
    struct rss_tags* head = NULL; 

    char *str = "index.html";
    download_feed(str);
    int amount_of_links = 0;
    ssize_t read;
    size_t len = 0;
    char * line = NULL;
    char *p;
    int i = 0;
    size_t c = 500;
    
    char * n = NULL;

    ///count titles
    count_links(&amount_of_links);
    printf("amount of links: %d\n", amount_of_links);
    
    // open file again to get titles
    FILE *fp;
    fp = fopen("index.html", "r");
    printf("file opened!");

    char **titles = (char **)malloc(amount_of_links * sizeof(char *));  
    for (i=0; i<amount_of_links; i++) 
        titles[i] = (char *)malloc(c * sizeof(char));

    i = 0;
    // put lines with <title> into string titles[i]
    while((read = getline(&n, &len, fp)) != -1) 
    {   
        p = strstr(n,"<title>");
        
        if(p)
        {    
            strcpy(titles[i], n); 
            append(&head, titles[i]);
            i++;
        }
    }

    print_list(head);
    fclose(fp);
    free(head);


    return 0;
} 
