/*=============================================================================
   Copyright (c) 2016-2020 Joel de Guzman

   Distributed under the MIT License [ https://opensource.org/licenses/MIT ]
=============================================================================*/
#include <elements/element/label.hpp>
#include <elements/support/enum_operator.hpp>

namespace cycfi { namespace elements
{
   view_limits default_label::limits(basic_context const& ctx) const
   {
      auto  size = measure_text(
         ctx.canvas, c_str()
       , get_font()
       , get_font_size()
      );
      return { { size.x, size.y }, { size.x, size.y } };
   }

   void default_label::draw(context const& ctx)
   {
      auto& canvas_ = ctx.canvas;
      auto  state = canvas_.new_state();
      auto  align = get_text_align();

      // default should reflect the theme's vertical label_text_align
      if ((align & canvas::text_alignment::vertical_mask) == 0)
         align |= get_theme().label_text_align & canvas::text_alignment::vertical_mask;

      canvas_.fill_style(get_font_color());
      canvas_.font(get_font(), get_font_size());

      float cx = ctx.bounds.left + (ctx.bounds.width() / 2);
      switch (canvas::text_alignment::horizontal_mask & align)
      {
         case canvas::text_alignment::left:
            cx = ctx.bounds.left;
            break;
         case canvas::text_alignment::center:
            break;
         case canvas::text_alignment::right:
            cx = ctx.bounds.right;
            break;
		 default:
			 break;
      }

      float cy = ctx.bounds.top + (ctx.bounds.height() / 2);
      switch (canvas::text_alignment::vertical_mask & align)
      {
         case canvas::text_alignment::top:
            cy = ctx.bounds.top;
            break;
         case canvas::text_alignment::middle:
            break;
         case canvas::text_alignment::bottom:
            cy = ctx.bounds.bottom;
            break;
		 default:
			 break;
      }

      canvas_.text_align(align);
      canvas_.fill_text({ cx, cy }, c_str());
   }
}}

