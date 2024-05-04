#include "Whatchamacallit.h"

#include <cstring>

bool ASubsetOfB(const std::vector<const char*> &A, const std::vector<const char*> &B)
{
    for(const auto &a : A)
    {
        bool found = false;
        for(const auto &b : B)
        {
            if(strcmp(a, b) == 0)
            {
                found = true;
                break;
            }
        }
        if(!found)
            return false;
    }
    return true;
}