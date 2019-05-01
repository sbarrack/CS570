#include <semaphore.h>

using namespace std;

#define MSPSEC 1000
#define NSPMS 1000000

enum Candy
{
    frog,
    snail
};

class Belt
{
  public:
    int limit;
    int produced;
    int processed;
    int frogs;
    int snails;
    int lfrogs;
    int lsnails;
    int efrogs;
    int esnails;
    int print;
    sem_t mutex;
    sem_t empty;
    sem_t full;
    sem_t max_frogs;
    Belt(int limit);
};

class Consumer
{
  public:
    int delay;
    string name;
    Belt *belt;
    Consumer(int delay, string name, Belt *belt);
};

class Producer
{
  public:
    int delay;
    Candy candy;
    Belt *belt;
    Producer(int delay, Candy candy, Belt *belt);
};

void enqueue(Candy candy);
Candy dequeue();

void *consume(void *arg);
void *produce(void *arg);
