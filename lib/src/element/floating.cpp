/*=============================================================================
   Copyright (c) 2016-2020 Joel de Guzman

   Distributed under the MIT License [ https://opensource.org/licenses/MIT ]
=============================================================================*/
#include <elements/element/floating.hpp>
#include <elements/support/context.hpp>

namespace cycfi { namespace elements
{
   view_limits floating_element::limits(basic_context const& ctx) const
   {
      auto e_limits = this->subject().limits(ctx);
	  e_limits.re_limits<view_limits::init_side::max_x_max_y>(full_extent<view_limits::coordinate_type>, full_extent<view_limits::coordinate_type>);
	  return e_limits;
//      return { { e_limits.min.x, e_limits.min.y }, { full_extent<view_limits::coordinate_type>, full_extent<view_limits::coordinate_type> } };
   }

   void floating_element::prepare_subject(context& ctx)
   {
      ctx.bounds = this->bounds();
      auto  e_limits = this->subject().limits(ctx);
      float w = ctx.bounds.width();
      float h = ctx.bounds.height();

      if (w < e_limits.min.x)
          ctx.bounds.widen(e_limits.min.x);
      else if (w > e_limits.max.x)
          ctx.bounds.widen(e_limits.max.x);

      if (h < e_limits.min.y)
          ctx.bounds.heighten(e_limits.min.y);
      else if (h > e_limits.max.y)
          ctx.bounds.heighten(e_limits.max.y);
   }
}}
