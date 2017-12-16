
/* Copyright(C) 2017 Zhang Wenjie */

/*

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.

*/

#ifndef MYXMAT_MATRIX_HEADER
#define MYXMAT_MATRIX_HEADER

#include <cstdio>

#ifndef MYXMAT_DEBUG_LEVEL
	#ifdef _DEBUG
		#define MYXMAT_DEBUG_LEVEL 2
	#else
		#define MYXMAT_DEBUG_LEVEL 0
	#endif
#endif

#if MYXMAT_DEBUG_LEVEL == 2
	inline int _myxmat_assert(const char* infor, const char* file, int line)
	{
		printf("Assertion Failed. %s:%d %s\n", file, line, infor);
		throw 0;
	}
	#define myxmat_assert(x) ( (!!(x)) || (_myxmat_assert(#x, __FILE__, __LINE__)) )
#elif MYXMAT_DEBUG_LEVEL == 1
	inline int __myxmat_die() { throw 0; }
	#define myxmat_assert(x) ( (!!(x)) || (__myxmat_die()) )
#else
	#define myxmat_assert(x) ((void)0)
#endif


#include <cstddef>
#include <stdexcept>
#include <cassert>

namespace xmat
{
	template<class _T>
	struct _MatrixData {
		_T* data;
		intptr_t ref;
		intptr_t rows;
		intptr_t cols;

		typedef _MatrixData<_T> _MyT;

		_MatrixData(intptr_t r, intptr_t c)
			: data(new _T[r*c]), ref(1), rows(r), cols(c) 
		{ }

		~_MatrixData()
		{
			delete[] data;
		}

		_MatrixData(_MyT& obj) = delete;
		_MyT& operator=(_MyT& obj) = delete;
	};

	template<class _T> class Matrix;

	template<class _T>
	struct _MatrixRow {
		_T& operator[](intptr_t c) {
			return mat->at(r, c);
		}
		_MatrixRow(Matrix<_T>* m, intptr_t row)
			: mat(m), r(row) {}
	private:
		Matrix<_T>* mat;
		intptr_t r;
	};

	template<class _T>
	struct _MatrixRowConst {
		const _T& operator[](intptr_t c) {
			return mat->at(r, c);
		}
		_MatrixRowConst(const Matrix<_T>* m, intptr_t row)
			: mat(m), r(row) {}
	private:
		const Matrix<_T>* mat;
		intptr_t r;
	};


	template<class _T>
	class Matrix
	{
		typedef Matrix<_T> _MyT;
		typedef _MatrixData<_T> _DataT;
	public:
		Matrix()
			: m_data(nullptr)
		{
		}


		Matrix(_MyT& obj)
			: m_data(obj.m_data)
		{
			if(m_data != nullptr)
				++m_data->ref;
		}

		Matrix(_MyT&& obj)
			: m_data(obj.m_data)
		{
			obj.m_data = nullptr;
		}

		_MyT& operator=(_MyT& obj)
		{
			release();
			m_data = obj.m_data;

			if (m_data != nullptr)
				++m_data->ref;

			return *this;
		}

		_MyT& operator=(_MyT&& obj)
		{
			release();
			m_data = obj.m_data;

			obj.m_data = nullptr;
			return *this;
		}

		~Matrix()
		{
			release();
		}

		
		_MyT clone()
		{
			myxmat_assert(!isNull());

			_MyT result;
			result.create(rows(), cols());

			intptr_t s = size();
			_T* dst = result.m_data->data;
			_T* src = m_data->data;

			for (intptr_t i = 0; i < s; ++i)
				dst[i] = src[i];

			return result;
		}

		template<class _TR>
		Matrix<_TR> castType()
		{
			myxmat_assert(!isNull());

			Matrix<_TR> result;
			result.create(rows(), cols());

			intptr_t s = size();
			_TR* dst = result.m_data->data;
			_T* src = m_data->data;

			for (intptr_t i = 0; i < s; ++i)
				dst[i] = (_TR)(src[i]);

			return result;
		}

