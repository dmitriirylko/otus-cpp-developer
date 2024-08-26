#pragma once

#include <cassert>

template<typename T, typename Alloc = std::allocator<T>>
class ForwardList
{
private:
    struct Node
    {
        Node(T item) : data{std::move(item)}
        {}
        Node *next = nullptr;
        T data;
    };
    Node *m_head = nullptr;
    Node *m_tail = nullptr;
    using AllocNode = typename std::allocator_traits<Alloc>::rebind_alloc<Node>;
    AllocNode alloc;

public:
    using value_type = T;
    using reference = T&;
    using const_reference = const T&;
    using difference_type = ptrdiff_t;
    using size_type = size_t;
    using pointer = T*;
    using const_pointer = const T*;

    class ForwardListConstIterator
    {
    private:
        friend class ForwardList;
        explicit ForwardListConstIterator(const Node *ptr) noexcept :
            m_current{ptr}
        {}
        const Node* get() const noexcept
        {
            return m_current;
        }
        const Node *m_current;

    public:
        using difference_type = ForwardList::difference_type;
        using value_type = ForwardList::value_type;
        using pointer = ForwardList::const_pointer;
        using reference = ForwardList::const_reference;
        using iterator_category = std::forward_iterator_tag;
        reference operator*() const noexcept
        {
            assert(m_current != nullptr);
            return m_current->data;
        }
        ForwardListConstIterator& operator++() noexcept
        {
            assert(m_current != nullptr);
            m_current = m_current->next;
            return *this;
        }
        ForwardListConstIterator operator++(int) noexcept
        {
            assert(m_current != nullptr);
            ForwardListConstIterator iterCopy = *this;
            m_current = m_current->next;
            return iterCopy;
        }
        bool operator==(ForwardListConstIterator &other) const noexcept
        {
            return m_current == other.m_current;
        }
        bool operator!=(ForwardListConstIterator &other) const noexcept
        {
            return m_current != other.m_current;
        }
    };

    class ForwardListIterator
    {
    private:
        friend class ForwardList;
        explicit ForwardListIterator(Node *ptr) noexcept :
            m_current{ptr}
        {}
        Node* get() const noexcept
        {
            return m_current;
        }
        Node *m_current;

    public:
        using difference_type = ForwardList::difference_type;
        using value_type = ForwardList::value_type;
        using pointer = ForwardList::const_pointer;
        using reference = ForwardList::reference;
        using iterator_category = std::forward_iterator_tag;
        reference operator*() const noexcept
        {
            assert(m_current != nullptr);
            return m_current->data;
        }
        ForwardListIterator& operator++() noexcept
        {
            assert(m_current != nullptr);
            m_current = m_current->next;
            return *this;
        }
        ForwardListIterator operator++(int) noexcept
        {
            assert(m_current != nullptr);
            ForwardListIterator iterCopy = *this;
            m_current = m_current->next;
            return iterCopy;
        }
        bool operator==(ForwardListIterator other) const noexcept
        {
            return m_current == other.m_current;
        }
        bool operator!=(ForwardListIterator other) const noexcept
        {
            return m_current != other.m_current;
        }
    };

    using iterator = ForwardListIterator;
    using const_iterator = ForwardListConstIterator;

    ForwardList() = default;
    void clear()
    {
        while(m_head)
        {
            Node *nodePtrCopy = m_head;
            m_head = m_head->next;
            std::allocator_traits<AllocNode>::destroy(alloc, nodePtrCopy);
            std::allocator_traits<AllocNode>::deallocate(alloc, nodePtrCopy, 1);
        }
        m_tail = nullptr;
    }
    ~ForwardList()
    {
        clear();
    }
    void push_front(value_type item)
    {
        Node *newNode = std::allocator_traits<AllocNode>::allocate(alloc, 1);
        std::allocator_traits<AllocNode>::construct(alloc, newNode, item);
        if(m_head)
        {
            newNode->next = m_head;
            m_head = newNode;
        }
        else
        {
            m_head = newNode;
            m_tail = newNode;
        }
    }
    void push_back(value_type item)
    {
        Node *newNode = std::allocator_traits<AllocNode>::allocate(alloc, 1);
        std::allocator_traits<AllocNode>::construct(alloc, newNode, item);
        if(m_tail)
        {
            m_tail->next = newNode;
            m_tail = newNode;
        }
        else
        {
            m_head = newNode;
            m_tail = newNode;
        }
    }
    void insert_after(iterator place, value_type item)
    {
        Node *nodePtr = place.get();
        if(!nodePtr)
        {
            push_front(std::move(item));
            return;
        }
        Node *newNode = std::allocator_traits<AllocNode>::allocate(alloc, 1);
        std::allocator_traits<AllocNode>::construct(alloc, newNode, item);
        newNode->next = nodePtr->next;
        nodePtr->next = newNode;
    }
    iterator begin() const noexcept
    {
        return iterator{m_head};
    }
    iterator end() const noexcept
    {
        return iterator{nullptr};
    }
    const_iterator cbegin() const noexcept
    {
        return const_iterator{m_head};
    }
    const_iterator cend() const noexcept
    {
        return const_iterator{nullptr};
    }
    iterator find(const_reference item) const noexcept
    {
        for(auto it = begin(); it != end(); ++it)
        {
            if(*it == item)
            {
                return it;
            }
        }
        return iterator{nullptr};
    }
};