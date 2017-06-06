#define _GNU_SOURCE
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "hashd.h"

void msg_unpack(struct msg *mbuf, char*);
void msg_free(struct msg*);

static void
copy_str(char *dst, char *src, size_t n)
{
    char *ptr;
    ptr = mempcpy(dst, src, n);
    *ptr = '\0';
}

static int
has_valid_header(struct msg *mbuf)
{
    size_t hlen, mlen;

    if (mbuf == NULL) {
       return (0);
    }

    mlen = KEYBUFSIZE + VALBFSIZE;

    /*+ 2 is for the additional '\n' in the end of both key and value*/
    if((hlen = mbuf->key_size + mbuf->value_size + 2) > mlen) {
        log_debug("Header longer than allowed");
        return (0);
    }

    return (1);
}

void
msg_unpack(struct msg *mbuf, char *buf)
{
    size_t hdrlen, offset;

    hdrlen = sizeof(mbuf->opcode) +
             sizeof(mbuf->key_size) +
             sizeof(mbuf->value_size);
    offset = hdrlen;

    memcpy(mbuf, buf, hdrlen);

    if (!has_valid_header(mbuf)) {
        log_debug("Invalid header: discading message");
        return;
    }

    /*TODO*/
    /*validate_payload_input()*/

    switch (mbuf->opcode) {
    case OPFIND:
    case OPDEL:
        copy_str(mbuf->key, buf + hdrlen, mbuf->key_size);
        break;
     case OPADD:
        copy_str(mbuf->key, buf + hdrlen, mbuf->key_size);
        offset += mbuf->key_size;
        copy_str(mbuf->value, buf + offset, mbuf->value_size);
        break;
     default:
        log_debug("Discarding invalid message");
        break;
    }
}

void
msg_free(struct msg *mbuf)
{
    free(mbuf->key);
    free(mbuf->value);
    free(mbuf);
}
    
