#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/un.h>
#include <netdb.h>
#include <ev.h>

#include "hashd.h"

void server_start(struct dict*, char*, volatile int*);
void server_cleanup();

struct dict *table;
char *port;

static int
server_init(char *port)
{
    int status;
    int yes = 1;
	int sfd = -1;
    struct addrinfo hints;
    struct addrinfo *servinfo, *sp;
    
	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_UNSPEC;     /*IPV4 and IPV6*/
	hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;     /*Fill in my IP*/

	if ((status = getaddrinfo(NULL, port, &hints, &servinfo)) != 0) {
		log_err("getaddrinfo: %s", gai_strerror(status));
        return (-1);
	}

	for (sp = servinfo; sp != NULL; sp = sp->ai_next) {
		if ((sfd = socket(sp->ai_family, sp->ai_socktype, sp->ai_protocol)) < 0) {
            continue;
        }
        if (setsockopt(sfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes)) == 0) {
            if (bind(sfd, sp->ai_addr, sp->ai_addrlen) == 0) {
                break;
            }
        }
        close(sfd);
	}

    if (sp == NULL) {
        log_err("server_init: could not bind");
    }
    
    freeaddrinfo(servinfo);
    return (sfd);
}

static int
server_listen(int sfd)
{
    if ((sfd != -1) && (listen(sfd, SOMAXCONN)) == -1) {
		return (-1);
    }
	
    return (0);
}

static void
server_dispatch_cb(struct ev_loop *loop, struct ev_io *watcher, int revents)
{
    char rbuf[KEYBUFSIZE + VALBFSIZE];
    ssize_t size;
    struct msg mbuf;
    struct node *np;

    if(EV_ERROR & revents) {
        log_err("Invalid event");
        return;
    }

    size = recv(watcher->fd, rbuf, sizeof(rbuf), 0);

    if(size > 0) {

        msg_unpack(&mbuf, rbuf);

        switch (mbuf.opcode) {
        case OPADD:
            dict_insert(table, mbuf.key, mbuf.value);
            break;
        case OPDEL:
            dict_delete(table, mbuf.key);
            break;
        case OPFIND:
            if ((np = dict_find(table, mbuf.key)) != NULL) {
                fprintf(stderr, "found: %s\t->\t%s\n", np->key, np->value);
            }
            break;
        default:
            fprintf(stderr, "Invalid\n");
            break;
        }


    } else if(size < 0) {
        log_err("Read error");
    } else {
        ev_io_stop(loop, watcher);
        close(watcher->fd);
        free(watcher);
    }
}

static void
server_accept_cb(struct ev_loop *loop, struct ev_io *watcher, int revents)
{
	int			 sfd;
	socklen_t		 len;
	struct sockaddr_un	 sun;
	struct ev_io	*w_client;

    /*event_add(&cs->cs_ev, NULL);*/
    /*if ((event & EV_TIMEOUT))*/
        /*return;*/

	len = sizeof(sun);
    if ((sfd = accept4(watcher->fd, (struct sockaddr *)&sun, &len, SOCK_NONBLOCK)) == -1) {
        log_err("%s: accept", __func__);
		return;
	}

    if ((w_client = calloc(1, sizeof(*w_client))) == NULL) {
        log_err("%s: calloc", __func__);
        close(sfd);
        return;
    }
    ev_io_init(w_client, server_dispatch_cb, sfd, EV_READ);
    ev_io_start(loop, w_client); 
}

void
server_start(struct dict *t, char *port, volatile int *quit)
{
    int sfd;
    struct ev_loop *loop;
    ev_io *watcher;
    
    loop = EV_DEFAULT;

    table = t;

    if ((watcher = calloc(1, sizeof(*watcher))) == NULL) {
        fatal("%s: calloc", "__func__");
    }

    if ((sfd = server_init(port)) < 0) {
        fatal("Server initialization failed");
    }

	if (server_listen(sfd) == -1) {
        fatal("Server socket listen failed");
    }
	
    /*TODO: check if these functions return something*/
    ev_io_init(watcher, server_accept_cb, sfd, EV_READ);
    ev_io_start(loop, watcher);

    while(!*quit) {
        ev_run (loop, EVRUN_ONCE);
    }

    free(watcher);
}

void
server_cleanup(void)
{
    /*TODO*/
}

