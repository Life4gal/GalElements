/*=============================================================================
   Copyright (c) 2016-2020 Joel de Guzman

   Distributed under the MIT License [ https://opensource.org/licenses/MIT ]
=============================================================================*/
#include <elements/element/image.hpp>
#include <elements/support.hpp>
#include <elements/support/context.hpp>
#include <algorithm>

namespace cycfi { namespace elements
{
   ////////////////////////////////////////////////////////////////////////////
   // image implementation
   ////////////////////////////////////////////////////////////////////////////
   image::image(char const* filename, float scale)
    : _pixmap(std::make_shared<elements::pixmap>(filename, scale))
   {
   }

   image::image(pixmap_ptr pixmap_)
    : _pixmap(pixmap_)
   {}

   extent image::size() const
   {
      return _pixmap->size();
   }

   rect image::source_rect(context const& ctx) const
   {
      return { 0, 0, ctx.bounds.width(), ctx.bounds.height() };
   }

   view_limits image::limits(basic_context const& /* ctx */) const
   {
      auto size_ = size();
      return { { size_.width, size_.height }, { size_.width, size_.height } };
   }

   void image::draw(context const& ctx)
   {
      auto src = source_rect(ctx);
      ctx.canvas.draw(pixmap(), src, ctx.bounds);
   }

   ////////////////////////////////////////////////////////////////////////////
   // gizmo implementation
   ////////////////////////////////////////////////////////////////////////////
   namespace
   {
      void gizmo_parts(rect src, rect dest, rect parts[9])
      {
         // Subdivide a rect into 9 parts. src is the original size and dest
         // is the resized rect. The 9 patches have a 1-pixel overlap to
         // maintain seemless rendering.

         float div_h = std::min<float>(src.width() / 2.4, dest.width() / 2);
         float div_v = std::min<float>(src.height() / 2.4, dest.height() / 2);

         auto corner = rect{ 0, 0, div_h+1, div_v+1 };

         parts[0] = corner.move(dest.left, dest.top);
         parts[1] = corner.move(dest.left, dest.bottom - (div_v+1));
         parts[2] = corner.move(dest.right - (div_h+1), dest.top);
         parts[3] = corner.move(dest.right - (div_h+1), dest.bottom - (div_v+1));

         parts[4] = parts[0].reconstruct_max_with(parts[1]).inset(0, div_v);
         parts[5] = parts[0].reconstruct_max_with(parts[2]).inset(div_h, 0);
         parts[6] = parts[2].reconstruct_max_with(parts[3]).inset(0, div_v);
         parts[7] = parts[1].reconstruct_max_with(parts[3]).inset(div_h, 0);
         parts[8] = dest.inset(div_h-1, div_v-1);
      }

      void hgizmo_parts(rect src, rect dest, rect parts[3])
      {
         // Variation of gizmo_parts allowing horizontal resizing only.

         float div_h = std::min<float>(src.width() / 2.4, dest.width() / 2);
         auto corner = rect{ 0, 0, div_h+1, src.height() };

         parts[0] = corner.move(dest.left, dest.top);
         parts[1] = corner.move(dest.right - (div_h+1), dest.top);
         parts[2] = parts[0].reconstruct_max_with(parts[1]).inset(div_h, 0);
      }

      void vgizmo_parts(rect src, rect dest, rect parts[9])
      {
         // Variation of gizmo_parts allowing vertical resizing only.

         float div_v = std::min<float>(src.height() / 2.4, dest.height() / 2);
         auto corner = rect{ 0, 0, src.width(), div_v+1 };

         parts[0] = corner.move(dest.left, dest.top);
         parts[1] = corner.move(dest.left, dest.bottom - (div_v+1));
         parts[2] = parts[0].reconstruct_max_with(parts[1]).inset(0, div_v);
      }
   }

   gizmo::gizmo(char const* filename, float scale)
    : image(filename, scale)
   {}

   gizmo::gizmo(pixmap_ptr pixmap_)
    : image(pixmap_)
   {}

   view_limits gizmo::limits(basic_context const& /* ctx */) const
   {
      auto size_ = size();
      return {size_.width, size_.height};
   }

   void gizmo::draw(context const& ctx)
   {
      rect  src[9];
      rect  dest[9];
      auto  size_ = size();
      rect  src_bounds{ 0, 0, size_.width, size_.height };

      gizmo_parts(src_bounds, src_bounds, src);
      gizmo_parts(src_bounds, ctx.bounds, dest);

      for (int i = 0; i < 9; i++)
         ctx.canvas.draw(pixmap(), src[i], dest[i]);
   }

   hgizmo::hgizmo(char const* filename, float scale)
    : image(filename, scale)
   {}

   hgizmo::hgizmo(pixmap_ptr pixmap_)
    : image(pixmap_)
   {}

   view_limits hgizmo::limits(basic_context const& /* ctx */) const
   {
      auto size_ = size();
      return { size_.width, size_.height , size_.height, full_extent<view_limits::coordinate_type> };
   }

   void hgizmo::draw(context const& ctx)
   {
      rect  src[3];
      rect  dest[3];
      auto  size_ = size();
      rect  src_bounds{ 0, 0, size_.width, size_.height };

      hgizmo_parts(src_bounds, src_bounds, src);
      hgizmo_parts(src_bounds, ctx.bounds, dest);
      ctx.canvas.draw(pixmap(), src[0], dest[0]);
      ctx.canvas.draw(pixmap(), src[1], dest[1]);
      ctx.canvas.draw(pixmap(), src[2], dest[2]);
   }

   vgizmo::vgizmo(char const* filename, float scale)
    : image(filename, scale)
   {}

   vgizmo::vgizmo(pixmap_ptr pixmap_)
    : image(pixmap_)
   {}

   view_limits vgizmo::limits(basic_context const& /* ctx */) const
   {
      auto size_ = size();
      return { { size_.width, size_.height }, { size_.width, full_extent<view_limits::coordinate_type> } };
   }

   void vgizmo::draw(context const& ctx)
   {
      rect  src[3];
      rect  dest[3];
      auto  size_ = size();
      rect  src_bounds{ 0, 0, size_.width, size_.height };

      vgizmo_parts(src_bounds, src_bounds, src);
      vgizmo_parts(src_bounds, ctx.bounds, dest);
      ctx.canvas.draw(pixmap(), src[0], dest[0]);
      ctx.canvas.draw(pixmap(), src[1], dest[1]);
      ctx.canvas.draw(pixmap(), src[2], dest[2]);
   }

   basic_sprite::basic_sprite(char const* filename, float height, float scale)
    : image(filename, scale)
    , _index(0)
    , _height(height)
   {}

   view_limits basic_sprite::limits(basic_context const& /* ctx */) const
   {
      auto width = pixmap().size().width;
      return { { width, _height }, { width, _height } };
   }

   std::size_t basic_sprite::num_frames() const
   {
      return pixmap().size().height / _height;
   }

   void basic_sprite::index(std::size_t index_)
   {
      if (index_ < num_frames())
         _index = index_;
   }

   extent basic_sprite::size() const
   {
      return { pixmap().size().width, _height };
   }

   rect basic_sprite::source_rect(context const& /* ctx */) const
   {
      auto width = pixmap().size().width;
      return rect{ 0, _height * _index, width, _height * (_index + 1) };
   }
}}
