#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <sys/stat.h>
#include <unistd.h>

int ps5_posix_fallocate(int fd, off_t offset, off_t length)
{
    if (offset < 0 || length <= 0)
        return EINVAL;
    struct stat status;
    if (fstat(fd, &status) != 0)
        return errno;
    if (!S_ISREG(status.st_mode))
        return ENODEV;
    const off_t end = offset + length;
    if (end < offset)
        return EFBIG;

    static const char zeros[16384];
    for (off_t at = status.st_size; at < end;) {
        const size_t piece = (size_t)(end - at < (off_t)sizeof(zeros) ? end - at : (off_t)sizeof(zeros));
        const ssize_t written = pwrite(fd, zeros, piece, at);
        if (written < 0) {
            if (errno == EINTR)
                continue;
            return errno;
        }
        at += written;
    }
    return 0;
}
