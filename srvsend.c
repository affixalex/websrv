#include <stdio.h>
#include "srvsend.h"
#include "strbuf.h"

void srvsend(const char *name, size_t size, int sockfd) {
    strbuf *headers = strbuf_create();
    char sizebuf[128];
    FILE *fptr;

    /* Send the headers */
    strbuf_addstring(headers, "HTTP/1.1 200: OK\n");
    sprintf(sizebuf, "Content-Length: %ld\n", size);
    strbuf_addstring(headers, sizebuf);
    add_mimetype(name, headers);
    strbuf_addchar(headers, '\n');
    if (!headers->buf) {
        send_message(INTERNAL_SERVER_ERROR, sockfd);
        return;
    }
    send(sock, headers->buf, headers->len, 0);
    strbuf_delete(headers);

    printf("Sending file %s\n", name);
    if ((fptr = fopen(name, "rb")) != NULL) {
        char buf[BUFSZ];
        const unsigned int size = sizeof(buf);
        int bytes;
        while ((bytes = fread(buf, 1, size, fptr)) > 0) {
            send(sockfd, buf, bytes, 0);
        }
        fclose(fptr);
    }
    else {
        send_message(NOT_FOUND, sockfd);
    }
}