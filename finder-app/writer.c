#include <syslog.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

// View syslog output using `journalctl -t writer`
int main(int argc, char *argv[]){
    int fd;
    ssize_t bytes_written;
    size_t len;
    
    openlog("writer", LOG_PID, LOG_USER);
    if (argc != 3) {
        syslog(LOG_ERR,
               "Invalid number of arguments: expected 2, got %d",
               argc - 1);
        closelog();
        return 1;
    }

    const char *write_file = argv[1];
    const char *write_str = argv[2];

    syslog(LOG_DEBUG,
           "Writing \"%s\" to \"%s\"",
           write_file,
           write_str);
    fd = open(write_file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd == -1) {
        syslog(LOG_ERR,
               "Failed to open file %s",
               write_file);
        closelog();
        return 1;
    }

    len = strlen(write_str);

    bytes_written = write(fd, write_str, len);
    if (bytes_written != (ssize_t)len) {
        syslog(LOG_ERR,
               "Failed to write to file %s",
               write_file);
        close(fd);
        closelog();
        return 1;
    }

    close(fd);
    closelog();

    return 0;
}