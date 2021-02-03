#ifndef ELEMENTS_RECT
#define ELEMENTS_RECT

#include <elements/support/point.hpp>

namespace cycfi::elements
{
    template<typename CoordinateType, typename = std::enable_if_t<std::is_arithmetic_v<CoordinateType>>>
    struct basic_rect;
    using rect = basic_rect<point::coordinate_type>;

    template<typename CoordinateType, typename>
    struct basic_rect
    {
        using coordinate_type = CoordinateType;

        constexpr basic_rect() noexcept
            :
              left(coordinate_type{}),
              top(coordinate_type{}),
              right(coordinate_type{}),
              bottom(coordinate_type{}) {}

        constexpr basic_rect(coordinate_type _left, coordinate_type _top, coordinate_type _right, coordinate_type _bottom) noexcept
                : left(_left), top(_top), right(_right), bottom(_bottom) {}

        template <typename T, typename = std::enable_if_t<std::is_convertible_v<T, coordinate_type>>>
        constexpr basic_rect(const basic_point<T>& origin, coordinate_type right, coordinate_type bottom) noexcept
            : basic_rect(origin.x, origin.y, right, bottom) {}

        template <typename T, typename = std::enable_if_t<std::is_convertible_v<T, coordinate_type>>>
        constexpr basic_rect(coordinate_type left, coordinate_type top, const basic_extent<T>& size) noexcept
            : basic_rect(left, top, left + size.width, top + size.height) {}

        template <typename T1, typename T2, typename T = std::common_type_t<T1, T2>, typename = std::enable_if_t<std::is_convertible_v<T1, T2>>>
        constexpr basic_rect(const basic_point<T1>& origin, const basic_extent<T2>& size) noexcept
            : basic_rect<T>(origin.x, origin.y, origin.x + size.width, origin.y + size.height) {}

        template <typename T, typename = std::enable_if_t<std::is_convertible_v<T, coordinate_type>>>
        constexpr bool operator==(const basic_rect<T> & other) const noexcept
        {
//            return basic_point<coordinate_type>{left, top} == basic_point<coordinate_type>{other.left, other.top}
//                   && basic_point<coordinate_type>{right, bottom} == basic_point<coordinate_type>{other.right, other.bottom};

          return other.left == left && other.top == top && other.right == right && other.bottom == bottom;
        }

        template <typename T, typename = std::enable_if_t<std::is_convertible_v<T, coordinate_type>>>
        constexpr bool operator!=(const basic_rect<T> & other) const noexcept
        {
            return !this->operator==(std::forward<const basic_rect<T> &>(other));
        }

        [[nodiscard]] constexpr bool is_empty() const noexcept
        {
            return left == right || top == bottom;
        }

        [[nodiscard]] constexpr bool is_valid() const noexcept
        {
            return left <= right && top <= bottom;
        }

        // is self include another point?
        // [x, y), allowed on the edge of the left but not on right, allowed on the edge of top but not on bottom
        template <typename T, typename = std::enable_if_t<std::is_convertible_v<T, coordinate_type>>>
        [[nodiscard]] constexpr bool includes(const basic_point<T>& p) const noexcept
        {
            return p.template horizontal_between(left, right) && p.template vertical_between(top, bottom);
        }

        // is self include another rectangle?
        // [x, y), allowed on the edge of the left but not on right, allowed on the edge of top but not on bottom
        template <typename T, typename = std::enable_if_t<std::is_convertible_v<T, coordinate_type>>>
        [[nodiscard]] constexpr bool includes(const basic_rect<T>& other) const noexcept
        {
            return (other.left >= left) && (other.left < right) &&
                   (other.top >= top) && (other.top < bottom) &&
                   (other.right >= left) && (other.right < right) &&
                   (other.bottom >= top) && (other.bottom < bottom);
        }

        [[nodiscard]] constexpr coordinate_type width() const noexcept
        {
            return right - left;
        }

        template <typename T, typename = std::enable_if_t<std::is_arithmetic_v<T> && std::is_convertible_v<T, coordinate_type>>>
        constexpr void widen(T width) noexcept
        {
            right = left + width;
        }

        [[nodiscard]] constexpr coordinate_type height() const noexcept
        {
            return bottom - top;
        }

        template <typename T, typename = std::enable_if_t<std::is_arithmetic_v<T> && std::is_convertible_v<T, coordinate_type>>>
        constexpr void heighten(T height) noexcept
        {
            bottom = top + height;
        }

        [[nodiscard]] constexpr basic_extent<coordinate_type> size() const noexcept
        {
            return {width(), height()};
        }

