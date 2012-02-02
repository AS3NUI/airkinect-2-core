/* -*- C++ -*- ------------------------------------------------------------
 
Copyright (c) 2007 Jesse Anders and Demian Nave http://cmldev.net/

The Configurable Math Library (CML) is distributed under the terms of the
Boost Software License, v1.0 (see cml/LICENSE for details).

 *-----------------------------------------------------------------------*/
/** @file
 *  @brief
 */

#ifndef fixed_h
#define fixed_h

namespace cml {


/** This is a selector for fixed 1D and 2D arrays.
 *
 * The fixed<> struct is used only to select a 1D or 2D array as the base
 * class of a vector or matrix.  The rebind<> template is used by
 * quaternion<> to select its vector length in a generic way.
 *
 * @sa dynamic
 * @sa external
 */
template<int Dim1 = -1, int Dim2 = -1> struct fixed {

    /** Rebind to a 1D type.
     *
     * This is used by quaternion<>.
     */
    template<int D> struct rebind { typedef fixed<D> other; };
};

} // namespace cml

#endif

// -------------------------------------------------------------------------
// vim:ft=cpp
