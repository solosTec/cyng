/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2021 Sylko Olzscher
 *
 */
#ifndef DOCC_VM_MEMORY_H
#define DOCC_VM_MEMORY_H

#include <cyng/vm/memory_interface.h>
#include <cyng/obj/intrinsics/container.h>

namespace docscript {

	/**
	 * The memory_interface isn't really necessary - it's more an attempt
	 * to get the interface clean.
	 */
	class memory : public memory_interface
	{
	public:
		memory();
		memory(memory&&) noexcept;

		/**
		 * set pc = 0
		 */
		memory(deque_t&& deq) noexcept;

		/** @brief step to next instruction
		 *
		 * pc++
		 */
		bool step();

		/** @brief jump absolute
		 *
		 * pc = x
		 */
		virtual void ja(std::size_t) override;

		/** @brief jump relative forward
		 *
		 * pc += x
		 */
		virtual void jrf(std::size_t) override;

		/** @brief jump relative backward
		 *
		 * pc -= x
		 */
		virtual void jrb(std::size_t) override;

		/** @brief has more test
		 *
		 * @return false if program counter has reached the end.
		 */
		explicit operator bool() const noexcept;

		/**
		 * Function call operator return current value.
		 * Could throw.
		 *
		 * @return current value
		 */
		object operator()() const;

		/**
		 * available instructions
		 */
		std::size_t op_count() const;

	protected:
		deque_t mem_;

	private:
		std::size_t pc_;

	};
}
#endif

