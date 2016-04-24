#define BUFSZ          4096  
#define SERVER_PORT    8080
#define MAXCONNS       32
#define BACKLOG        16
#define BAD_REQUEST    "400: Bad Request"
#define NOT_FOUND      "404: Not Found"
#define NOT_ALLOWED    "405: Method Not Allowed"
#define INTERNAL_ERROR "500: Internal Server Error"

void srvsend(const char *name, size_t size, int sockfd);