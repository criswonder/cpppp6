// ex.12.05.cpp -- find a value for number of customers per 
// hour that leads to an average wait time of one minute.
// compile with queue.cpp
#include <iostream>
#include <cstdlib> // for rand() and srand()
#include <ctime>   // for time()
#include "queue.h"
const int MIN_PER_HR = 60;

bool newcustomer(double x); // is there a new customer?

int main()
{
    using std::cin;
    using std::cout;
    using std::endl;
    using std::ios_base;
// setting things up
    std::srand(std::time(0));    //  random initializing of rand()

    Queue line(10);         // line queue holds up to 10 people
    int hours = 200;        //  hours of simulation
    // simulation will run 1 cycle per minute
    long cyclelimit = MIN_PER_HR * hours; // # of cycles
    double perhour = 31;    //  average # of arrival per hour;
    double min_per_cust;    //  average time between arrivals
    
    Item temp;              //  new customer data
    long turnaways;         //  turned away by full queue
    long customers;         //  joined the queue
    long served;            //  served during the simulation
    long sum_line;          //  cumulative line length
    int wait_time;          //  time until autoteller is free
    long line_wait;         //  cumulative time in line

// running the simulation
    do
    {
        while (line.dequeue(temp))   // empty queue
            ;
        turnaways = customers = served = sum_line = wait_time = line_wait = 0;
        min_per_cust = MIN_PER_HR / --perhour;

        for (int cycle = 0; cycle < cyclelimit; cycle++)
        {
            if (newcustomer(min_per_cust))   // have newcomer
            {
                if (line.isfull())
                    turnaways++;
                else
                {
                    customers++;
                    temp.set(cycle);    // cycle = time of arrival
                    line.enqueue(temp); // add newcomer to line
                }
            }
            if (wait_time <= 0 && !line.isempty())
            {
                line.dequeue (temp);      // attend next customer
                wait_time = temp.ptime(); // for wait_time minutes
                line_wait += cycle - temp.when();
                served++;
            }
            if (wait_time > 0)
                wait_time--;
            sum_line += line.queuecount();
        }
    } 
    while ( ( (double) line_wait / served ) >= 1.02 );

// reporting results
    if (customers > 0)
    {
        cout << perhour << " customers per hour leads to \n"
             << "an average wait time of one minute.\n\n";

        cout << "simulation period: " << hours << " hours\n";
        cout << "customers accepted: " << customers << endl;
        cout << "  customers served: " << served << endl;
        cout << "         turnaways: " << turnaways << endl;
        cout << "average queue size: ";
        cout.precision(2);
        cout.setf(ios_base::fixed, ios_base::floatfield);
        cout << (double) sum_line / cyclelimit << endl;
        cout << " average wait time: "
             << (double) line_wait / served << " minutes\n";
    }
    else
        cout << "No customers!\n";
    cout << "Done!\n";
    // cin.get();
    // cin.get();
    return 0;
}

//  x = average time, in minutes, between customers
//  return value is true if customer shows up this minute
bool newcustomer(double x)
{
    return (std::rand() * x / RAND_MAX < 1); 
}
