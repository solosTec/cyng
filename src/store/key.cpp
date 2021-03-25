#include <cyng/store/key.hpp>


namespace cyng {

	body::body()
		: data_()
		, generation_(0)
		, m_()
	{}

	body::body(data_t&& data, std::uint64_t gen)
		: data_(std::move(data))
		, generation_(gen)
		, m_()
	{}

	body::body(data_t const& data, std::uint64_t gen)
		: data_(data)
		, generation_(gen)
		, m_()
	{}

}

namespace std {

	bool equal_to<cyng::key_t>::operator()(cyng::key_t const& pk1, cyng::key_t const& pk2) const noexcept {

		static const hash<cyng::key_t> h;

		//
		//	same values generate the same hash
		//
		return (pk1.size() == pk2.size())
			? h(pk1) == h(pk2)
			: false
			;
	}

	bool less<cyng::key_t>::operator()(cyng::key_t const& pk1, cyng::key_t const& pk2) const noexcept {

		static const hash<cyng::key_t> h;
		return (pk1.size() == pk2.size())
			? h(pk1) < h(pk2)
			: true
			;
	}

}

