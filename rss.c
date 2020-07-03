#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <curl/curl.h>
#include "parse.h"

void print_list(struct rss_tags* n)
{
  FILE *fp;
   fp = fopen("test.txt", "w+");
    while (n != NULL) 
    {
      printf("%s", n->title);
      fprintf(fp,"%s\n",n->title);

      printf("%s", n->link);
      fprintf(fp,"%s\n",n->link);

      printf("%s", n->desc);
      fprintf(fp,"%s\n",n->desc);

      n = n->next;
    }
    fclose(fp);
}

void append(struct rss_tags** head_ref, char *new_title, char *new_link, char *new_desc){//, char *new_link){

    struct rss_tags* new_node = malloc(sizeof(struct rss_tags));

    new_node->title = malloc(strlen(new_title) + 1);
    strcpy(new_node->title, new_title);

    new_node->link = malloc(strlen(new_link) + 1);
    strcpy(new_node->link, new_link);

    new_node->desc = malloc(strlen(new_desc) + 1);
    strcpy(new_node->desc, new_desc);

    new_node->next = NULL;

    while( *head_ref != NULL )
        head_ref = &((*head_ref)->next);   
    *head_ref = new_node;  
    
}
 
static size_t WriteMemoryCallback(void *contents, size_t size, size_t nmemb, void *userp)
{
  size_t realsize = size * nmemb;
  struct MemoryStruct *mem = (struct MemoryStruct *)userp;
 
  char *ptr = realloc(mem->memory, mem->size + realsize + 1);
  if(ptr == NULL) 
  {
    /* out of memory! */ 
    printf("not enough memory (realloc returned NULL)\n");
    return 0;
  }
 
  mem->memory = ptr;
  memcpy(&(mem->memory[mem->size]), contents, realsize);
  mem->size += realsize;
  mem->memory[mem->size] = 0;
 
  return realsize;
}

int count_links()
{
    FILE *fp;
    fp = fopen("index.html", "r");
    int amount_of_links = 0;
    ssize_t read = 0;
    size_t len = 0;
    char * line = NULL;
    char *find_titles = NULL;

    while((read = getline(&line, &len, fp)) != -1) 
    {
        find_titles = strstr(line, "<feedburner:origEnclosureLink>");

        if(find_titles)
        {
            amount_of_links++;
        }  
    }

    fclose(fp);
    free(line);
    free(find_titles);
    return amount_of_links;
}

char *download_feed()
{
  CURL *curl_handle;
  CURLcode res;
 
  struct MemoryStruct chunk;
 
  chunk.memory = malloc(1);  /* will be grown as needed by the realloc above */ 
  chunk.size = 0;    /* no data at this point */ 
 
  curl_global_init(CURL_GLOBAL_ALL);
 
  /* init the curl session */ 
  curl_handle = curl_easy_init();
 
  /* specify URL to get */ 
  curl_easy_setopt(curl_handle, CURLOPT_URL, "http://feeds.noisia.nl/NoisiaRadio");
 
  /* send all data to this function  */ 
  curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);
 
  /* we pass our 'chunk' struct to the callback function */ 
  curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA, (void *)&chunk);
 
  /* some servers don't like requests that are made without a user-agent
     field, so we provide one */ 
  curl_easy_setopt(curl_handle, CURLOPT_USERAGENT, "libcurl-agent/1.0");
 
  /* get it! */ 
  res = curl_easy_perform(curl_handle);
 
  /* check for errors */ 
  if(res != CURLE_OK) 
  {
    fprintf(stderr, "curl_easy_perform() failed: %s\n",
    curl_easy_strerror(res));
  }
  else 
  {
    return chunk.memory;
  }
         /* cleanup curl stuff */ 
  curl_easy_cleanup(curl_handle);
    /* we're done with libcurl, so clean it up */ 
  curl_global_cleanup();
    
}

int main(void)
{
  struct rss_tags* head = NULL; 
  ssize_t read = 0;
  size_t len = 0;
  char * line = NULL;
  char *find_titles = NULL;
  char *find_links = NULL;
  char *find_descriptions = NULL;
  char *site = download_feed();
  //fix_formatting(site);
  printf("%s", site);
  //char *line = NULL;
  //strcpy(line, cleansed_buffer);
  //printf("%s",line);
  int i = 0;
  
  char titles[50];

  char link[4096];
  char descriptions[5000];
  char temp[50];

  FILE *fp;
  fp = fopen("test.txt","w+");
  fprintf(fp, "%s", site);
  rewind(fp);
  


    while((read = getline(&line, &len, fp)) != -1) 
    {
        find_titles = strstr(line, "<title>");
        find_links = strstr(line, "Enclosure");
        find_descriptions = strstr(line, "<description>");
        if(find_titles)
        {
          strcpy(titles, line);
        } 
        if(find_links)
        {
          strcpy(link, line);
        } 
        if(find_descriptions)
        {
          strcpy(descriptions, line);
          append(&head, titles, link, descriptions); 
        } 
    }
  
  fclose(fp);
  //free(chunk.memory);
  print_list(head);
  rewind(fp); 

  free(find_titles);
  free(find_links);
  free(find_descriptions);
  free(line);
  free(head);
    

  return 0;  
    }

  