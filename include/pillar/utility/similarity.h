#pragma once
#include <vector>

#include "pillar/utility/ext/math.h"

namespace yuzu
{
namespace math
{
float cosineSimilarity(float* A, float* B, unsigned int len);

// Calculate cosine sililarity of vectors.
template <class T = float>
std::vector<T> cosineSimilarity(const std::vector<T>& vectorA, const std::vector<std::vector<T>>& matrix)
{
    T vectorANorm = ext::norm(vectorA);
    std::vector<T> similarity;
    similarity.reserve(matrix.size());

    for (const std::vector<T>& vectorB : matrix)
    {
        T dot = ext::dotProduct(vectorA, vectorB);
        T vectorBNorm = ext::norm(vectorB);
        similarity.emplace_back(dot / (vectorANorm * vectorBNorm));
    }

    return similarity;
}
} // namespace math
} // namespace yuzu
