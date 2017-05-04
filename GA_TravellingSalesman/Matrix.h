#pragma once

#include <cstdlib>
#include <memory>
#include <algorithm>
template<typename T>
class Matrix
{
	//#TODO consider typedefing std::size_t ? No obvious short name at the time
public:
	
	/*!
		ctor
	*/
	Matrix();
	Matrix(std::size_t size1, std::size_t size2);
	Matrix(const Matrix &M);
	virtual ~Matrix();

	/*!
		returns object at given positions
	*/
	inline T at(std::size_t size1=0, std::size_t col = 0) const ;

	/*!
		checks if the object has been allocated with any values 
	*/
	bool isAllocated() const;

	void fillRow(T *const data, std::size_t n, std::size_t row);
	void fillCol(T *const data, std::size_t n, std::size_t col);


	std::size_t getSize1() const;
	std::size_t getSize2() const;


private:
	T** allocate(std::size_t size1, std::size_t size2);

	std::size_t size1; //rows
	std::size_t size2; //columns

	T **ptr;

	bool allocated;


};




template<typename T>
Matrix<T>::Matrix() :
	size1(0), size2(0), allocated(false)
{

}

template<typename T>
Matrix<T>::Matrix(std::size_t size1, std::size_t size2) :
	size1(size1), size2(size2)
{
	ptr = allocate(size1, size2);
}

template<typename T>
Matrix<T>::Matrix(const Matrix & M)
{
	this->ptr = allocate(M.size1, M.size2);
	for (std::size_t i = 0; i < M.size2; i++)
		for (std::size_t j = 0; j < M.size1; j++)
			this->ptr[j][i] = M.at(j, i);
		//std::copy(M.ptr[i][0], M.ptr[i][size2], this->ptr[i][0]);
	this->allocated = M.allocated;
}

template<typename T>
bool Matrix<T>::isAllocated() const
{
	return allocated;
}

template<typename T>
inline void Matrix<T>::fillRow( T *const data, std::size_t n, std::size_t row)
{
	if (row > size1 || n > size2) //check if trying to access out of bounds regions
	{
		//#TODO throw exception?
		return;
	}
	T *data_ptr = data;

	for (std::size_t i = 0; i < n; i++)
	{
		ptr[row][i] = *(data_ptr++);
	}

}

template<typename T>
inline void Matrix<T>::fillCol( T *const data, std::size_t n, std::size_t col)
{
	if (col > size2 || n > size1) //check if trying to access out of bounds regions
	{
		//#TODO throw exception?
		return;
	}
	T *data_ptr = data;

	for (std::size_t i = 0; i < n; i++)
	{
		ptr[i][col] = data_ptr++;
	}
}

template<typename T>
inline std::size_t Matrix<T>::getSize1() const
{
	return size1;
}

template<typename T>
inline std::size_t Matrix<T>::getSize2() const
{
	return size1;
}

template<typename T>
Matrix<T>::~Matrix()
{
	for (std::size_t i = 0; i < size1; i++)
		delete[] ptr[i];
}

template<typename T>
T Matrix<T>::at(std::size_t row, std::size_t col) const
{
	return ptr[row][col];
}

template<typename T>
T** Matrix<T>::allocate(std::size_t size1, std::size_t size2)
{
	T** pointer = nullptr;


	try
	{
		pointer = new T*[size1]();
	}
	catch (const std::bad_alloc&)
	{

		throw std::bad_exception("Failed to allocate memory for Matrix");
		exit(0);
	}
	for (std::size_t i = 0; i < size1; i++)
	{
		try
		{
			pointer[i] = new T[size2]();
		}
		catch (const std::bad_alloc&)
		{
			exit(0);
		}
	}
	allocated = true;
	this->size1 = size1;
	this->size2 = size2;
	return pointer;
}


