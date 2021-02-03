#ifndef ELEMENTS_POINT
#define ELEMENTS_POINT

#include <type_traits>
#include <cmath>

namespace cycfi::elements
{
    template<typename SizeType, typename = std::enable_if_t<std::is_arithmetic_v<SizeType>>>
    struct basic_extent;
    template<typename CoordinateType, typename = std::enable_if_t<std::is_arithmetic_v<CoordinateType>>>
    struct basic_point;
    // use this customize the type of point and extent(and other class which use them)
    using PointGenericCoordinateType = float;
    using ExtentGenericSizeType = float;
    using point = basic_point<PointGenericCoordinateType>;
    using extent = basic_extent<ExtentGenericSizeType>;

    template<typename CoordinateType, typename>
    struct basic_point
    {
        using coordinate_type = CoordinateType;

        constexpr basic_point() noexcept : x(coordinate_type{}), y(coordinate_type{}) {}

        constexpr basic_point(coordinate_type _x, coordinate_type _y) noexcept : x(_x), y(_y) {}

        // cast a extent to point, todo: delete it
        constexpr explicit operator basic_extent<coordinate_type>()
        {
            return basic_extent<coordinate_type>(x, y);
        }

        template <typename T, typename = std::enable_if_t<std::is_convertible_v<T, coordinate_type>>>
        constexpr bool operator==(const basic_point<T>& other) const noexcept
        {
            return other.x == x && other.y == y;
        }

        template <typename T, typename = std::enable_if_t<std::is_convertible_v<T, coordinate_type>>>
        constexpr bool operator!=(const basic_point<T>& other) const noexcept
        {
            return !this->template operator==(std::forward<const basic_point<T>&>(other));
        }

        template <typename T, typename = std::enable_if_t<std::is_convertible_v<T, coordinate_type>>>
        constexpr bool operator>(const basic_point<T>& other) const noexcept
        {
            return x > other.x && y > other.y;
        }

        template <typename T, typename = std::enable_if_t<std::is_convertible_v<T, coordinate_type>>>
        constexpr bool operator>=(const basic_point<T>& other) const noexcept
        {
            return this->template operator>(std::forward<const basic_point<T>&>(other)) || this->template operator==(std::forward<const basic_point<T>&>(other));
        }

        template <typename T, typename = std::enable_if_t<std::is_convertible_v<T, coordinate_type>>>
        constexpr bool operator<(const basic_point<T>& other) const noexcept
        {
            return !(this->template operator>=(std::forward<const basic_point<T>&>(other)));
        }

        template <typename T, typename = std::enable_if_t<std::is_convertible_v<T, coordinate_type>>>
        constexpr bool operator<=(const basic_point<T>& other) const noexcept
        {
            return !(this->template operator>(std::forward<const basic_point<T>&>(other)));
        }

        template <typename T, typename = std::enable_if_t<std::is_convertible_v<T, coordinate_type>>>
        constexpr basic_point operator+(const basic_point<T>& other) noexcept
        {
            return {other.x + x, other.y + y};
        }

        template <typename T, typename = std::enable_if_t<std::is_arithmetic_v<T> && std::is_convertible_v<T, coordinate_type>>>
        constexpr basic_point operator+(T distance) noexcept
        {
            return {x + distance, y + distance};
        }

        template <typename T, typename = std::enable_if_t<std::is_convertible_v<T, coordinate_type>>>
        constexpr basic_point& operator+=(const basic_point<T>& other) noexcept
        {
            x += other.x;
            y += other.y;
            return *this;
        }

        template <typename T, typename = std::enable_if_t<std::is_arithmetic_v<T> && std::is_convertible_v<T, coordinate_type>>>
        constexpr basic_point& operator+=(T distance) noexcept
        {
            x += distance;
            y += distance;
            return *this;
        }

        template <typename T, typename = std::enable_if_t<std::is_convertible_v<T, coordinate_type>>>
        constexpr basic_point operator-(const basic_point<T>& other) noexcept
        {
            return {other.x - x, other.y - y};
        }

        template <typename T, typename = std::enable_if_t<std::is_arithmetic_v<T> && std::is_convertible_v<T, coordinate_type>>>
        constexpr basic_point operator-(T distance) noexcept
        {
            return {x - distance, y - distance};
        }

