#include "math.h"

namespace ml {
    std::random_device rd;

    std::uniform_int_distribution<long long int> dist(0, std::llround(std::pow(2, 63)));

    size_t random(size_t min, size_t max) {
        std::mt19937_64 e2(rd());

        return min + dist(e2) % (max - min + 1);
    }

    size_t random(size_t min, size_t max, size_t seed) {
        std::mt19937_64 e2(rd());
        e2.seed(seed);

        return min + dist(e2) % (max - min + 1);
    }
}
