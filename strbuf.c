#include <string.h>
#include <stdlib.h>
#include "strbuf.h"
strbuf *strbuf_create() {
  strbuf *buf = malloc(sizeof(strbuf));
  if ( buf ) {
    buf->size = 16;
    buf->len = 0;
    buf->buf = malloc(buf->size);
    if (buf->buf) {
      buf->buf[0] = '\0';
    }
  }
  return buf;
}

void strbuf_delete(strbuf * buf) {
  if ( buf ) {
    free(buf->buf);
    free(buf);
  }
}


void strbuf_resize(strbuf *buf, size_t newsize) {
  void *temp = realloc(buf->buf, newsize);
  if (temp) {
    buf->size = newsize;
  } else {
    free(buf->buf);
  }
  buf->buf = temp;
}


void strbuf_addstr(strbuf *buf, const char * s) {
  const size_t len = strlen(s);
  if (buf->len + len + 1 > buf->size) {
    strbuf_resize(buf, buf->size + len);
  }
  if (buf->buf) {
    strncat(buf->buf, s, len);
    buf->len += len;
  }
}

void strbuf_addchar(strbuf *buf, char c) {
  if (buf->len == buf->size - 1) {
    strbuf_resize(buf, buf->size * 2);
  }
  if (buf->buf) {
    buf->buf[buf->len] = c;
    buf->buf[buf->len + 1] = '\0';
    buf->len++;
  }
}