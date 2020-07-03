struct rss_tags {
	char *title;
	char *link;
	char *desc;
	char *mp3_link;
	struct rss_tags* next;
};	

struct MemoryStruct {
  char *memory;
  char site_address[100];
  size_t size;
};