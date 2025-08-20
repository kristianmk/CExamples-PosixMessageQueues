// Written by K. M. Knausgård 2022-01.
//
// Hello world using Posix message queues. This is the receiver.
// Ubuntu manual for mq_open here: https://manpages.ubuntu.com/manpages/noble/man3/mq_open.3.html
//
//
// Remember, link with -lrt:
// gcc hello_receiver.c -lrt -o hello_receiver

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>     // Assert macro
#include <fcntl.h>      // For O_* constants
#include <sys/stat.h>   // For mode constants
#include <mqueue.h>
#include <errno.h>
#include <string.h>


void receivePosixQueueMessage()
{
  // Open Posix Message Queue read only
  mqd_t mqd = mq_open("/mas418queue", O_CREAT | O_RDONLY, 0666, NULL); // Receiver creates queue.
  if (-1 == mqd) {
    printf("Error description from mq_open create: %s\n", strerror(errno));
    exit(1);
  }


  // Get attributes into attr
  struct mq_attr attr;
  assert(-1 != mq_getattr(mqd, &attr));

  // Allocate memory for message and zero it
  char* buffer = calloc(attr.mq_msgsize, 1);
  assert(NULL != buffer);

  // Receive message
  unsigned int priority = 0;
  if(-1 == (mq_receive(mqd, buffer, attr.mq_msgsize, &priority)))  // Size must match message size!
  {
    perror("Error receiving message\n");
    exit(1);
  }


  printf("Received Posix Message of priority: %u\n", priority);
  printf("   \"%s\"\n", buffer);


  // Free allocated memory
  free(buffer);
  buffer = NULL;

  // Close message queue
  mq_close(mqd);
}


int main()
{
  
  printf("Starting up receiver..\n");
  
  receivePosixQueueMessage();

  return 0;
}
