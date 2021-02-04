#ifndef ELEMENTS_ENUM_OPERATOR
#define ELEMENTS_ENUM_OPERATOR

/*
 _______   ______      .__   __.   ______   .___________.    __  .__   __.   ______  __       __    __   _______   _______
|       \ /  __  \     |  \ |  |  /  __  \  |           |   |  | |  \ |  |  /      ||  |     |  |  |  | |       \ |   ____|
|  .--.  |  |  |  |    |   \|  | |  |  |  | `---|  |----`   |  | |   \|  | |  ,----'|  |     |  |  |  | |  .--.  ||  |__
|  |  |  |  |  |  |    |  . `  | |  |  |  |     |  |        |  | |  . `  | |  |     |  |     |  |  |  | |  |  |  ||   __|
|  '--'  |  `--'  |    |  |\   | |  `--'  |     |  |        |  | |  |\   | |  `----.|  `----.|  `--'  | |  '--'  ||  |____
|_______/ \______/     |__| \__|  \______/      |__|        |__| |__| \__|  \______||_______| \______/  |_______/ |_______|

.___________. __    __   __       _______.    _______  __   __       _______     __  .__   __.
|           ||  |  |  | |  |     /       |   |   ____||  | |  |     |   ____|   |  | |  \ |  |
`---|  |----`|  |__|  | |  |    |   (----`   |  |__   |  | |  |     |  |__      |  | |   \|  |
    |  |     |   __   | |  |     \   \       |   __|  |  | |  |     |   __|     |  | |  . `  |
    |  |     |  |  |  | |  | .----)   |      |  |     |  | |  `----.|  |____    |  | |  |\   |
    |__|     |__|  |__| |__| |_______/       |__|     |__| |_______||_______|   |__| |__| \__|

 __    __   _______     ___       _______   _______ .______          _______  __   __       _______
|  |  |  | |   ____|   /   \     |       \ |   ____||   _  \        |   ____||  | |  |     |   ____|
|  |__|  | |  |__     /  ^  \    |  .--.  ||  |__   |  |_)  |       |  |__   |  | |  |     |  |__
|   __   | |   __|   /  /_\  \   |  |  |  ||   __|  |      /        |   __|  |  | |  |     |   __|
|  |  |  | |  |____ /  _____  \  |  '--'  ||  |____ |  |\  \----.   |  |     |  | |  `----.|  |____
|__|  |__| |_______/__/     \__\ |_______/ |_______|| _| `._____|   |__|     |__| |_______||_______|

   ______   .___________. __    __   _______ .______     ____    __    ____  __       _______. _______
 /  __  \  |           ||  |  |  | |   ____||   _  \    \   \  /  \  /   / |  |     /       ||   ____|
|  |  |  | `---|  |----`|  |__|  | |  |__   |  |_)  |    \   \/    \/   /  |  |    |   (----`|  |__
|  |  |  |     |  |     |   __   | |   __|  |      /      \            /   |  |     \   \    |   __|
|  `--'  |     |  |     |  |  |  | |  |____ |  |\  \----.  \    /\    /    |  | .----)   |   |  |____
 \______/      |__|     |__|  |__| |_______|| _| `._____|   \__/  \__/     |__| |_______/    |_______|

.___________. __    __   _______    ____    __    ____  ______   .______       __       _______
|           ||  |  |  | |   ____|   \   \  /  \  /   / /  __  \  |   _  \     |  |     |       \
`---|  |----`|  |__|  | |  |__       \   \/    \/   / |  |  |  | |  |_)  |    |  |     |  .--.  |
    |  |     |   __   | |   __|       \            /  |  |  |  | |      /     |  |     |  |  |  |
    |  |     |  |  |  | |  |____       \    /\    /   |  `--'  | |  |\  \----.|  `----.|  '--'  |
    |__|     |__|  |__| |_______|       \__/  \__/     \______/  | _| `._____||_______||_______/

____    __    ____  __   __       __         .______    _______
\   \  /  \  /   / |  | |  |     |  |        |   _  \  |   ____|
 \   \/    \/   /  |  | |  |     |  |        |  |_)  | |  |__
  \            /   |  | |  |     |  |        |   _  <  |   __|
   \    /\    /    |  | |  `----.|  `----.   |  |_)  | |  |____
    \__/  \__/     |__| |_______||_______|   |______/  |_______|

 _______   _______     _______.___________..______        ______   ____    ____
|       \ |   ____|   /       |           ||   _  \      /  __  \  \   \  /   /
|  .--.  ||  |__     |   (----`---|  |----`|  |_)  |    |  |  |  |  \   \/   /
|  |  |  ||   __|     \   \       |  |     |      /     |  |  |  |   \_    _/
|  '--'  ||  |____.----)   |      |  |     |  |\  \----.|  `--'  |     |  |
|_______/ |_______|_______/       |__|     | _| `._____| \______/      |__|
 */

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
	template <typename EnumType, typename Enum, typename = std::enable_if_t<std::is_enum_v<Enum> && std::is_convertible_v<EnumType, std::underlying_type_t<Enum>>>>
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
	template <typename Enum, typename EnumType, typename = std::enable_if_t<std::is_enum_v<Enum>>, typename = std::enable_if_t<std::is_convertible_v<EnumType, std::underlying_type_t<Enum>>>>
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
	template <typename Enum, typename EnumType = std::underlying_type_t<Enum>, typename = std::enable_if_t<std::is_enum_v<Enum>>>
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
	template <typename EnumType, typename Enum, typename = std::enable_if_t<std::is_enum_v<Enum>>, typename = std::enable_if_t<std::is_convertible_v<EnumType, std::underlying_type_t<Enum>>>>
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
	template <typename EnumType, typename Enum, typename = std::enable_if_t<std::is_enum_v<Enum> && std::is_convertible_v<EnumType, std::underlying_type_t<Enum>>>>
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
	template <typename Enum, typename EnumType, typename = std::enable_if_t<std::is_enum_v<Enum>>, typename = std::enable_if_t<std::is_convertible_v<EnumType, std::underlying_type_t<Enum>>>>
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
	template <typename Enum, typename EnumType = std::underlying_type_t<Enum>, typename = std::enable_if_t<std::is_enum_v<Enum>>>
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
	template <typename EnumType, typename Enum, typename = std::enable_if_t<std::is_enum_v<Enum>>, typename = std::enable_if_t<std::is_convertible_v<EnumType, std::underlying_type_t<Enum>>>>
	constexpr EnumType operator&=(EnumType& lhs, const Enum& rhs)
	{
		return (lhs = static_cast<EnumType>(lhs & static_cast<EnumType>(rhs)));
	}

	template <typename EnumType, typename Enum, typename = std::enable_if_t<std::is_enum_v<Enum> && std::is_convertible_v<EnumType, std::underlying_type_t<Enum>>>>
	constexpr EnumType operator-(const EnumType& lhs, const Enum& rhs)
	{
		return lhs - static_cast<EnumType>(rhs);
	}

	template <typename Enum, typename EnumType, typename = std::enable_if_t<std::is_enum_v<Enum>>, typename = std::enable_if_t<std::is_convertible_v<EnumType, std::underlying_type_t<Enum>>>>
	constexpr EnumType operator-(const Enum& lhs, const EnumType& rhs)
	{
		return lhs - static_cast<EnumType>(rhs);
	}

	template <typename EnumType, typename Enum, typename = std::enable_if_t<std::is_enum_v<Enum> && std::is_convertible_v<EnumType, std::underlying_type_t<Enum>>>>
	constexpr EnumType operator-=(EnumType& lhs, const Enum& rhs)
	{
		return (lhs -= static_cast<EnumType>(rhs));
	}

	template <typename EnumType, typename Enum, typename = std::enable_if_t<std::is_enum_v<Enum> && std::is_convertible_v<EnumType, std::underlying_type_t<Enum>>>>
	constexpr EnumType operator+(const EnumType& lhs, const Enum& rhs)
	{
		return lhs + static_cast<EnumType>(rhs);
	}

	template <typename Enum, typename EnumType, typename = std::enable_if_t<std::is_enum_v<Enum>>, typename = std::enable_if_t<std::is_convertible_v<EnumType, std::underlying_type_t<Enum>>>>
	constexpr EnumType operator+(const Enum& lhs, const EnumType& rhs)
	{
		return lhs + static_cast<EnumType>(rhs);
	}

	template <typename Enum, typename EnumType = std::underlying_type_t<Enum>, typename = std::enable_if_t<std::is_enum_v<Enum>>, typename = std::enable_if_t<std::is_convertible_v<EnumType, std::underlying_type_t<Enum>>>>
	constexpr EnumType operator+=(EnumType& lhs, const Enum& rhs)
	{
		return (lhs += static_cast<EnumType>(rhs));
	}

	template<typename Enum1, typename Enum2, typename EnumType = std::common_type_t<std::underlying_type_t<Enum1>, std::underlying_type_t<Enum2>>, typename = std::enable_if_t<std::is_enum_v<Enum1> && std::is_enum_v<Enum2>>>
	constexpr EnumType operator-(const Enum1& lhs, const Enum2& rhs)
	{
		return static_cast<EnumType>(static_cast<EnumType>(lhs) - rhs);
	}

	template<typename Enum1, typename Enum2, typename EnumType = std::common_type_t<std::underlying_type_t<Enum1>, std::underlying_type_t<Enum2>>, typename = std::enable_if_t<std::is_enum_v<Enum1> && std::is_enum_v<Enum2>>>
	constexpr EnumType operator+(const Enum1& lhs, const Enum2& rhs)
	{
		return static_cast<EnumType>(static_cast<EnumType>(lhs) + rhs);
	}

	template <typename EnumType, typename Enum, typename = std::enable_if_t<std::is_enum_v<Enum> && std::is_convertible_v<EnumType, std::underlying_type_t<Enum>>>>
	constexpr bool operator>(const EnumType& lhs, const Enum& rhs)
	{
		return lhs > static_cast<EnumType>(rhs);
	}

	template <typename Enum, typename EnumType, typename = std::enable_if_t<std::is_enum_v<Enum>>, typename = std::enable_if_t<std::is_convertible_v<EnumType, std::underlying_type_t<Enum>>>>
	constexpr bool operator>(const Enum& lhs, const EnumType& rhs)
	{
		return static_cast<EnumType>(lhs) > rhs;
	}

	template<typename Enum1, typename Enum2, typename EnumType = std::common_type_t<std::underlying_type_t<Enum1>, std::underlying_type_t<Enum2>>, typename = std::enable_if_t<std::is_enum_v<Enum1>>, typename = std::enable_if_t<std::is_enum_v<Enum2>>>
	constexpr bool operator>(const Enum1& lhs, const Enum2& rhs)
	{
		return static_cast<EnumType>(lhs) > static_cast<EnumType>(rhs);
	}

	template <typename EnumType, typename Enum, typename = std::enable_if_t<std::is_enum_v<Enum> && std::is_convertible_v<EnumType, std::underlying_type_t<Enum>>>>
	constexpr bool operator==(const EnumType& lhs, const Enum& rhs)
	{
		return lhs == static_cast<EnumType>(rhs);
	}

	template <typename Enum, typename EnumType, typename = std::enable_if_t<std::is_enum_v<Enum>>, typename = std::enable_if_t<std::is_convertible_v<EnumType, std::underlying_type_t<Enum>>>>
	constexpr bool operator==(const Enum& lhs, const EnumType& rhs)
	{
		return static_cast<EnumType>(lhs) == rhs;
	}

	template<typename Enum1, typename Enum2, typename EnumType = std::common_type_t<std::underlying_type_t<Enum1>, std::underlying_type_t<Enum2>>, typename = std::enable_if_t<std::is_enum_v<Enum1>>, typename = std::enable_if_t<std::is_enum_v<Enum2>>>
	constexpr bool operator==(const Enum1& lhs, const Enum2& rhs)
	{
		return static_cast<EnumType>(lhs) == static_cast<EnumType>(rhs);
	}

	template <typename EnumType, typename Enum, typename = std::enable_if_t<std::is_enum_v<Enum> && std::is_convertible_v<EnumType, std::underlying_type_t<Enum>>>>
	constexpr bool operator!=(const EnumType& lhs, const Enum& rhs)
	{
		return !(lhs == rhs);
	}

	template <typename Enum, typename EnumType, typename = std::enable_if_t<std::is_enum_v<Enum>>, typename = std::enable_if_t<std::is_convertible_v<EnumType, std::underlying_type_t<Enum>>>>
	constexpr bool operator!=(const Enum& lhs, const EnumType& rhs)
	{
		return !(lhs == rhs);
	}

	template<typename Enum1, typename Enum2, typename EnumType = std::common_type_t<std::underlying_type_t<Enum1>, std::underlying_type_t<Enum2>>, typename = std::enable_if_t<std::is_enum_v<Enum1>>, typename = std::enable_if_t<std::is_enum_v<Enum2>>>
	constexpr bool operator!=(const Enum1& lhs, const Enum2& rhs)
	{
		return !(lhs == rhs);
	}

	template <typename EnumType, typename Enum, typename = std::enable_if_t<std::is_enum_v<Enum> && std::is_convertible_v<EnumType, std::underlying_type_t<Enum>>>>
	constexpr bool operator<(const EnumType& lhs, const Enum& rhs)
	{
		return !(lhs > rhs || lhs == rhs);
	}

	template <typename Enum, typename EnumType, typename = std::enable_if_t<std::is_enum_v<Enum>>, typename = std::enable_if_t<std::is_convertible_v<EnumType, std::underlying_type_t<Enum>>>>
	constexpr bool operator<(const Enum& lhs, const EnumType& rhs)
	{
		return !(lhs > rhs || lhs == rhs);
	}

	template<typename Enum1, typename Enum2, typename EnumType = std::common_type_t<std::underlying_type_t<Enum1>, std::underlying_type_t<Enum2>>, typename = std::enable_if_t<std::is_enum_v<Enum1>>, typename = std::enable_if_t<std::is_enum_v<Enum2>>>
	constexpr bool operator<(const Enum1& lhs, const Enum2& rhs)
	{
		return !(lhs > rhs || lhs == rhs);
	}

	template <typename EnumType, typename Enum, typename = std::enable_if_t<std::is_enum_v<Enum> && std::is_convertible_v<EnumType, std::underlying_type_t<Enum>>>>
	constexpr auto operator*(const EnumType& lhs, const Enum& rhs)
	{
		return lhs * static_cast<EnumType>(rhs);
	}

	template <typename Enum, typename EnumType, typename = std::enable_if_t<std::is_enum_v<Enum>>, typename = std::enable_if_t<std::is_convertible_v<EnumType, std::underlying_type_t<Enum>>>>
	constexpr auto operator*(const Enum& lhs, const EnumType& rhs)
	{
		return rhs * lhs;
	}

	template<typename Enum1, typename Enum2, typename EnumType = std::common_type_t<std::underlying_type_t<Enum1>, std::underlying_type_t<Enum2>>, typename = std::enable_if_t<std::is_enum_v<Enum1>>, typename = std::enable_if_t<std::is_enum_v<Enum2>>>
	constexpr auto operator*(const Enum1& lhs, const Enum2& rhs)
	{
		return static_cast<EnumType>(lhs) * rhs;
	}

	template <typename EnumType, typename Enum, typename = std::enable_if_t<std::is_enum_v<Enum> && std::is_convertible_v<EnumType, std::underlying_type_t<Enum>>>>
	constexpr auto operator/(const EnumType& lhs, const Enum& rhs)
	{
		return lhs / static_cast<EnumType>(rhs);
	}

	template <typename Enum, typename EnumType, typename = std::enable_if_t<std::is_enum_v<Enum>>, typename = std::enable_if_t<std::is_convertible_v<EnumType, std::underlying_type_t<Enum>>>>
	constexpr auto operator/(const Enum& lhs, const EnumType& rhs)
	{
		return 1 / (rhs / lhs);
	}

	template<typename Enum1, typename Enum2, typename EnumType = std::common_type_t<std::underlying_type_t<Enum1>, std::underlying_type_t<Enum2>>, typename = std::enable_if_t<std::is_enum_v<Enum1>>, typename = std::enable_if_t<std::is_enum_v<Enum2>>>
	constexpr auto operator/(const Enum1& lhs, const Enum2& rhs)
	{
		return static_cast<EnumType>(lhs) / rhs;
	}
}

#endif
