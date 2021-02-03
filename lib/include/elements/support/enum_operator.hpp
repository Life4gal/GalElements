#ifndef ELEMENTS_ENUM_OPERATOR
#define ELEMENTS_ENUM_OPERATOR

namespace cycfi::elements
{
	// operator for enum class

	/*
	 * <del>enum = flag1 | enum1</del>
	 * <del>usage:</del>
	 * <del>     if(flag1 | enum1 == ENUM::XXX){xxx;}</del>
	 * <del>     switch(flag1 | enum1){case ENUM:XXX: xxx;}</del>
	 *
	 * <del>note: if no exist enumerates equal flag, it will lose all data(it maybe equal to the first enumerate but no guarantee)</del>
	 *
	 * flag = flag1 | enum1
	 * make return type to EnumType(which used to be Enum)
	 *
	 * usage:
	 *      EnumType flag = xxx;
	 *      ......
	 *      EnumType another_flag = flag | some_enum;
	 *
	 *      <del>std::array<CoordinateType, (1 | EnumMax)> replace std::array<CoordinateType, static_cast<size_t>(EnumMax)> also supported</del>
	 */
	template <typename Enum, typename EnumType, typename = std::enable_if_t<std::is_enum_v<Enum>>, typename = std::enable_if_t<std::is_same_v<EnumType, std::underlying_type_t<Enum>>>>
	constexpr EnumType operator|(const EnumType& lhs, const Enum& rhs)
	{
		// using EnumType | EnumType
		return static_cast<EnumType>(lhs | static_cast<EnumType>(rhs));
	}

	/*
	 * <del>enum = enum1 | flag1</del>
	 * <del>usage:</del>
	 * <del>     if(enum1 | flag1 == ENUM::XXX){xxx;}</del>
	 * <del>     switch(enum1 | flag1){case ENUM:XXX: xxx;}</del>
	 *
	 * <del>note: if no exist enumerates equal flag, it will lose all data(it maybe equal to the first enumerate but no guarantee)</del>
	 *
	 * enum = enum1 | flag1
	 * although the enum may not exist, but it will store it's data correctly by use it's underlying type
	 *
	 * usage:
	 *      Enum enum = xxx;
	 *      ......
	 *      Enum another_enum = enum | some_enum_type_val;
	 */
	template <typename Enum, typename EnumType, typename = std::enable_if_t<std::is_enum_v<Enum>>, typename = std::enable_if_t<std::is_same_v<EnumType, std::underlying_type_t<Enum>>>>
	constexpr Enum operator|(const Enum& lhs, const EnumType& rhs)
	{
		// forward to EnumType operator|(const EnumType& lhs, const Enum& rhs)
		// forward is necessary ?
		return static_cast<Enum>(std::forward<const EnumType&>(rhs) | std::forward<const Enum&>(lhs));
	}

	/*
	 * <del>flag = enum1 | enum2</del>
	 * <del>if no exist enumerates equal flag, it will lose all data(it maybe equal to the first enumerate but no guarantee)</del>
	 * <del>so cast it's type to EnumType</del>
	 *
	 * enum = enum1 | enum2
	 * although the enum may not exist, but it will store it's data correctly by use it's underlying type
	 * make return type to Enum(which used to be EnumType)
	 *
	 * usage:
	 *      void Foo(Enum arg);
	 *      call Foo(enum1 | enum2) -> (enum1 | enum2) <=> enum
	 *
	 *      void Bar(Enum arg)
	 *      {
	 *          switch(arg | EnumMask)
	 *          {
	 *              case enum1: xxx
	 *              case enum2: xxx
	 *              ...
	 *          }
	 *      }
	 */
	template <typename Enum, typename EnumType = std::underlying_type_t<Enum>, typename = std::enable_if_t<std::is_enum_v<Enum>>, typename = std::enable_if_t<std::is_same_v<EnumType, std::underlying_type_t<Enum>>>>
	constexpr Enum operator |(const Enum& lhs, const Enum& rhs)
	{
		// using EnumType | EnumType
		return static_cast<Enum>(static_cast<EnumType>(lhs) | static_cast<EnumType>(rhs));
	}

	/*
	 * flag |= enum
	 * only flag |= enum operator is valid
	 * use enum |= flag is unreasonable
	 */
	template <typename Enum, typename EnumType, typename = std::enable_if_t<std::is_enum_v<Enum>>, typename = std::enable_if_t<std::is_same_v<EnumType, std::underlying_type_t<Enum>>>>
	constexpr EnumType operator|=(EnumType& lhs, const Enum& rhs)
	{
		// forward to EnumType operator|(const EnumType& lhs, const Enum& rhs)
		// forward is necessary ?
		return (lhs = (std::forward<const EnumType&>(lhs) | std::forward<const Enum&>(rhs)));
	}

