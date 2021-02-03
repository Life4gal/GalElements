#ifndef ELEMENTS_CIRCLE
#define ELEMENTS_CIRCLE

#include <elements/support/rect.hpp>
#include <algorithm>

namespace cycfi::elements
{
	template<typename CoordinateType, typename = std::enable_if_t<std::is_arithmetic_v<CoordinateType>>>
	struct basic_circle;
	using circle = basic_circle<point::coordinate_type>;

	template<typename CoordinateType, typename>
	struct basic_circle
	{
		using coordinate_type = CoordinateType;

		constexpr basic_circle() noexcept : center(), radius(coordinate_type{}) {}

		constexpr basic_circle(coordinate_type x, coordinate_type y, coordinate_type _radius) noexcept : center(x, y), radius(_radius) {}

		template <typename T, typename = std::enable_if_t<std::is_convertible_v<T, coordinate_type>>>
		constexpr basic_circle(const basic_point<T>& c, coordinate_type _radius) noexcept : center(c), radius(_radius) {}

		// get a rectangle's inscribed circle
		// we assume that the given rectangle is regular and inscribed circle has the same center point
		template <typename T>
		constexpr static basic_circle<T> get_inscribed_circle(const basic_rect<T>& r) noexcept
		{
			return {r.center_point(), std::min(r.width(), r.height())};
		}

		// get a circle's inscribed rectangle
		// inscribed rect has the same center point
		constexpr basic_rect<coordinate_type> get_inscribed_rect() const noexcept
		{
			auto size = std::sqrt(2) / 2 * radius;
			return {center.x - size, center.y - size, center.x + size, center.y + size};
		}

		// get a rectangle's circumscribed circle
		// we assume that the given rectangle is regular and inscribed circle has the same center point
		template <typename T>
		constexpr static basic_circle<T> get_circumscribed_circle(const basic_rect<T>& r) noexcept
		{
			auto center = r.center_point();
			auto radius = center.template get_distance(r.left_top());
			return {r.center_point(), radius};
		}

		// get a circle's circumscribed rectangle
		// circumscribed rect has the same center point
		constexpr basic_rect<coordinate_type> get_circumscribed_rect() const noexcept
		{
			auto size = radius;
			return {center.x - size, center.y - size, center.x + size, center.y + size};
		}

		template <typename T, typename = std::enable_if_t<std::is_convertible_v<T, coordinate_type>>>
		constexpr bool operator==(const basic_circle<T>& other) const noexcept
		{
			return other.center == center && other.radius == radius;
		}

		template <typename T, typename = std::enable_if_t<std::is_convertible_v<T, coordinate_type>>>
		constexpr bool operator!=(const basic_circle<T>& other) const noexcept
		{
			return !this->template operator==(std::forward<const basic_circle<T>&>(other));
		}

		template <typename T, typename = std::enable_if_t<std::is_convertible_v<T, coordinate_type>>>
		constexpr basic_circle<coordinate_type> inset(T x) const noexcept
		{
			auto c = *this;
			c.radius -= x;
			return c;
		}

		template <typename T1, typename T2, typename = std::enable_if_t<std::is_arithmetic_v<T1> && std::is_arithmetic_v<T2> && std::is_convertible_v<T1, coordinate_type> && std::is_convertible_v<T2, coordinate_type>>>
		constexpr basic_circle<coordinate_type> move(T1 dx, T2 dy) const noexcept
		{
			auto c = center;
			c.template move_to(dx, dy);
			return {c, radius};
		}

		template <typename T1, typename T2, typename = std::enable_if_t<std::is_arithmetic_v<T1> && std::is_arithmetic_v<T2> && std::is_convertible_v<T1, coordinate_type> && std::is_convertible_v<T2, coordinate_type>>>
		constexpr void move_to(T1 x, T2 y) const noexcept
		{
			center.template move_to(x, y);
		}

		basic_point<coordinate_type> center;
		coordinate_type radius;
   };
}

#endif
