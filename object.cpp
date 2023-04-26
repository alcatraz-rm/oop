#include <cstdlib>
#include "container.cpp"

class Object
{
protected:
    inline static size_t counter = 0;

public:
    virtual std::string toString() const = 0;
    Object()
    {
        counter++;
    }

    ~Object()
    {
        counter--;
    }

    static size_t get_counter()
    {
        return counter;
    }
};

class Task : public Object
{
protected:
    bool executed;

public:
    bool has_result;
    Task(bool has_result) : Object(), executed(false), has_result(has_result) {}
    virtual void execute() = 0;
};

class Named : public Object
{
protected:
    std::string name;

public:
    Named() : Object() {}
    Named(std::string name) : Object(), name(name) {}

    std::string get_name()
    {
        return name;
    }
};

class BinaryOperationTask : public Task, public Named
{
private:
    const double argument_1;
    const double argument_2;
    double result;

public:
    // default constructor - generates random task
    BinaryOperationTask() : Task(true),
                            argument_1(rand() % 1000 / 10),
                            argument_2(1 + rand() % 1000 / 10)
    {
        size_t operation = rand() % 4;
        switch (operation)
        {
        case 0:
            name = "+";
            break;
        case 1:
            name = "-";
            break;
        case 2:
            name = "*";
            break;
        case 3:
            name = "/";
            break;
        default:
            break;
        }
        counter--;
    }

    BinaryOperationTask(
        double argument_1,
        double argument_2,
        std::string name) : Task(true), Named(name), argument_1(argument_1), argument_2(argument_2)
    {
        counter--; // counter increments twice (in Named and Task constructors), so we must decrement it
    }

    virtual ~BinaryOperationTask()
    {
        counter++;
    }

    virtual std::string toString() const
    {
        std::string info = "Operation: " + std::to_string(argument_1) + " " + name + " " + std::to_string(argument_2);
        if (executed)
        {
            info += "; Result: " + std::to_string(result);
        }

        return info;
    }

    double get_result()
    {
        if (!executed)
        {
            execute();
        }
        return result;
    }

    virtual void execute()
    {
        if (name.size() != 1)
        {
            throw std::invalid_argument("Invalid operation");
        }

        switch (name[0]) // name for that operation contains only 1 symbol
        {
        case '+':
            result = argument_1 + argument_2;
            break;
        case '-':
            result = argument_1 - argument_2;
            break;
        case '*':
            result = argument_1 * argument_2;
            break;
        case '/':
            result = argument_1 / argument_2;
            break;
        default:
            throw std::invalid_argument("Invalid operation");
        }
        executed = true;
    }
};

class AddToContainerTask : public Task, public Named
{
private:
    Container<std::shared_ptr<Task>> *tasks;
    std::shared_ptr<Task> task;

public:
    AddToContainerTask(
        Container<std::shared_ptr<Task>> *tasks,
        std::shared_ptr<Task> task) : Task(false), Named("Adding to container"), tasks(tasks), task(task)
    {
        counter--; // counter increments twice (in Named and Task constructors), so we must decrement it
    }

    virtual ~AddToContainerTask()
    {
        counter++; // like in costructor, but reversed (-2 + 1)
    }

    virtual void execute()
    {
        tasks->push_back(task);
        executed = true;
    }

    virtual std::string toString() const
    {
        std::string info = name + ": {" + task->toString() + "}";
        if (executed)
        {
            info += "; Done.";
        }
        return info;
    }
};

class ObjectsNumber : public Task, public Named
{
private:
    Container<std::shared_ptr<Task>> *objects;
    size_t result;

public:
    ObjectsNumber(Container<std::shared_ptr<Task>> *objects) : Task(true), Named("Objects number"), objects(objects)
    {
        counter--; // counter increments twice (in Named and Task constructors), so we must decrement it
    }

    virtual ~ObjectsNumber()
    {
        counter++; // like in costructor, but reversed (-2 + 1)
    }

    virtual std::string toString() const
    {
        std::string info = "Objects number task";
        if (executed)
        {
            info += "; Result: " + std::to_string(result);
        }
        return info;
    }

    size_t get_result()
    {
        if (!executed)
        {
            execute();
        }
        return result;
    }

    virtual void execute()
    {
        result = objects->size();
        executed = true;
    }
};

class TasksWithResultNumber : public Task, public Named
{
private:
    Container<std::shared_ptr<Task>> *tasks;
    size_t result;

public:
    TasksWithResultNumber(Container<std::shared_ptr<Task>> *tasks) : Task(true), Named("Tasks with result number"), tasks(tasks)
    {
        counter--; // counter increments twice (in Named and Task constructors), so we must decrement it
    }

    virtual ~TasksWithResultNumber()
    {
        counter++; // like in costructor, but reversed (-2 + 1)
    }

    virtual std::string toString() const
    {
        std::string info = "Tasks with result number tasks";
        if (executed)
        {
            info += "; Result: " + std::to_string(result);
        }
        return info;
    }

    size_t get_result()
    {
        if (!executed)
        {
            execute();
        }
        return result;
    }

    virtual void execute()
    {
        result = 0;
        for (auto element = tasks->begin(); element != tasks->end(); ++element)
        {
            if (element.value()->has_result)
            {
                result++;
            }
        }
        executed = true;
    }
};

class ClearTask : public Task
{
private:
    Container<std::shared_ptr<Task>> *objects;

public:
    ClearTask(Container<std::shared_ptr<Task>> *objects) : Task(false), objects(objects) {}

    virtual ~ClearTask() {}

    virtual void execute()
    {
        objects->clear();
        executed = true;
    }

    virtual std::string toString() const
    {
        std::string info = "Clear task";
        if (executed)
        {
            info += "; Done.";
        }
        return info;
    }
};

class ObjectsNumberGlobal : public Task, public Named
{
private:
    size_t result;

public:
    ObjectsNumberGlobal() : Task(true), Named("Objects number global")
    {
        counter--; // counter increments twice (in Named and Task constructors), so we must decrement it
    }

    virtual ~ObjectsNumberGlobal()
    {
        counter++; // like in costructor, but reversed (-2 + 1)
    }

    virtual void execute()
    {
        result = Object::get_counter();
        executed = true;
    }

    size_t get_result()
    {
        if (!executed)
        {
            execute();
        }
        return result;
    }

    virtual std::string toString() const
    {
        std::string info = "Objects number global task";
        if (executed)
        {
            info += "; Result: " + std::to_string(result);
        }
        return info;
    }
};
