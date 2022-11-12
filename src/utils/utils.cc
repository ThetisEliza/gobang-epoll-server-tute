#include <string>
#include "utils.hh"

void Errif(bool condition, const std::string errmsg)
{
    if (condition) {
        perror(errmsg.c_str());
        exit(EXIT_FAILURE);
    }
}