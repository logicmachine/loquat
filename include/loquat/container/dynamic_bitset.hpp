#pragma once
#include <memory>
#include <stdexcept>
#include <cstdint>
#include "loquat/math/bitmanip.hpp"

namespace loquat {

class dynamic_bitset {

public:
	class reference {

	private:
		friend class dynamic_bitset;

		uint64_t *m_ptr;
		size_t m_shift;

		reference() noexcept
			: m_ptr(nullptr)
			, m_shift(0)
		{ }

		reference(uint64_t *ptr, size_t shift)
			: m_ptr(ptr)
			, m_shift(shift)
		{ }

		void operator&(); // not defined

	public:
		~reference() noexcept = default;

		reference& operator=(bool value){
			if(value){
				*m_ptr |= (1ull << m_shift);
			}else{
				*m_ptr &= ~(1ull << m_shift);
			}
			return *this;
		}

		reference& operator=(const reference& rhs) noexcept {
			if(rhs){
				*m_ptr |= (1ull << m_shift);
			}else{
				*m_ptr &= ~(1ull << m_shift);
			}
			return *this;
		}

		reference& operator&=(bool value) noexcept {
			if(!value){ *m_ptr &= ~(1ull << m_shift); }
			return *this;
		}

		reference& operator|=(bool value) noexcept {
			if(value){ *m_ptr |= (1ull << m_shift); }
			return *this;
		}

		reference& operator^=(bool value) noexcept {
			if(value){ *m_ptr ^= (1ull << m_shift); }
			return *this;
		}

		bool operator~() const noexcept {
			return ((*m_ptr >> m_shift) & 1) == 0;
		}

		operator bool() const {
			return ((*m_ptr >> m_shift) & 1) != 0;
		}

	};

private:
	size_t m_size;
	std::unique_ptr<uint64_t[]> m_bits;

	size_t block_count() const noexcept {
		return (m_size + 63) / 64;
	}

	uint64_t last_mask() const noexcept {
		if(m_size % 64 == 0){
			return ~0ull;
		}else{
			return (1ull << (m_size % 64)) - 1;
		}
	}

public:
	dynamic_bitset() noexcept
		: m_size(0)
		, m_bits()
	{ }

	dynamic_bitset(size_t n, bool value = false)
		: m_size(n)
		, m_bits(new uint64_t[(n + 63) / 64])
	{
		if(value){
			set();
		}else{
			reset();
		}
	}

	dynamic_bitset(const dynamic_bitset& s)
		: m_size(s.m_size)
		, m_bits(new uint64_t[(m_size + 63) / 64])
	{
		const auto m = block_count();
		for(size_t i = 0; i < m; ++i){ m_bits[i] = s.m_bits[i]; }
	}

	dynamic_bitset(dynamic_bitset&& s) noexcept
		: m_size(s.m_size)
		, m_bits(std::move(s.m_bits))
	{
		s.m_size = 0;
	}


	dynamic_bitset& operator=(const dynamic_bitset& rhs){
		const auto m = rhs.block_count();
		std::unique_ptr<uint64_t[]> bits(new uint64_t[m]);
		for(size_t i = 0; i < m; ++i){ bits[i] = rhs.m_bits[i]; }
		m_size = rhs.m_size;
		m_bits = std::move(bits);
		return *this;
	}

	dynamic_bitset& operator=(dynamic_bitset&& rhs) noexcept {
		m_size = rhs.m_size;
		m_bits = std::move(rhs.m_bits);
		rhs.m_size = 0;
		return *this;
	}


	bool operator==(const dynamic_bitset& rhs) const noexcept {
		if(m_size != rhs.m_size){ return false; }
		const auto m = block_count();
		for(size_t i = 0; i < m; ++i){
			if(m_bits[i] != rhs.m_bits[i]){ return false; }
		}
		return true;
	}

	bool operator!=(const dynamic_bitset& rhs) const noexcept {
		return !(*this == rhs);
	}


	dynamic_bitset& operator&=(const dynamic_bitset& rhs){
		if(m_size != rhs.m_size){ throw std::logic_error("size mismatch"); }
		const auto m = block_count();
		for(size_t i = 0; i < m; ++i){ m_bits[i] &= rhs.m_bits[i]; }
		return *this;
	}

	dynamic_bitset operator&(const dynamic_bitset &rhs) const {
		return dynamic_bitset(*this) &= rhs;
	}

