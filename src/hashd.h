#include <stdarg.h>

#define KEYBUFSIZE  32
#define VALBFSIZE   512

struct msg {
    unsigned short opcode;
    unsigned short key_size;
    unsigned int value_size;
    char key[KEYBUFSIZE];
    char value[VALBFSIZE];
};

enum COMMANDS {
    OPFIND = 0xff,
    OPDEL = 0xfd,
    OPADD = 0xfe
};

struct dict {
    struct node **nodes;
    unsigned long size;
    unsigned long used;
};

struct node {
    char *key;
    char  *value;
    struct node *prev;
    struct node *next;
};

//server.c
void server_start(struct dict*, char*, volatile int*);
void server_cleanup();

/*log.c*/
void log_init(int);
void logx(int, const char*, va_list) __attribute__ ((format (printf, 2, 0)));
void log_err(const char*, ...) __attribute__ ((format (printf, 1, 2)));
void log_warn(const char*, ...) __attribute__ ((format (printf, 1, 2)));
void log_info(const char*, ...) __attribute__ ((format (printf, 1, 2)));
void log_debug(const char*, ...) __attribute__ ((format (printf, 1, 2)));
void fatal(const char*, ...) __attribute__ ((noreturn)) __attribute__ ((format (printf, 1, 2)));

//proto.c
void msg_unpack(struct msg*, char*);
void msg_free(struct msg*);

//dict.c
unsigned int hash_func(const char*, int);
struct dict* dict_new(unsigned long);
int dict_insert(struct dict*, const char*, const char*);
int dict_delete(struct dict*, const char*);
struct node* dict_find(struct dict*, const char*);

//node.c
struct node* node_new(const char*, const char*);
struct node* node_insert(struct node*, const char*, const char*);
struct node* node_delete(struct node*);

