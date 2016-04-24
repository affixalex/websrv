// A string that can grow with us!
typedef struct {
  char *buf;
  size_t size;
  size_t len;
} strbuf;

strbuf *strbuf_create();
void strbuf_delete(strbuf * buf);
void strbuf_resize(strbuf *buf, size_t newsize);
void strbuf_addstr(strbuf *buf, const char * s);
void strbuf_addchar(strbuf *buf, char c);