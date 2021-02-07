/*=============================================================================
   Copyright (c) 2016-2020 Joel de Guzman

   Distributed under the MIT License [ https://opensource.org/licenses/MIT ]
=============================================================================*/
#if !defined(ELEMENTS_PROXY_APRIL_10_2016)
#define ELEMENTS_PROXY_APRIL_10_2016

#include <elements/element/element.hpp>
#include <type_traits>

namespace cycfi::elements
{
	////////////////////////////////////////////////////////////////////////////
	// Proxies
	//
	// A element that encapsulates another element (its 'subject'). The proxy
	// delegates its methods to its encapsulated subject, but may augment
	// or totally override its behavior.
	////////////////////////////////////////////////////////////////////////////
	class proxy_base : public element
	{
	public:

		// Display
		view_limits limits(basic_context const& ctx) const override;
		view_stretch stretch() const override;
		unsigned span() const override;
		element* hit_test(context const& ctx, point p) override;
		void draw(const context & ctx) override;
		void layout(const context & ctx) override;
		void refresh(const context & ctx, element& element, int outward /* = 0 */) override;
		virtual void prepare_subject(context& ctx);
		virtual void prepare_subject(context& ctx, point& p);
		virtual void restore_subject(context& ctx);

		using element::refresh;

		// Control
		bool wants_control() const override;
		bool click(const context & ctx, mouse_button btn) override;
		void drag(const context & ctx, mouse_button btn) override;
		bool key(const context & ctx, key_info k) override;
		bool text(const context & ctx, text_info info) override;
		bool cursor(const context & ctx, point p, cursor_tracking status) override;
		bool scroll(const context & ctx, point dir, point p) override;

		bool wants_focus() const override;
		void begin_focus() override;
		void end_focus() override;
		const element * focus() const override;
		element* focus() override;

		// Proxy
		virtual const element & subject() const = 0;
		virtual element& subject() = 0;
	};

	template <typename Subject, typename Base = proxy_base>
	class proxy : public Base
	{
	public:

		static_assert(std::is_base_of_v<proxy_base, Base>,
		        "proxy Base type needs to be or inherit from proxy_base");
		static_assert(!std::is_reference_v<Subject>,
		        "Subject must not be a reference type - maybe you want to use reference class instead");
		static_assert(!std::is_const_v<Subject>, "Subject must not be const");

		template <typename... T>
		// need implicit conversions
		proxy(Subject subject_, T&&... args)
			: Base(std::forward<T>(args)...), _subject(std::move(subject_)) {}

		void subject(Subject&& subject)
		{
			_subject = std::move(subject);
		}

		void subject(const Subject & subject)
		{
			_subject = subject;
		}

		[[nodiscard]] const element& subject() const override
		{
			return _subject;
		}

		[[nodiscard]] element& subject() override
		{
			return _subject;
		}

	private:
		Subject _subject;
	};
}

#endif
