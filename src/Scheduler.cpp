#include "Scheduler.h"

TTaskInfo::TTaskInfo(ITask* task=nullptr) :
    Task(task),
    Interval(0),
    NextStart(0),
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
    TaskCount(0)
{
    // Empty
}

ITask* Scheduler::addTask(ITask* task, uint32_t interval, String name)
{
    for (int i = 0; i < MAX_TASKS; ++i) {
        if (TaskInfos[i].Task == nullptr) {
            TaskInfos[i].Task = task;
            TaskInfos[i].Interval = interval;
            TaskInfos[i].NextStart = 0;
            TaskInfos[i].Name = name;
            TaskCount++;
            return task;
        }
    }
    return nullptr;
}

/**
 * Reschedules a task. The next execution will be scheduled at current planned 
 * execution start + task interval.
*/
void Scheduler::reschedule(TTaskInfo* task_info) 
{
    if (task_info->NextStart == 0 && !task_info->Overflow) {
        // If this will be the first execution, initiate the next start to 
        // something sensible.
        // The naive strategy is to try to spread out the tasks over the 
        // timeline so they don't start at the same time or overlap.
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
                for (int i = 0; i < TaskCount; ++i) {
                    TaskInfos[i].Overflow = false;
                }
                Overflowed = 0;
            }
        }
    }
}

void Scheduler::begin(unsigned long timeout)
{
    bool useTimeout = (timeout > 0);

    TTaskInfo* task_info = nullptr;
    unsigned long now = 0;

    // Select first task to execute
    task_info = nextTask();

    if (task_info == nullptr) {
        Serial.println("Error: No task to execute.");
        return;
    }

    if (useTimeout) {
        timeout = millis() + timeout;
    }

    while (true) {
        // Schedule next execution of the task
        reschedule(task_info);

        // Execute the task
        task_info->Task->taskStep();

        // Select next task to execute
        task_info = nextTask();
        now = millis();

        if (task_info) {            
            // Sleep until it's time to execute the next task, unless the next
            // task should already have been started.
            if (task_info->NextStart > now) {
                delay(task_info->NextStart - now);
            }
        }
        else {
            Serial.println("Warning: No more task to execute. Exiting scheduler.");
            break;
        }

        if (useTimeout && millis() >= timeout) {
            break;
        }

    }
}

TTaskInfo* Scheduler::nextTask()
{
    if (TaskCount == 0) {
        return nullptr;
    }

    TTaskInfo* result = &TaskInfos[0];

    for (int i = 1; i < TaskCount; ++i) {
        if (TaskInfos[i] < *result) {
            result = &(TaskInfos[i]);
        }
    }

    return result;
}
