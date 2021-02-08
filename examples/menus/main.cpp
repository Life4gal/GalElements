#include <elements.hpp>
#include <algorithm>
#include <random>
#include <iostream>

template<typename Text, typename... More>
void print(Text t, More... more)
{
	using std::cout, std::endl;
	cout << t << ' ';
	if constexpr (sizeof...(more) == 0)
	{
		cout << endl;
	}
	else
	{
		print(more...);
	}
}

using namespace cycfi::elements;

auto make_selection_menu()
{
	return selection_menu(
			"You need select a item!",
			[](std::string_view select, size_t where)
			{
			  print("build_selection_menu_label:", select, "has been selected with index", where);
			},
			{
					"menu1",
					"menu2",
					"menu3",
					"menu4",
					"menu5",
					"menu6",
			}
	).first; // We return only the first, the menu. the second is a shared pointer to the label.
}

auto make_popup_menu(const char * title, menu_position pos)
{
	auto popup  = button_menu(title, pos);

	// just for demo; avoid statics!
	static bool enable = false;
	auto controller = menu_item("Flip Button1 and Button2 Availability");

	controller.on_click = []{enable = !enable;};

	auto button1 = menu_item("Button1");
	auto button2 = menu_item("Button2");

	button1.is_enabled = []{return !enable;};
	button2.is_enabled = []{return enable;};

	auto sk1 = shortcut_key{ key_code::g, mod_action };
	auto sk2 = shortcut_key{ key_code::c, mod_action + mod_shift };
	auto sk3 = shortcut_key{ key_code::b, mod_action + mod_alt };

	auto button3 = menu_item("Button3", sk1);
	auto button4 = menu_item("Button4");
	auto button5 = menu_item("Button5", sk2);
	auto button6 = menu_item("Button6");

	button3.on_click = []{print("Button3 has been clicked but nothing happened!");};
	button4.on_click = []{print("Button4 has been clicked but nothing happened!");};
	button5.on_click = []{print("Button5 has been clicked but nothing happened!");};
	button6.on_click = []{print("Button6 has been clicked but nothing happened!");};

	auto menu =
			layer(
					vtile(
							controller,
							button1,
							button2,
							menu_item_spacer(),
							button3,
							button4,
							button5,
							button6,
							menu_item_spacer(),
							menu_item("extra button", sk3),
							menu_item("one more")
					),
					panel{}
			);

	popup.menu(hsize(300, menu));

	return popup;
}

auto make_dynamic_menu(const char * title, menu_position pos)
{
	auto popup  = button_menu(title, pos);

	// This will be called every time the menu button is clicked
	// We will simulate dynamic creation of the menu items here.
	auto populate_menu =
			[](auto& popup)
			{
			  // Sample menu
			  const char * items[] = {"So", "So", "So", "So", "So", "So", "So", "So", "Many", "Buttons", "Here!"};

			  // Dynamically generate your menu here
			  vtile_composite list;
			  for (auto item : items)
				  list.push_back(share(menu_item(item)));

			  // Shuffle the items just for kicks
			  std::random_device rd;
			  std::mt19937 g(rd());
			  std::shuffle(list.begin(), list.end(), g);

			  // Show only a portion of the menu, providing a scroller
			  // to allow scrolling over all the items.
			  auto menu = layer(vsize(150, vscroller(list)), panel{});

			  // Install the menu
			  popup.menu(menu);
			};

	// Populate menu dynamically
	popup.on_open_menu = populate_menu;
	return popup;
}

auto make_menus()
{
	return margin(
			/* ______________________________________________________
			 * | = margin-top                                       |
			 * | = margin-top                                       |
			 * | = margin-top                                       |
			 * | <- margin-left ->    ---------   <- margin-right ->|
			 * |                      |       |                     |
			 * |                      |       | <-------------------|------ draw your widget
			 * |                      |       |                     |
			 * |                      |       |                     | <========== our view
			 * |                      |_______|                     |
			 * | = margin-bottom                                    |
			 * | = margin-bottom                                    |
			 * | = margin-bottom                                    |
			 * _____________________________________________________|
			 */
			{ 20, 0, 20, 20 },
			vtile(
					hmin_size(300, make_selection_menu()),
					top_margin(20, make_popup_menu("Drop-down Menu", menu_position::bottom_right)),
					top_margin(20, make_dynamic_menu("Dynamic Menu", menu_position::bottom_right)),
					top_margin(20, scroller(image{ "deep_space.jpg" })),
					top_margin(20, make_popup_menu("Drop-up Menu", menu_position::top_right))
			)
	);
}

auto make_controls()
{
	return margin({ 20, 20, 20, 20 }, pane("Menus", make_menus()));
}

int main(int argc, char* argv[])
{
	// Main window background color
	auto constexpr bkd_color = color::build_color(35, 35, 37, 255);
	auto background = box(bkd_color);

	app app(argc, argv, "Menus", "com.cycfi.menus");
	window win(app.name());
	win.on_close = [&app]() { app.stop(); };

	view view(win);

	view.content(
			make_controls(),
			background
			);

	return app.run();
}
