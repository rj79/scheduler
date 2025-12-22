#include "Scheduler.h"
#include "watchdog.h"

TTaskInfo::TTaskInfo(ITask* task=nullptr) :
    Task(task),
    Interval(0),
    NextStart(0),
    Started(false),
    Overflow(false),
    Name("")
{
    // Empty
}

bool TTaskInfo::operator<(const TTaskInfo& other) const
{
    if (Overflow == other.Overflow) {
        return NextStart < other.NextStart;
    }
    else {
        return !Overflow;
    }
}

String TTaskInfo::toString() const
{
    char buffer[64];
    snprintf(buffer, sizeof(buffer), "<TaskInfo \"%s\" interval=%lu next_start=%lu", Name.c_str(), Interval, NextStart);
    return String(buffer);
}

Scheduler::Scheduler() :
    TaskCount(0),
    IdleTask(nullptr)
{
    // Empty
}

ITask* Scheduler::addTask(ITask* task, uint32_t interval, String name)
{
    for (int i = 0; i < MAX_TASKS; ++i) {
        if (TaskInfos[i].Task == nullptr) {
            TaskInfos[i].Task = task;
            TaskInfos[i].Interval = interval;
            TaskInfos[i].Started = false;
            TaskInfos[i].Overflow = false;
            TaskInfos[i].NextStart = 0;
            TaskInfos[i].Name = name;
            TaskCount++;
            return task;
        }
    }
    return nullptr;
}

ITask* Scheduler::setIdleTask(ITask* task)
{
    IdleTask = task;
    return IdleTask;
}

void Scheduler::removeTask(ITask* task)
{
    for (int i = 0; i < MAX_TASKS; ++i) {
        if (TaskInfos[i].Task == task) {
            TaskInfos[i].Task = nullptr;
            TaskCount--;
            return;
        }
    }
}

/**
 * Reschedules a task. The next execution will be scheduled at current planned 
 * execution start + task interval.
*/
void Scheduler::reschedule(TTaskInfo* task_info) 
{
    if (!task_info->Started) {
        // If this will be the first execution, initiate the next start to 
        // something sensible.
        // The naive strategy is to try to spread out the tasks over the 
        // timeline so they don't start at the same time or overlap.
        task_info->Started = true;
        task_info->NextStart = millis() + task_info->Interval;
    } 
    else {
        // For subsequent executions, schedule the next start to the planned 
        // start time + interval. This will avoid drift over time. 
        // It is still possible that tasks accidentally can overlap which will
        // cause the later task to be executed as soon as the previous has 
        // finished executing.
        uint32_t save_start = task_info->NextStart;
        task_info->NextStart += task_info->Interval;

        // Check for wrap around/overflow.
        // If the NextStart variable has wapped around, set the overflow flag
        // of that task.
        // When all tasks have the overflow flag set, remove it for all.
        if (task_info->NextStart < save_start) {
            task_info->Overflow = true;
            Overflowed++;
            if (Overflowed >= TaskCount) {
                for (int i = 0; i < MAX_TASKS; ++i) {
                    if (TaskInfos[i].Task != nullptr) {
                        TaskInfos[i].Overflow = false;
                    }
                }
                Overflowed = 0;
            }
        }
    }
}

bool Scheduler::internalStep()
{
    TTaskInfo* task_info = nullptr;
    
    // Select first task to execute
    task_info = nextTask();

    if (task_info) {
        // Only execute task if it is time
        if (millis() >= task_info->NextStart) {
            // Schedule next execution of the task
            reschedule(task_info);

            // Execute the task
            task_info->Task->taskStep();

            // Select next task to execute
            task_info = nextTask();
        }
    }

    if (task_info) {            
        // Run the idle task unless it is already time to execute the next task.
        if (millis() < task_info->NextStart) {
            if (IdleTask) {
                IdleTask->taskStep();
            }
        }
    }
    else if (IdleTask) {
        // There is no other task, so run the idle task.
        IdleTask->taskStep();
    }
    else {
        // There is no next task to run
        return false;
    }

    // There is at least one task to run in next iteration
    return true;
}

void Scheduler::begin(unsigned long timeout)
{
    bool useTimeout = (timeout > 0);

    if (useTimeout) {
        timeout = millis() + timeout;
    }

    while (true) {
        feedWatchdog();

        if (!internalStep()) {
            Serial.println("Warning: No more task to execute. Exiting scheduler.");
            break;
        }

        if (useTimeout && millis() >= timeout) {
            break;
        }
    }
}

void Scheduler::step()
{
    internalStep();
}

TTaskInfo* Scheduler::nextTask()
{
    if (TaskCount == 0) {
        return nullptr;
    }

    TTaskInfo* result = nullptr;

    for (int i = 0; i < MAX_TASKS; ++i) {
        if (TaskInfos[i].Task != nullptr) {
            if (result == nullptr) {
                result = &(TaskInfos[i]);
            }
            else if (TaskInfos[i] < *result) {
                result = &(TaskInfos[i]);
            }
        }
    }

    return result;
}
