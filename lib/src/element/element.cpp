/*=============================================================================
   Copyright (c) 2016-2020 Joel de Guzman

   Distributed under the MIT License [ https://opensource.org/licenses/MIT ]
=============================================================================*/
#include <elements/element/element.hpp>
#include <elements/support.hpp>
#include <elements/view.hpp>

namespace cycfi::elements
{
	////////////////////////////////////////////////////////////////////////////
	// element class implementation
	////////////////////////////////////////////////////////////////////////////
	view_limits element::limits(const basic_context & /* ctx */) const
	{
        return full_limits<view_limits::coordinate_type>;
	}

	view_stretch element::stretch() const
	{
        return { 1.0f, 1.0f };
	}

	unsigned element::span() const
	{
        return 1;
	}

	element* element::hit_test(const context & ctx, point p)
	{
        return (ctx.bounds.includes(p)) ? this : nullptr;
	}

	void element::draw(const context & /* ctx */)
	{}

	void element::layout(const context & /* ctx */)
	{}

	void element::refresh(const context & ctx, element& element, int outward)
	{
        if (&element == this)
			ctx.view.refresh(ctx, outward);
	}

	bool element::click(const context & /* ctx */, mouse_button /* btn */)
	{
		return false;
	}

	void element::drag(const context & /* ctx */, mouse_button /* btn */)
	{}

	bool element::key(const context & /* ctx */, key_info /* k */)
	{
		return false;
	}

	bool element::text(const context & /* ctx */, text_info /* info */)
	{
		return false;
	}

	bool element::cursor(const context & /* ctx */, point /* p */, cursor_tracking /* status */)
	{
		return false;
	}

	bool element::scroll(const context & /* ctx */, point /* dir */, point /* p */)
	{
		return false;
	}

	bool element::wants_focus() const
	{
		return false;
	}

	void element::begin_focus()
	{}

	void element::end_focus()
	{}

	const element * element::focus() const
	{
        return this;
	}

	element* element::focus()
	{
        return this;
	}

	bool element::wants_control() const
	{
        return false;
	}

	void element::on_tracking(const context & ctx, tracking state)
	{
        ctx.view.manage_on_tracking(*this, state);
	}
}
