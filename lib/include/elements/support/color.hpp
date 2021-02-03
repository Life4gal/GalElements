#ifndef ELEMENTS_COLOR
#define ELEMENTS_COLOR

#include <type_traits>
#include <limits>
#include <utility>

namespace cycfi::elements
{
	template<typename ColorType, typename = std::enable_if_t<std::is_arithmetic_v<ColorType>>>
	struct basic_color;

	using ColorGenericColorType = float;
	using color = basic_color<ColorGenericColorType>;

	template<typename ColorType, typename>
	struct basic_color
	{
		using color_type = ColorType;

		constexpr basic_color() noexcept : red(color_type{}), green(color_type{}), blue(color_type{}), alpha(color_type{}) {}

		constexpr basic_color(color_type red, color_type green, color_type blue, color_type alpha = 1) noexcept
			: red(red), green(green), blue(blue), alpha(alpha) {}

		template <typename T, typename = std::enable_if_t<std::is_convertible_v<T, color_type>>>
		constexpr bool operator==(const basic_color<T>& other) const noexcept
		{
			return other.red == red && other.green == green && other.blue == blue && other.alpha == alpha;
		}

		template <typename T, typename = std::enable_if_t<std::is_convertible_v<T, color_type>>>
		constexpr bool operator!=(const basic_color<T>& other) const noexcept
		{
			return this->template operator==(std::forward<const basic_color<T>&>(other));
		}

		template <typename T, typename = std::enable_if_t<std::is_convertible_v<T, color_type>>>
		[[nodiscard]] constexpr basic_color<color_type> opacity(T new_alpha) const noexcept
		{
			auto r = *this;
			r.alpha = new_alpha;
			return r;
		}

		template <typename T, typename = std::enable_if_t<std::is_convertible_v<T, color_type>>>
		[[nodiscard]] constexpr basic_color<color_type> level(T amount) const noexcept
		{
			auto r = *this;
			r.red *= amount;
			r.green *= amount;
			r.blue *= amount;
			return r;
		}

		color_type red;
		color_type green;
		color_type blue;
		color_type alpha;

		// get color from rgb, make it as a static member function to specify the type of color
		// you need call this through basic_color<T>::rgb_to_color
		template<typename T, typename = std::enable_if_t<std::numeric_limits<T>::is_integer>>
		constexpr static basic_color<color_type> rgb_to_color(T rgb) noexcept
		{
			return {
					((rgb >> 16) & 0xff) / static_cast<color_type>(255),
					((rgb >> 8) & 0xff) / static_cast<color_type>(255),
					((rgb >> 0) & 0xff) / static_cast<color_type>(255)
			};
		}

		// get color from rgba, make it as a static member function to specify the type of color
		// you need call this through basic_color<T>::rgba_to_color
		template<typename T, typename = std::enable_if_t<std::numeric_limits<T>::is_integer>>
		constexpr static basic_color<color_type> rgba_to_color(T rgba) noexcept
		{
			return {
					((rgba >> 24) & 0xff) / static_cast<color_type>(255),
					((rgba >> 16) & 0xff) / static_cast<color_type>(255),
					((rgba >> 8) & 0xff) / static_cast<color_type>(255),
					((rgba >> 0) & 0xff) / static_cast<color_type>(255)
			};
		}

		// get color from r/g/b, make it as a static member function to specify the type of color
		// you need call this through basic_color<T>::build_color
		template<typename T, typename = std::enable_if_t<std::numeric_limits<T>::is_integer>>
		constexpr static basic_color<color_type> build_color(T red, T green, T blue) noexcept
		{
			return {
					red / static_cast<color_type>(255),
					green / static_cast<color_type>(255),
					blue / static_cast<color_type>(255)
			};
		}

		// get color from r/g/b/a, make it as a static member function to specify the type of color
		// you need call this through basic_color<T>::build_color
		template<typename T, typename = std::enable_if_t<std::numeric_limits<T>::is_integer>>
		constexpr static basic_color<color_type> build_color(T red, T green, T blue, T alpha) noexcept
		{
			return {
					red / static_cast<color_type>(255),
					green / static_cast<color_type>(255),
					blue / static_cast<color_type>(255),
					alpha / static_cast<color_type>(255)
			};
		}
	};

