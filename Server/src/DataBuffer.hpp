#pragma once

#include <mutex>
#include <vector>
#include <boost/array.hpp>

class DataBuffer
{
private:
	struct Data
	{
		float x = 0;
		float y = 0;
		float z = 0;

		float angleOrientation = 0;

		std::size_t id = -1;
	};

public:
	static inline constexpr int dataSize = sizeof(Data);
	DataBuffer() = default;
	DataBuffer(std::size_t maxNrOfPlayers)
	{
		std::lock_guard<std::mutex> lock(data_mtx_);
		data_.reserve(maxNrOfPlayers);
	}
	void UpdateElement(const Data& rhs)
	{
		std::lock_guard<std::mutex> lock(data_mtx_);
		data_[rhs.id] = rhs;
	}
	void UpdateElement(
		const boost::array<std::uint8_t, DataBuffer::dataSize>& buffer, const std::size_t& id)
	{
		Data data;
		memcpy_s(&data, dataSize, &buffer, sizeof(buffer));
		data.id = id;

		std::lock_guard<std::mutex> lock(data_mtx_);
		UpdateElement(data);
	}
	std::vector<Data>& GetDataVector()
	{
		std::lock_guard<std::mutex> lock(data_mtx_);
		return data_;
	}
	void CreateNewElement(const std::size_t& id)
	{ 
		Data aux;
		aux.id = id;

		std::lock_guard<std::mutex> lock(data_mtx_);
		data_.push_back(aux);
	}
	void AddNewElement(const Data& rhs)
	{
		std::lock_guard<std::mutex> lock(data_mtx_);
		data_.push_back(rhs);
	}

private:
	std::mutex data_mtx_;
	std::vector<Data> data_;
};