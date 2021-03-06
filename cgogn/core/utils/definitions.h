/*******************************************************************************
* CGoGN: Combinatorial and Geometric modeling with Generic N-dimensional Maps  *
* Copyright (C) 2015, IGG Group, ICube, University of Strasbourg, France       *
*                                                                              *
* This library is free software; you can redistribute it and/or modify it      *
* under the terms of the GNU Lesser General Public License as published by the *
* Free Software Foundation; either version 2.1 of the License, or (at your     *
* option) any later version.                                                   *
*                                                                              *
* This library is distributed in the hope that it will be useful, but WITHOUT  *
* ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or        *
* FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License  *
* for more details.                                                            *
*                                                                              *
* You should have received a copy of the GNU Lesser General Public License     *
* along with this library; if not, write to the Free Software Foundation,      *
* Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301 USA.           *
*                                                                              *
* Web site: http://cgogn.unistra.fr/                                           *
* Contact information: cgogn@unistra.fr                                        *
*                                                                              *
*******************************************************************************/

#ifndef CORE_UTILS_DEFINITIONS_H_
#define CORE_UTILS_DEFINITIONS_H_

/**
 * \brief No execpt declaration for CGOGN symbols.
 * For a given type T, std::vector<T> will only use move constructor of T if it's marked noexcept. Same for std::swap.
 */
#if defined(_MSC_VER) && _MSC_VER < 1900
#define CGOGN_NOEXCEPT throw()
#else
#define CGOGN_NOEXCEPT noexcept
#endif

/*
 * Thread local storage. In VS <1900 it works only with POD types.
*/
#if defined(_MSC_VER) && _MSC_VER < 1900
#define CGOGN_TLS __declspec( thread )
#else
#define CGOGN_TLS __thread
#endif

/*
 * a constexrp function can be evaluated at compile time.
 * Can be used to optimize the code.
 * WARNING : cases where the use of constexpr is mandatory for the compilation are not allowed since it's not available in VS 2013.
*/
#if defined(_MSC_VER) && _MSC_VER < 1900
#define CGOGN_CONSTEXPR const
#else
#define CGOGN_CONSTEXPR constexpr
#endif

/**
 * \brief No return declaration for CGOGN symbols.
 */
#ifndef CGOGN_NORETURN
#if defined(_MSC_VER)
#define CGOGN_NORETURN __declspec(noreturn)
#else
#define CGOGN_NORETURN [[noreturn]]
#endif
#endif

/**
 * \def CGOGN_DEBUG
 * \brief This macro is set when compiling in debug mode
 *
 * \def CGOGN_PARANO
 * \brief This macro is set when compiling in debug mode
 */
#ifdef NDEBUG
#undef CGOGN_DEBUG
#undef CGOGN_PARANO
#else
#define CGOGN_DEBUG
#define CGOGN_PARANO
#endif


// #ifdef __GNUG__
#if (defined(__GNUC__) || defined(__GNUG__)) && !(defined(__clang__) || defined(__INTEL_COMPILER))
	#define CGOGN_PRAGMA_EIGEN_REMOVE_WARNINGS_ON \
	_Pragma("GCC diagnostic push") \
	_Pragma("GCC diagnostic ignored \"-Winline\"")

	#define CGOGN_PRAGMA_EIGEN_REMOVE_WARNINGS_OFF \
	_Pragma("GCC diagnostic pop")
#endif

#ifdef __clang__
	#define CGOGN_PRAGMA_EIGEN_REMOVE_WARNINGS_ON \
	_Pragma("clang diagnostic push") \
	_Pragma("clang diagnostic ignored \"-Winline\"") \
	_Pragma("clang diagnostic ignored \"-Wold-style-cast\"") \
	_Pragma("clang diagnostic ignored \"-Wdeprecated\"") \
	_Pragma("clang diagnostic ignored \"-Wsign-conversion\"") \
	_Pragma("clang diagnostic ignored \"-Wreserved-id-macro\"") \
	_Pragma("clang diagnostic ignored \"-Wshadow\"") \
	_Pragma("clang diagnostic ignored \"-Wmissing-noreturn\"") \
	_Pragma("clang diagnostic ignored \"-Wused-but-marked-unused\"")

	#define CGOGN_PRAGMA_EIGEN_REMOVE_WARNINGS_OFF \
	_Pragma("clang diagnostic pop")
#endif

#ifdef _MSC_VER
	// no warning with VS 2013 and \W4
	#define CGOGN_PRAGMA_EIGEN_REMOVE_WARNINGS_ON 
	#define CGOGN_PRAGMA_EIGEN_REMOVE_WARNINGS_OFF 
#endif


#define CGOGN_QUOTE(name) #name
#define CGOGN_STR(macro) CGOGN_QUOTE(macro)

#endif // CORE_UTILS_DEFINITIONS_H_
