#ifndef ELEMENTS_FONT
#define ELEMENTS_FONT

#include <infra/string_view.hpp>
#include <infra/filesystem.hpp>
#include <vector>

extern "C"
{
	using cairo_font_face_t = struct _cairo_font_face;
}

namespace cycfi::elements
{
	namespace font_constants
	{
		using font_enum_type = int;
		constexpr font_enum_type unknown_enum = -1;

		enum class weight_enum : font_enum_type
		{
			thin                = 10,
			extra_light         = 20,
			light               = 30,
			normal              = 40,
			medium              = 50,
			semi_bold           = 60,
			bold                = 70,
			extra_bold          = 80,
			black               = 90,
			extra_black         = 95,
			ultra_black         = 100
		};

		enum class slant_enum : font_enum_type
		{
			normal              = 0,
			italic              = 90,
			oblique             = 100
		};

		enum class stretch_enum : font_enum_type
		{
			ultra_condensed     = 25,
			extra_condensed     = 31,
			condensed           = 38,
			semi_condensed      = 44,
			normal              = 50,
			semi_expanded       = 57,
			expanded            = 63,
			extra_expanded      = 75,
			ultra_expanded      = 100
		};
	}

	class font;
	struct font_descriptor
	{
		[[nodiscard]] font build_font() const;

		[[maybe_unused]] constexpr void clear_to_normal() noexcept
		{
			weight = font_constants::weight_enum::normal;
			slant = font_constants::slant_enum::normal;
			stretch = font_constants::stretch_enum::normal;
		}

		constexpr void set_weight(font_constants::weight_enum _weight) noexcept
		{
			weight = _weight;
		}

		[[maybe_unused]] [[nodiscard]] constexpr font_descriptor get_weight_thin() const noexcept
		{
			auto f = *this;
			f.set_weight(font_constants::weight_enum::thin);
			return f;
		}

		[[maybe_unused]] [[nodiscard]] constexpr font_descriptor get_weight_extra_light() const noexcept
		{
			auto f = *this;
			f.set_weight(font_constants::weight_enum::extra_light);
			return f;
		}

		[[maybe_unused]] [[nodiscard]] constexpr font_descriptor get_weight_light() const noexcept
		{
			auto f = *this;
			f.set_weight(font_constants::weight_enum::light);
			return f;
		}

		[[maybe_unused]] [[nodiscard]] constexpr font_descriptor get_weight_normal() const noexcept
		{
			auto f = *this;
			f.set_weight(font_constants::weight_enum::normal);
			return f;
		}

		[[maybe_unused]] [[nodiscard]] constexpr font_descriptor get_weight_medium() const noexcept
		{
			auto f = *this;
			f.set_weight(font_constants::weight_enum::medium);
			return f;
		}

		[[maybe_unused]] [[nodiscard]] constexpr font_descriptor get_weight_semi_bold() const noexcept
		{
			auto f = *this;
			f.set_weight(font_constants::weight_enum::semi_bold);
			return f;
		}

		[[maybe_unused]] [[nodiscard]] constexpr font_descriptor get_weight_bold() const noexcept
		{
			auto f = *this;
			f.set_weight(font_constants::weight_enum::bold);
			return f;
		}

		[[maybe_unused]] [[nodiscard]] constexpr font_descriptor get_weight_extra_bold() const noexcept
		{
			auto f = *this;
			f.set_weight(font_constants::weight_enum::extra_bold);
			return f;
		}

		[[maybe_unused]] [[nodiscard]] constexpr font_descriptor get_weight_black() const noexcept
		{
			auto f = *this;
			f.set_weight(font_constants::weight_enum::black);
			return f;
		}

		[[maybe_unused]] [[nodiscard]] constexpr font_descriptor get_weight_extra_black() const noexcept
		{
			auto f = *this;
			f.set_weight(font_constants::weight_enum::extra_black);
			return f;
		}

		[[maybe_unused]] [[nodiscard]] constexpr font_descriptor get_weight_ultra_black() const noexcept
		{
			auto f = *this;
			f.set_weight(font_constants::weight_enum::ultra_black);
			return f;
		}

