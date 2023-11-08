#include <math.h>

#include "pillar/utility/similarity.h"

namespace yuzu
{
namespace math
{
float cosineSimilarity(const float* A, const float* B, unsigned int len)
{
    double dot = 0.0, denomA = 0.0, denomB = 0.0;
    for (unsigned int i = 0u; i < len; ++i)
    {
        dot += A[i] * B[i];
        denomA += A[i] * A[i];
        denomB += B[i] * B[i];
    }
    return dot / (sqrt(denomA * denomB));
}
} // namespace math
} // namespace yuzu