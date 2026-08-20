// Written by K. M. Knausgård 2022-01.
//
// Hello world using POSIX message queues. This is the sender.
// Ubuntu manual for mq_open here: https://manpages.ubuntu.com/manpages/noble/man3/mq_open.3.html
//
// Remember creating the queue before using it, as described here:
// https://manpages.ubuntu.com/manpages/noble/man7/mq_overview.7.html
//
//
// Remember, link with -lrt

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>           /* For O_* constants */
#include <mqueue.h>


void sayHelloToOtherProcess(void)
{
  // Open queue
  mqd_t mqd = mq_open("/mas418queue", O_WRONLY);

  // Check if queue opened successfully.
  if (mqd == (mqd_t)-1)
  {
    perror("mq_open failed!");
    exit(EXIT_FAILURE);
  }

  // Send message with priority 11.
  // The specification requires at least priorities 0 to 31 to be available.
  // See the manual for information on high/low priorities!
  const char message[] = "Hello world!";
  if (mq_send(mqd, message, sizeof(message), 11) == -1)
  {
    perror("mq_send failed");
    mq_close(mqd);
    exit(EXIT_FAILURE);
  }

  // Close queue
  if (mq_close(mqd) == -1)
  {
    perror("mq_close failed");
    exit(EXIT_FAILURE);
  }
}


int main(void)
{
  printf("Starting up sender..\n");

  sayHelloToOtherProcess();

  return EXIT_SUCCESS;
}
