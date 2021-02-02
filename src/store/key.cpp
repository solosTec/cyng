#include <cyng/store/key.hpp>


namespace docscript {

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

	bool equal_to<docscript::key_t>::operator()(docscript::key_t const& pk1, docscript::key_t const& pk2) const noexcept {

		static const hash<docscript::key_t> h;

		//
		//	same values generate the same hash
		//
		return (pk1.size() == pk2.size())
			? h(pk1) == h(pk2)
			: false
			;
	}
}

