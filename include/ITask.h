#ifndef ITASK_H
#define ITASK_H

/**
 * Interface that should be implemented by classes which should be schedulable.
 * @see Scheduler
*/
class ITask {
public:
    virtual ~ITask() { /* Empty */ };

    /** 
     * This method is the equivalent of the "loop" function. It will be called
     * repeatedly by the Scheduler. The method should give up control by 
     * returning as soon as possible so other tasks can run.
     */
    virtual void taskStep() = 0;
};

#endif