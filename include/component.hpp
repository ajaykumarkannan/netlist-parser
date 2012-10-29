#pragma once

#include <string>
#include <sstream>
#include <utility>
#include <deque>


class Node;


// Component types detected by our parser
enum ComponentType { UNSET, RESISTOR, VOLTAGE_SOURCE, CURRENT_SOURCE, CAPACITOR };


/**
 * @brief Abstract base class for all component types.
 */
class Component
{
public:
    typedef std::deque<Node>::iterator node_iterator;

    /**
     * @brief Create a component from the given netlist style line.
     */
    static Component* CreateFromString(std::string strRep)
    {
        ComponentType type = UNSET;

        // First character identifies the type
        switch (strRep[0])
        {
        case 'r':
        case 'R':
            type = RESISTOR;
            break;

        case 'v':
        case 'V':
            type = VOLTAGE_SOURCE;
            break;

        case 'i':
        case 'I':
            type = CURRENT_SOURCE;
            break;

        case 'c':
        case 'C':
            type = CAPACITOR;
            break;

        case '.':
            // Line must be handled specially.
            break;

        default:
            // Type invalid or not supported
            break;
        }

        if (type == UNSET)
            return 0L; // Null pointer
    }

    std::pair<node_iterator, node_iterator> // Begin and end iterator
    Nodes()
    {
        std::make_pair(nodes_.begin(), nodes_.end());
    }

private:
    std::deque<Node> nodes_;
};


class Resistor : public Component
{
public:
    Resistor(int n1, int n2, float r)
        : n1_(n1), n2_(n2), r_(r)
    {}

    Resistor(std::string strRep)
    {
        std::istringstream iss(strRep);

        iss >> n1_ >> n2_ >> r_;
    }

private:
    int n1_;
    int n2_;
    float r_; // Resistance of the resistor
};
