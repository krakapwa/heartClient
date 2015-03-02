#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <bluetooth/bluetooth.h>
#include <bluetooth/rfcomm.h>
#include <time.h>

const int msgSize = 256;

int main(int argc, char **argv)
{
    struct sockaddr_rc addr = { 0 };
    int s, status;
    char dest[18] = "00:02:72:C9:1B:25";
    char* msg;

    // allocate a socket
    s = socket(AF_BLUETOOTH, SOCK_STREAM, BTPROTO_RFCOMM);

    // set the connection parameters (who to connect to)
    addr.rc_family = AF_BLUETOOTH;
    addr.rc_channel = (uint8_t) 1;
    str2ba( dest, &addr.rc_bdaddr );

    // connect to server
    status = connect(s, (struct sockaddr *)&addr, sizeof(addr));

    time_t thisTime;
    struct tm *info;
    char tFormat[] = "%d-%m-%y_%H-%M-%S";
    char timeBuffer[80];

    // send a message
    if( status == 0 ) {
	strncpy(msg, "clock",msgSize);
        status = write(s, msg, msgSize);
	time(&thisTime);
	info = localtime(&thisTime);
	strftime(timeBuffer,80,tFormat,info);
	printf("Formatted date & time : %s\n", timeBuffer );
        status = write(s, timeBuffer, sizeof(timeBuffer));
	strncpy(msg, "start",msgSize);
        status = write(s, msg, msgSize);
	sleep(15);
	strncpy(msg, "stop",msgSize);
        status = write(s, msg, msgSize);
	sleep(1);
	strncpy(msg, "kill",msgSize);
        status = write(s, msg, msgSize);
    }

    if( status < 0 ) perror("uh oh");

    close(s);
    return 0;
}
