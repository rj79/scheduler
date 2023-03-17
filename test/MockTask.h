#ifndef MOCK_TASK_H
#define MOCK_TASK_H

#include "ITask.h"

class MockTask: public ITask
{
private:
    int StepCount;

public:
    MockTask();
    void taskStep();
    int stepCount() const;
};

#endif
