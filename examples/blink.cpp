/*
    Example of how to make a "Blink" program using the Scheduler.
*/
#include <Arduino.h>
#include "Scheduler.h"

class BlinkTask: public ITask
{
private:
    bool Light;

public:
    BlinkTask() :
        Light(false)
    {
        pinMode(LED_BUILTIN, OUTPUT);
    }

    void taskStep() {
        Light = !Light;
        digitalWrite(LED_BUILTIN, Light);
    }
};

void setup()
{
    Scheduler scheduler;
    BlinkTask blinkTask;
    scheduler.addTask(&blinkTask, 1000);  // Request that BlinkTask::taskStep is run with a 1000 ms interval.
    scheduler.begin();  // Start executing the tasks.
}

void loop()
{
    // Will not be reached as long as there are tasks to execute
}
