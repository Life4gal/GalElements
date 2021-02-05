/*=============================================================================
   Copyright (c) 2016-2020 Joel de Guzman. All rights reserved.

   Distributed under the MIT License [ https://opensource.org/licenses/MIT ]
=============================================================================*/
#if !defined(CYCFI_ELEMENTS_BASE_VIEW_AUGUST_20_2016)
#define CYCFI_ELEMENTS_BASE_VIEW_AUGUST_20_2016

#include <utility>
#include <memory>
#include <string>
#include <cstdint>
#include <functional>
#include <cairo.h>

#include <infra/support.hpp>
#include <elements/support/point.hpp>
#include <elements/support/rect.hpp>

#if defined(ELEMENTS_HOST_UI_LIBRARY_WIN32)
# include <windows.h>
#endif

namespace cycfi::elements
{
	////////////////////////////////////////////////////////////////////////////
	// Mouse Button
	////////////////////////////////////////////////////////////////////////////
	struct mouse_button
	{
		enum class what { left, middle, right, x1, x2 };

		bool     down = false;
		int      num_clicks = 0;
		what     state = what::left;
		int      modifiers = 0;
		point    pos;
	};

	////////////////////////////////////////////////////////////////////////////
	// Cursor tracking
	////////////////////////////////////////////////////////////////////////////
	enum class cursor_tracking
	{
		entering,   // Sent when the cursor is entering
		hovering,   // Sent when the cursor is hovering
		leaving     // Sent when the cursor is leaving
	};

	template<typename CoordinateType, typename = std::enable_if_t<std::is_arithmetic_v<CoordinateType>>>
	struct basic_view_limits;
	using view_limits = basic_view_limits<point::coordinate_type>;
	template<typename SizeType, typename = std::enable_if_t<std::is_arithmetic_v<SizeType>>>
	struct basic_view_stretch;
	using view_stretch = basic_view_stretch<extent::size_type>;

//	constexpr float full_extent    = 1E30;
	template<typename T, typename = std::enable_if_t<std::is_arithmetic_v<T>>>
	constexpr T full_extent = std::numeric_limits<T>::max();

	////////////////////////////////////////////////////////////////////////////
	// View Limits
	////////////////////////////////////////////////////////////////////////////
	template<typename CoordinateType, typename>
	struct basic_view_limits
	{
		using coordinate_type = CoordinateType;

		enum class init_side
		{
			min_x_min_y,
			min_x_max_x,
			min_y_max_y,
			max_x_max_y
		};

		constexpr basic_view_limits() : min(0, 0), max(full_extent<coordinate_type>, full_extent<coordinate_type>) {}

		constexpr basic_view_limits(
				basic_point<coordinate_type> p_min,
				basic_point<coordinate_type> p_max
				)
			: min(p_min), max(p_max) {}

		constexpr basic_view_limits(
				coordinate_type min_x,
				coordinate_type min_y,
				coordinate_type max_x = full_extent<coordinate_type>,
				coordinate_type max_y = full_extent<coordinate_type>
				)
			: min(min_x, min_y), max(max_x, max_y) {}

		constexpr basic_view_limits(init_side side, coordinate_type x, coordinate_type y)
		{
			if (side == init_side::min_x_min_y)
			{
				*this = basic_view_limits(x, y, full_extent<coordinate_type>, full_extent<coordinate_type>);
			}
			else if (side == init_side::min_x_max_x)
			{
				*this = basic_view_limits(x, 0, y, full_extent<coordinate_type>);
			}
			else if (side == init_side::min_y_max_y)
			{
				*this = basic_view_limits(0, x, full_extent<coordinate_type>, y);
			}
			else if (side == init_side::max_x_max_y)
			{
				*this = basic_view_limits(0, 0, x, y);
			}
			else
			{
				*this = basic_view_limits(0, 0, full_extent<coordinate_type>, full_extent<coordinate_type>);
			}
		}

		template<init_side side, typename T1, typename T2, typename = std::enable_if_t<std::is_arithmetic_v<T1> && std::is_convertible_v<T1, coordinate_type> && std::is_arithmetic_v<T2> && std::is_convertible_v<T2, coordinate_type>>>
		constexpr void re_limits(T1 x, T2 y)
		{
			if constexpr (side == init_side::min_x_min_y)
			{
				min.x = x;
				min.y = y;
			}
			else if constexpr (side == init_side::min_x_max_x)
			{
				min.x = x;
				max.x = y;
			}
			else if constexpr (side == init_side::min_y_max_y)
			{
				min.y = x;
				max.y = y;
			}
			else if constexpr (side == init_side::max_x_max_y)
			{
				max.x = x;
				max.y = y;
			}
			else
			{
				min.x = 0;
				min.y = 0;
				max.x = full_extent<coordinate_type>;
				max.y = full_extent<coordinate_type>;
			}
		}

