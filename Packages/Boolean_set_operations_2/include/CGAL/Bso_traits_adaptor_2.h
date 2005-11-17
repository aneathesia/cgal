// Copyright (c) 2005  Tel-Aviv University (Israel).
// All rights reserved.
//
// This file is part of CGAL (www.cgal.org); you may redistribute it under
// the terms of the Q Public License version 1.0.
// See the file LICENSE.QPL distributed with CGAL.
//
// Licensees holding a valid commercial license may use this file in
// accordance with the commercial license agreement provided with the software.
//
// This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
// WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
//
// $Source$
// $Revision$ $Date$
// $Name$
//
// Author(s)     : Baruch Zukerman <baruchzu@post.tau.ac.il>

#ifndef BSO_TRAITS_ADAPTOR_2_H
#define BSO_TRAITS_ADAPTOR_2_H

#include <CGAL/General_polygon_2.h>
#include <CGAL/General_polygon_with_holes_2.h>

CGAL_BEGIN_NAMESPACE

template <class Arr_traits,
          class Gen_polygon_ = General_polygon_2<Arr_traits> >
class Bso_traits_adaptor_2 : public Arr_traits
{
  typedef Arr_traits                          Base;
  typedef Bso_traits_adaptor_2<Arr_traits>    Self;

  public:
    typedef Gen_polygon_                                 Polygon_2;
    typedef General_polygon_with_holes_2<Polygon_2>      Polygon_with_holes_2;
    typedef typename Polygon_2::Curve_const_iterator     Curve_const_iterator;


  class Construct_polygon_2
  {
  public:

    template<class XCurveIterator>
    void operator()(XCurveIterator begin,
                    XCurveIterator end,
                    Polygon_2& pgn)
    {
      pgn.insert(begin, end);
    }
  };

  Construct_polygon_2 construct_polygon_2_object() const
  {
    return Construct_polygon_2();
  }

  class Construct_curves_2
  {
  public:

    std::pair<Curve_const_iterator,
              Curve_const_iterator> operator()(const Polygon_2& pgn)
    {
      return std::make_pair(pgn.curves_begin(), pgn.curves_end());
    }
  };

  Construct_curves_2 construct_curves_2_object()
  {
    return Construct_curves_2();
  }
};

CGAL_END_NAMESPACE

#endif
