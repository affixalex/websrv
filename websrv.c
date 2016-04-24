#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <getopt.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#include "strbuf.h"
#include "srvsend.h"
#include "mimemap.h"
#include "http-parser/http_parser.h"

int main(int argc, char *argv[]) {
  int sockfd = 0, slot = 0, c = 0;
  int clients[MAXCONNS];  
  //  char input[4096], output[512];
  socklen_t sin_size;
  //  pid_t pid;
  static http_parser parser; // request
  static http_parser_settings settings;
  struct sockaddr_in serveraddr;
  struct sockaddr_in clientaddr;

  while (( c = getopt(argc, argv, "h" )) != -1) {
      switch (c) {
      case 'h':
          printf("Usage: %s [/path/to/root/dir]\n", argv[0]);
          printf("Serves files from $PWD by default.\n");
          exit(-1);
      default:
          printf("Run %s --h for usage information.\n", argv[0]);
          exit(-1);
      }
  }

  sockfd = socket(AF_INET, SOCK_STREAM, 0);
  if ( sockfd == -1 ) {
    fprintf(stderr, "socket() failed: %s\n", strerror(errno));
    exit(-1);
  }

  bzero(&serveraddr, sizeof(serveraddr));
  serveraddr.sin_family = AF_INET;
  serveraddr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
  serveraddr.sin_port = htons(SERVER_PORT);
  
  if ( bind(sockfd, (struct sockaddr *)&serveraddr, sizeof(serveraddr)) < 0 ) {
    fprintf(stderr, "bind(sockfd) failed: %s\n", strerror(errno));
    exit(-1);
  }

  if ( listen(sockfd, BACKLOG) < 0 ) {
    fprintf(stderr, "listen(sockfd) failed: %s\n", strerror(errno));
  }
  
  for ( int i = 0; i < MAXCONNS; i++ ) {
    clients[i] = -1;
  }
  
  settings.on_url = NULL;
  
  while(1) {
    sin_size = sizeof(struct sockaddr_in);

    clients[slot] = accept(sockfd, (struct sockaddr *)&clientaddr, &sin_size);
    
    if ( clients[slot] == -1 ) {
      fprintf(stderr, "accept(sockfd) failed: %s\n", strerror(errno));
      exit(-1);
    }

    if ( fork() == 0 ) {

      if ( close(sockfd) == -1 ) {
        fprintf(stderr, "close(sockfd) failed: %s\n", strerror(errno));
        exit(-1);
      }

      char *req;
      
      http_parser_init(&parser, HTTP_REQUEST);
      size_t nparsed = http_parser_execute(&parser, &settings, req, strlen(req));

      printf("Doing stuff!\n");

      exit(0);
    }

    if ( close(clients[slot]) == -1 ) {
      fprintf(stderr, "close(conn) failed: %s\n", strerror(errno));
      exit(-1);
    }

    while ( clients[slot] != -1 ) {
      slot = (slot+1) % MAXCONNS;
    }
      
    waitpid(-1, NULL, WNOHANG);
  }

  return -1; // Mission: Impossible!
}