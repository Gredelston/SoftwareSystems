#include <errno.h>
#include <termios.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

 #include <stdio.h>   /* Standard input/output definitions */
 #include <fcntl.h>   /* File control definitions */


int
set_interface_attribs (int fd, int speed, int parity)
{
        struct termios tty;
        memset (&tty, 0, sizeof tty);
        if (tcgetattr (fd, &tty) != 0)
        {
                error("error %d from tcgetattr", errno);
                return -1;
        }

        cfsetospeed (&tty, speed);
        cfsetispeed (&tty, speed);

        tty.c_cflag = (tty.c_cflag & ~CSIZE) | CS8;     // 8-bit chars
        // disable IGNBRK for mismatched speed tests; otherwise receive break
        // as \000 chars
        tty.c_iflag &= ~IGNBRK;         // ignore break signal
        tty.c_lflag = 0;                // no signaling chars, no echo,
                                        // no canonical processing
        tty.c_oflag = 0;                // no remapping, no delays
        tty.c_cc[VMIN]  = 0;            // read doesn't block
        tty.c_cc[VTIME] = 5;            // 0.5 seconds read timeout

        tty.c_iflag &= ~(IXON | IXOFF | IXANY); // shut off xon/xoff ctrl

        tty.c_cflag |= (CLOCAL | CREAD);// ignore modem controls,
                                        // enable reading
        tty.c_cflag &= ~(PARENB | PARODD);      // shut off parity
        tty.c_cflag |= parity;
        tty.c_cflag &= ~CSTOPB;
        tty.c_cflag &= ~CRTSCTS;

        if (tcsetattr (fd, TCSANOW, &tty) != 0)
        {
                error ("error %d from tcsetattr", errno);
                return -1;
        }
        return 0;
}

void
set_blocking (int fd, int should_block)
{
        struct termios tty;
        memset (&tty, 0, sizeof tty);
        if (tcgetattr (fd, &tty) != 0)
        {
                error("error %d from tggetattr", errno);
                return;
        }

        tty.c_cc[VMIN]  = should_block ? 1 : 0;
        tty.c_cc[VTIME] = 5;            // 0.5 seconds read timeout

        if (tcsetattr (fd, TCSANOW, &tty) != 0)
                error ("error %d setting term attributes", errno);
}


int processTime(char *time_char){
  // if time sync available from serial port, update time and return true   
      time_t int_time = 0;
      int i;
      for(i = 0; i < 10; i++){            
        if( *time_char>= '0' && *time_char <= '9'){  
          int_time = (10 * int_time) + (*time_char - '0') ; // convert digits to a number    
        }
      }
      return int_time;
 }


char *portname = "/dev/tty";

int serialInitialization(){

    int fd = open (portname, O_RDWR | O_NOCTTY | O_SYNC);
    if (fd < 0)
    {
        error ("error %d opening %s: %s", errno, portname, strerror (errno));
        return -1;
    }

    set_interface_attribs (fd, B115200, 0);  // set speed to 115,200 bps, 8n1 (no parity)
    set_blocking (fd, 0);                // set no blocking
    return fd;
}

int main(){

    int fd = serialInitialization();

    if (fd == -1) {
        return;
    }

    FILE *f = fopen("test.txt", "w");
    if (f == NULL)
    {
        printf("Error opening file!\n");
        exit(1);
    }

    //creates string similar to what is coming from arduino
    //time_t current_time = time(NULL);  //1399402008
    //char time_str[11];
    //sprintf(time_str, "T%ld\n", current_time);
    
    //ssize_t length = write (fd, time_str, 11); 
    //sprintf(time_str, "T %ld \n", current_time);
    //ssize_t length = write (fd, "hello", 11); 
    //unsigned char cmd[] = {'I', 'N', 'I', 'T', ' ', '\r', '\0'};
    //int n_written = write( fd, cmd, sizeof(cmd) -1 );

    int flag = 0;
    char *time_str = (char *)malloc(sizeof(char)*11);
 
    while(1)
    {
        char buffer[11];
        ssize_t length = read(fd, buffer, sizeof(buffer));
        
        if (length == -1)
        {
            printf("Error reading from serial port\n");

        }
        if(length > 0)
        {
            buffer[length] = '\0';
            if (*buffer == 'q'){
                break;
            }

            if (flag == 1){
                time_str = strcat(time_str,buffer);
                if(strlen(time_str) == 11){
                    printf("\nfound time: %s \r", time_str);
                    fprintf(f, "time: %s", time_str);
                    time_str[0] = '\0';
                    flag = 0;
                }
            }
            if (*buffer == 't'){
                flag = 1;
            }
        }
    }
}