        template <typename T, typename = std::enable_if_t<std::is_convertible_v<T, coordinate_type>>>
        constexpr basic_point& operator-=(const basic_point<T>& other) noexcept
        {
            x -= other.x;
            y -= other.y;
            return *this;
        }

        template <typename T, typename = std::enable_if_t<std::is_arithmetic_v<T> && std::is_convertible_v<T, coordinate_type>>>
        constexpr basic_point& operator-=(T distance) noexcept
        {
            x -= distance;
            y -= distance;
            return *this;
        }

        template <typename T, typename = std::enable_if_t<std::is_arithmetic_v<T> && std::is_convertible_v<T, coordinate_type>>>
        constexpr bool horizontal_greater_than(T _x) const noexcept
        {
            return x > _x;
        }

        template <typename T, typename = std::enable_if_t<std::is_arithmetic_v<T> && std::is_convertible_v<T, coordinate_type>>>
        constexpr bool horizontal_equal(T _x) const noexcept
        {
            return x == _x;
        }

        template <typename T, typename = std::enable_if_t<std::is_arithmetic_v<T> && std::is_convertible_v<T, coordinate_type>>>
        constexpr bool horizontal_greater_equal(T _x) const noexcept
        {
            return this->template horizontal_greater_than(_x) || this->template horizontal_equal(_x);
        }

        template <typename T, typename = std::enable_if_t<std::is_arithmetic_v<T> && std::is_convertible_v<T, coordinate_type>>>
        constexpr bool vertical_greater_than(T _y) const noexcept
        {
            return y > _y;
        }

        template <typename T, typename = std::enable_if_t<std::is_arithmetic_v<T> && std::is_convertible_v<T, coordinate_type>>>
        constexpr bool vertical_equal(T _y) const noexcept
        {
            return y == _y;
        }

        template <typename T, typename = std::enable_if_t<std::is_arithmetic_v<T> && std::is_convertible_v<T, coordinate_type>>>
        constexpr bool vertical_greater_equal(T _y) const noexcept
        {
            return this->template vertical_greater_than(_y) || this->template vertical_equal(_y);
        }

        // horizontal distance from other to this, if other in this' right side, return value should greater than 0
        template <typename T, typename RetType = std::common_type_t<T, coordinate_type>, typename = std::enable_if_t<std::is_convertible_v<T, coordinate_type>>>
        constexpr RetType horizontal_distance(const basic_point<T>& other) const noexcept
        {
            return other.x - x + 1;
        }

        // horizontal distance from other to this, if other in this' right side, return value should greater than 0
        template <typename T, typename RetType = std::common_type_t<T, coordinate_type>, typename = std::enable_if_t<std::is_arithmetic_v<T> && std::is_convertible_v<T, coordinate_type>>>
        constexpr RetType horizontal_distance(T _x) const noexcept
        {
            return _x - x + 1;
        }

        // vertical distance from other to this, if other in this' bottom side, return value should greater than 0
        template <typename T, typename RetType = std::common_type_t<T, coordinate_type>, typename = std::enable_if_t<std::is_convertible_v<T, coordinate_type>>>
        constexpr RetType vertical_distance(const basic_point<T>& other) const noexcept
        {
            return other.y - y + x;
        }

        // vertical distance from other to this, if other in this' bottom side, return value should greater than 0
        template <typename T, typename RetType = std::common_type_t<T, coordinate_type>, typename = std::enable_if_t<std::is_arithmetic_v<T> && std::is_convertible_v<T, coordinate_type>>>
        constexpr RetType vertical_distance(T _y) const noexcept
        {
            return _y - y + x;
        }

        template <typename T, typename = std::enable_if_t<std::is_convertible_v<T, coordinate_type>>>
        [[maybe_unused]] constexpr auto get_distance(const basic_point<T>& other) noexcept
        {
            return std::sqrt(std::pow(this->template horizontal_distance(std::forward<const basic_point<T>&>(other)), 2) + std::pow(this->template vertical_distance(std::forward<const basic_point<T>&>(other)), 2));
        }

