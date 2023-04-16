#pragma once

#include <boost/array.hpp>
#include <boost/serialization/access.hpp>
#include <boost/serialization/nvp.hpp>

#include <mutex>
#include <thread>

#if defined(__x86_64__)
#define CACHE_LINE_SIZE 64
#elif defined(__arm__)
#define CACHE_LINE_SIZE 64
#elif defined(__powerpc__)
#define CACHE_LINE_SIZE 128
#else
#define CACHE_LINE_SIZE 64
#endif


struct ClientInitialData
{
	////////////////////////////////////////////////////////////////////
	// Daca adaugi o variabila noua, adaug-o si in metoda serialize!!!!
	// Also, modifica si dimensiunea padding-ului si modul de calcul!!!
	////////////////////////////////////////////////////////////////////
	std::size_t nrOfClients;
	std::size_t id;

private:
	static_assert(CACHE_LINE_SIZE > sizeof(std::size_t) * 2);
	char padding[CACHE_LINE_SIZE - sizeof(std::size_t) * 2];

	// Serializare
	friend class boost::serialization::access;

	template <typename Archive>
	void serialize(Archive& ar, const unsigned int version)
	{
		ar& nrOfClients;
		ar& id;
	}
};

struct ClientData
{
	////////////////////////////////////////////////////////////////////
	// Daca adaugi o variabila noua, adaug-o si in metoda serialize!!!!
	// Also, modifica si dimensiunea padding-ului si modul de calcul!!!
	////////////////////////////////////////////////////////////////////
	float posX, posY, posZ;
	float OXangle;
	std::size_t id;  // For verification only

private:
	static_assert(CACHE_LINE_SIZE > sizeof(std::size_t) + sizeof(float) * 4);
	char padding[CACHE_LINE_SIZE - sizeof(std::size_t) - sizeof(float) * 4];

	// Serializare
	friend class boost::serialization::access;

	template <typename Archive>
	void serialize(Archive& ar, const unsigned int version)
	{
		ar& posX;
		ar& posY;
		ar& posZ;
		ar& OXangle;
		ar& id;
	}
};

template <typename T, std::size_t N>
class DataArray : public boost::array<T, N>
{
public:
	using base_type = boost::array<T, N>;

	DataArray() : base_type() {}
	~DataArray() = default;

	T& at(std::size_t index)
	{
		std::lock_guard<std::mutex> lock(mutex_);
		return base_type::at(index);
	}

	const T& at(std::size_t index) const
	{
		std::lock_guard<std::mutex> lock(mutex_);
		return base_type::at(index);
	}

	T& operator[](std::size_t index) { return at(index); }

	const T& operator[](std::size_t index) const { return at(index); }

	void fill(const T& value)
	{
		std::lock_guard<std::mutex> lock(mutex_);
		base_type::fill(value);
	}

	void swap(DataArray& other)
	{
		if (this == &other)
			return;

		std::lock(mutex_, other.mutex_);
		std::lock_guard<std::mutex> lock1(mutex_, std::adopt_lock);
		std::lock_guard<std::mutex> lock2(other.mutex_, std::adopt_lock);
		base_type::swap(other);
	}

	base_type& unsafe_array() { return *this; }
	const base_type& unsafe_array() const { return *this; }

private:
	mutable std::mutex mutex_;

private:
	// Boost serialization helper
	friend class boost::serialization::access;

	template <typename Archive>
	void serialize(Archive& ar, const unsigned int version)
	{
		std::lock_guard<std::mutex> lock(mutex_);
		ar& BOOST_SERIALIZATION_BASE_OBJECT_NVP(base_type);
	}
};

static inline constexpr std::size_t clientInitialDataSize = sizeof(ClientInitialData);
static inline constexpr std::size_t clientDataSize = sizeof(ClientData);
static inline constexpr std::size_t maxNumberOfClients = 2;