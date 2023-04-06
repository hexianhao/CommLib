#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <fcntl.h>

#define SHM_SIZE 1024  // Size of shared memory segment

int main(int argc, char **argv)
{
  const char *shm_file = "./my_shm";  // Name of shared memory file
  int fd;
  char *shm;

  // Create shared memory file
  if (strcmp(argv[1], "sender") == 0) {
    if ((fd = open(shm_file, O_CREAT | O_TRUNC | O_RDWR, 0777)) < 0) {
      perror("sender open");
      exit(1);
    }
  } else {
    if ((fd = open(shm_file, O_RDWR, 0777)) < 0) {
      perror("receiver open");
      exit(1);
    }
  }

  if (strcmp(argv[1], "sender") == 0) {
    if (ftruncate(fd, SHM_SIZE) == -1) {
      perror("ftruncate failed:");
      exit(1);
    }
  }

  shm = (char *)mmap(NULL, SHM_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
  if (shm == MAP_FAILED) {
    perror("mmap failed:");
    exit(1);
  }

  printf("mmap %s success\n", shm_file);
  close(fd); 

  if (strcmp(argv[1], "sender") == 0) {
    char *content = "hello world";
    strncpy(shm, content, strlen(content));
  } else {
    printf("read msg %s\n", shm);
  }

  return 0;
}