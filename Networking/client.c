#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>
#include <assert.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define MAX_VAL 4096

int32_t read_full(int fd, char* buf, int n) {
    while (n > 0) {
        ssize_t readValue = read(fd, buf, n);
        if (readValue <= 0) {
            return -1;
        }
        assert((size_t)readValue <= (size_t)n);
        n -= (size_t)readValue;
        buf += readValue;
    }
    return 0;
}

int32_t write_full(int fd, char* buf, int n) {
    while (n > 0) {
        ssize_t writeValue = write(fd, buf, n);
        if (writeValue <= 0) {
            return -1;
        }
        assert((size_t)writeValue <= (size_t)n);
        n -= (size_t)writeValue;
        buf += writeValue;
    }
    return 0;
}

int32_t query(int s2, const char* text) {
    int32_t len = (int32_t)strlen(text);
    if (len > MAX_VAL) {
        printf("Too large\n");
        return -1;
    }
    
    char wbuf[4 + MAX_VAL];
    memcpy(wbuf, &len, 4);  // little endian
    memcpy(&wbuf[4], text, len);
    if (write_full(s2, wbuf, 4 + len) < 0) {
        perror("write_full failed");
        return -1;
    }


    // Read response length
    int32_t rlen;
    if (read_full(s2, (char*)&rlen, 4) < 0) {
        perror("read_full length failed");
        return -1;
    }
    printf("%d\n",rlen);
    // Read actual response
    char rbuf[MAX_VAL + 1];
    if (read_full(s2, rbuf, rlen) < 0) {
        perror("read_full body failed");
        return -1;
    }

    printf("1\n");
    rbuf[rlen] = '\0';
    printf("Response: %s\n", rbuf);

    return 0;
}

int main() {
    int fd = socket(AF_INET, SOCK_STREAM, 0);
    if (fd < 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    struct sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(12048);
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    if (connect(fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        perror("Connection failed");
        close(fd);
        exit(EXIT_FAILURE);
    }

    // Send multiple queries
    while (1) {
        char val[16384];
        if (!fgets(val, sizeof(val), stdin)) break;

        // Strip newline
        size_t len = strlen(val);
        if (len > 0 && val[len - 1] == '\n') {
            val[len - 1] = '\0';
        }
        if (strlen(val) == 0) continue;  // skip empty lines

        int32_t err = query(fd, val);
        if (err) {
            break;
        }
    }
    close(fd);
    return 0;
}
