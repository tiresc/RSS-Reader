struct feed {
	char *rss_url;
	char **titles;
	char **links;
	char *find_titles; //searches each line for titles
	char *find_links; //searches each line for links
	int amount_of_titles;
    int amount_of_links;
	struct rss_tags* d;
};	
struct rss_tags {

	char *title;
	char *link;
	char *description;
	char *mp3_link;
	struct Node* next;
};	

struct MemoryStruct {
  char *memory;
  char site_address[100];
  size_t size;
};