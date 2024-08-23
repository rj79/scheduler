#ifndef MOCK_TASK_H
#define MOCK_TASK_H

#include "ITask.h"
#include "Scheduler.h"

class MockTask: public ITask
{
private:
    int StepCount;
    Scheduler* Sched;
    int RemoveAfterSteps;
public:
    MockTask(Scheduler* scheduler=nullptr, int removeAfterSteps=0);
    void taskStep();
    int stepCount() const;
};

#endif
