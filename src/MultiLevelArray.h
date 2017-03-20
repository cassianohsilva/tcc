/*
 * MultiLevelArray.h
 *
 *  Created on: 16 de mar de 2017
 *      Author: cassiano
 */

#ifndef SRC_MULTILEVELARRAY_H_
#define SRC_MULTILEVELARRAY_H_

#include <cstddef>
#include <utility>
#include <vector>

template<class T>
class MultiLevelArray {
public:
	MultiLevelArray(std::size_t sizeByLevel = 100, std::size_t numLevels = 4);
	virtual ~MultiLevelArray();

	void add(const T& element, std::size_t level);

	void fitAllLevels(bool exactFit = false);
	void fit(std::size_t level, bool exactFit = false);
	void fit(std::initializer_list<bool> fits);

	void remove(const std::pair<std::size_t, std::size_t>& position)
			throw (std::overflow_error);

	T& operator[](const std::pair<std::size_t, std::size_t>& position)
			throw (std::overflow_error);
	const T& operator[](
			const std::pair<std::size_t, std::size_t>& position) const
					throw (std::overflow_error);

	class Iterator {
	public:
		Iterator(MultiLevelArray<T> * array, std::size_t level,
				std::size_t position = 0);
		Iterator(MultiLevelArray<T> * array,
				std::pair<std::size_t, std::size_t> position = { 0, 0 });

		bool operator==(const Iterator& it2);
		bool operator!=(const Iterator& it2);
		Iterator& operator++(int);

		inline const std::pair<std::size_t, std::size_t>& getPosition() const {
			return position;
		}

	private:
		MultiLevelArray<T> *array;
		bool allLevels;

		std::pair<std::size_t, std::size_t> position;
	};

	void remove(const Iterator& iterator)
				throw (std::overflow_error);

	Iterator begin();
	Iterator end();

	Iterator begin(std::size_t level);
	Iterator end(std::size_t level);

private:

	bool checkBounds(const std::pair<std::size_t, std::size_t>& position);

	std::vector<std::pair<std::size_t, std::size_t>> sizes;
	T** data;
	std::size_t defaultSize;
};

template<class T>
inline MultiLevelArray<T>::MultiLevelArray(std::size_t sizeByLevel,
		std::size_t numLevels) :
		sizes( { 0, sizeByLevel }, numLevels), data(
				new T*[numLevels] { nullptr }), defaultSize(sizeByLevel) {

	for (auto i = 0; i < sizeByLevel; ++i) {
		data[i] = new T[sizeByLevel];
	}
}

template<class T>
inline MultiLevelArray<T>::~MultiLevelArray() {

	for (auto i = 0; i < sizes.size(); ++i) {
		delete data[i];
	}

	delete data;
}

template<class T>
inline T& MultiLevelArray<T>::operator [](
		const std::pair<std::size_t, std::size_t>& position)
				throw (std::overflow_error) {

	if ((position.first < sizes.size())
			&& (position.second < sizes[position.first].second)) {
		return data[position.first][position.second];
	} else {
		throw std::overflow_error(
				"Trying to access position " + std::to_string(position.first)
						+ ", " + std::to_string(position.second));
	}
}

template<class T>
inline void MultiLevelArray<T>::add(const T& element, std::size_t level) {
	if (level >= sizes.size()) {
		auto newData = new T*[level] { nullptr };

		for (auto i = 0; i < sizes.size(); ++i) {
			newData[i] = data[i];
		}

		for (auto i = sizes.size(); i < level; ++i) {
			newData = new T[defaultSize];
			sizes.push_back( { 0, defaultSize });
		}

		delete data;

		data = newData;
	} else if (sizes[level].first >= sizes[level].second) {
		auto newData = new T[sizes[level].second << 1];

		for (auto i = 0; i < sizes[level].second; ++i) {
			newData[i] = data[level][i];
		}

		delete data[level];

		data[level] = newData;
	}

	data[level][sizes[level].first + 1] = element;

	sizes[level].first++;
}

