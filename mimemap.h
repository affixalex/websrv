typedef struct {
    char *extension;
    char *mimetype;
} mimemap;

mimemap mimetypes[] = {
    {"html", "text/html"},
    {"htm",  "text/html"},
    {"xml",  "application/xml"},
    {"txt",  "text/plain"},
    {"gif",  "image/gif"},
    {"jpg",  "image/jpeg"},
    {"png",  "image/png"},
    // Could add moar here
};