	////////////////////////////////////////////////////////////////////////////
	// Common colors
	////////////////////////////////////////////////////////////////////////////
	namespace colors
	{
		constexpr color alice_blue             = color::build_color(240, 248, 255);
		constexpr color antique_white          = color::build_color(250, 235, 215);
		constexpr color aquamarine             = color::build_color(50, 191, 193);
		constexpr color azure                  = color::build_color(240, 255, 255);
		constexpr color beige                  = color::build_color(245, 245, 220);
		constexpr color bisque                 = color::build_color(255, 228, 196);
		constexpr color black                  = color::build_color(0, 0, 0);
		constexpr color blanched_almond        = color::build_color(255, 235, 205);
		constexpr color blue                   = color::build_color(0, 0, 255);
		constexpr color blue_violet            = color::build_color(138, 43, 226);
		constexpr color brown                  = color::build_color(165, 42, 42);
		constexpr color burly_wood             = color::build_color(222, 184, 135);
		constexpr color cadet_blue             = color::build_color(95, 146, 158);
		constexpr color kchartreuse            = color::build_color(127, 255, 0);
		constexpr color chocolate              = color::build_color(210, 105, 30);
		constexpr color coral                  = color::build_color(255, 114, 86);
		constexpr color cornflower_blue        = color::build_color(34, 34, 152);
		constexpr color corn_silk              = color::build_color(255, 248, 220);
		constexpr color cyan                   = color::build_color(0, 255, 255);
		constexpr color dark_goldenrod         = color::build_color(184, 134, 11);
		constexpr color dark_green             = color::build_color(0, 86, 45);
		constexpr color dark_khaki             = color::build_color(189, 183, 107);
		constexpr color dark_olive_green       = color::build_color(85, 86, 47);
		constexpr color dark_orange            = color::build_color(255, 140, 0);
		constexpr color dark_orchid            = color::build_color(139, 32, 139);
		constexpr color dark_salmon            = color::build_color(233, 150, 122);
		constexpr color dark_sea_green         = color::build_color(143, 188, 143);
		constexpr color dark_slate_blue        = color::build_color(56, 75, 102);
		constexpr color dark_slate_gray        = color::build_color(47, 79, 79);
		constexpr color dark_turquoise         = color::build_color(0, 166, 166);
		constexpr color dark_violet            = color::build_color(148, 0, 211);
		constexpr color deep_pink              = color::build_color(255, 20, 147);
		constexpr color deep_sky_blue          = color::build_color(0, 191, 255);
		constexpr color dim_gray               = color::build_color(84, 84, 84);
		constexpr color dodger_blue            = color::build_color(30, 144, 255);
		constexpr color firebrick              = color::build_color(142, 35, 35);
		constexpr color floral_white           = color::build_color(255, 250, 240);
		constexpr color forest_green           = color::build_color(80, 159, 105);
		constexpr color gains_boro             = color::build_color(220, 220, 220);
		constexpr color ghost_white            = color::build_color(248, 248, 255);
		constexpr color gold                   = color::build_color(218, 170, 0);
		constexpr color goldenrod              = color::build_color(239, 223, 132);
		constexpr color green                  = color::build_color(0, 255, 0);
		constexpr color green_yellow           = color::build_color(173, 255, 47);
		constexpr color honeydew               = color::build_color(240, 255, 240);
		constexpr color hot_pink               = color::build_color(255, 105, 180);
		constexpr color indian_red             = color::build_color(107, 57, 57);
		constexpr color ivory                  = color::build_color(255, 255, 240);
		constexpr color khaki                  = color::build_color(179, 179, 126);
		constexpr color lavender               = color::build_color(230, 230, 250);
		constexpr color lavender_blush         = color::build_color(255, 240, 245);
		constexpr color lawn_green             = color::build_color(124, 252, 0);
		constexpr color lemon_chiffon          = color::build_color(255, 250, 205);
		constexpr color light_blue             = color::build_color(176, 226, 255);
		constexpr color light_coral            = color::build_color(240, 128, 128);
		constexpr color light_cyan             = color::build_color(224, 255, 255);
		constexpr color light_goldenrod        = color::build_color(238, 221, 130);
		constexpr color light_goldenrod_yellow = color::build_color(250, 250, 210);
		constexpr color light_gray             = color::build_color(168, 168, 168);
		constexpr color light_pink             = color::build_color(255, 182, 193);
		constexpr color light_salmon           = color::build_color(255, 160, 122);
		constexpr color light_sea_green        = color::build_color(32, 178, 170);
		constexpr color light_sky_blue         = color::build_color(135, 206, 250);
		constexpr color light_slate_blue       = color::build_color(132, 112, 255);
		constexpr color light_slate_gray       = color::build_color(119, 136, 153);
		constexpr color light_steel_blue       = color::build_color(124, 152, 211);
		constexpr color light_yellow           = color::build_color(255, 255, 224);
		constexpr color lime_green             = color::build_color(0, 175, 20);
		constexpr color linen                  = color::build_color(250, 240, 230);
		constexpr color magenta                = color::build_color(255, 0, 255);
		constexpr color maroon                 = color::build_color(143, 0, 82);
		constexpr color medium_aquamarine      = color::build_color(0, 147, 143);
		constexpr color medium_blue            = color::build_color(50, 50, 204);
		constexpr color medium_forest_green    = color::build_color(50, 129, 75);
		constexpr color medium_goldenrod       = color::build_color(209, 193, 102);
		constexpr color medium_orchid          = color::build_color(189, 82, 189);
		constexpr color medium_purple          = color::build_color(147, 112, 219);
		constexpr color medium_sea_green       = color::build_color(52, 119, 102);
		constexpr color medium_slate_blue      = color::build_color(106, 106, 141);
		constexpr color medium_spring_green    = color::build_color(35, 142, 35);
		constexpr color medium_turquoise       = color::build_color(0, 210, 210);
		constexpr color medium_violet_red      = color::build_color(213, 32, 121);
		constexpr color midnight_blue          = color::build_color(47, 47, 100);
		constexpr color mint_cream             = color::build_color(245, 255, 250);
		constexpr color misty_rose             = color::build_color(255, 228, 225);
		constexpr color moccasin               = color::build_color(255, 228, 181);
		constexpr color navajo_white           = color::build_color(255, 222, 173);
		constexpr color navy                   = color::build_color(35, 35, 117);
		constexpr color navy_blue              = color::build_color(35, 35, 117);
		constexpr color old_lace               = color::build_color(253, 245, 230);
		constexpr color olive_drab             = color::build_color(107, 142, 35);
		constexpr color orange                 = color::build_color(255, 135, 0);
		constexpr color orange_red             = color::build_color(255, 69, 0);
		constexpr color orchid                 = color::build_color(239, 132, 239);
		constexpr color pale_goldenrod         = color::build_color(238, 232, 170);
		constexpr color pale_green             = color::build_color(115, 222, 120);
		constexpr color pale_turquoise         = color::build_color(175, 238, 238);
		constexpr color pale_violet_red        = color::build_color(219, 112, 147);
		constexpr color papaya_whip            = color::build_color(255, 239, 213);
		constexpr color peach_puff             = color::build_color(255, 218, 185);
		constexpr color peru                   = color::build_color(205, 133, 63);
		constexpr color pink                   = color::build_color(255, 181, 197);
		constexpr color plum                   = color::build_color(197, 72, 155);
		constexpr color powder_blue            = color::build_color(176, 224, 230);
		constexpr color purple                 = color::build_color(160, 32, 240);
		constexpr color red                    = color::build_color(255, 0, 0);
		constexpr color rosy_brown             = color::build_color(188, 143, 143);
		constexpr color royal_blue             = color::build_color(65, 105, 225);
		constexpr color saddle_brown           = color::build_color(139, 69, 19);
		constexpr color salmon                 = color::build_color(233, 150, 122);
		constexpr color sandy_brown            = color::build_color(244, 164, 96);
		constexpr color sea_green              = color::build_color(82, 149, 132);
		constexpr color sea_shell              = color::build_color(255, 245, 238);
		constexpr color sienna                 = color::build_color(150, 82, 45);
		constexpr color sky_blue               = color::build_color(114, 159, 255);
		constexpr color slate_blue             = color::build_color(126, 136, 171);
		constexpr color slate_gray             = color::build_color(112, 128, 144);
		constexpr color snow                   = color::build_color(255, 250, 250);
		constexpr color spring_green           = color::build_color(65, 172, 65);
		constexpr color steel_blue             = color::build_color(84, 112, 170);
		constexpr color tan                    = color::build_color(222, 184, 135);
		constexpr color thistle                = color::build_color(216, 191, 216);
		constexpr color tomato                 = color::build_color(255, 99, 71);
		constexpr color transparent            = color::build_color(0, 0, 1);
		constexpr color turquoise              = color::build_color(25, 204, 223);
		constexpr color violet                 = color::build_color(156, 62, 206);
		constexpr color violet_red             = color::build_color(243, 62, 150);
		constexpr color wheat                  = color::build_color(245, 222, 179);
		constexpr color white                  = color::build_color(255, 255, 255);
		constexpr color white_smoke            = color::build_color(245, 245, 245);
		constexpr color yellow                 = color::build_color(255, 255, 0);
		constexpr color yellow_green           = color::build_color(50, 216, 56);

