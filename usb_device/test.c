#include <stdio.h>
#include <fcntl.h> // open close read write operation
#include <unistd.h>

int main()
{
    char buf[64];
    int fd, ret;

    printf("------------Test Application------------\n");
    printf("Enter signal: \nLED ON -> enter 1 \nLED OFF -> enter 0 \n");
    scanf("%s", buf);

    fd = open("/dev/ttyACM1", O_RDWR);
    if(fd == -1)
    {
        printf("Failed to open device file..!\n");
        return 0;
    }
    
    ret = write(fd, buf, sizeof(buf));
    if(ret == -1)
    {
        printf("Failed to write on device file..!\n");
        return 0;
    }

    ret = read(fd, buf, sizeof(buf));
    if(ret == -1)
    {
        printf("Failed to read from device file..!\n");
        return 0;
    }
    printf("%s", buf);

    ret = close(fd);
    if(ret == -1)
    {
        printf("Failed to close device file..!\n");
        return 0;
    }
    return 0;
}