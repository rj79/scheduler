/*
    Program that demonstrates how to run multiple tasks using the Scheduler.
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

class PrintTask: public ITask
{
public:
    void taskStep() {
        char buffer[64];
        snprintf(buffer, sizeof(buffer), "Time since start: %ld ms\n", millis());
        Serial.print(buffer);
    }
};

void setup()
{
    Serial.begin(9600);
    Scheduler scheduler;
    BlinkTask blinkTask;
    PrintTask printTask;
    scheduler.addTask(&blinkTask, 300);  // Request that BlinkTask::taskStep is run with a 300 ms interval.
    scheduler.addTask(&printTask, 1000);  // Request that PrintTask::taskStep is run with a 1000 ms interval.
    scheduler.begin();  // Start executing the tasks.
}

void loop()
{
    // Will not be reached as long as there are tasks to execute
}
