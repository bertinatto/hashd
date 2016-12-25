#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <syslog.h>
#include <time.h>
#include <errno.h>

void log_init(int);
void logx(int, const char*, va_list) __attribute__ ((format (printf, 2, 0)));
void log_err(const char*, ...) __attribute__ ((format (printf, 1, 2)));
void log_warn(const char*, ...) __attribute__ ((format (printf, 1, 2)));
void log_info(const char*, ...) __attribute__ ((format (printf, 1, 2)));
void log_debug(const char*, ...) __attribute__ ((format (printf, 1, 2)));
void fatal(const char*, ...) __attribute__ ((noreturn)) __attribute__ ((format (printf, 1, 2)));

int debug;

void
log_init(int dbg)
{
    int facility;

    debug = dbg;
    facility = LOG_PID | LOG_NDELAY;

    if (debug) {
        facility |= LOG_PERROR;
    } else {
        facility |= LOG_CONS;
        tzset();
    }
    openlog(NULL, facility, LOG_DAEMON);
}

void
logx(int level, const char *msg, va_list ap)
{
    if (debug) {
        vfprintf(stderr, msg, ap);
        fprintf(stderr, "\n");
        return;
    }

    vsyslog(level, msg, ap);
}

void
log_err(const char *msg, ...)
{
    va_list ap;
    va_start(ap, msg);

    if (debug) {
        fprintf(stderr, msg, ap);
        fprintf(stderr, "\n");
    } else {
        logx(LOG_ERR, msg,  ap);
    }

    va_end(ap);
}

void
log_warn(const char *msg, ...)
{
    va_list ap;
    va_start(ap, msg);
    logx(LOG_WARNING, msg,  ap);
    va_end(ap);
}

void
log_info(const char *msg, ...)
{
    va_list ap;
    va_start(ap, msg);
    logx(LOG_INFO, msg,  ap);
    va_end(ap);
}

void
log_debug(const char *msg, ...)
{
    va_list ap;
    va_start(ap, msg);
    logx(LOG_DEBUG, msg,  ap);
    va_end(ap);
}

void
fatal(const char *msg, ...)
{
    va_list ap;
    char errmsg[BUFSIZ];
    int errsv = errno;

    va_start(ap, msg);

    logx(LOG_CRIT, msg,  ap);
    
    if (errsv) {
        snprintf(errmsg, sizeof(errmsg),  "%s", strerror(errsv));
        log_err("%s", errmsg);
    }
    
    va_end(ap);
    exit(1);
}
