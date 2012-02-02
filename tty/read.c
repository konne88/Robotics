#include <unistd.h>
#include <fcntl.h>
#include <termios.h>

// stty -F /dev/ttyACM1 115200 cs8 cread clocal 
// read /dev/ttyACM1

int main(int argc, char **argv) {

    int fd;
    ssize_t len;
    const unsigned int buflen = 4096;
    unsigned char buf[buflen];
    struct termios tio;

    fd = open( argv[1], O_RDONLY | O_NOCTTY | O_NONBLOCK );
    cfmakeraw(&tio);
    cfsetispeed(&tio,B9600);
    cfsetospeed(&tio,B9600);
    tcsetattr(fd,TCSANOW,&tio);

    for(;;) {
        len = read( fd, buf, buflen );
        if( len > 0 ) write(1,buf,len);
    }

}