		constexpr void set_slant(font_constants::slant_enum _slant) noexcept
		{
			slant = _slant;
		}

		[[maybe_unused]] [[nodiscard]] constexpr font_descriptor get_slant_normal() const noexcept
		{
			auto f = *this;
			f.set_slant(font_constants::slant_enum::normal);
			return f;
		}

		[[maybe_unused]] [[nodiscard]] constexpr font_descriptor get_slant_italic() const noexcept
		{
			auto f = *this;
			f.set_slant(font_constants::slant_enum::italic);
			return f;
		}

		[[maybe_unused]] [[nodiscard]] constexpr font_descriptor get_slant_oblique() const noexcept
		{
			auto f = *this;
			f.set_slant(font_constants::slant_enum::oblique);
			return f;
		}

		constexpr void set_stretch(font_constants::stretch_enum _stretch) noexcept
		{
			stretch = _stretch;
		}

		[[maybe_unused]] [[nodiscard]] constexpr font_descriptor get_stretch_ultra_condensed() const noexcept
		{
			auto f = *this;
			f.set_stretch(font_constants::stretch_enum::ultra_condensed);
			return f;
		}

		[[maybe_unused]] [[nodiscard]] constexpr font_descriptor get_stretch_extra_condensed() const noexcept
		{
			auto f = *this;
			f.set_stretch(font_constants::stretch_enum::extra_condensed);
			return f;
		}

		[[maybe_unused]] [[nodiscard]] constexpr font_descriptor get_stretch_condensed() const noexcept
		{
			auto f = *this;
			f.set_stretch(font_constants::stretch_enum::condensed);
			return f;
		}

		[[maybe_unused]] [[nodiscard]] constexpr font_descriptor get_stretch_semi_condensed() const noexcept
		{
			auto f = *this;
			f.set_stretch(font_constants::stretch_enum::semi_condensed);
			return f;
		}

		[[maybe_unused]] [[nodiscard]] constexpr font_descriptor get_stretch_normal() const noexcept
		{
			auto f = *this;
			f.set_stretch(font_constants::stretch_enum::normal);
			return f;
		}

		[[maybe_unused]] [[nodiscard]] constexpr font_descriptor get_stretch_semi_expanded() const noexcept
		{
			auto f = *this;
			f.set_stretch(font_constants::stretch_enum::semi_expanded);
			return f;
		}

		[[maybe_unused]] [[nodiscard]] constexpr font_descriptor get_stretch_expanded() const noexcept
		{
			auto f = *this;
			f.set_stretch(font_constants::stretch_enum::expanded);
			return f;
		}

		[[maybe_unused]] [[nodiscard]] constexpr font_descriptor get_stretch_extra_expanded() const noexcept
		{
			auto f = *this;
			f.set_stretch(font_constants::stretch_enum::extra_expanded);
			return f;
		}

		[[maybe_unused]] [[nodiscard]] constexpr font_descriptor ultra_expanded() const noexcept
		{
			auto f = *this;
			f.set_stretch(font_constants::stretch_enum::ultra_expanded);
			return f;
		}

		string_view font_families;
		font_constants::weight_enum weight = font_constants::weight_enum::normal;
		font_constants::slant_enum slant = font_constants::slant_enum::normal;
		font_constants::stretch_enum stretch = font_constants::stretch_enum::normal;
	};

	class font
	{
	public:
		font() = default;

		explicit font(font_descriptor descriptor);
		font(const font& rhs);
		font(font&& rhs) noexcept;
		~font();
		font& operator=(const font & rhs);
		font& operator=(font&& rhs) noexcept;

		explicit operator bool() const noexcept
		{
			return font_handle != nullptr;
		}

	private:
		friend class canvas;
		cairo_font_face_t* font_handle = nullptr;
	};

	inline font font_descriptor::build_font() const
	{
		return font(*this);
	}

#ifdef __APPLE__
   fs::path get_user_fonts_directory();
#endif
   std::vector<fs::path>& font_paths();
}

#endif
