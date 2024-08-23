#include "MockTask.h"

MockTask::MockTask(Scheduler* scheduler, int removeAfterSteps) :
    StepCount(0),
    Sched(scheduler),
    RemoveAfterSteps(removeAfterSteps)
{
    // Empty
}

void MockTask::taskStep() {
    StepCount++;
    if (Sched) {
        if (StepCount == RemoveAfterSteps) {
            Sched->removeTask(this);
        }
    }
}

int MockTask::stepCount() const
{
    return StepCount;
}
