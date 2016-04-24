all: websrv

websrv: websrv.c
	gcc -std=c99 -Wall \
		http-parser/http_parser.c \
		strbuf.c \
		websrv.c \
		-o websrv
	
clean:
	rm websrv