		basic_point<coordinate_type> min;
		basic_point<coordinate_type> max;
	};

	template<typename T, typename = std::enable_if_t<std::is_arithmetic_v<T>>>
	constexpr basic_view_limits<T> full_limits =
	{
		{ 0.0, 0.0 },
		{ full_extent<T>, full_extent<T> }
	};

	////////////////////////////////////////////////////////////////////////////
	// View stretch
	////////////////////////////////////////////////////////////////////////////
	template<typename SizeType, typename>
	struct basic_view_stretch
	{
		using size_type = SizeType;

		size_type x = 1;
		size_type y = 1;
	};

	////////////////////////////////////////////////////////////////////////////
	// Text info
	////////////////////////////////////////////////////////////////////////////
	struct text_info
	{
		uint32_t codepoint;
		int modifiers;
	};

	////////////////////////////////////////////////////////////////////////////
	// Keyboard information
	//
	// Key mapping ported to C++ from GLFW3
	//
	// Copyright (c) 2009-2016 Camilla Berglund <elmindreda@glfw.org>
	//
	// This software is provided 'as-is', without any express or implied
	// warranty. In no event will the authors be held liable for any damages
	// arising from the use of this software.
	//
	// Permission is granted to anyone to use this software for any purpose,
	// including commercial applications, and to alter it and redistribute it
	// freely, subject to the following restrictions:
	//
	// 1. The origin of this software must not be misrepresented; you must not
	//    claim that you wrote the original software. If you use this software
	//    in a product, an acknowledgment in the product documentation would
	//    be appreciated but is not required.
	//
	// 2. Altered source versions must be plainly marked as such, and must not
	//    be misrepresented as being the original software.
	//
	// 3. This notice may not be removed or altered from any source
	//    distribution.
	//
	////////////////////////////////////////////////////////////////////////////
	enum class key_action
	{
		unknown           = -1,
		release           = 0,
		press             = 1,
		repeat            = 2,
	};

	enum : uint16_t
	{
		mod_shift         = 0x0001,
		mod_control       = 0x0002,

		// mod_alt maps to the Alt key on PC keyboards
		// and maps to the Option key on MacOS
		mod_alt           = 0x0004,

		// mod_super maps to the Windows key on PC keyboards
		// and maps to the Command key on MacOS
		mod_super         = 0x0008,
		mod_command       = mod_super, // synonym for MacOS

		// mod_action maps to mod_control on Windows and Linux
		// and maps to mod_super on MacOS
		mod_action        = 0x0010,
	};

	enum class key_code : int16_t
	{
		unknown           = -1,

		// Printable keys
		space             = 32,
		apostrophe        = 39,    // '
		comma             = 44,    // ,
		minus             = 45,    // -
		period            = 46,    // .
		slash             = 47,    // /
		_0                = 48,
		_1                = 49,
		_2                = 50,
		_3                = 51,
		_4                = 52,
		_5                = 53,
		_6                = 54,
		_7                = 55,
		_8                = 56,
		_9                = 57,
		semicolon         = 59,    // ;
		equal             = 61,    // =
		a                 = 65,
		b                 = 66,
		c                 = 67,
		d                 = 68,
		e                 = 69,
		f                 = 70,
		g                 = 71,
		h                 = 72,
		i                 = 73,
		j                 = 74,
		k                 = 75,
		l                 = 76,
		m                 = 77,
		n                 = 78,
		o                 = 79,
		p                 = 80,
		q                 = 81,
		r                 = 82,
		s                 = 83,
		t                 = 84,
		u                 = 85,
		v                 = 86,
		w                 = 87,
		x                 = 88,
		y                 = 89,
		z                 = 90,
		left_bracket      = 91,    // [
		backslash         = 92,    // \ (back-slash)
		right_bracket     = 93,    // ]
		grave_accent      = 96,    // `
		world_1 [[maybe_unused]] = 161,   // non-US #1
		world_2 [[maybe_unused]] = 162,   // non-US #2

