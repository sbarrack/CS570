#include <iostream>
#include <ctime>
#include "belt.hpp"

void *produce(void *arg)
{
    Producer *p = (Producer *)arg;

    struct timespec delay = {p->delay / MSPSEC, (p->delay % MSPSEC) * NSPMS};

    while (true)
    {
        if (p->candy == frog)
        {
            sem_wait(&p->belt->max_frogs);
        }
        sem_wait(&p->belt->empty);
        sem_wait(&p->belt->mutex);

        if (p->belt->produced >= p->belt->limit)
        {
            sem_post(&p->belt->mutex);
            sem_post(&p->belt->full);
            return nullptr;
        }

        enqueue(p->candy);

        switch (p->candy)
        {
        case frog:
            p->belt->frogs++;
            break;
        case snail:
            p->belt->snails++;
            break;
        }

        cout << "Belt: " << p->belt->frogs << " frogs + " << p->belt->snails
             << " escargots = " << p->belt->frogs + p->belt->snails
             << ". produced: " << ++p->belt->produced << "\tAdded ";
        if (p->candy == frog)
        {
            cout << "crunchy frog bite";
        }
        else if (p->candy == snail)
        {
            cout << "escargot sucker";
        }
        cout << ".\n";

        sem_post(&p->belt->mutex);
        sem_post(&p->belt->full);

        nanosleep(&delay, NULL);
    }
}
