/*=============================================================================
   Copyright (c) 2016-2020 Joel de Guzman

   Distributed under the MIT License [ https://opensource.org/licenses/MIT ]
=============================================================================*/
#if !defined(ELEMENTS_ELEMENT_APRIL_10_2016)
#define ELEMENTS_ELEMENT_APRIL_10_2016

#include <elements/base_view.hpp>
#include <elements/support/receiver.hpp>
#include <elements/support/rect.hpp>

#include <infra/string_view.hpp>
#include <memory>
#include <type_traits>

namespace cycfi::elements
{
	struct basic_context;
	class context;

	////////////////////////////////////////////////////////////////////////////
	// Elements
	//
	// This is the class that deals with the graphic representation of fine-
	// grained elements inside a window which may be static graphics or active
	// controls.
	////////////////////////////////////////////////////////////////////////////
	class element : public std::enable_shared_from_this<element>
	{
	public:
		virtual ~element() = default;

		// Display
		virtual view_limits limits(const basic_context & ctx) const;
		virtual view_stretch stretch() const;
		virtual unsigned span() const;
		virtual element* hit_test(const context & ctx, point p);
		virtual void draw(const context & ctx);
		virtual void layout(const context & ctx);
		virtual void refresh(const context & ctx, element& element, int outward /* = 0 */);
		void refresh(const context & ctx, int outward = 0) { refresh(ctx, *this, outward); }

		// Control
		virtual bool wants_control() const;
		virtual bool click(const context & ctx, mouse_button btn);
		virtual void drag(const context & ctx, mouse_button btn);
		virtual bool key(const context & ctx, key_info k);
		virtual bool text(const context & ctx, text_info info);
		virtual bool cursor(const context & ctx, point p, cursor_tracking status);
		virtual bool scroll(const context & ctx, point dir, point p);

		virtual bool wants_focus() const;
		virtual void begin_focus();
		virtual void end_focus();
		virtual const element * focus() const;
		virtual element* focus();

		enum class tracking
		{
			none,
			begin_tracking,
			while_tracking,
			end_tracking
		};

	protected:
		void on_tracking(context const& ctx, tracking state);
	};

	////////////////////////////////////////////////////////////////////////////
	using element_ptr = std::shared_ptr<element>;
	using element_const_ptr = std::shared_ptr<const element>;
	using weak_element_ptr = std::weak_ptr<element>;
	using weak_element_const_ptr = std::weak_ptr<const element>;

	template <typename Element>
	inline auto share(Element&& e)
	{
		using element_type = typename std::decay<Element>::type;
		return std::make_shared<element_type>(std::forward<Element>(e));
	}

	template <typename Element>
	inline auto get(const std::shared_ptr<Element> & ptr)
	{
		return std::weak_ptr<Element>(ptr);
	}

	inline element empty()
	{
		return {};
	}
}

#endif
