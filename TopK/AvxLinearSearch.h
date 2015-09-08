#include <immintrin.h>
#include <stdint.h>
#include "DataDefinition.h"
#include <vector>
#define _mm256_extract_epi32(mm, i) _mm_extract_epi32(_mm256_extractf128_si256(mm, i >= 4), i % 4)
#define _mm256_extract_epi64(mm, i) _mm_extract_epi64(_mm256_extractf128_si256(mm, i >= 2), i % 2)
template<typename T>
std::vector<uint32_t> topk_search_detail(const T* all_x, const T* all_y, uint32_t size,
	Position<T> lower, Position<T> upper, uint32_t k);
template <>
std::vector<uint32_t> topk_search_detail<float>(const float* all_x, const float* all_y, uint32_t size,
	Position<float> lower, Position<float> upper, uint32_t k)
{
	int n = 0;
	__m256 lower_x = _mm256_broadcast_ss(&lower.x);
	__m256 lower_y = _mm256_broadcast_ss(&lower.y);
	__m256 upper_x = _mm256_broadcast_ss(&upper.x);
	__m256 upper_y = _mm256_broadcast_ss(&upper.y);
	std::vector<uint32_t> result;
	uint32_t i = 0;
	for (i = 0; i < size - 7; i += 8)
	{
		__m256 x = _mm256_load_ps(all_x + i);
		__m256 y = _mm256_load_ps(all_y + i);

		__m256 x_in = _mm256_and_ps(_mm256_cmp_ps(lower_x, x, _CMP_LE_OQ),
			_mm256_cmp_ps(upper_x, x, _CMP_GE_OQ));
		__m256 y_in = _mm256_and_ps(_mm256_cmp_ps(lower_y, y, _CMP_LE_OQ),
			_mm256_cmp_ps(upper_y, y, _CMP_GE_OQ));

		if (!_mm256_testz_ps(x_in, y_in))
		{
			__m256i mask = _mm256_castps_si256(_mm256_and_ps(x_in, y_in));
			if (_mm256_extract_epi32(mask, 0))
			{
				result.push_back(i + 0); n++;
			}
			if (_mm256_extract_epi32(mask, 1))
			{
				result.push_back(i + 1); n++;
			}
			if (_mm256_extract_epi32(mask, 2))
			{
				result.push_back(i + 2); n++;
			}
			if (_mm256_extract_epi32(mask, 3))
			{
				result.push_back(i + 3); n++;
			}
			if (_mm256_extract_epi32(mask, 4))
			{
				result.push_back(i + 4); n++;
			}
			if (_mm256_extract_epi32(mask, 5))
			{
				result.push_back(i + 5); n++;
			}
			if (_mm256_extract_epi32(mask, 6))
			{
				result.push_back(i + 6); n++;
			}
			if (_mm256_extract_epi32(mask, 7))
			{
				result.push_back(i + 7); n++;
			}
		}
		if (n >= k)
		{
			while (n != k)
			{
				result.pop_back();
				n--;
			}
			return result;
		}
	}
	for (; i < size; i++)
	{
		if (all_x[i] <= upper.x&&all_x[i] >= lower.x&&
			all_y[i] <= upper.y&&all_y[i] >= lower.y)
		{
			n++;
			result.push_back(i);
			if (n == k)
			{
				return result;
			}
		}
	}
	return result;
}
template <>
std::vector<uint32_t> topk_search_detail<double>(const double* all_x, const double* all_y, uint32_t size,
	Position<double> lower, Position<double> upper, uint32_t k)
{
	int n = 0;
	__m256d lower_x = _mm256_broadcast_sd(&lower.x);
	__m256d lower_y = _mm256_broadcast_sd(&lower.y);
	__m256d upper_x = _mm256_broadcast_sd(&upper.x);
	__m256d upper_y = _mm256_broadcast_sd(&upper.y);
	std::vector<uint32_t> result;
	uint32_t i = 0;
	for (i = 0; i < size - 3; i += 4)
	{
		__m256d x = _mm256_load_pd(all_x + i);
		__m256d y = _mm256_load_pd(all_y + i);

		__m256d x_in = _mm256_and_pd(_mm256_cmp_pd(lower_x, x, _CMP_LE_OQ),
			_mm256_cmp_pd(upper_x, x, _CMP_GE_OQ));
		__m256d y_in = _mm256_and_pd(_mm256_cmp_pd(lower_y, y, _CMP_LE_OQ),
			_mm256_cmp_pd(upper_y, y, _CMP_GE_OQ));

		if (!_mm256_testz_pd(x_in, y_in))
		{
			__m256i mask = _mm256_castpd_si256(_mm256_and_pd(x_in, y_in));
			if (_mm256_extract_epi64(mask, 0))
			{
				result.push_back(i + 0); n++;
			}
			if (_mm256_extract_epi64(mask, 1))
			{
				result.push_back(i + 1); n++;
			}
			if (_mm256_extract_epi64(mask, 2))
			{
				result.push_back(i + 2); n++;
			}
			if (_mm256_extract_epi64(mask, 3))
			{
				result.push_back(i + 3); n++;
			}

		}
		if (n >= k)
		{
			while (n != k)
			{
				result.pop_back();
				n--;
			}
			return result;
		}
	}
	for (; i < size; i++)
	{
		if (all_x[i] <= upper.x&&all_x[i] >= lower.x&&
			all_y[i] <= upper.y&&all_y[i] >= lower.y)
		{
			n++;
			result.push_back(i);
			if (n == k)
			{
				return result;
			}
		}
	}
	return result;
}


