/*
 *          Copyright Andrey Semashev 2007 - 2010.
 * Distributed under the Boost Software License, Version 1.0.
 *    (See accompanying file LICENSE_1_0.txt or copy at
 *          http://www.boost.org/LICENSE_1_0.txt)
 */
/*!
 * \file   formatters/exception_policies.hpp
 * \author Andrey Semashev
 * \date   01.11.2009
 *
 * The header contains declaration of policies for exception throwing from formatters
 */

#if (defined(_MSC_VER) && _MSC_VER > 1000)
#pragma once
#endif // _MSC_VER > 1000

#ifndef BOOST_LOG_FORMATTERS_EXCEPTION_POLICIES_HPP_INCLUDED_
#define BOOST_LOG_FORMATTERS_EXCEPTION_POLICIES_HPP_INCLUDED_

#include <boost/log/detail/prologue.hpp>
#include <boost/log/exceptions.hpp>

namespace boost {

namespace BOOST_LOG_NAMESPACE {

namespace formatters {

/*!
 * \brief No-throw exception policy
 *
 * The policy will not throw exceptions on formatting errors
 */
struct no_throw_policy
{
    static void on_attribute_value_not_found(const char*, unsigned int)
    {
    }
};

/*!
 * \brief Throwing exception policy
 *
 * The policy will throw exceptions on formatting errors
 */
struct throw_policy
{
    static void on_attribute_value_not_found(const char* file, unsigned int line)
    {
        missing_value::throw_(file, line, "Requested attribute value not found");
    }
};

} // namespace formatters

} // namespace log

} // namespace boost

#endif // BOOST_LOG_FORMATTERS_EXCEPTION_POLICIES_HPP_INCLUDED_
