/*=============================================================================
   Copyright (c) 2016-2020 Joel de Guzman

   Distributed under the MIT License [ https://opensource.org/licenses/MIT ]
=============================================================================*/
#if !defined(ELEMENTS_TRACKER_AUGUST_29_2016)
#define ELEMENTS_TRACKER_AUGUST_29_2016

#include <elements/support/point.hpp>
#include <elements/element/element.hpp>
#include <memory>
#include <type_traits>

namespace cycfi::elements
{
	struct tracker_info
	{
		explicit tracker_info(point start_, int modifiers_ = 0)
			: start(start_), modifiers(modifiers_) {}

		point start;
		point current = start;
		point previous = start;
		point offset = point{ 0, 0 };
		int modifiers = 0;
	};

	////////////////////////////////////////////////////////////////////////////
	// tracker tracks the mouse movement.
	////////////////////////////////////////////////////////////////////////////
	template <typename Base = element>
	class tracker : public Base
	{
	public:

		template <typename... T>
		explicit tracker(T&&... args)
			: Base(args...) {}

		tracker(const tracker & rhs) : Base(rhs) {}
		tracker& operator=(const tracker & rhs)
		{
			Base::operator=(rhs);
			state.reset();
			return *this;
		}

		tracker(tracker&& rhs) noexcept = default;
		tracker& operator=(tracker&& rhs) noexcept = default;

		[[nodiscard]] bool wants_control() const override
		{
			return true;
		}

		bool click(const context & ctx, mouse_button btn) override
		{
			if (btn.down)
			{
				state = new_state(ctx, btn.pos, btn.modifiers);
				this->on_tracking(ctx, Base::tracking::begin_tracking);
				begin_tracking(ctx, *state);
			}
			else if (state)
			{
				this->on_tracking(ctx, Base::tracking::end_tracking);
				end_tracking(ctx, *state);
				state.reset();
			}
			return true;
		}

		void drag(const context & ctx, mouse_button btn) override
		{
			this->on_tracking(ctx, Base::tracking::while_tracking);
			state->previous = state->current;
			state->current = btn.pos;
			state->current.move(-state->offset.x, -state->offset.y);
			state->modifiers = btn.modifiers;
			keep_tracking(ctx, *state);
		}

	protected:
		using tracker_info_ptr = std::unique_ptr<tracker_info>;

		virtual tracker_info_ptr new_state(const context & /* ctx */, point start, int modifiers)
		{
			return std::make_unique<tracker_info>(start, modifiers);
		}

		virtual void begin_tracking(const context & /* ctx */, tracker_info& /* track_info */) {}
		virtual void keep_tracking(const context & /* ctx */, tracker_info& /* track_info */) {}
		virtual void end_tracking(const context & /* ctx */, tracker_info& /* track_info */) {}

		void track_scroll(const context & ctx, point /* dir */, point /* p */)
		{
			this->on_tracking(ctx, element::tracking::while_tracking);
		}

	private:
		tracker_info_ptr state;
	};
}

#endif