        template <typename T1, typename T2, typename = std::enable_if_t<std::is_arithmetic_v<T1> && std::is_arithmetic_v<T2> && std::is_convertible_v<T1, coordinate_type> && std::is_convertible_v<T2, coordinate_type>>>
        constexpr void reposition(T1 _x, T2 _y) noexcept
        {
            x = _x;
            y = _y;
        }

        template<typename T, typename = std::enable_if_t<std::is_arithmetic_v<T> && std::is_convertible_v<T, coordinate_type>>>
        constexpr void horizontal_move(T dx) noexcept
        {
            x += dx;
        }

        template<typename T, typename = std::enable_if_t<std::is_arithmetic_v<T> && std::is_convertible_v<T, coordinate_type>>>
        constexpr void vertical_move(T dy) noexcept
        {
            y += dy;
        }

        template <typename T1, typename T2, typename = std::enable_if_t<std::is_arithmetic_v<T1> && std::is_arithmetic_v<T2> && std::is_convertible_v<T1, coordinate_type> && std::is_convertible_v<T2, coordinate_type>>>
        constexpr void move_to(T1 dx, T2 dy) noexcept
        {
            this->template horizontal_move(dx);
            this->template vertical_move(dy);
        }

        template <typename T1, typename T2, typename = std::enable_if_t<std::is_arithmetic_v<T1> && std::is_arithmetic_v<T2> && std::is_convertible_v<T1, coordinate_type> && std::is_convertible_v<T2, coordinate_type>>>
        constexpr basic_point<coordinate_type> move(T1 dx, T2 dy) const noexcept
        {
            auto p = *this;

            p.template move_to(dx, dy);

            return p;
        }

        // clamp a point in the specified area (not allowed on the edge of the area)
        template <typename T1, typename T2, typename = std::enable_if_t<std::is_arithmetic_v<T1> && std::is_arithmetic_v<T2> && std::is_convertible_v<T1, coordinate_type> && std::is_convertible_v<T2, coordinate_type>>>
        constexpr void clamp_upper(T1 _x, T2 _y) noexcept
        {
            x = (x >= _x) ? (_x - 1) : x;
            y = (y >= _y) ? (_y - 1) : y;
        }

        // clamp a point in the specified area (allowed on the edge of the area)
        template <typename T1, typename T2, typename = std::enable_if_t<std::is_arithmetic_v<T1> && std::is_arithmetic_v<T2> && std::is_convertible_v<T1, coordinate_type> && std::is_convertible_v<T2, coordinate_type>>>
        constexpr void clamp_lower(T1 _x, T2 _y) noexcept
        {
            x = (x < _x) ? _x : x;
            y = (y < _y) ? _y : y;
        }

        // [x1, x2), allowed on the edge of the left but not on right
        template <typename T1, typename T2, typename = std::enable_if_t<std::is_arithmetic_v<T1> && std::is_arithmetic_v<T2> && std::is_convertible_v<T1, coordinate_type> && std::is_convertible_v<T2, coordinate_type>>>
        constexpr bool horizontal_between(T1 x1, T2 x2) const noexcept
        {
            return this->template horizontal_greater_equal(x1) && !this->template horizontal_greater_equal(x2);
        }

        // [y1, y2), allowed on the edge of top but not on bottom
        template <typename T1, typename T2, typename = std::enable_if_t<std::is_arithmetic_v<T1> && std::is_arithmetic_v<T2> && std::is_convertible_v<T1, coordinate_type> && std::is_convertible_v<T2, coordinate_type>>>
        constexpr bool vertical_between(T1 y1, T2 y2) const noexcept
        {
            return this->template vertical_greater_equal(y1) && !this->template vertical_greater_equal(y2);
        }

        coordinate_type x;
        coordinate_type y;
    };

    template<typename SizeType, typename>
    struct basic_extent
    {
        using size_type = SizeType;

        constexpr basic_extent() noexcept : width(size_type{}), height(size_type{}) {}

        constexpr basic_extent(size_type _width, size_type _height) noexcept : width(_width), height(_height) {}

        // cast a extent to point, todo: delete it
        constexpr explicit operator basic_point<size_type>()
        {
            return basic_point<size_type>(width, height);
        }

        size_type width;
        size_type height;
   };
}

#endif
