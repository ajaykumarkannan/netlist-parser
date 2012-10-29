#pragma once

#include <deque>
#include <utility>


class Component;


/**
 * @brief A node structure to maintain connectivity information in a netlist graph.
 */
struct Node
{
public:
    typedef std::deque<Component &>::iterator component_iterator;

    Node(int id)
        : id_(id)
    {}

    int Id() const
    {
        return id_;
    }

    void AddComponent(Component& component)
    {
        components_.push_back(component);
    }

    std::pair<component_iterator, component_iterator> // Begin and end iterator
    Components()
    {
        return std::make_pair(components_.begin(), components_.end());
    }

private:
    const int id_; // Unique identifier for this node
    std::deque<Component *> components_; // Components that connet to this node
};