	/*
	 * <del>enum = flag1 & enum1</del>
	 * <del>usage:</del>
	 * <del>     if(flag1 & enum1 == ENUM::XXX){xxx;}</del>
	 * <del>     switch(flag1 & enum1){case ENUM:XXX: xxx;}</del>
	 *
	 * <del>note: if no exist enumerates equal flag, it will lose all data(it maybe equal to the first enumerate but no guarantee)</del>
	 *
	 * flag = flag1 & enum1
	 * make return type to EnumType(which used to be Enum)
	 *
	 * usage:
	 *      EnumType flag = xxx;
	 *      ......
	 *      EnumType another_flag = flag & some_enum;
	 */
	template <typename Enum, typename EnumType, typename = std::enable_if_t<std::is_enum_v<Enum>>, typename = std::enable_if_t<std::is_same_v<EnumType, std::underlying_type_t<Enum>>>>
	constexpr EnumType operator&(const EnumType& lhs, const Enum& rhs)
	{
		// using EnumType & EnumType
		return static_cast<EnumType>(lhs & static_cast<EnumType>(rhs));
	}

	/*
	 * <del>enum = enum1 & flag1</del>
	 * <del>usage:</del>
	 * <del>     if(enum1 & flag1 == ENUM::XXX){xxx;}</del>
	 * <del>     switch(enum1 & flag1){case ENUM:XXX: xxx;}</del>
	 *
	 * <del>note: if no exist enumerates equal flag, it will lose all data(it maybe equal to the first enumerate but no guarantee)</del>
	 *
	 * enum = enum1 & flag1
	 * although the enum may not exist, but it will store it's data correctly by use it's underlying type
	 *
	 * usage:
	 *      Enum enum = xxx;
	 *      ......
	 *      Enum another_enum = enum & some_enum_type_val;
	 */
	template <typename Enum, typename EnumType, typename = std::enable_if_t<std::is_enum_v<Enum>>, typename = std::enable_if_t<std::is_same_v<EnumType, std::underlying_type_t<Enum>>>>
	constexpr Enum operator&(const Enum& lhs, const EnumType& rhs)
	{
		// forward to EnumType operator&(const EnumType& lhs, const Enum& rhs)
		// forward is necessary ?
		return static_cast<Enum>(std::forward<const EnumType&>(rhs) & std::forward<const Enum&>(lhs));
	}

	/*
	 * <del>flag = enum1 & enum2</del>
	 * <del>if no exist enumerates equal flag, it will lose all data(it maybe equal to the first enumerate but no guarantee)</del>
	 * <del>so cast it's type to EnumType</del>
	 *
	 * enum = enum1 & enum2
	 * although the enum may not exist, but it will store it's data correctly by use it's underlying type
	 * make return type to Enum(which used to be EnumType)
	 *
	 * usage:
	 *      void Foo(Enum arg);
	 *      call Foo(enum1 | enum2) -> (enum1 | enum2) <=> enum
	 *
	 *      void Bar(Enum arg)
	 *      {
	 *          switch(arg & EnumMask)
	 *          {
	 *              case enum1: xxx
	 *              case enum2: xxx
	 *              ...
	 *          }
	 *      }
	 */
	template <typename Enum, typename EnumType = std::underlying_type_t<Enum>, typename = std::enable_if_t<std::is_enum_v<Enum>>, typename = std::enable_if_t<std::is_same_v<EnumType, std::underlying_type_t<Enum>>>>
	constexpr Enum operator&(const Enum& lhs, const Enum& rhs)
	{
		// using EnumType & EnumType
		return static_cast<Enum>(static_cast<EnumType>(lhs) & static_cast<EnumType>(rhs));
	}

	/*
	 * flag &= enum
	 * only flag &= enum operator is valid
	 * use enum &= flag is unreasonable
	 */
	template <typename EnumType, typename Enum, typename = std::enable_if_t<std::is_enum_v<Enum>>, typename = std::enable_if_t<std::is_same_v<EnumType, std::underlying_type_t<Enum>>>>
	constexpr EnumType operator&=(EnumType& lhs, const Enum& rhs)
	{
		return (lhs = static_cast<EnumType>(lhs & static_cast<EnumType>(rhs)));
	}

	template <typename Enum, typename EnumType = std::underlying_type_t<Enum>, typename = std::enable_if_t<std::is_enum_v<Enum>>, typename = std::enable_if_t<std::is_same_v<EnumType, std::underlying_type_t<Enum>>>>
	constexpr EnumType operator-(const EnumType& lhs, const Enum& rhs)
	{
		return lhs - static_cast<EnumType>(rhs);
	}

	template <typename Enum, typename EnumType = std::underlying_type_t<Enum>, typename = std::enable_if_t<std::is_enum_v<Enum>>, typename = std::enable_if_t<std::is_same_v<EnumType, std::underlying_type_t<Enum>>>>
	constexpr EnumType operator-=(EnumType& lhs, const Enum& rhs)
	{
		return (lhs -= static_cast<EnumType>(rhs));
	}

	template <typename Enum, typename EnumType = std::underlying_type_t<Enum>, typename = std::enable_if_t<std::is_enum_v<Enum>>, typename = std::enable_if_t<std::is_same_v<EnumType, std::underlying_type_t<Enum>>>>
	constexpr EnumType operator+(const EnumType& lhs, const Enum& rhs)
	{
		return lhs + static_cast<EnumType>(rhs);
	}

	template <typename Enum, typename EnumType = std::underlying_type_t<Enum>, typename = std::enable_if_t<std::is_enum_v<Enum>>, typename = std::enable_if_t<std::is_same_v<EnumType, std::underlying_type_t<Enum>>>>
	constexpr EnumType operator+=(EnumType& lhs, const Enum& rhs)
	{
		return (lhs += static_cast<EnumType>(rhs));
	}
}

#endif
