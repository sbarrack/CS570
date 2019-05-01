#include <queue>
#include <string>
#include <unistd.h>
#include "belt.hpp"

#define BELT_LEN 10

queue<Candy> q;

int main(int argc, char **argv)
{
    int c, E, L, f, e;
    Belt *belt = new Belt(100);
    pthread_t thread[4];

    c = E = L = f = e = 0;
    while ((c = getopt(argc, argv, "ELfe")) != -1)
    {
        switch (c)
        {
        case 'E':
            E = stoi(optarg);
            break;
        case 'L':
            L = stoi(optarg);
            break;
        case 'f':
            f = stoi(optarg);
            break;
        case 'e':
            e = stoi(optarg);
            break;
        }
    }

    Consumer *lucy = new Consumer(L, "Lucy", belt);
    Consumer *ethel = new Consumer(E, "Ethel", belt);
    Producer *frogger = new Producer(f, frog, belt);
    Producer *snailor = new Producer(e, snail, belt);

    pthread_create(&thread[0], NULL, produce, (void *)frogger);
    pthread_create(&thread[1], NULL, produce, (void *)snailor);
    pthread_create(&thread[2], NULL, consume, (void *)lucy);
    pthread_create(&thread[3], NULL, consume, (void *)ethel);
    pthread_exit(NULL);

    return 0;
}

Belt::Belt(int limit)
{
    this->limit = limit;
    produced = processed = frogs = snails = 0;
    sem_init(&mutex, 0, 1);
    sem_init(&empty, 0, BELT_LEN);
    sem_init(&full, 0, 0);
    sem_init(&max_frogs, 0, 3);
}

Consumer::Consumer(int delay, string name, Belt *belt)
{
    this->delay = delay;
    this->name = name;
    this->belt = belt;
}

Producer::Producer(int delay, Candy candy, Belt *belt)
{
    this->delay = delay;
    this->candy = candy;
    this->belt = belt;
}

void enqueue(Candy candy)
{
    if (q.size() <= BELT_LEN)
    {
        q.push(candy);
    }
}

Candy dequeue()
{
    Candy temp;
    if (!q.empty())
    {
        temp = q.front();
        q.pop();
    }
    return temp;
}