        // shrink the right-bottom side of self
        // new width == size.width and left not changed
        // new height == size.height and top not changed
        template <typename T, typename = std::enable_if_t<std::is_convertible_v<T, coordinate_type>>>
        constexpr void resize_right(const basic_extent<T>& size) noexcept
        {
            right = left + size.width;
            bottom = top + size.height;
        }

        // shrink the left-top side of self
        // new width = size.width and right not changed
        // new height = size.height and bottom not changed
        template <typename T, typename = std::enable_if_t<std::is_convertible_v<T, coordinate_type>>>
        constexpr void resize_left(const basic_extent<T>& size) noexcept
        {
            left = right - size.width;
            top = bottom - size.height;
        }

        constexpr void clear() noexcept
        {
            *this = basic_rect<coordinate_type>();
        }

        [[nodiscard]] constexpr basic_point<coordinate_type> left_top() const noexcept
        {
            return {left, top};
        }

        [[nodiscard]] constexpr basic_point<coordinate_type> right_bottom() const noexcept
        {
            return {right, bottom};
        }

        [[nodiscard]] constexpr basic_point<coordinate_type> right_top() const noexcept
        {
            return {right, top};
        }

        [[nodiscard]] constexpr basic_point<coordinate_type> left_bottom() const noexcept
        {
            return {left, bottom};
        }

        [[nodiscard]] constexpr basic_point<coordinate_type> center_point() const noexcept
        {
            return {(left + right) / 2, (top + bottom) / 2};
        }

        // move self
        template <typename T1, typename T2, typename = std::enable_if_t<std::is_arithmetic_v<T1> && std::is_arithmetic_v<T2> && std::is_convertible_v<T1, coordinate_type> && std::is_convertible_v<T2, coordinate_type>>>
        constexpr void move_to(T1 dx, T2 dy) noexcept
        {
            left += dx;
            right += dx;

            top += dy;
            bottom += dy;
        }

        // move a copy of self
        template <typename T1, typename T2, typename = std::enable_if_t<std::is_arithmetic_v<T1> && std::is_arithmetic_v<T2> && std::is_convertible_v<T1, coordinate_type> && std::is_convertible_v<T2, coordinate_type>>>
        constexpr basic_rect<coordinate_type> move(T1 dx, T2 dy) const noexcept
        {
            auto r = *this;

            r.template move_to(dx, dy);

            return r;
        }

        // reposition self
        template <typename T1, typename T2, typename = std::enable_if_t<std::is_arithmetic_v<T1> && std::is_arithmetic_v<T2> && std::is_convertible_v<T1, coordinate_type> && std::is_convertible_v<T2, coordinate_type>>>
        constexpr void reposition(T1 x, T2 y) noexcept
        {
            right -= (left - x);
            left = x;

            bottom -= (top - y);
            top = y;
        }

        // reposition self
        template <typename T, typename = std::enable_if_t<std::is_convertible_v<T, coordinate_type>>>
        constexpr void reposition(const basic_point<T>& p) noexcept
        {
            this->template reposition(p.x, p.y);
        }

        // get a copy of self, then inset
        // is result rectangle is not valid, it will be a empty rectangle
        template <typename T1, typename T2, typename = std::enable_if_t<std::is_arithmetic_v<T1> && std::is_arithmetic_v<T2> && std::is_convertible_v<T1, coordinate_type> && std::is_convertible_v<T2, coordinate_type>>>
        [[nodiscard]] constexpr basic_rect<coordinate_type> inset(T1 horizontal = 1, T2 vertical = 1) const noexcept
        {
            auto r = *this;
            r.left += horizontal;
            r.right -= horizontal;

            r.top += vertical;
            r.bottom -= vertical;

            if(!r.is_valid())
            {
                r.clear();
            }
            return r;
        }

        // is two rectangle are intersects?
        template <typename T, typename = std::enable_if_t<std::is_convertible_v<T, coordinate_type>>>
        constexpr bool is_intersects(const basic_rect<T>& other) const noexcept
        {
            if(other.left >= right || other.right <= left || other.top >= bottom || other.bottom <= top)
            {
                return false;
            }
            return true;
        }

        // use self and another rectangle to construct a new rectangle
        // it like a union, but more bigger, include some areas that did not include before
        template <typename T, typename RetType = std::common_type_t<T, coordinate_type>, typename = std::enable_if_t<std::is_convertible_v<T, coordinate_type>>>
        constexpr basic_rect<RetType> reconstruct_max_with(const basic_rect<T>& other) const noexcept
        {
            return {std::min(left, other.left), std::min(top, other.top), std::max(right, other.right), std::max(bottom, other.bottom)};
        }