template<typename T>
std::vector<uint32_t> topk_search_detail(const PackedPos<T>* all_pos, uint32_t size,
	Position<T> lower, Position<T> upper, uint32_t k);
template<>
std::vector<uint32_t> topk_search_detail<float>(const PackedPos<float>* all_pos, uint32_t size,
	Position<float> lower, Position<float> upper, uint32_t k)
{
	__declspec(align(16)) float searchWindow[4];
	searchWindow[0] = upper.x;
	searchWindow[1] = upper.y;
	searchWindow[2] = -lower.x;
	searchWindow[3] = -lower.y;
	__m128 searchWindow128 = _mm_load_ps(searchWindow);
	int n = 0;
	std::vector<uint32_t> result;
	for (uint32_t i = 0; i < size; i++)
	{
		__m128 current_node = _mm_load_ps(reinterpret_cast<const float*>(all_pos + i));
		__m128  cmp_result = _mm_cmp_ps(current_node, searchWindow128, _CMP_LE_OQ);
		if (_mm_testz_ps(cmp_result, cmp_result))
		{
			result.push_back(i);
			n++;
			if (n == k)
			{
				return result;
			}
		}
	}
}
template<>
std::vector<uint32_t> topk_search_detail<double>(const PackedPos<double>* all_pos, uint32_t size,
	Position<double> lower, Position<double> upper, uint32_t k)
{
	__declspec(align(32)) double searchWindow[4];
	searchWindow[0] = upper.x;
	searchWindow[1] = upper.y;
	searchWindow[2] = -lower.x;
	searchWindow[3] = -lower.y;
	__m256d searchWindow256 = _mm256_load_pd(searchWindow);
	int n = 0;
	std::vector<uint32_t> result;
	for (uint32_t i = 0; i < size; i++)
	{
		__m256d current_node = _mm256_load_pd(reinterpret_cast<const double*>(all_pos + i));
		__m256d  cmp_result = _mm256_cmp_pd(current_node, searchWindow256, _CMP_LE_OQ);
		if (_mm256_testz_pd(cmp_result, cmp_result))
		{
			result.push_back(i);
			n++;
			if (n == k)
			{
				return result;
			}
		}
	}
}