		// greys
		constexpr color gray[] =
		{
			color::build_color(0, 0, 0),
			color::build_color(3, 3, 3),
			color::build_color(5, 5, 5),
			color::build_color(8, 8, 8),
			color::build_color(10, 10, 10),
			color::build_color(13, 13, 13),
			color::build_color(15, 15, 15),
			color::build_color(18, 18, 18),
			color::build_color(20, 20, 20),
			color::build_color(23, 23, 23),
			color::build_color(26, 26, 26),
			color::build_color(28, 28, 28),
			color::build_color(31, 31, 31),
			color::build_color(33, 33, 33),
			color::build_color(36, 36, 36),
			color::build_color(38, 38, 38),
			color::build_color(41, 41, 41),
			color::build_color(43, 43, 43),
			color::build_color(46, 46, 46),
			color::build_color(48, 48, 48),
			color::build_color(51, 51, 51),
			color::build_color(54, 54, 54),
			color::build_color(56, 56, 56),
			color::build_color(59, 59, 59),
			color::build_color(61, 61, 61),
			color::build_color(64, 64, 64),
			color::build_color(66, 66, 66),
			color::build_color(69, 69, 69),
			color::build_color(71, 71, 71),
			color::build_color(74, 74, 74),
			color::build_color(77, 77, 77),
			color::build_color(79, 79, 79),
			color::build_color(82, 82, 82),
			color::build_color(84, 84, 84),
			color::build_color(87, 87, 87),
			color::build_color(89, 89, 89),
			color::build_color(92, 92, 92),
			color::build_color(94, 94, 94),
			color::build_color(97, 97, 97),
			color::build_color(99, 99, 99),
			color::build_color(102, 102, 102),
			color::build_color(105, 105, 105),
			color::build_color(107, 107, 107),
			color::build_color(110, 110, 110),
			color::build_color(112, 112, 112),
			color::build_color(115, 115, 115),
			color::build_color(117, 117, 117),
			color::build_color(120, 120, 120),
			color::build_color(122, 122, 122),
			color::build_color(125, 125, 125),
			color::build_color(127, 127, 127),
			color::build_color(130, 130, 130),
			color::build_color(133, 133, 133),
			color::build_color(135, 135, 135),
			color::build_color(138, 138, 138),
			color::build_color(140, 140, 140),
			color::build_color(143, 143, 143),
			color::build_color(145, 145, 145),
			color::build_color(148, 148, 148),
			color::build_color(150, 150, 150),
			color::build_color(153, 153, 153),
			color::build_color(156, 156, 156),
			color::build_color(158, 158, 158),
			color::build_color(161, 161, 161),
			color::build_color(163, 163, 163),
			color::build_color(166, 166, 166),
			color::build_color(168, 168, 168),
			color::build_color(171, 171, 171),
			color::build_color(173, 173, 173),
			color::build_color(176, 176, 176),
			color::build_color(179, 179, 179),
			color::build_color(181, 181, 181),
			color::build_color(184, 184, 184),
			color::build_color(186, 186, 186),
			color::build_color(189, 189, 189),
			color::build_color(191, 191, 191),
			color::build_color(194, 194, 194),
			color::build_color(196, 196, 196),
			color::build_color(199, 199, 199),
			color::build_color(201, 201, 201),
			color::build_color(204, 204, 204),
			color::build_color(207, 207, 207),
			color::build_color(209, 209, 209),
			color::build_color(212, 212, 212),
			color::build_color(214, 214, 214),
			color::build_color(217, 217, 217),
			color::build_color(219, 219, 219),
			color::build_color(222, 222, 222),
			color::build_color(224, 224, 224),
			color::build_color(227, 227, 227),
			color::build_color(229, 229, 229),
			color::build_color(232, 232, 232),
			color::build_color(235, 235, 235),
			color::build_color(237, 237, 237),
			color::build_color(240, 240, 240),
			color::build_color(242, 242, 242),
			color::build_color(245, 245, 245),
			color::build_color(247, 247, 247),
			color::build_color(250, 250, 250),
			color::build_color(252, 252, 252),
			color::build_color(255, 255, 255),
		};
	}
}

 #endif
