#include <cyng/vm/memory.h>
#include <boost/assert.hpp>

namespace docscript {

	memory::memory()
		: mem_()
		, pc_(0)
	{}

	memory::memory(memory&& mem) noexcept
		: mem_(std::move(mem.mem_))
		, pc_(mem.pc_)
	{}

	memory::memory(deque_t&& deq) noexcept
		: mem_(std::move(deq))
		, pc_(0)
	{}

	bool memory::step()
	{
		BOOST_ASSERT(pc_ < mem_.size());
		if (pc_ < mem_.size()) {
			++pc_;
			return true;
		}
		return false;
	}

	void memory::ja(std::size_t pos)
	{
		BOOST_ASSERT(pos < mem_.size());
		pc_ = pos;
	}

	void memory::jrf(std::size_t n)
	{
		pc_ += n;
		BOOST_ASSERT(pc_ < mem_.size());
	}

	void memory::jrb(std::size_t n)
	{
		BOOST_ASSERT(n < pc_);
		if (n < pc_)	pc_ -= n;
	}

	memory::operator bool() const noexcept
	{
		return pc_ < mem_.size();
	}

	object memory::operator()() const
	{
		BOOST_ASSERT(pc_ < mem_.size());
		return mem_[pc_];
	}

	std::size_t memory::op_count() const {
		return mem_.size() - pc_;
	}

}