		template<class _TR>
		void cloneFrom(const Matrix<_TR>& obj)
		{
			myxmat_assert(!obj.isNull());

			release();

			create(obj.rows(), obj.cols());

			intptr_t s = size();
			_T* dst = m_data->data;
			_TR* src = obj.m_data->data;

			for (intptr_t i = 0; i < s; ++i)
				dst[i] = (_T)(src[i]);
		}

		template<class _TR>
		void cloneFrom(const Matrix<_TR>&& obj)
		{
			myxmat_assert(!obj.isNull());

			release();

			create(obj.rows(), obj.cols());

			intptr_t s = size();
			_T* dst = m_data->data;
			_TR* src = obj.m_data->data;

			for (intptr_t i = 0; i < s; ++i)
				dst[i] = (_T)(src[i]);

			obj.release();
		}

		void create(intptr_t rows, intptr_t cols)
		{
			myxmat_assert(rows > 0 && cols > 0);
			release();
			m_data = new _DataT(rows, cols);
		}

		void release()
		{
			if (isNull())
				return;

			--m_data->ref;
			if (m_data->ref == 0)
				delete m_data;

			m_data = nullptr;
		}

		template<class _It>
		void loadData(_It it)
		{
			myxmat_assert(!isNull());

			_T* begin = data();
			_T* end = begin + size();

			for (_T* cur = begin; cur < end; ++cur, ++it)
				*cur = *it;
		}

		bool isNull() const
		{
			return m_data == nullptr;
		}

		intptr_t rows() const
		{
			myxmat_assert(!isNull());

			return m_data->rows;
		}
		intptr_t cols() const
		{
			myxmat_assert(!isNull());

			return m_data->cols;
		}
		intptr_t size() const
		{
			myxmat_assert(!isNull());

			return m_data->cols * m_data->rows;
		}

		_T* data()
		{
			myxmat_assert(!isNull());

			return m_data->data;
		}
		const _T* data() const
		{
			myxmat_assert(!isNull());

			return m_data->data;
		}
		_MatrixRow<_T> operator[](intptr_t index)
		{
			myxmat_assert(!isNull());
			myxmat_assert(index >= 0 && index < rows());

			return _MatrixRow<_T>(this, index);
		}
		_MatrixRowConst<_T> operator[](intptr_t index) const
		{
			myxmat_assert(!isNull());
			myxmat_assert(index >= 0 && index < rows());

			return _MatrixRowConst<_T>(this, index);
		}
		_T& at(intptr_t r, intptr_t c)
		{
			myxmat_assert(!isNull());
			myxmat_assert(r >= 0 && r < rows());
			myxmat_assert(c >= 0 && c < cols());

			return m_data->data[m_data->cols * r + c];
		}
		const _T& at(intptr_t r, intptr_t c) const
		{
			myxmat_assert(!isNull());
			myxmat_assert(r >= 0 && r < rows());
			myxmat_assert(c >= 0 && c < cols());

			return m_data->data[m_data->cols * r + c];
		}

		_T& vecAt(intptr_t i)
		{
			myxmat_assert(!isNull());
			myxmat_assert(i >= 0 && i < rows()*cols());

			return m_data->data[i];
		}
		const _T& vecAt(intptr_t i) const
		{
			myxmat_assert(!isNull());
			myxmat_assert(i >= 0 && i < rows()*cols());

			return m_data->data[i];
		}

		void resize(intptr_t r, intptr_t c)
		{
			myxmat_assert(!isNull());
			myxmat_assert(rows > 0 && cols > 0);
			myxmat_assert(r * c == cols() * rows());

			m_data->cols = c;
			m_data->rows = r;
		}

		_MyT transpose() const
		{
			myxmat_assert(!isNull());

			_MyT result;
			result.create(cols(), rows());
			
			for (intptr_t i = 0; i < rows(); ++i)
				for (intptr_t j = 0; j < cols(); ++j)
					result[j][i] = (*this)[i][j];

			return result;
		}

		template<class _TR>
		friend class Matrix;
	private:
		_DataT *m_data;
	};

	typedef Matrix<int> MatrixI;
	typedef Matrix<float> MatrixF;
	typedef Matrix<double> MatrixD;
}

#endif /* MATRIX_HEADER */