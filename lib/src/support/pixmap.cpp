/*=============================================================================
   Copyright (c) 2016-2020 Joel de Guzman

   Distributed under the MIT License [ https://opensource.org/licenses/MIT ]
=============================================================================*/
#include <elements/support/pixmap.hpp>
#include <elements/support/resource_paths.hpp>
#define STB_IMAGE_IMPLEMENTATION
#define STBI_NO_PNG 1
#include <elements/support/detail/stb_image.h>
#include <infra/assert.hpp>
#include <infra/filesystem.hpp>
#include <string>

namespace cycfi::elements
{
	template<typename T, typename>
	pixmap::pixmap(size_type width, size_type height, T scale)
		: surface(cairo_image_surface_create(CAIRO_FORMAT_ARGB32, width, height))
	{
		if (!surface)
			throw failed_to_load_pixmap{ "Failed to create pixmap." };

		// Set scale and flag the surface as dirty
		cairo_surface_set_device_scale(surface, static_cast<double>(1)/scale, static_cast<double>(1)/scale);
		cairo_surface_mark_dirty(surface);
   }

   pixmap::pixmap(const char* filename, float scale)
	   : surface(nullptr)
   {
	   auto  path = std::string(filename);
	   auto  pos = path.find_last_of('.');
	   if (pos == std::string::npos)
	   {
		   auto error = std::string("Error: file ") + filename + " is unknown type!";
		   throw failed_to_load_pixmap{ error };
	   }

	   fs::path full_path = find_file(filename);
	   if (full_path.empty())
	   {
		   auto error = std::string("Error: file ") + filename + " is not exist!";
		   throw failed_to_load_pixmap{ error };
	   }

	   auto  ext = path.substr(pos);
	   if (ext == ".png" || ext == ".PNG")
	   {
		   // For PNGs, use Cairo's native PNG loader
		   surface = cairo_image_surface_create_from_png(full_path.string().c_str());
	   }
	   else
	   {
		   // For everything else, use stb_image
		   int w, h, components;
		   uint8_t* src_data = stbi_load(full_path.string().c_str(), &w, &h, &components, 4);

		   if (src_data)
		   {
			   surface = cairo_image_surface_create(CAIRO_FORMAT_ARGB32, w, h);

			   uint8_t* dest_data = cairo_image_surface_get_data(surface);
			   size_t   src_stride = w * 4;
			   size_t   dest_stride = cairo_image_surface_get_stride(surface);

			   for (int y = 0; y != h; ++y)
			   {
				   uint8_t* src = src_data + (y * src_stride);
				   uint8_t* dest = dest_data + (y * dest_stride);
				   for (int x = 0; x != w; ++x)
				   {
					   // $$$ TODO: Deal with endian issues $$$

					   dest[0] = src[2];    // blue
					   dest[1] = src[1];    // green
					   dest[2] = src[0];    // red
					   dest[3] = src[3];    // alpha

					   src += 4;
					   dest += 4;
				   }
			   }

			   stbi_image_free(src_data);
		   }
	   }

	   if (!surface)
		   throw failed_to_load_pixmap{ "Failed to load pixmap." };

	   // Set scale and flag the surface as dirty
	   cairo_surface_set_device_scale(surface, static_cast<double>(1)/scale, static_cast<double>(1)/scale);
	   cairo_surface_mark_dirty(surface);
   }
}
