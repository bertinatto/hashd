#define _GNU_SOURCE
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

void
msg_unpack(struct msg *mbuf, char *buf)
{
    size_t hdrlen, offset;

    hdrlen = sizeof(mbuf->opcode) +
             sizeof(mbuf->key_size) +
             sizeof(mbuf->value_size);
    memcpy(mbuf, buf, hdrlen);
    offset = hdrlen;

    /*TODO*/
    /*validate_header_input()*/
    /*validate_buf_size_sent_by_client()*/
    /*validate_payload_input()*/

    switch (mbuf->opcode) {
    case OPFIND:
    case OPDEL:
        copy_str(mbuf->key, buf + offset, sizeof(mbuf->key));
     case OPADD:
        copy_str(mbuf->key, buf + hdrlen, mbuf->key_size);
        offset += mbuf->key_size;
        copy_str(mbuf->value, buf + offset, mbuf->value_size);
     default:
        log_debug("Discarding invalid message");
    }
}

void
msg_free(struct msg *mbuf)
{
    free(mbuf->key);
    free(mbuf->value);
    free(mbuf);
}
    
