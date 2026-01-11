#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <linux/fs.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>

int is_all_zero(void *buf, size_t n){
    unsigned char *p = buf;
    for(size_t i=0;i<n;i++) if(p[i]) return 0;
    return 1;
}

int main(int argc,char **argv){
    const char *dev = "/dev/sdb"; // anpassen
    int fd = open(dev, O_RDONLY | O_DIRECT);
    if(fd<0){ perror("open"); return 1; }
    unsigned long long bytes=0;
    ioctl(fd, BLKGETSIZE64, &bytes); // Größe in Bytes
    int blksz=512;
    ioctl(fd, BLKSSZGET, &blksz);    // Sektorgröße
    size_t chunk = 1024*1024; // 1MiB, muss aligned sein
    void *buf;
    if(posix_memalign(&buf, 4096, chunk)){ perror("memalign"); return 1; }
    for(unsigned long long off=0; off<bytes; off+=chunk){
        ssize_t r = pread(fd, buf, chunk, off);
        if(r<=0) break;
        if(!is_all_zero(buf, r)){
            printf("Nonzero at offset 0x%llx len %zd\n", off, r);
        }
    }
    free(buf); close(fd);
    return 0;
}
