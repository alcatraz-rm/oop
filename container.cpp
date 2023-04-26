#include <iostream>

template <typename T>
class Container
{
private:
    template <typename ElementType>
    struct Element
    {
        ElementType data;
        std::shared_ptr<Element<ElementType>> next;
        std::shared_ptr<Element<ElementType>> prev;

        Element() {}
        Element(ElementType data) : data(data), next(nullptr), prev(nullptr) {}
    };

    std::shared_ptr<Element<T>> _top_limiter{new Element<T>()};
    std::shared_ptr<Element<T>> _lower_limiter{new Element<T>()};
    size_t _size;

public:
    Container() : _size(0)
    {
        _top_limiter->next = _lower_limiter;
        _lower_limiter->prev = _top_limiter;
        _size = 0;
    }

    ~Container()
    {
        clear();
        _top_limiter.reset();
        _lower_limiter.reset();
    }

    class Iterator
    {
    private:
        std::shared_ptr<Element<T>> current;

    public:
        Iterator(std::shared_ptr<Element<T>> _current) : current(_current) {}

        bool operator!=(const Iterator &other)
        {
            return current != other.current;
        }

        Iterator &operator++()
        {
            current = current->next;
            return *this;
        }

        Iterator &operator--()
        {
            current = current->prev;
            return *this;
        }

        T const &value()
        {
            return current->data;
        }
    };

    void pop()
    {
        if (_size == 0)
        {
            throw std::length_error("Container is empty.");
        }

        std::shared_ptr<Element<T>> first_element = _top_limiter->next;
        std::shared_ptr<Element<T>> second_element = _top_limiter->next->next;

        second_element->prev = _top_limiter;
        _top_limiter->next = second_element;

        _size--;
    }

    void pop_back()
    {
        if (_size == 0)
        {
            throw std::length_error("Container is empty.");
        }

        std::shared_ptr<Element<T>> last_element = _lower_limiter->prev;
        std::shared_ptr<Element<T>> second_last_element = last_element->prev;

        second_last_element->next = _lower_limiter;
        _lower_limiter->prev = second_last_element;

        last_element.reset();
        _size--;
    }

    void push(T data)
    {
        std::shared_ptr<Element<T>> old_first = _top_limiter->next;
        std::shared_ptr<Element<T>> new_element{new Element<T>(data)};

        new_element->next = old_first;
        new_element->prev = _top_limiter;

        old_first->prev = new_element;
        _top_limiter->next = new_element;
        _size++;
    }

    void push_back(T data)
    {
        std::shared_ptr<Element<T>> old_last = _lower_limiter->prev;
        std::shared_ptr<Element<T>> new_element{new Element<T>(data)};

        new_element->prev = old_last;
        old_last->next = new_element;
        new_element->next = _lower_limiter;
        _lower_limiter->prev = new_element;
        _size++;
    }

    Iterator begin()
    {
        if (_size == 0)
        {
            return end();
        }
        return Iterator(this->_top_limiter->next);
    }

    Iterator end()
    {
        return Iterator(this->_lower_limiter);
    }

    T const &first() const
    {
        if (_size == 0)
        {
            throw std::out_of_range("Container is empty.");
        }
        return _top_limiter->next->data;
    }

    T const &last() const
    {
        if (_size == 0)
        {
            throw std::out_of_range("Container is empty.");
        }
        return _lower_limiter->prev->data;
    }

    void reverse()
    {
        if (_size < 2)
        {
            return;
        }

        std::shared_ptr<Element<T>> current = _top_limiter;

        while (current)
        {
            std::swap(current->next, current->prev);
            current = current->prev;
        }
        std::swap(_top_limiter, _lower_limiter);
    }

    size_t size() const
    {
        return _size;
    }

    bool empty() const
    {
        return _size == 0;
    }

    void print() const
    {
        std::shared_ptr<Element<T>> tmp_element = _top_limiter->next;

        while (tmp_element != _lower_limiter)
        {
            std::cout << tmp_element->data << std::endl;
            tmp_element = tmp_element->next;
        }

        std::cout << std::endl;
    }

    void clear()
    {
        while (_size > 0)
        {
            pop();
        }
        // std::shared_ptr<Element<T>> current = _top_limiter->next;
        // std::shared_ptr<Element<T>> next = nullptr;

        // while (current != _lower_limiter)
        // {
        //     next = current->next;
        //     current.reset();
        //     current = next;
        // }
        // _top_limiter->next = _lower_limiter;
        // _lower_limiter->prev = _top_limiter;

        // _size = 0;
    }

    static void switch_containers(Container &container_1, Container &container_2)
    {
        std::swap(container_1._top_limiter, container_2._top_limiter);
        std::swap(container_1._lower_limiter, container_2._lower_limiter);
        std::swap(container_1._size, container_2._size);
    }
};
