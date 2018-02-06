#pragma once
#include "NullableWrapper.h"

template<class T>
NullableWrapper<T>::NullableWrapper() :
	is_null(true)
{

}

template<class T>
NullableWrapper<T>::~NullableWrapper(){
}

template<class T>
T NullableWrapper<T>::getInternal() const {
	return internal;
}

template<class T>
bool NullableWrapper<T>::isNull() const {
	return is_null;
}

template<class T>
void NullableWrapper<T>::reset() {
	is_null = true;;
}

template<class T>
void NullableWrapper<T>::set(const T& new_internal) {
	is_null = false;
	internal = new_internal;
}