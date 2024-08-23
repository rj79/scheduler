#ifndef SCHEDULER_H
#define SCHEDULER_H

#include "ITask.h"
#include <stdint.h>
#include <Arduino.h>

#ifndef MAX_TASKS
#define MAX_TASKS 10
#endif

/** 
 * Class used internally by the scheduler to keep track of task execution.
*/
class TTaskInfo {
public:
    TTaskInfo(ITask* task);
    ITask* Task;
    uint32_t Interval;
    uint32_t NextStart;
    bool Started;
    bool Overflow;
    String Name;

    bool operator<(const TTaskInfo& other) const;
    String toString() const;
};


/**
 * The Scheduler will attempt to run tasks periodically with their specified 
 * interval. The scheduler is not preemptive meaning the tasks must "play nice"
 * by releasing control to allow other tasks to run. Task objects implement 
 * the ITask interface. The ITask::taskStep() method is called by the 
 * Scheduler.
 * 
 * Tasks are added to the Scheduler by calling addTask(). To start the 
 * Scheduler, call the begin() method.
*/
class Scheduler
{
public:
    Scheduler();

    /**
     * Add a task to be executed with the given interval.
     * @param task The task object to add
     * @param interval The interval in milliseconds with which the task should
     *  be run.
     * @param name An optional name which can be used for debugging purposes.
    */
    ITask* addTask(ITask* task, uint32_t interval_ms, String name="");

    /**
     * Will remove a task from the set of tasks to execute. 
    */
    void removeTask(ITask* task);

    /**
     * Starts the scheduler. If timeout == 0, it will run as long as there is 
     * at least one task scheduled. If timeout > 0 it will run for the 
     * specified number of milliseconds.
    */
    void begin(unsigned long timeout=0);
    
    /**
     * Runs the scheduler and returns immediately. This is useful in case giving
     * up control completely to the scheduler using begin() is not suitable.
     * Call step() from within the loop() function. step() will execute the 
     * tasks when it's time.
     */
    void step();

private:
    TTaskInfo TaskInfos[MAX_TASKS];
    uint8_t TaskCount;
    
    TTaskInfo* nextTask();
    uint8_t Overflowed;
    void reschedule(TTaskInfo* task_info);
};

#endif