template<class T>
inline const T& MultiLevelArray<T>::operator [](
		const std::pair<std::size_t, std::size_t>& position) const
				throw (std::overflow_error) {

	if ((position.first < sizes.size())
			&& (position.second < sizes[position.first].second)) {
		return data[position.first][position.second];
	} else {
		throw std::overflow_error(
				"Trying to access position " + std::to_string(position.first)
						+ ", " + std::to_string(position.second));
	}
}

template<class T>
inline MultiLevelArray<T>::Iterator::Iterator(MultiLevelArray<T>* array,
		std::size_t level, std::size_t position) :
		array(array), allLevels(false), position { level, position } {
}

template<class T>
inline MultiLevelArray<T>::Iterator::Iterator(MultiLevelArray<T>* array,
		std::pair<std::size_t, std::size_t> position) :
		array(array), allLevels(true), position(position) {

}

template<class T>
inline bool MultiLevelArray<T>::Iterator::operator ==(const Iterator& it2) {

	return (this->allLevels == it2.allLevels) && (this->array == it2.array)
			&& (this->position == it2.position);
}

template<class T>
inline bool MultiLevelArray<T>::Iterator::operator !=(const Iterator& it2) {

	return !(*this == it2);
}

template<class T>
typename MultiLevelArray<T>::Iterator& MultiLevelArray<T>::Iterator::operator++(
		int) {

	if (!allLevels) {
		++this->position.second;
	} else {
		if ((++this->position.second)
				== array->sizes[this->position.first].second) {
			++this->position.first;
			this->position.second = 0;
		}
	}
}

template<class T>
inline typename MultiLevelArray<T>::Iterator MultiLevelArray<T>::begin() {
	return Iterator(this);
}

template<class T>
inline typename MultiLevelArray<T>::Iterator MultiLevelArray<T>::end() {
	return Iterator(this, { sizes.size(), 0 });
}

template<class T>
inline typename MultiLevelArray<T>::Iterator MultiLevelArray<T>::begin(
		std::size_t level) {
	return Iterator(this, level);
}

template<class T>
void MultiLevelArray<T>::fitAllLevels(bool exactFit) {

	for (auto i = 0; i < sizes.size(); ++i) {

		fit(i, exactFit);
	}
}

template<class T>
void MultiLevelArray<T>::fit(std::size_t level, bool exactFit) {
	auto& size = sizes[level];

	auto newData = data[level];

	if (exactFit) {
		if (size.first < size.second) {
			newData = new T[size.first];
		}
	} else {
		if (size.second > (size.first << 1)) {
			newData = new T[size.first << 1];
		}
	}

	if (newData != data[level]) {
		for (auto j = 0; j < size.first; ++j) {
			newData[j] = data[level][j];
		}
	}

	delete data[level];

	data[level] = newData;
}

template<class T>
void MultiLevelArray<T>::fit(std::initializer_list<bool> fits) {

	auto size = (fits.size() > sizes.size()) ? sizes.size() : fits.size();
	auto it = std::begin(fits);

	for (auto level = 0; level < size; ++level) {
		fit(level, *(it++));
	}
}

template<class T>
void MultiLevelArray<T>::remove(
		const std::pair<std::size_t, std::size_t>& position)
				throw (std::overflow_error) {

	if (checkBounds(position)) {

		auto& size = sizes[position.first];

		// Remove if element exists
		if (position.second < size.first) {
			data[position.first][position.second] =
					data[position.first][--size.first];
		}
		// Else do nothing

	} else {
		throw std::overflow_error(
				"Trying to access position " + std::to_string(position.first)
						+ ", " + std::to_string(position.second));
	}
}

template<class T>
inline typename MultiLevelArray<T>::Iterator MultiLevelArray<T>::end(
		std::size_t level) {

	return Iterator(this, { level + 1, 0 });
}

template<class T>
inline void MultiLevelArray<T>::remove(const MultiLevelArray<T>::Iterator& iterator)
		throw (std::overflow_error) {

	remove(iterator.getPosition());
}

template<class T>
inline bool MultiLevelArray<T>::checkBounds(
		const std::pair<std::size_t, std::size_t>& position) {

	return (position.first < sizes.size())
			&& (position.second < sizes[position.first].second);
}

#endif /* SRC_MULTILEVELARRAY_H_ */