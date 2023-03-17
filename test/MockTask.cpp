#include "MockTask.h"

MockTask::MockTask() :
    StepCount(0)
{
    // Empty
}

void MockTask::taskStep() {
    StepCount++;
}

int MockTask::stepCount() const
{
    return StepCount;
}