		// Function keys
		escape            = 256,
		enter             = 257,
		tab               = 258,
		backspace         = 259,
		insert            = 260,
		_delete           = 261,
		right             = 262,
		left              = 263,
		down              = 264,
		up                = 265,
		page_up           = 266,
		page_down         = 267,
		home              = 268,
		end               = 269,
		caps_lock         = 280,
		scroll_lock       = 281,
		num_lock          = 282,
		print_screen      = 283,
		pause             = 284,
		f1                = 290,
		f2                = 291,
		f3                = 292,
		f4                = 293,
		f5                = 294,
		f6                = 295,
		f7                = 296,
		f8                = 297,
		f9                = 298,
		f10               = 299,
		f11               = 300,
		f12               = 301,
		f13               = 302,
		f14               = 303,
		f15               = 304,
		f16               = 305,
		f17               = 306,
		f18               = 307,
		f19               = 308,
		f20               = 309,
		f21               = 310,
		f22               = 311,
		f23               = 312,
		f24               = 313,
		f25               = 314,
		kp_0              = 320,
		kp_1              = 321,
		kp_2              = 322,
		kp_3              = 323,
		kp_4              = 324,
		kp_5              = 325,
		kp_6              = 326,
		kp_7              = 327,
		kp_8              = 328,
		kp_9              = 329,
		kp_decimal        = 330,
		kp_divide         = 331,
		kp_multiply       = 332,
		kp_subtract       = 333,
		kp_add            = 334,
		kp_enter          = 335,
		kp_equal          = 336,
		left_shift        = 340,
		left_control      = 341,
		left_alt          = 342,
		left_super        = 343,
		right_shift       = 344,
		right_control     = 345,
		right_alt         = 346,
		right_super       = 347,
		menu              = 348,

		last              = menu,
	};

	struct key_info
	{
		key_code          key;
		key_action        action;
		int               modifiers;
	};

	////////////////////////////////////////////////////////////////////////////
	// The base view base class
	////////////////////////////////////////////////////////////////////////////

#if defined(ELEMENTS_HOST_UI_LIBRARY_COCOA) || defined(ELEMENTS_HOST_UI_LIBRARY_GTK)
	struct host_view;
	using host_view_handle = host_view*;
	struct host_window;
	using host_window_handle = host_window*;
#elif defined(ELEMENTS_HOST_UI_LIBRARY_WIN32)
	using host_view_handle = HWND;
	using host_window_handle = HWND;
#else
	#error no ELEMENTS_HOST_UI_LIBRARY_* set
#endif

	class base_view : non_copyable
	{
	public:

#if defined(ELEMENTS_HOST_UI_LIBRARY_COCOA) || defined(ELEMENTS_HOST_UI_LIBRARY_GTK)
		explicit base_view(host_view_handle h);
#endif
		explicit base_view(extent size);
		explicit base_view(host_window_handle h);
		virtual ~base_view();

		virtual void draw(cairo_t* /*ctx*/, rect /*area*/) {}
		virtual void click(mouse_button /*btn*/) {}
		virtual void drag(mouse_button /*btn*/) {}
		virtual void cursor(point /*p*/, cursor_tracking /*status*/) {}
		virtual void scroll(point /*dir*/, point /*p*/) {}
		virtual bool key(key_info const& /*k*/) {return false;}
		virtual bool text(text_info const& /*info*/) {return false;}
		virtual void begin_focus() {}
		virtual void end_focus() {}
		virtual void poll() {}

		virtual void refresh();
		virtual void refresh(rect area);

		[[nodiscard]] float hdpi_scale() const;
		[[nodiscard]] point cursor_pos() const;
		[[nodiscard]] extent size() const;
		void resize(extent new_size);
		[[nodiscard]] host_view_handle host() const { return _view; }

	private:

		host_view_handle _view;
	};
	////////////////////////////////////////////////////////////////////////////
	// The clipboard
	std::string clipboard();
	void clipboard(std::string const& text);

	////////////////////////////////////////////////////////////////////////////
	// The Cursor
	enum class cursor_type
	{
		arrow,
		ibeam,
		cross_hair,
		hand,
		h_resize,
		v_resize
	};

	void set_cursor(cursor_type type);

	////////////////////////////////////////////////////////////////////////////
	// Scroll direction
	point scroll_direction();
}

#endif
