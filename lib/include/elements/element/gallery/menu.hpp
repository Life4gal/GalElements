/*=============================================================================
   Copyright (c) 2016-2020 Joel de Guzman

   Distributed under the MIT License [ https://opensource.org/licenses/MIT ]
=============================================================================*/
#if !defined(ELEMENTS_GALLERY_MENU_JUNE_5_2016)
#define ELEMENTS_GALLERY_MENU_JUNE_5_2016

#include <elements/element/indirect.hpp>
#include <elements/element/menu.hpp>
#include <elements/element/size.hpp>
#include <elements/support/theme.hpp>
#include <elements/element/gallery/button.hpp>
#include <infra/string_view.hpp>
#include <string>

namespace cycfi::elements
{
	// this function will be call when menu been clicked
	// change callback in menu.cpp -->
	// std::pair<basic_menu, std::shared_ptr<basic_label>>
	// build_selection_menu_label(
	//			const menu_select_callback& on_select,
	//			menu_selector const& items
	//			)
	template<typename... Args>
	using menu_select_callback_template = std::function<void(string_view item, Args...args)>;

	// use of alias template requires template arguments
	// argument deduction not allowed in function prototype
	// so you can not simply use menu_select_callback_template
	// otherwise you need use template arg where it been used
	using menu_select_callback = std::function<void(string_view item, size_t where)>;

	////////////////////////////////////////////////////////////////////////////
	// Popup Button
	////////////////////////////////////////////////////////////////////////////
	basic_menu button_menu(
			std::string text,
			menu_position pos = menu_position::bottom_right,
			color body_color = get_theme().default_button_color
	);

	basic_menu button_menu(
			menu_position pos = menu_position::bottom_right,
			color body_color = get_theme().default_button_color
	);

	basic_menu icon_menu(
			uint32_t code,
			float size,
			menu_position pos = menu_position::bottom_right
	);

	////////////////////////////////////////////////////////////////////////////
	// Menu Background
	////////////////////////////////////////////////////////////////////////////
	using menu_background = panel; // We'll just reuse the panel now

	////////////////////////////////////////////////////////////////////////////
	// Menu Items
	////////////////////////////////////////////////////////////////////////////
	std::pair<std::string, std::string>
	display_shortcut(key_code k, int mod);

	inline std::pair<std::string, std::string>
	display_shortcut(key_info k)
	{
		return display_shortcut(k.key, k.modifiers);
	}

	inline auto menu_item_text(std::string text)
	{
        return hmargin({ 20, 20 }, align_left(label(std::move(text))));
	}

	inline auto menu_item_text(std::string text, shortcut_key shortcut)
	{
		auto [mod, key] = display_shortcut(shortcut.key, shortcut.modifiers);
#if defined(__APPLE__)
		auto sk_font = get_theme().system_font;
#else
		auto sk_font = get_theme().label_font;
#endif
		return hmargin({ 20, 10 },
					   htile(
							   htile(
									   align_left(label(std::move(text))),
									   align_right(label(mod).font(sk_font))
									   ),
#if defined(__APPLE__)
								left_margin(5, hsize(10, align_left(label(key))))
#else
								hsize(10, align_left(label(key)))
#endif
							   )
					   );
	}

	inline auto menu_item(std::string text)
	{
		return basic_menu_item(menu_item_text(std::move(text)));
	}

	inline auto menu_item(std::string text, shortcut_key shortcut)
	{
		auto r = basic_menu_item(menu_item_text(std::move(text), shortcut));
		r.shortcut = shortcut;
		return r;
	}

	struct menu_item_spacer_element : public element
	{
		view_limits          limits(basic_context const& ctx) const override;
		void                 draw(context const& ctx) override;
	};

	inline auto menu_item_spacer()
	{
		return menu_item_spacer_element{};
	}

	////////////////////////////////////////////////////////////////////////////
	// Selection Menu
	////////////////////////////////////////////////////////////////////////////
	struct menu_selector
	{
        virtual ~menu_selector() = default;

        [[nodiscard]] virtual std::size_t size() const = 0;
        virtual string_view operator[](std::size_t index) const = 0;
	};

	std::pair<basic_menu, std::shared_ptr<basic_label>>
	build_selection_menu_label(std::string&& init);

	std::pair<basic_menu, std::shared_ptr<basic_label>>
	build_selection_menu(
			std::string&& init,
			const menu_select_callback& on_select,
			const menu_selector & items
	);

	template <typename Sequence>
	inline std::pair<basic_menu, std::shared_ptr<basic_label>>
	selection_menu(
			std::string&& init,
			menu_select_callback on_select,
			const Sequence & seq,
			typename std::enable_if_t<!std::is_base_of_v<menu_selector, Sequence>>* = nullptr
	)
	{
		struct seq_menu_selector : menu_selector
		{
			explicit seq_menu_selector(const Sequence & seq_)
				: _seq(seq_){}

			[[nodiscard]] std::size_t size() const override
			{
				return std::size(_seq);
			}

			string_view
			operator[](std::size_t index) const override
			{
				return _seq[index];
			}

			const Sequence & _seq;
		};

		return build_selection_menu(std::move(init), on_select, seq_menu_selector{ seq });
	}

	template <typename T>
	std::pair<basic_menu, std::shared_ptr<basic_label>>
	selection_menu(
			std::string&& init,
			menu_select_callback on_select,
			std::initializer_list<T> list
			)
	{
		struct init_list_menu_selector : menu_selector
		{
			init_list_menu_selector(std::initializer_list<T> list_)
				: _list(list_) {}

			[[nodiscard]] std::size_t size() const override
			{
				return _list.size();
			}

			string_view operator[](std::size_t index) const override
			{
				return *(_list.begin()+index);
			}

			std::initializer_list<T> _list;
		};

		return build_selection_menu(std::move(init), on_select, init_list_menu_selector{ list });
	}
}

#endif
