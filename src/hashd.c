#include <unistd.h>
#include <signal.h>

#include "hashd.h"

#define DEFAULT_PORT "9999"

void sighdr(int);
void usage(void) __attribute__ ((noreturn));

int verbosity = 0;
int debug = 0;
struct dict *table;

volatile sig_atomic_t quit = 0;

void
usage(void)
{
	extern char	*__progname;

	fatal("Usage: %s [-v]\n", __progname);
}

void
sighdr(int sig)
{
	switch (sig) {
	case SIGTERM:
	case SIGINT:
		quit = 1;
		break;
	}
}

int
main(int argc, char *argv[])
{
    int ch;
    char *port = DEFAULT_PORT;
    while ((ch = getopt(argc, argv, "p:vd")) != -1) {
        switch (ch) {
        case 'd':
            debug = 1;
            break;
        case 'v':
            verbosity++;
            break;
        case 'p':
            port = optarg;
            break;
        default:
            usage();
        }
    }
	signal(SIGTERM, sighdr);
	signal(SIGINT, sighdr);
    table = dict_new(1000);
    log_init(verbosity);
    server_start(table, port, &quit);
    server_cleanup();
    return 0;
}

/*TODO:*/
/*log.c similar to ntpd*/ //DONE
/*prototypes*/
/*check necesssary free() calls | valgrind*/

/*length checking in proto*/
/*input validation*/
/*errno*/
/*unit tests*/
/*review log*/
/*README*/
/*struct hashd_conf*/
/*sighup*/
/*release!*/
/*grow dict*/
/*fuzz testing*/

///////////////////
/*save to file*/
/*privilege separation*/