        // use self and another rectangle to construct a new rectangle
        // if self and other have intersection, the new rectangle is the intersection, otherwise, is not a valid rectangle
        template <typename T, typename RetType = std::common_type_t<T, coordinate_type>, typename = std::enable_if_t<std::is_convertible_v<T, coordinate_type>>>
        constexpr basic_rect<RetType> reconstruct_min_with(const basic_rect<T>& other) const noexcept
        {
            return {std::max(left, other.left), std::max(top, other.top), std::min(right, other.right), std::min(bottom, other.bottom)};
        }

        // move self to another rectangle's center position
        // self has the same center point with enclosure
        // (left + right) / 2 = (enclosure.left + enclosure.right) / 2
        // (top + bottom) / 2 = (enclosure.top + enclosure.bottom) / 2
        // better make sure self has smaller size than enclosure
        template <typename T, typename = std::enable_if_t<std::is_convertible_v<T, coordinate_type>>>
        constexpr void move_to_center(const basic_rect<T>& enclosure) noexcept
        {
            this->template reposition(enclosure.center_point());
            this->template move_to(-width() / 2, -height() / 2);
        }

        // move a copy of self to another rectangle's center position
        // self has the same center point with enclosure
        // (left + right) / 2 = (enclosure.left + enclosure.right) / 2
        // (top + bottom) / 2 = (enclosure.top + enclosure.bottom) / 2
        // better make sure self has smaller size than enclosure
        template <typename T, typename RetType = std::common_type_t<T, coordinate_type>, typename = std::enable_if_t<std::is_convertible_v<T, coordinate_type>>>
        constexpr basic_rect<RetType> get_rect_in_center(const basic_rect<T>& enclosure) const noexcept
        {
            auto r = *this;
            r.template move_to_center(std::forward<const basic_rect<T>&>(enclosure));
            return r;
        }

        // move self to another rectangle's horizontal center position
        // self has the same top, has the same horizontal center
        // (left + right) / 2 = (enclosure.left + enclosure.right) / 2
        // top = enclosure.top
        // better make sure self has smaller size than enclosure
        template <typename T, typename = std::enable_if_t<std::is_convertible_v<T, coordinate_type>>>
        constexpr void move_to_horizontal_center(const basic_rect<T>& enclosure) noexcept
        {
            this->template reposition(enclosure.center_point());
            this->template move_to(-width() / 2, -enclosure.height() / 2);
        }

        // move a copy of self to another rectangle's horizontal center position
        // self has the same top, has the same horizontal center
        // (left + right) / 2 = (enclosure.left + enclosure.right) / 2
        // top = enclosure.top
        // better make sure self has smaller size than enclosure
        template <typename T, typename RetType = std::common_type_t<T, coordinate_type>, typename = std::enable_if_t<std::is_convertible_v<T, coordinate_type>>>
        constexpr basic_rect<RetType> get_rect_in_horizontal_center(const basic_rect<T>& enclosure) const noexcept
        {
            auto r = *this;
            r.template move_to_horizontal_center(std::forward<const basic_rect<T>&>(enclosure));
            return r;
        }

        // move self to another rectangle's vertical center position
        // self has the same left, has the same vertical center
        // left = enclosure.left
        // (top + bottom) / 2 = (enclosure.top + enclosure.bottom) / 2
        // better make sure self has smaller size than enclosure
        template <typename T, typename = std::enable_if_t<std::is_convertible_v<T, coordinate_type>>>
        constexpr void move_to_vertical_center(const basic_rect<T>& enclosure) noexcept
        {
            this->template reposition(enclosure.center_point());
            this->template move_to(-enclosure.width() / 2, -height() / 2);
        }

        // move a copy of self to another rectangle's vertical center position
        // self has the same left, has the same vertical center
        // left = enclosure.left
        // (top + bottom) / 2 = (enclosure.top + enclosure.bottom) / 2
        // better make sure self has smaller size than enclosure
        template <typename T, typename RetType = std::common_type_t<T, coordinate_type>, typename = std::enable_if_t<std::is_convertible_v<T, coordinate_type>>>
        constexpr basic_rect<RetType> get_rect_in_vertical_center(const basic_rect<T>& enclosure) const noexcept
        {
            auto r = *this;
            r.template move_to_vertical_center(std::forward<const basic_rect<T>&>(enclosure));
            return r;
        }

        coordinate_type left;
        coordinate_type top;
        coordinate_type right;
        coordinate_type bottom;
   };
}

#endif
