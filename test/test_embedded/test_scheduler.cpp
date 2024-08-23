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
    scheduler2.begin(15);
    TEST_ASSERT_EQUAL(2, task2.stepCount());
}

void test_one_task_accuracy()
{
    Scheduler scheduler;
    MockTask task;
    scheduler.addTask(&task, 10);
    scheduler.begin(10000);
    TEST_ASSERT_TRUE(task.stepCount() - 1000 <= 1);
}

void test_two_tasks_accuracy()
{
    Scheduler scheduler;
    MockTask task1;
    MockTask task2;
    scheduler.addTask(&task1, 10);
    scheduler.addTask(&task2, 40);
    scheduler.begin(10000);
    TEST_ASSERT_TRUE(task1.stepCount() - 1000 <= 1);
    TEST_ASSERT_EQUAL(250, task2.stepCount());
}

void test_remove_task() {
    Scheduler scheduler;
    MockTask task1(&scheduler, 6);
    MockTask task2(&scheduler, 4);
    MockTask task3(&scheduler, 8);
    
    scheduler.addTask(&task1, 10);
    scheduler.addTask(&task2, 10);
    scheduler.addTask(&task3, 10);

    scheduler.begin(10000);

    TEST_ASSERT_EQUAL(6, task1.stepCount());
    TEST_ASSERT_EQUAL(4, task2.stepCount());
    TEST_ASSERT_EQUAL(8, task3.stepCount());
}

void test_step() {
    Scheduler scheduler;

    MockTask task1;
    scheduler.addTask(&task1, 10);

    unsigned long stop = millis() + 1000;
    while (millis() < stop) {
        scheduler.step();
    }
    TEST_ASSERT_TRUE(task1.stepCount() - 100 <= 1);
}

int runUnityTests(void) 
{
    delay(2000);
    UNITY_BEGIN();
    RUN_TEST(test_create);
    RUN_TEST(test_one_task_timeout_accuracy_short);
    RUN_TEST(test_one_task_accuracy);
    RUN_TEST(test_two_tasks_accuracy);
    RUN_TEST(test_remove_task);
    RUN_TEST(test_step);    
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
