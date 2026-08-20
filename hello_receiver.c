// Written by K. M. Knausgård 2022-01.
//
// Hello world using POSIX message queues. This is the receiver.
// Ubuntu manual for mq_open here: https://manpages.ubuntu.com/manpages/noble/man3/mq_open.3.html
//
//
// Remember, link with -lrt

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>      // For O_* constants
#include <sys/stat.h>   // For mode constants
#include <mqueue.h>


void receivePosixQueueMessage(void)
{
  // Create POSIX Message Queue read only
  mqd_t mqd = mq_open("/mas418queue", O_CREAT | O_EXCL | O_RDONLY, 0600, NULL);
  if (mqd == (mqd_t)-1)
  {
    perror("mq_open failed");
    exit(EXIT_FAILURE);
  }


  // Get attributes into attr
  struct mq_attr attr;
  if (mq_getattr(mqd, &attr) == -1)
  {
    perror("mq_getattr failed");
    mq_close(mqd);
    mq_unlink("/mas418queue");
    exit(EXIT_FAILURE);
  }

  // Allocate memory for message and a terminating null character
  const size_t messageSize = (size_t)attr.mq_msgsize;
  char* buffer = calloc(messageSize + 1, 1);
  if (buffer == NULL)
  {
    perror("calloc failed");
    mq_close(mqd);
    mq_unlink("/mas418queue");
    exit(EXIT_FAILURE);
  }

  // Receive message
  unsigned int priority = 0;
  const ssize_t receivedBytes =
      mq_receive(mqd, buffer, messageSize, &priority);

  if (receivedBytes == -1)
  {
    perror("mq_receive failed");
    free(buffer);
    mq_close(mqd);
    mq_unlink("/mas418queue");
    exit(EXIT_FAILURE);
  }

  buffer[(size_t)receivedBytes] = '\0';

  printf("Received POSIX Message of priority: %u\n", priority);
  printf("   \"%s\"\n", buffer);

  // Free allocated memory
  free(buffer);
  buffer = NULL;

  // Close message queue
  if (mq_close(mqd) == -1)
  {
    perror("mq_close failed");
    mq_unlink("/mas418queue");
    exit(EXIT_FAILURE);
  }

  // Unlink message queue
  if (mq_unlink("/mas418queue") == -1)
  {
    perror("mq_unlink failed");
    exit(EXIT_FAILURE);
  }
}


int main(void)
{
  printf("Starting up receiver..\n");

  receivePosixQueueMessage();

  return EXIT_SUCCESS;
}
