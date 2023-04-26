#include <cstdlib>
#include <ctime>
#include "object.cpp"

// function for generating random BinaryOperationTask (0), ObjectsNumberGlobal (1),
// TasksWithResultNumber (2), ObjectsNumber (3)
void add_random_tasks(Container<std::shared_ptr<Task>> *tasks)
{
    size_t tasks_number = 5 + rand() % 6;
    for (size_t i = 0; i < tasks_number; i++)
    {
        size_t type = rand() % 4;
        switch (type)
        {
        case 0:
            tasks->push_back(std::shared_ptr<BinaryOperationTask>(new BinaryOperationTask()));
            break;
        case 1:
            tasks->push_back(std::shared_ptr<ObjectsNumberGlobal>(new ObjectsNumberGlobal()));
            break;
        case 2:
            tasks->push_back(std::shared_ptr<TasksWithResultNumber>(new TasksWithResultNumber(tasks)));
            break;
        case 3:
            tasks->push_back(std::shared_ptr<ObjectsNumber>(new ObjectsNumber(tasks)));
            break;
        default:
            break;
        }
    }
}

void task3()
{
    Container<std::shared_ptr<Task>> *tasks = new Container<std::shared_ptr<Task>>;
    Container<std::string> *results = new Container<std::string>;

    add_random_tasks(tasks); // add from 5 to 10 random tasks
    std::shared_ptr<BinaryOperationTask> task_1{new BinaryOperationTask()};
    std::shared_ptr<ObjectsNumberGlobal> task_2{new ObjectsNumberGlobal()};
    std::shared_ptr<ClearTask> task_3{new ClearTask(tasks)};

    std::shared_ptr<AddToContainerTask> task_4{new AddToContainerTask(tasks, task_1)};
    std::shared_ptr<AddToContainerTask> task_5{new AddToContainerTask(tasks, task_2)};
    std::shared_ptr<AddToContainerTask> task_6{new AddToContainerTask(tasks, task_3)};

    tasks->push_back(task_4);
    tasks->push_back(task_5);
    tasks->push_back(task_6);

    std::cout << "Total objects number: " << std::to_string(Object::get_counter()) << std::endl;

    for (auto t = tasks->begin(); t != tasks->end(); ++t)
    {
        t.value()->execute();
        results->push_back(t.value()->toString());
    }

    for (auto s = results->begin(); s != results->end(); ++s)
    {
        std::cout << s.value() << std::endl;
    }

    tasks->clear();
    results->clear();
    delete tasks;
    delete results;
}

int main()
{
    srand(time(0));
    task3();
    std::cout << "Total objects number: " << std::to_string(Object::get_counter()) << std::endl;
    return 0;
}
