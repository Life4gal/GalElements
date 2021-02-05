/*=============================================================================
   Copyright (c) 2016-2020 Joel de Guzman

   Distributed under the MIT License [ https://opensource.org/licenses/MIT ]
=============================================================================*/
#if !defined(ELEMENTS_PIXMAP_SEPTEMBER_5_2016)
#define ELEMENTS_PIXMAP_SEPTEMBER_5_2016

#include <vector>
#include <memory>
#include <cairo.h>
#include <elements/support/point.hpp>
#include <stdexcept>

namespace cycfi::elements
{
	class canvas;

	// customize your errors
	using failed_to_load_pixmap = std::runtime_error;

	// pixel map surface's width and height only support int type
	using pixmap_extent = basic_extent<int>;

	class pixmap
	{
	public:
		using size_type = pixmap_extent::size_type;

		template<typename T, typename = std::enable_if_t<std::is_arithmetic_v<T> && std::is_convertible_v<T, size_type>>>
		explicit pixmap(size_type width, size_type height, T scale = 1);

		// basic_extent<size_type>
		template<typename T, typename = std::enable_if_t<std::is_arithmetic_v<T> && std::is_convertible_v<T, size_type>>>
		explicit pixmap(pixmap_extent size, T scale = 1) : pixmap(size.width, size.height, scale) {}

		// basic_extent<T> -> is_convertible<T, size_type>
		template<typename Extent, typename T, typename = std::enable_if_t<std::is_convertible_v<Extent, size_type> && std::is_arithmetic_v<T> && std::is_convertible_v<T, size_type>>>
		explicit pixmap(basic_extent<Extent> size, T scale = 1) : pixmap(size.width, size.height, scale) {}

		explicit pixmap(const char* filename, size_type scale = 1);

		pixmap(pixmap const& rhs) = delete;
		pixmap& operator=(pixmap const& rhs) = delete;

		pixmap(pixmap&& rhs) noexcept : _surface(std::exchange(rhs._surface, nullptr)) {}

		pixmap& operator=(pixmap&& rhs) noexcept
		{
			if(this == &rhs)
			{
				return *this;
			}

			_surface = std::exchange(rhs._surface, nullptr);
		}

		~pixmap()
		{
			if(_surface)
				cairo_surface_destroy(_surface);
		}

		// todo
		template<typename Extent = extent::size_type>
		[[nodiscard]] basic_extent<Extent> size() const
		{
			double scx;
			double scy;
			cairo_surface_get_device_scale(_surface, &scx, &scy);
			return {
					static_cast<Extent>(cairo_image_surface_get_width(_surface) / scx),
					static_cast<Extent>(cairo_image_surface_get_height(_surface) / scy)
			};
		}

		[[nodiscard]] size_type scale() const
		{
			double scx;
			double scy;
			cairo_surface_get_device_scale(_surface, &scx, &scy);

			return static_cast<size_type>(1/scx);
		}

		void scale(size_type val)
		{
			cairo_surface_set_device_scale(_surface, static_cast<double>(1) / val, static_cast<double>(1) / val);
		}

	private:

		friend class canvas;
		friend class pixmap_context;

		cairo_surface_t*  _surface = nullptr;
	};

	using pixmap_ptr = std::shared_ptr<pixmap>;

	////////////////////////////////////////////////////////////////////////////
	// pixmap_context allows drawing into a pixmap
	////////////////////////////////////////////////////////////////////////////
	class pixmap_context
	{
	public:

		explicit pixmap_context(pixmap& pm)
		{
			_context = cairo_create(pm._surface);
		}

		~pixmap_context()
		{
			if (_context)
				cairo_destroy(_context);
		}

		pixmap_context(pixmap_context&& rhs) noexcept
			: _context(rhs._context)
		{
			rhs._context = nullptr;
		}

		[[nodiscard]] cairo_t* context() const { return _context; }

		pixmap_context(pixmap_context const&) = delete;

	private:
		cairo_t*          _context;
	};
}

#endif
