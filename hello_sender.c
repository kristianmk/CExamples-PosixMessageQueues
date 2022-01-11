// Written by K. M. Knausgård 2022-01.
//
// Hello world using Posix message queues. This is the sender.
// Ubuntu manual for mq_open here: http://manpages.ubuntu.com/manpages/focal/man3/mq_open.3.html
//
// Remember creating the queue before using it, as described here:
// http://manpages.ubuntu.com/manpages/focal/man7/mq_overview.7.html
//
// As super user (sudo):
// mkdir /dev/mas418queue
// mount -t mqueue none /dev/mas418queue
//
// Remember, link with -lrt:
// gcc hello_sender.c -lrt -o hello_sender


#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>           /* For O_* constants */
#include <sys/stat.h>        /* For mode constants */
#include <mqueue.h>


void sayHelloToOtherProcess()
{
  // Open queue
  mqd_t mqd = mq_open("/mas418queue", O_EXCL | O_WRONLY,  0600, NULL);

  // Check if queue created successfully.
  if (mqd == -1)
  {
    perror("mq_open failed!");
    exit(1);
  }
  
  // Send message with priority 11.
  // The specifications requires at least priorities 0 to 31 to be available.
  // See the manual for information on high/low priorities!
  mq_send(mqd, "Hello world!", 13, 11); // Length 13 including string termination character.
  
  // Close queue
  mq_close(mqd);
}


int main()
{
  
  printf("Starting up..");
  
  sayHelloToOtherProcess();

  return 0;
}
