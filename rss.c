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

void download_feed(struct feed **rss_url){
    
    // allocate memory for rss feed.
    struct feed *feed;
    feed = malloc(sizeof(feed));
    feed->rss_url = malloc(100*sizeof(char));

    *rss_url = feed;
    CURL *curl_handle;
    static const char *pagefilename = "index.html";
    FILE *pagefile;
    FILE *fp;
 
    curl_global_init(CURL_GLOBAL_ALL);
 
    /* init the curl session */ 
    curl_handle = curl_easy_init();

    printf("URL? ");

    fscanf( stdin, "%s", feed->rss_url );  
    
    /* set URL to get here */ 
    curl_easy_setopt(curl_handle, CURLOPT_URL, feed->rss_url);
 
    /* Switch on full protocol/debug output while testing */ 
    curl_easy_setopt(curl_handle, CURLOPT_VERBOSE, 1L);
 
    /* disable progress meter, set to 0L to enable it */ 
    curl_easy_setopt(curl_handle, CURLOPT_NOPROGRESS, 1L);
 
    /* send all data to this function  */ 
    curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION, write_data);
 
    /* open the file */ 
    pagefile = fopen(pagefilename, "wb");
    if(pagefile) {
 
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
  printf("downloaded!\n");
}

void count_tags(struct feed **new_feed, FILE *fp){

    struct feed *n;
    n = malloc(sizeof(*n));
    ssize_t read;
    size_t len = 0;
    char * line;
    int amount_of_titles = 0;
    char *p;
   
    while((read = getline(&line, &len, fp)) != -1) 
    {
        n->find_titles = strstr(line, "<title>");

        if(n->find_titles)
        {
            n->amount_of_titles++;
            
        }  
    }
    *new_feed = n;
    
}


int main(void)
{   
    struct feed *nf;
    download_feed(&nf);

    ssize_t read;
    size_t len = 0;
    char * line;
    int amount_of_titles = 0;
    char *p;
    int i;
    int r = 100 , c = 256, j;
    char * n;
    
    
    char *get_real_string;

    FILE *fp;
    fp = fopen("index.html", "r");
  

    ///count titles
    count_tags(&nf, fp);
    fclose(fp);

    // open file again to get titles
    fp = fopen("index.html", "r");
    len = 0;

    //allocate list titles
    char **titles = (char **)malloc(nf->amount_of_titles * sizeof(char *));  
    for (i=0; i<nf->amount_of_titles; i++) 
        titles[i] = (char *)malloc(c * sizeof(char));

    i = 0;
    
    // put lines with <title> into string titles[i]
    while((read = getline(&n, &len, fp)) != -1) 
    {
        p = strstr(n, "<title>");
        
        if(p)
        {
            strcpy(titles[i], n);
            i++;   
        }
       
    }

    // removes tags for titles
    for(i = 0; i < nf->amount_of_titles; i++)
    {
        get_real_string = malloc(100 * sizeof(strlen(titles[i])));
        strncat (get_real_string, titles[i], strlen(titles[i])-10);
        strncpy(titles[i], get_real_string+11,strlen(titles[i]));
        printf("\n");
        printf("title %d: %s\n", i, titles[i]);

    }
    printf("\n");

    // free the stuff.
    free(nf);
    free(get_real_string);
    for (int i=0; i<amount_of_titles; i++) 
    {
        free(titles[i]);
    }   
    free(titles);
    
    exit(EXIT_SUCCESS);
} 
