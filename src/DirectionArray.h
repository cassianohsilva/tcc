/*
 * DirectionArray.h
 *
 *  Created on: 19 de nov de 2016
 *      Author: cassiano
 */

#ifndef SRC_DIRECTIONARRAY_H_
#define SRC_DIRECTIONARRAY_H_

#include <vector>
#include <map>
#include <tuple>

template<class DType, class Element>
class DirectionArray {
public:

	// todo Create an constructor that receive all elements and values
	DirectionArray(const std::vector<const DType*>& allElements, Element defaultValue);

//	DirectionArray(const std::vector<DType>& allElements, Element values...);
	DirectionArray(const std::vector<const DType*>& allElements, const std::vector<Element>& values);

	virtual ~DirectionArray();

	Element& getAt(const DType& position);
	const Element& getAt(const DType& position) const;

	Element& operator[](const DType& position);
	const Element& operator[](const DType& position) const;

private:
	std::map<DType, Element> elements;
};

#include "DirectionArray.cpp"

#endif /* SRC_DIRECTIONARRAY_H_ */
