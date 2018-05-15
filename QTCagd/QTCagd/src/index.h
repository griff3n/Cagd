#pragma once

#include <fstream>
#include <limits>

namespace viscas
{
    /*!
     * Helper class to define a individually magic number. Used as indicator for invalid indices.
     */
	template <typename Type>
	struct get_magic_number
	{
		static const Type value = std::numeric_limits<Type>::max();
	};

    /* Basic index type. See specialization.*/
    template <typename Element>
	struct Index
	{
		// Nothing to see.
	};

    /*!
     * \brief Specialization of Index<T>. Index<T<X>> for convenience.
     *
     * This class is used inside HalfEdgeMesh for indexing. It encapsulates an integer type and defines a few
     * methods for convenience.
     */
	template <template <typename> class Element, typename IndexType>
	struct Index<Element<IndexType>>
	{
        // static assert to be sure, that IndexType is an integer type
		static_assert(std::numeric_limits<IndexType>::is_integer, "Type of index must be an integer");

		// type aliases

		using element_type  = Element<IndexType>;
		using index_type	= IndexType;

		// attributes

        /*!
         * \brief The encapsulated data.
         */
		IndexType data = get_magic_number<IndexType>::value;
			
		// constructors

		Index() = default;
		Index(IndexType data) noexcept : data(data) {}

		// methods

		bool operator==(const Index& other) const noexcept
		{
			return data == other.data;
		}

		bool operator!=(const Index& other) const noexcept
		{
			return data != other.data;
		}

		/* Conversion function for Index<Element<IndexType>> to IndexType (e.g. Index<Vertex<unsigned int>> to unsigned int). */
		operator IndexType() const noexcept
		{
			return data;
		}
	};

    /*!
     * \brief This helper method will always fail in case of invalid types T.
     */
	template <typename T>
	inline auto valid(T index) noexcept
	{
		static_assert(false, "type not supported");
	}

    /*!
     * \brief Helper method that checks, if the given index is valid.
     *
     * \param index An index that should be checked.
     * \return True if the index is valid and false if the index is invalid.
     */
	template <template <typename> class Index, template <typename> class Element, typename IndexType>
	inline auto valid(Index<Element<IndexType>> index) noexcept
	{
		return index.data != get_magic_number<IndexType>::value;
	}

    /*!
     * \brief This helper method will always fail in case of invalid types T.
     */
    template <typename T>
	inline auto invalid(T index) noexcept
	{
		static_assert(false, "type not supported");
	}

    /*!
     * \brief Helper method that checks, if the given index is invalid.
     *
     * \param index An index that should be checked.
     * \return True if the index is valid and false if the index is invalid.
     */
	template <template <typename> class Index, template <typename> class Element, typename IndexType>
	inline auto invalid(Index<Element<IndexType>> index) noexcept
	{
		return index.data == get_magic_number<IndexType>::value;
	}

    /*!
     * \brief This helper method will always fail in case of invalid types T.
     */
	template <typename T>
	inline auto data(T index) noexcept
	{
		static_assert(false, "type not supported");
	}
	
    /*!
     * \brief Helper method that returns the encapsulated data of an index (aka the value).
     *
     * \param index The index for which you want the encapsulated data.
     * \return The (encapsulated) data of the given index.
     */
    template <template <typename> class Index, template <typename> class Element, typename IndexType>
	inline auto data(Index<Element<IndexType>> index) noexcept
	{
		return index.data;
	}
} // namespace viscas

/*!
 * \brief Pretty output for convenience.
 */
template <template <typename> class Index, template <typename> class Element, typename IndexType>
std::ostream& operator<<(std::ostream& stream, const Index<Element<IndexType>>& index)
{
	auto elementName = typeid(Element<IndexType>).name();
	return stream << "Index<" << elementName << ", " << index.data << ">";
}

namespace std
{
    /*!
     * \brief Hash method for convencience. Index can be used in maps, sets and other has based containers/algorithms.
     */
    template <template <typename> class Index, template <typename> class Element, typename IndexType>
	struct hash<Index<Element<IndexType>>>
	{
		size_t operator() (const Index<Element<IndexType>>& value) const noexcept
		{
			return hash<IndexType>{}(value.data);
		}
	};
}
