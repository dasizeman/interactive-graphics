#ifndef __DAVE_UTILS_H_
#define __DAVE_UTILS_H_
#include <vector>
#include <string>
#include <sstream>

namespace daveutils {
    float randomFloat(float min, float max);
    template <typename T>
    std::string printVector ( std::vector<T> &vec) {
        std::stringstream ss;
        ss << "{ ";
       for ( T item : vec )
            ss << item << ", ";

       ss << "}";
       return ss.str();
    }
}
#endif
