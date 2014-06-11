#include <pthread.h>

int main(int argc, char **argv)
{
  pthread_mutex_t lock;
  pthread_mutex_init(&lock, NULL);
}
