#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

int main(void)
{
    int fd, ret;
    char rtc_data[19];

    printf("Press \"ENTER\" to get RTC data form device\n");
    getchar();
    while(1)
    {
        fd = open("/dev/ttyACM1", O_RDONLY);
        if(fd == -1)
        {
            printf("Failed to open device file..!\n");
            return(-1);
        }
        
        ret = read(fd, rtc_data, sizeof(rtc_data));
        if(ret != 0)
        {
            printf("RTC data: ");
            printf("%s", rtc_data);
        }
        printf("\n");
        close(fd);
    }
   

    return(0);
}
