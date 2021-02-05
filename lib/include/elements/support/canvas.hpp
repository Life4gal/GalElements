/*=============================================================================
   Copyright (c) 2016-2020 Joel de Guzman

   Distributed under the MIT License [ https://opensource.org/licenses/MIT ]
=============================================================================*/
#if !defined(ELEMENTS_CANVAS_MAY_3_2016)
#define ELEMENTS_CANVAS_MAY_3_2016

#include <elements/support/color.hpp>
#include <elements/support/rect.hpp>
#include <elements/support/circle.hpp>
#include <elements/support/pixmap.hpp>
#include <elements/support/font.hpp>
#include <infra/filesystem.hpp>

#include <vector>
#include <functional>
#include <stack>
#include <cmath>
#include <cassert>

extern "C"
{
	typedef struct _cairo cairo_t;
}

namespace cycfi::elements
{
	class canvas
	{
	public:
		using coordinate_type = point::coordinate_type;
		using size_type = extent::size_type;

		explicit canvas(cairo_t& context) : _context(context) {}
		canvas(canvas&& rhs) noexcept : _context(rhs._context) {}
		~canvas() = default;

		canvas(const canvas & rhs) = delete;
		canvas& operator=(const canvas & rhs) = delete;

		[[nodiscard]] cairo_t& cairo_context() const;

		void pre_scale(size_type sc)
		{
			scale(sc, sc);
			_pre_scale = sc;
		}

		[[nodiscard]] float pre_scale() const
		{
			return _pre_scale;
		}

		///////////////////////////////////////////////////////////////////////////////////
		// Transforms
		template<typename T1, typename T2, typename = std::enable_if_t<std::is_convertible_v<T1, double> && std::is_convertible_v<T2, double>>>
		void translate(T1 x, T2 y)
		{
			cairo_translate(&_context, static_cast<double>(x), static_cast<double>(y));
		}

		template<typename T, typename = std::enable_if_t<std::is_convertible_v<T, coordinate_type>>>
		void translate(basic_point<T> p)
		{
			this->template translate(p.x, p.y);
		}

		template<typename T, typename = std::enable_if_t<std::is_convertible_v<T, double>>>
		void rotate(T angle)
		{
			cairo_rotate(&_context, static_cast<double>(angle));
		}

		template<typename T1, typename T2, typename = std::enable_if_t<std::is_convertible_v<T1, double> && std::is_convertible_v<T2, double>>>
		void scale(T1 x, T2 y)
		{
			cairo_scale(&_context, static_cast<double>(x), static_cast<double>(y));
		}

		template<typename T, typename = std::enable_if_t<std::is_convertible_v<T, coordinate_type>>>
		void scale(basic_point<T> p)
		{
			this->template scale(p.x, p.y);
		}

		template<typename T1, typename T2, typename = std::enable_if_t<std::is_convertible_v<T1, double> && std::is_convertible_v<T2, double>>>
		void skew(T1 sx, T2 sy)
		{
			cairo_matrix_t matrix;
			cairo_matrix_init(&matrix, 1, 0, static_cast<double>(sx), 1, 0, static_cast<double>(sy));
			cairo_transform(&_context, &matrix);
		}

		point device_to_user(point p)
		{
			double x = p.x * _pre_scale;
			double y = p.y * _pre_scale;
			cairo_device_to_user(&_context, &x, &y);
			return {static_cast<point::coordinate_type>(x), static_cast<point::coordinate_type>(y)};
		}

		extent device_to_user(extent e)
		{
			double x = e.width * _pre_scale;
			double y = e.height * _pre_scale;
			cairo_device_to_user(&_context, &x, &y);
			return {static_cast<extent::size_type>(x), static_cast<extent::size_type>(y)};
		}

		point user_to_device(point p)
		{
			double x = p.x;
			double y = p.y;
			cairo_user_to_device(&_context, &x, &y);
			return {static_cast<point::coordinate_type>(x / _pre_scale), static_cast<point::coordinate_type>(y / _pre_scale)};
		}

		extent user_to_device(extent e)
		{
			double x = e.width;
			double y = e.height;
			cairo_user_to_device(&_context, &x, &y);
			return {static_cast<extent::size_type>(x / _pre_scale), static_cast<extent::size_type>(y / _pre_scale)};
		}

