/*=============================================================================
   Copyright (c) 2016-2020 Joel de Guzman

   Distributed under the MIT License [ https://opensource.org/licenses/MIT ]
=============================================================================*/
#if !defined(ELEMENTS_CONTEXT_APRIL_17_2016)
#define ELEMENTS_CONTEXT_APRIL_17_2016

#include <infra/string_view.hpp>
#include <elements/support/point.hpp>
#include <elements/support/rect.hpp>
#include <functional>

namespace cycfi::elements
{
	////////////////////////////////////////////////////////////////////////////////////////////////
	// Forward declarations
	class view;
	class element;
	class canvas;

	point cursor_pos(const view & v);
	rect view_bounds(const view & v);
	point device_to_user(point p, canvas& cnv);
	rect device_to_user(const rect & r, canvas& cnv);

	////////////////////////////////////////////////////////////////////////////////////////////////
	// Contexts
	////////////////////////////////////////////////////////////////////////////////////////////////

	struct basic_context
	{
		basic_context(elements::view& view_, elements::canvas& cnv)
			: view(view_), canvas(cnv) {}

		basic_context(const basic_context &) = default;
		basic_context& operator=(const basic_context &) = delete;

		[[nodiscard]] rect view_bounds() const
		{
			return device_to_user(elements::view_bounds(view), canvas);
		}

		[[nodiscard]] point cursor_pos() const
		{
			return device_to_user(elements::cursor_pos(view), canvas);
		}

		elements::view& view;
		elements::canvas& canvas;
	};

	////////////////////////////////////////////////////////////////////////////////////////////////
	class context : public basic_context
	{
	public:

		context(const context & rhs, elements::rect bounds_)
			:
			  basic_context(rhs.view, rhs.canvas),
			  element(rhs.element), parent(rhs.parent), bounds(bounds_) {}

		context(const context & parent_, element* element_, elements::rect bounds_)
			:
			  basic_context(parent_.view, parent_.canvas),
			  element(element_), parent(&parent_), bounds(bounds_) {}

		context(elements::view& view_, elements::canvas& canvas_, element* element_, elements::rect bounds_)
			:
			  basic_context(view_, canvas_),
			  element(element_), parent(nullptr), bounds(bounds_) {}

		context(const context &) = default;
		context& operator=(const context &) = delete;

		[[nodiscard]] context sub_context() const
		{
			auto ctx = context{ *this };
			ctx.parent = this;
			return ctx;
		}

		template <typename T, typename F>
		void listen(F&& f)
		{
			_listener =
					[f](const auto & ctx, auto* e, auto what)
			{
				if (auto te = dynamic_cast<T*>(e))
					f(ctx, *te, what);
			};
		}

		void notify(const context & ctx, string_view what, elements::element* e) const
		{
			if (_listener)
				_listener(ctx, e, what);
			if (parent)
				parent->notify(ctx, what, e);
		}

		elements::element* element;
		const context * parent;
		elements::rect bounds;

	private:

		using listener_function =
				std::function<
				        void(const context & ctx, elements::element*, string_view what)
						>;

		listener_function _listener;
	};
}

#endif
