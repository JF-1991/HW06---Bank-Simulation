//
// Josh Ferrari - <jf1991@live.missouristate.edu>
// 4/26/2019
//

#include <iostream>
#include <queue>
#include <fstream>

struct Simulation
{
    int aTime; // arrival time
    int tTime; // transaction time
};

int currentTime;
bool tellerAvailable = true;
int departureTime;
int avgWaitTime;
int totalWaitTime;

// arrival
void arrival(struct Simulation newEvent, std::priority_queue<struct Simulation> priorityQueue, std::queue<struct Simulation> Queue)
{
    priorityQueue.pop(); // remove event from list

    Simulation customer = newEvent;

    if (Queue.empty() && tellerAvailable)
    {
        departureTime = currentTime + customer.tTime;

        Simulation departEvent;

        departEvent.tTime = departureTime;

        priorityQueue.push(departEvent);

        tellerAvailable = false;
    }
    else
    {
        Queue.push(customer);
    }
}

//departure
void departure(struct Simulation newEvent, std::priority_queue<struct Simulation> priorityQueue, std::queue<struct Simulation> Queue)
{
    priorityQueue.pop(); // remove event from event list

    if (!Queue.empty())
    {
        // customer and fron of the line begins transaction
        Simulation customer = Queue.front();

        Queue.pop();

        departureTime = currentTime + customer.tTime;

        Simulation departEvent;

        departEvent.tTime = departureTime;

        priorityQueue.push(departEvent);

        totalWaitTime += currentTime - customer.aTime;
    }
    else
    {
        tellerAvailable = true;
    }
}

// banksimulation
void bankSim(void)
{
    std::ifstream input("input.txt");

    std::priority_queue<struct Simulation> eventListPQueue; // event list
    std::queue<struct Simulation> bankQueue; // bank line

    int a;
    int t;

    tellerAvailable = true;

    while(input >> a >> t)
    {
        Simulation newArrivalEvent;
        newArrivalEvent.aTime = a;
        newArrivalEvent.tTime = t;
        eventListPQueue.push(newArrivalEvent);
    }

    while (!eventListPQueue.empty())
    {
        Simulation newEvent = eventListPQueue.top();

        currentTime = newEvent.aTime;

        if (newEvent.aTime == currentTime)
        {
            arrival(newEvent, eventListPQueue, bankQueue);
        }
        else
        {
            departure(newEvent, eventListPQueue, bankQueue);
        }
    }

    avgWaitTime = totalWaitTime / 10 /*number of customers served*/;

    std::cout << avgWaitTime << std::endl;
}


int main()
{
    bankSim();

    return 0;
};