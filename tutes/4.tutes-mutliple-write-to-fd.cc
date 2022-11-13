#include <iostream>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <fcntl.h>
#include <sys/epoll.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <memory.h>
#include <sys/uio.h>

int pipes[2];

#define BUFFER_SIZE 1024


int main()
{
    char file_name[] = "tmp/a.txt";

    char header_buf[BUFFER_SIZE];
    memset(header_buf, 0, BUFFER_SIZE);

    char* file_buf;
    struct stat file_stat;
    bool valid = true;
    
    int len = 0;
    if (stat(file_name, &file_stat) < 0) {
        valid = false;
        printf("Check file %s\n", file_name);
    }
    else if (S_ISDIR(file_stat.st_mode)) {
        valid = false;
        printf("Check is dir %d\n", file_stat.st_mode);
    }
    else if (file_stat.st_mode & S_IROTH) {
        int fd = open(file_name, O_RDONLY);
        file_buf = new char[file_stat.st_size+1];
        memset(file_buf, 0, file_stat.st_size + 1);
        read(fd, file_buf, file_stat.st_size);
        printf("Check file %s\n", file_buf);
    }

    int ret;
    if (valid) {
        ret = snprintf(header_buf, BUFFER_SIZE-1, "%s %s\r\n", "HTTP/1.1", "OK");
        len += ret;
        ret = snprintf(header_buf+len, BUFFER_SIZE-1-len, "Content-Length: %ld\r\n",file_stat.st_size);
        len += ret;
        ret = snprintf(header_buf+len, BUFFER_SIZE-1-len, "%s", "\r\n");

        struct iovec iv[2];
        iv[0].iov_base = header_buf;
        iv[0].iov_len = strlen(header_buf);
        iv[1].iov_base = file_buf;
        iv[1].iov_len = file_stat.st_size;
        ret = writev(STDOUT_FILENO, iv, 2);
    }

    return 0;
}