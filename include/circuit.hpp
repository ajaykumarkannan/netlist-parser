#pragma once

#include "component.hpp"
#include "node.hpp"

#include <deque>
#include <map>
#include <utility>


class Circuit
{
public:
    void AddComponent(Component* component)
    {
        // Store component and add information to connecting nodes
        components_.push_back(component);

        typename Component::node_iterator it, end;
        std::make_pair(it, end) = component.Nodes();

        for (; it != end; ++it)
        {
            // Add *this* component to all connected nodes
            id2node_[it->Id()].AddComponent(component);
        }
    }

private:
    std::map<int, Node> id2node_;
    std::deque<Components *> components_;
};
