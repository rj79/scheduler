#include "unity.h"
#include "Scheduler.h"
#include "MockTask.h"

void test_create() 
{
    Scheduler scheduler;
}

void test_one_task_timeout_accuracy_short()
{
    Scheduler scheduler;
    MockTask task;
    scheduler.addTask(&task, 10);
    scheduler.begin(9);
    TEST_ASSERT_EQUAL(1, task.stepCount());

    Scheduler scheduler2;
    MockTask task2;
    scheduler2.addTask(&task2, 10);
    scheduler2.begin(11);
    TEST_ASSERT_EQUAL(2, task2.stepCount());
}

void test_one_task_accuracy()
{
    Scheduler scheduler;
    MockTask task;
    scheduler.addTask(&task, 10);
    scheduler.begin(10000);
    TEST_ASSERT_EQUAL(1000, task.stepCount());
}

void test_two_tasks_accuracy()
{
    Scheduler scheduler;
    MockTask task1;
    MockTask task2;
    scheduler.addTask(&task1, 10);
    scheduler.addTask(&task2, 40);
    scheduler.begin(10000);
    TEST_ASSERT_EQUAL(1000, task1.stepCount());
    TEST_ASSERT_EQUAL(250, task2.stepCount());
}

int runUnityTests(void) 
{
    UNITY_BEGIN();
    RUN_TEST(test_create);
    RUN_TEST(test_one_task_timeout_accuracy_short);
    RUN_TEST(test_one_task_accuracy);
    RUN_TEST(test_two_tasks_accuracy);
    return UNITY_END();
}

void setup()
{
    runUnityTests();
}

void loop()
{
    // Empty
}
