#ifndef INTERACTIONS_H
# define INTERACTIONS_H

#include <unordered_map>
#include <vector>
#include <memory>


enum class Element {
    // enums representing each element, because I couldn't find a way to actully store
    // types in something.
    STEAM,
    WATER,
    SAND,
    FIRE,
    NONE
};

enum class Interaction {
    // Defines the interaction which this element has upon being near another element
    
    // E.g., for water, its FIRE interaction is REPLACE with STEAM, because the water
    // itself will handle that. But for fire, its WATER interaction is DESTROY, since
    // the water extinguishes the fire, so the fire itself will handle that.
    REPLACE,
    DESTROY
};
#endif


