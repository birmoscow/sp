#define _GNU_SOURCE

#include <stdio.h>
#include <sys/socket.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <netdb.h>

static const char ARG_SERVER[] = "--server";
static const char ARG_CLIENT[] = "--client";
static const char ARG_HELP[] = "--help";
static const char EXIT[] = "exit()";
static char IP[20];
static unsigned PORT;
static int SOCKET;
static char MODE;

enum CONSTS
{
    MAX_LEN = 100,
};


void
startTCPServer(struct sockaddr_in addr)
{

    SOCKET = socket(AF_INET, SOCK_STREAM, 0);
    if (bind(SOCKET, &addr, sizeof(addr))) {
        fprintf(stderr, "[file: %s][line %d] Error: %s\n", __FILE__, __LINE__, strerror(errno));
        exit(1);
    }

    printf("SOCKET - OK\n");
    fflush(stdout);

    if (listen(SOCKET, 1) != 0) {
        fprintf(stderr, "[file: %s][line %d] Error: %s\n", __FILE__, __LINE__, strerror(errno));
        exit(1);
    }

    int isActive = 1;
    ssize_t msgLen;
    char msg[MAX_LEN + 1] = {0};
    while (isActive) {
        int curClient = accept(SOCKET, NULL, 0);
        if (send(curClient, "Hi!\n", 5, 0) < 0) {
            fprintf(stderr, "[file: %s][line %d] Error: %s\n", __FILE__, __LINE__, strerror(errno));
            exit(1);
        }
        while ((msgLen = recv(curClient, msg, MAX_LEN, 0))) {
            if (msgLen < 0) {
                fprintf(stderr, "[file: %s][line %d] Error: %s\n", __FILE__, __LINE__, strerror(errno));
                exit(1);
            }
            printf("Msg : %s\n\n", msg);
            printf("Your answer or write 'exit()' to exit: ");
            fflush(stdout);
            for (int i = 0; i < MAX_LEN + 1; ++i) {
                msg[i] = 0;
            }
            scanf("%s", msg);

            if (strncmp(msg, EXIT, sizeof(EXIT) + 1) == 0) {
                close(curClient);
                break;
            }
            if (send(curClient, msg, strnlen(msg, MAX_LEN), 0) < 0) {
                fprintf(stderr, "[file: %s][line %d] Error: %s\n", __FILE__, __LINE__, strerror(errno));
                exit(1);
            }
        }

        char bufChar;
        int flag = 1;
        while (flag) {
            printf("Exit? (y/n) ? :");
            fflush(stdout);
            scanf("%c", &bufChar);
            if (bufChar == 'y') {
                isActive = 0;
                flag = 0;
            } else if (bufChar == 'n') {
                flag = 0;
            }
        }
    }
}


void
startTCPClient(struct sockaddr_in addr)
{


    SOCKET = socket(AF_INET, SOCK_STREAM, 0);
    char msg[MAX_LEN + 1] = {0};
    ssize_t msgLen;
    if (connect(SOCKET, &addr, sizeof(addr)) < 0) {
        fprintf(stderr, "[file: %s][line %d] Error: %s\n", __FILE__, __LINE__, strerror(errno));
        exit(1);
    }

    while ((msgLen = recv(SOCKET, msg, MAX_LEN, 0))) {
        if (msgLen < 0) {
            fprintf(stderr, "[file: %s][line %d] Error: %s\n", __FILE__, __LINE__, strerror(errno));
            exit(1);
        }
        printf("Msg : %s\n\n", msg);
        printf("Your answer or write 'exit()' to exit: ");
        fflush(stdout);
        for (int i = 0; i < MAX_LEN + 1; ++i) {
            msg[i] = 0;
        }
        scanf("%s", msg);

        if (strncmp(msg, EXIT, sizeof(EXIT)) == 0) {
            break;
        }
        if (send(SOCKET, msg, strnlen(msg, MAX_LEN), 0) < 0) {
            fprintf(stderr, "[file: %s][line %d] Error: %s\n", __FILE__, __LINE__, strerror(errno));
            exit(1);
        }
    }

}


int
main(int argc, char **argv)
{
    fprintf(stderr, "HELLO WORLD!\n");

    errno = 0;
    if (argc < 3) {
        fprintf(stderr, "[file %s][line %d] Error: incorrect mode\n", __FILE__, __LINE__);
        exit(1);
    } else if (strncmp(argv[1], ARG_SERVER, sizeof(ARG_SERVER)) == 0) {
        MODE = 's';
    } else if (strncmp(argv[1], ARG_CLIENT, sizeof(ARG_CLIENT)) == 0) {
        MODE = 'c';
    } else if (strncmp(argv[1], ARG_HELP, sizeof(ARG_HELP)) == 0) {
        printf("--server for server mode\n"
               "--client for client mode\n");
        exit(0);
    } else {
        fprintf(stderr, "[file: %s][line %d] Error: incorrect mode\n", __FILE__, __LINE__);
        exit(1);
    }
    fflush(stderr);


    struct sockaddr_in addr;
    struct hostent *hp = gethostbyname(argv[2]);
    if (!hp) {
        fprintf(stderr, "[file: %s][line %d] %s\n", __FILE__, __LINE__, strerror(errno));
        exit(1);
    }

    bcopy(hp->h_addr, &addr.sin_addr, hp->h_length);
    addr.sin_family = AF_INET;
    addr.sin_port = htons(8888);



    fprintf(stderr, "MY IP IS: %x\n... AND PORT IS 8888\n", ntohl(addr.sin_addr.s_addr));

    if (MODE == 's') {
        startTCPServer(addr);
    } else {
        startTCPClient(addr);
    }

    close(SOCKET);
    exit(0);
}
