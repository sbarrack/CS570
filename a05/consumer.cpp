#include <iostream>
#include <ctime>
#include "belt.hpp"

void *consume(void *arg)
{
    Consumer *c = (Consumer *)arg;

    struct timespec delay = {c->delay / MSPSEC, (c->delay % MSPSEC) * NSPMS};

    sem_t *mutex;
    sem_t *empty;
    sem_t *full;
    sem_t *max_frogs;
    mutex = &c->belt->mutex;
    empty = &c->belt->empty;
    full = &c->belt->full;
    max_frogs = &c->belt->max_frogs;

    int frogs;
    int snails;
    int produced;
    int processed;
    Candy candy;
    int lfrogs;
    int efrogs;
    int lsnails;
    int esnails;

    while (true)
    {
        sem_wait(full);
        sem_wait(mutex);

        if (c->belt->processed >= c->belt->limit)
        {
            if (c->belt->print)
            {
                lfrogs = c->belt->lfrogs;
                efrogs = c->belt->efrogs;
                lsnails = c->belt->lsnails;
                esnails = c->belt->esnails;
                cout << "\nPRODUCTION REPORT\n";
                cout << "----------------------------------------\n";
                cout << "crunchy frog bite producer generated ";
                cout << lfrogs + efrogs << " candies\nescargot sucker ";
                cout << "producer generated " << lsnails + esnails;
                cout << " candies\nLucy consumed " << lfrogs << " crunchy ";
                cout << "frog bites + " << lsnails << " escargot suckers =";
                cout << " " << lfrogs + lsnails << "\nEthel consumed ";
                cout << efrogs << " crunchy frog bites + " << esnails;
                cout << " escargot suckers = " << efrogs + esnails << endl;
            }
            c->belt->print = 1;
            sem_post(mutex);
            sem_post(empty);
            return nullptr;
        }

        candy = dequeue();

        cout << "Belt: " << frogs << " frogs + " << snails << " escargots = ";
        cout << frogs + snails << ". produced: " << produced << "\t" << c->name << " consumed ";
        if (candy == frog)
        {
            c->belt->frogs--;
            cout << "cruncy frog bite";
        }
        else if (candy == snail)
        {
            c->belt->snails--;
            cout << "escargot sucker";
        }
        cout << ".\n";

        frogs = c->belt->frogs;
        snails = c->belt->snails;
        produced = c->belt->produced;

        if (candy == frog)
        {
            if (c->name == "Lucy")
            {
                c->belt->lfrogs++;
            }
            else if (c->name == "Ethel")
            {
                c->belt->efrogs++;
            }
        }
        else if (candy == snail)
        {
            if (c->name == "Lucy")
            {
                c->belt->lsnails++;
            }
            else if (c->name == "Ethel")
            {
                c->belt->esnails++;
            }
        }

        c->belt->processed++;

        sem_post(mutex);
        sem_post(empty);

        if (candy == frog)
        {
            sem_post(max_frogs);
        }

        nanosleep(&delay, NULL);
    }
}
