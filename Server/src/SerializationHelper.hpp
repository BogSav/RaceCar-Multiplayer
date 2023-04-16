#pragma once

#include "DataArray.hpp"

#include <sstream>
#include <boost/archive/binary_iarchive.hpp>
#include <boost/archive/binary_oarchive.hpp>

// boost::serialization injection
namespace boost::serialization
{

template <typename T, std::size_t N, typename Archive>
void serialize(Archive& ar, boost::array<T, N>& arr, const unsigned int version)
{
	for (std::size_t i = 0; i < N; ++i)
	{
		ar& arr[i];
	}
}

}  // namespace boost::serialization

class SerializationHelper
{
public:
	template <typename T, std::size_t N>
	static std::string SerializeDataArray(const DataArray<T, N>& array)
	{
		std::ostringstream oss;
		boost::archive::binary_oarchive oa(oss);
		oa << array;
		return oss.str();
	}

	template <typename T, std::size_t N>
	static DataArray<T, N> DeserializeDataArray(const std::string& serialized_data)
	{
		DataArray<T, N> array;
		std::istringstream iss(serialized_data);
		boost::archive::binary_iarchive ia(iss);
		ia >> array;
		return array;
	}

	static std::string SerializeClientData(const ClientData& data)
	{
		std::ostringstream oss;
		boost::archive::binary_oarchive oa(oss);
		oa << data;
		return oss.str();
	}

	static ClientData DeserializeClientData(const std::string& serialized_data)
	{
		std::istringstream iss(serialized_data);
		boost::archive::binary_iarchive ia(iss);
		ClientData s;
		ia >> s;
		return s;
	}

	static std::string SerializeClientIntialData(const ClientInitialData& data)
	{
		std::ostringstream oss;
		boost::archive::binary_oarchive oa(oss);
		oa << data;
		return oss.str();
	}

	static ClientInitialData DeserializeClientInitialData(const std::string& serialized_data)
	{
		std::istringstream iss(serialized_data);
		boost::archive::binary_iarchive ia(iss);
		ClientInitialData s;
		ia >> s;
		return s;
	}
};