	dynamic_bitset& operator|=(const dynamic_bitset& rhs){
		if(m_size != rhs.m_size){ throw std::logic_error("size mismatch"); }
		const auto m = block_count();
		for(size_t i = 0; i < m; ++i){ m_bits[i] |= rhs.m_bits[i]; }
		return *this;
	}

	dynamic_bitset operator|(const dynamic_bitset &rhs) const {
		return dynamic_bitset(*this) |= rhs;
	}

	dynamic_bitset& operator^=(const dynamic_bitset& rhs){
		if(m_size != rhs.m_size){ throw std::logic_error("size mismatch"); }
		const auto m = block_count();
		for(size_t i = 0; i < m; ++i){ m_bits[i] ^= rhs.m_bits[i]; }
		return *this;
	}

	dynamic_bitset operator^(const dynamic_bitset &rhs) const {
		return dynamic_bitset(*this) ^= rhs;
	}

	dynamic_bitset operator~() const {
		return dynamic_bitset(*this).flip();
	}

	dynamic_bitset& operator<<=(size_t shift) noexcept {
		const size_t c = shift / 64, f = shift % 64, m = block_count();
		if(f == 0){
			for(size_t ii = 0; ii < m; ++ii){
				const auto i = m - 1 - ii;
				m_bits[i] = (i >= c) ? m_bits[i - c] : 0;
			}
		}else{
			for(size_t ii = 0; ii < m; ++ii){
				const auto i  = m - 1 - ii;
				const auto hi = (i >= c) ? m_bits[i - c] : 0;
				const auto lo = (i >= c + 1) ? m_bits[i - c - 1] : 0;
				m_bits[i] = (hi << f) | (lo >> (64 - f));
			}
		}
		return *this;
	}

	dynamic_bitset operator<<(size_t shift){
		return dynamic_bitset(*this) <<= shift;
	}

	dynamic_bitset& operator>>=(size_t shift) noexcept {
		const size_t c = shift / 64, f = shift % 64, m = block_count();
		if(f == 0){
			for(size_t i = 0; i < m; ++i){
				m_bits[i] = (i + c < m) ? m_bits[i + c] : 0;
			}
		}else{
			for(size_t i = 0; i < m; ++i){
				const auto hi = (i + c + 1 < m) ? m_bits[i + c + 1] : 0;
				const auto lo = (i + c < m) ? m_bits[i + c] : 0;
				m_bits[i] = (hi << (64 - f)) | (lo >> f);
			}
		}
		return *this;
	}

	dynamic_bitset operator>>(size_t shift){
		return dynamic_bitset(*this) >>= shift;
	}

	dynamic_bitset& set() noexcept {
		const auto m = block_count();
		for(size_t i = 0; i + 1 < m; ++i){ m_bits[i] = ~0ull; }
		if(m > 0){ m_bits[m - 1] = last_mask(); }
		return *this;
	}

	dynamic_bitset& set(size_t pos, bool value = true) noexcept {
		if(value){
			m_bits[pos / 64] |= (1ull << (pos % 64));
		}else{
			m_bits[pos / 64] &= ~(1ull << (pos % 64));
		}
		return *this;
	}

	dynamic_bitset& reset() noexcept {
		const auto m = block_count();
		for(size_t i = 0; i < m; ++i){ m_bits[i] = 0; }
		return *this;
	}

	dynamic_bitset& reset(size_t pos) noexcept {
		m_bits[pos / 64] &= ~(1ull << (pos % 64));
		return *this;
	}

	dynamic_bitset& flip() noexcept {
		const auto m = block_count();
		for(size_t i = 0; i + 1 < m; ++i){ m_bits[i] = ~m_bits[i]; }
		if(m > 0){ m_bits[m - 1] ^= last_mask(); }
		return *this;
	}

	dynamic_bitset& flip(size_t pos) noexcept {
		m_bits[pos / 64] ^= (1ull << (pos % 64));
		return *this;
	}


	bool operator[](size_t i) const noexcept {
		return test(i);
	}

	reference operator[](size_t i) noexcept {
		return reference(&m_bits[i / 64], i % 64);
	}

	size_t count() const noexcept {
		const auto m = block_count();
		size_t sum = 0;
		for(size_t i = 0; i < m; ++i){ sum += bitmanip::popcount(m_bits[i]); }
		return sum;
	}

	bool test(size_t i) const noexcept {
		return ((m_bits[i / 64] >> (i % 64)) & 1) != 0;
	}


	bool empty() const noexcept {
		return m_size == 0;
	}

	size_t size() const noexcept {
		return m_size;
	}

};

}