		///////////////////////////////////////////////////////////////////////////////////
		// Paths
		void begin_path();
		void close_path();
		void fill();
		void fill_preserve();
		void stroke();
		void stroke_preserve();
		void clip();
		[[nodiscard]] elements::rect clip_extent() const;
		[[nodiscard]] bool hit_test(point p) const;
		[[nodiscard]] elements::rect fill_extent() const;

		void move_to(point p);
		void line_to(point p);
		void arc_to(point p1, point p2, float radius);
		void arc(point p, float radius, float start_angle, float end_angle, bool ccw = false);
		void rect(elements::rect r);
		void round_rect(elements::rect r, float radius);
		void circle(elements::circle c);

		///////////////////////////////////////////////////////////////////////////////////
		// Styles
		void fill_style(color c);
		void stroke_style(color c);
		void line_width(float w);

		///////////////////////////////////////////////////////////////////////////////////
		// Gradients
		struct color_stop
		{
			float offset{};
			elements::color color;
		};

		struct linear_gradient
		{
			point start = {};
			point end = {};

			void add_color_stop(color_stop cs);
			std::vector<color_stop> space = {};
		};

		struct radial_gradient
		{
			point c1 = {};
			float c1_radius = {};
			point c2 = c1;
			float c2_radius = c1_radius;

			void add_color_stop(color_stop cs);
			std::vector<color_stop> space = {};
		};

		void fill_style(linear_gradient const& gr);
		void fill_style(radial_gradient const& gr);

		enum fill_rule_enum
		{
			fill_winding,
			fill_odd_even
		};

		void fill_rule(fill_rule_enum rule);

		///////////////////////////////////////////////////////////////////////////////////
		// Rectangles
		void fill_rect(elements::rect r);
		void fill_round_rect(elements::rect r, float radius);
		void stroke_rect(elements::rect r);
		void stroke_round_rect(elements::rect r, float radius);

		///////////////////////////////////////////////////////////////////////////////////
		// Font
		void font(elements::font const& font_);
		void font(elements::font const& font_, float size);
		void font_size(float size);

		///////////////////////////////////////////////////////////////////////////////////
		// Text
		enum class text_alignment
		{
			// Horizontal align
			left     = 0,        // Default, align text horizontally to left.
			center   = 1,        // Align text horizontally to center.
			right    = 2,        // Align text horizontally to right.
			horizontal_mask = 3,

			// Vertical align
			baseline = 4,        // Default, align text vertically to baseline.
			top      = 8,        // Align text vertically to top.
			middle   = 12,       // Align text vertically to middle.
			bottom   = 16,        // Align text vertically to bottom.
			vertical_mask = 28
		};

		struct text_metrics
		{
            float ascent;
            float descent;
            float leading;
            point size;
		};

		void fill_text(point p, char const* utf8);
		void stroke_text(point p, char const* utf8);
		text_metrics measure_text(char const* utf8);
		void text_align(text_alignment align);

		///////////////////////////////////////////////////////////////////////////////////
		// Pixmaps
		void draw(pixmap const& pm, elements::rect src, elements::rect dest);
		void draw(pixmap const& pm, elements::rect dest);
		void draw(pixmap const& pm, point pos);

		///////////////////////////////////////////////////////////////////////////////////
		// States
		class state
		{
		public:
			state(canvas& cnv_);
			state(state&& rhs) noexcept;
			state&  operator=(state&& rhs) noexcept;
			~state();

			state(state const&) = delete;
			state& operator=(state const&) = delete;


		private:
			canvas* cnv;
		};

		state new_state() { return state{ *this }; }
		void save();
		void restore();

	private:

		friend class glyphs;

		void apply_fill_style();
		void apply_stroke_style();

		struct canvas_state
		{
			canvas_state();

            std::function<void()> stroke_style;
            std::function<void()> fill_style;
            canvas::text_alignment align;

            enum class pattern_state
			{
				none_set,
				stroke_set,
				fill_set
			};
            pattern_state pattern_set;
		};

		using state_stack = std::stack<canvas_state>;

		cairo_t& _context;
		canvas_state _state;
		state_stack _state_stack;
		size_type _pre_scale = 1;
	};
}

#include <elements/support/detail/canvas_impl.hpp>
#endif
