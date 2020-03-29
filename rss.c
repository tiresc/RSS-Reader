/***************************************************************************
 *                                  _   _ ____  _
 *  Project                     ___| | | |  _ \| |
 *                             / __| | | | |_) | |
 *                            | (__| |_| |  _ <| |___
 *                             \___|\___/|_| \_\_____|
 *
 * Copyright (C) 1998 - 2019, Daniel Stenberg, <daniel@haxx.se>, et al.
 *
 * This software is licensed as described in the file COPYING, which
 * you should have received as part of this distribution. The terms
 * are also available at https://curl.haxx.se/docs/copyright.html.
 *
 * You may opt to use, copy, modify, merge, publish, distribute and/or sell
 * copies of the Software, and permit persons to whom the Software is
 * furnished to do so, under the terms of the COPYING file.
 *
 * This software is distributed on an "AS IS" basis, WITHOUT WARRANTY OF ANY
 * KIND, either express or implied.
 *
 ***************************************************************************/ 
/* <DESC>
 * Download a given URL into a local file named page.out.
 * </DESC>
 */ 
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <curl/curl.h>
void count_links(long *count,FILE *fp ){
  char *test;

  while(fscanf(fp, "%s", test) == 1){
    if(test[0] == '<' && test[1] == 'l'){
      (*count)++;
    }
  }
}

struct MemoryStruct {
  char *memory;
  char site_address[100];
  size_t size;

};
static size_t write_data(void *ptr, size_t size, size_t nmemb, void *stream)
{
  size_t written = fwrite(ptr, size, nmemb, (FILE *)stream);
  return written;
}


 
int main(int argc, char *argv[])
{
  CURL *curl_handle;
  static const char *pagefilename = "index.html";
  FILE *pagefile;
  FILE *fp;
 
  /*if(argc < 2) {
    printf("Usage: %s <URL>\n", argv[0]);
    return 1;
  }*/
 
  curl_global_init(CURL_GLOBAL_ALL);
 
  /* init the curl session */ 
  curl_handle = curl_easy_init();

  printf("URL? ");
  char site_url[50];

  fscanf( stdin, "%s", site_url );  
 
  /* set URL to get here */ 
  curl_easy_setopt(curl_handle, CURLOPT_URL, site_url);
 
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
  long count = 0;
  //open downloaded file
  fp = fopen("index.html", "r");
  count_links(&count, fp);
  printf("links: %d\n", count);
  free(fp);
}
  /*int i = 0;
  int j = 0;
  */

  //count number of lines with <link> at beginning
  /*char *plinks;
  plinks = malloc(count * sizeof(char));
  while(fscanf(fp, "%s", links) == 1){
    if(links[0] == '<' && links[1] == 'l' && links[2] == 'i'){
      links[0] = ' ';
      for(int i = 0; i < 295; i++){
         //for(int j = 5; j > 0; j--){
          //*(*(link+i)+j) = ' ';
        //}
      
      //strncat(link[i], links,1000);
      //i++;
    }
    puts(*link);
  }

  printf("Links: %d\n", count);   
 
  return 0;
}}*/
