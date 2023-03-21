# About
This is a simple non-preemptive scheduler library for ATmega328P, ESP8266, ESP32 and other microcontrollers.

# Usage
The main class is the Scheduler. The Scheduler will run one or more tasks periodically at a specified interval.

A task class must implement the ITask interface. The logic of the task is placed in the taskStep() method. 

To make a task schedulable, call add_task(<task_object>, <interval_ms>).

To start the scheduler, call begin().

The following pseudocode will make taskA run every 1000 ms and taskB run every 200 ms.
```C++
Scheduler scheduler;
scheduler.addTask(taskA, 1000);
scheduler.addTask(taskB, 200);
scheduler.begin();
```

The "Blink" program can look as follows:
```C++
// Example of a "Blink" program using Scheduler.

#include <Arduino>
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
}

void setup()
{
    Scheduler scheduler;
    BlinkTask blinkTask;
    // Request that BlinkTask::taskStep is run with a 1000 ms interval.
    scheduler.addTask(blinkTask, 1000);  
    // Start executing the tasks.
    scheduler.begin();  
}

void loop()
{
    // Will not be reached as long as there are tasks to execute
}
```

# Cooperative multitasking
The Scheduler is non-preemptive, meaning it is not able to interrupt the current task in favor of running another task. While the taskStep() method of a task is executing, no other task can run. Tasks must therefore cooperate/"play nice" by being written in a way that does not block other tasks to run. What this means in practice is:
1. Endless loops must not exist inside a task's taskStep() method.
2. Operations that may take a long time to run should be split in smaller parts so that the taskStep() method does not take a long time to run.

# Accuracy
The Scheduler will attempt to run each task at the given interval. On average there will be no drift relative to the system clock. However, if the total execution time of all tasks during a certain period exceeds the available CPU time, the specified execution intervals will not be possible for the Scheduler to keep.

# Running the examples
## Using VSCode with PlatformIO
1. Connect an Arduino-compatible board via USB (e.g. Uno, Lolin D1 Mini or similar)
2. Click the PlatformIO "ant" icon in the toolbar to the left.
3. Expand the project task starting example_blink_<your_board name>
4. Click "Upload and Monitor"

## Using command line
For Arduino Uno module (ATMega 328P):
```sh
pio run -e example_blink_uno -t upload
pio run -e example_two_tasks_uno -t upload -t monitor
```
For Lolin D1 module (ESP8266):
```sh
pio run -e example_blink_lolin_d1 -t upload
pio run -e example_two_tasks_lolin_d1 -t upload -t monitor
```
For Lolin C3 module (ESP32-C3):
```sh
pio run -e example_blink_lolin_c3_mini -t upload
pio run -e example_two_tasks_lolin_c3_mini -t upload -t monitor
```

# Running the unit tests
The unit test are located in the tests/test_embedded folder.

## Using VSCode with PlatformIO
1. Connect an Arduino-compatible board via USB (e.g. Uno, Lolin D1 Mini or similar)
2. Check platform.ini that there is a test_<boardname> environment that matches your board.
3. Click the testing icon in the toolbar to the left.
4. Click the play icon next to the test environment name that matches your board.

## Using Command line
1. Open a console and go to the directory of the library.
2. Depending on your board type:
For Arduino Uno module (ATMega 328P):
```sh
pio test -e test_arduino_uno
```
For Lolin D1 module (ESP8266):
```sh
pio test -e test_lolin_d1
```
For Lolin C3 module (ESP32-C3):
```sh
pio test -e test_lolin_c3_mini
```
