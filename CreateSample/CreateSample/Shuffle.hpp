#pragma once

#include <algorithm>
#include <random>
#include <unordered_map>
#include <vector>

using namespace std;

mt19937 create_rand_engine() {
	random_device rnd;
	vector<std::uint_least32_t> v(10);// 初期化用ベクタ
	generate(v.begin(), v.end(), ref(rnd));// ベクタの初期化
	seed_seq seed(v.begin(), v.end());
	return std::mt19937(seed);// 乱数エンジン
}

namespace detail {
	template<typename T> auto diff(T n1, T n2) -> typename std::make_unsigned<T>::type {
		static_assert(std::is_integral<T>::value, "T is not integral.");
		if (n1 < n2) std::swap(n1, n2);
		return static_cast<typename std::make_unsigned<T>::type>(n1 - n2);
	}
}

template<typename type> vector<type> make_nonrepeat_rand_array_select_with_hash(const size_t size, type rand_min, type rand_max) {
	if (rand_min > rand_max) swap(rand_min, rand_max);
	auto max_min_diff = detail::diff(rand_max, rand_min) + 1;
	if (max_min_diff < size) throw runtime_error("Invalid argument");
	using hash_map = unordered_map<type, type>;

	vector<type> tmp;
	tmp.reserve(size);

	hash_map Map;

	auto engine = create_rand_engine();
	for (size_t cnt = 0; cnt < size; ++cnt) {
		type val = uniform_int_distribution<type>(rand_min, rand_max)(engine);
		auto itr = Map.find(val);

		type replaced_val;
		auto replaced_itr = Map.find(rand_max);
		if (replaced_itr != Map.end()) replaced_val = replaced_itr->second;
		else replaced_val = rand_max;

		if (itr == Map.end()) {
			tmp.push_back(val);
			if (val != rand_max)Map.insert(std::make_pair(val, replaced_val));
		}
		else {
			tmp.push_back(itr->second);
			itr->second = replaced_val;
		}

		//Map.erase(replaced_val);

		--rand_max;
	}

	return tmp;
}
