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

/*
.___  ___.  _______ .__   __.  __    __
|   \/   | |   ____||  \ |  | |  |  |  |
|  \  /  | |  |__   |   \|  | |  |  |  |
|  |\/|  | |   __|  |  . `  | |  |  |  |
|  |  |  | |  |____ |  |\   | |  `--'  |
|__|  |__| |_______||__| \__|  \______/
*/
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
			{ 10, 10, 10, 10 },
			vtile(
					hmin_size(300, make_selection_menu()),
					top_margin(20, make_popup_menu("Drop-down Menu", menu_position::bottom_right)),
					top_margin(20, make_dynamic_menu("Dynamic Menu", menu_position::bottom_right)),
					top_margin(20, scroller(image{ "deep_space.jpg" })),
					top_margin(20, make_popup_menu("Drop-up Menu", menu_position::top_right))
					)
			);
}

auto make_menu_controls()
{
	return margin({ 10, 10, 10, 10 }, pane("Menus", make_menus()));
}

/*
     _______. __       __   _______   _______ .______              ___      .__   __.  _______
    /       ||  |     |  | |       \ |   ____||   _  \            /   \     |  \ |  | |       \
   |   (----`|  |     |  | |  .--.  ||  |__   |  |_)  |          /  ^  \    |   \|  | |  .--.  |
    \   \    |  |     |  | |  |  |  ||   __|  |      /          /  /_\  \   |  . `  | |  |  |  |
.----)   |   |  `----.|  | |  '--'  ||  |____ |  |\  \----.    /  _____  \  |  |\   | |  '--'  |
|_______/    |_______||__| |_______/ |_______|| _| `._____|   /__/     \__\ |__| \__| |_______/

 __  ___ .__   __.   ______   .______
|  |/  / |  \ |  |  /  __  \  |   _  \
|  '  /  |   \|  | |  |  |  | |  |_)  |
|    <   |  . `  | |  |  |  | |   _  <
|  .  \  |  |\   | |  `--'  | |  |_)  |
|__|\__\ |__| \__|  \______/  |______/
 */
using slider_ptr = std::shared_ptr<basic_slider_base>;
using dial_ptr = std::shared_ptr<dial_base>;

template<bool is_vertical>
auto make_slider_marker()
{
	auto track = basic_track<5 /* the width of the groove of the slider */, is_vertical>();
	// not necessary, if you do not need mark and label, just return track
	return slider_labels<10 /* the label size, or simply the distance between groove and label */>
	        (
					slider_marks<40 /* length of mark */>(track),    // Track with marks
					0.8,                        // Label font size (relative size)
					"0", "1", "2", "3", "4",    // The number of labels depends on how many "labels" you provide
					"5", "6", "7", "8", "9"
			);
}

auto make_horizontal_sliders(slider_ptr (&horizontal)[3])
{
	auto&& maker = [](slider_ptr& p_slider, double init_value)
	{
		p_slider = share(
				slider(
						basic_thumb<25 /* size of thumb */>(),
						        make_slider_marker<false>(),
						                init_value
						)
				);
		return align_middle(hmargin({20, 20}, hold(p_slider)));
	};

	return hmin_size(
			300,
			vtile(
					maker(horizontal[0], 0.0),
					maker(horizontal[1], 0.0),
					maker(horizontal[2], 0.0)
					)
			);
}

auto make_vertical_sliders(slider_ptr (&vertical)[3])
{
	auto&& maker = [](slider_ptr& p_slider, double init_value)
	{
		p_slider = share(
				slider(
						basic_thumb<25 /* size of thumb */>(),
						        make_slider_marker<true>(),
						                init_value
						)
				);
		return align_center(vmargin({20, 20}, hold(p_slider)));
	};

	return hmin_size(
			300,
			htile(
					maker(vertical[0], 0.0),
					maker(vertical[1], 0.0),
					maker(vertical[2], 0.0)
			)
	);
}

auto make_dials(dial_ptr (&dials)[3])
{
	auto&& maker = [](dial_ptr& p_dial, double init_value)
	{
		p_dial = share(
				dial(
						radial_marks<20/* length of radial mark line */>( // radial mark is not necessary
								basic_knob<50/* size of knob */>()
								        ),
						init_value)
				);
		auto markers = radial_labels<15> // also not necessary, use hold(p_dial)
				(
						hold(p_dial),
						0.7,
						"0", "1", "2", "3", "4",
						"5", "6", "7", "8", "9"
						);
		return align_center_middle(markers);
	};

	return hmargin(
			20,
			vtile(
					maker(dials[0], 0.0),
					maker(dials[1], 0.0),
					maker(dials[2], 0.0)
					)
			);
}

auto make_slider_and_knob_control(slider_ptr (&horizontal)[3], slider_ptr (&vertical)[3], dial_ptr (&dials)[3])
{
	return margin
			(
					{10, 10, 10, 10},
					vmin_size(300,
							  htile(
									  margin(
											  {10, 10, 10, 10},
											  pane(
													  "Horizontal Sliders",
													  make_horizontal_sliders(horizontal),
													  0.8f
													  )
											  ),
									  margin(
											  {10, 10, 10, 10},
											  pane(
													  "Vertical Sliders",
													  make_vertical_sliders(vertical),
													  0.8f
													  )
											  ),
									  hstretch(
											  0.5f,
											  margin(
													  {10, 10, 10, 10},
													  pane(
															  "Knobs",
															  make_dials(dials),
															  0.8f
															  )
													  )
											  )
									  )
							  )
					);
}

void link_slider_and_knob(slider_ptr (&horizontal)[3], slider_ptr (&vertical)[3], dial_ptr (&dials)[3], view& view)
{
	for(auto i = 0; i < 3; ++i)
	{
		horizontal[i]->on_change = [i, &vertical, &dials, &view](double value)
		{
			vertical[i]->slider_base::value(value);
			dials[i]->dial_base::value(value);
			view.refresh(vertical[i].operator*());
			view.refresh(dials[i].operator*());
			print("horizontal slider value changed to ->", value);
		};
		vertical[i]->on_change = [i, &horizontal, &dials, &view](double value)
		{
			horizontal[i]->slider_base::value(value);
			dials[i]->dial_base::value(value);
			view.refresh(horizontal[i].operator*());
			view.refresh(dials[i].operator*());
			print("vertical slider value changed to ->", value);
		};
		dials[i]->on_change = [i, &horizontal, &vertical, &view](double value)
		{
			horizontal[i]->slider_base::value(value);
			vertical[i]->slider_base::value(value);
			view.refresh(horizontal[i].operator*());
			view.refresh(vertical[i].operator*());
			print("dials value changed to ->", value);
		};
	}
}

/*
.______    __    __  .___________.___________.  ______   .__   __.
|   _  \  |  |  |  | |           |           | /  __  \  |  \ |  |
|  |_)  | |  |  |  | `---|  |----`---|  |----`|  |  |  | |   \|  |
|   _  <  |  |  |  |     |  |        |  |     |  |  |  | |  . `  |
|  |_)  | |  `--'  |     |  |        |  |     |  `--'  | |  |\   |
|______/   \______/      |__|        |__|      \______/  |__| \__|
 */
auto make_buttons(view& view)
{
	constexpr auto red = colors::red.opacity(0.4);
	constexpr auto green = colors::green.level(0.7).opacity(0.4);
	constexpr auto blue = colors::blue.opacity(0.4);
	constexpr auto royal_blue = colors::royal_blue.opacity(0.4);
	constexpr auto gold = colors::gold.opacity(0.8);

	auto momentary = button("Momentary Button");
	auto toggle = toggle_button("Toggle Button", 1.0, red);

	auto latching = share(latching_button("Latching Button", 1.0, green));
	auto reset = button("Clear Latch", icons::lock_open, 1.0, blue);

	auto note = button(icons::cog, "Setup", 1.0, royal_blue);

	auto progress = share(progress_bar(rbox(colors::black), rbox(gold)));
	auto progress_controller = button("Advance Progress Bar");

	reset.on_click = [latching, &view](bool)
	{
		latching->value(false);
		view.refresh(latching.operator*());
	};

	progress_controller.on_click = [progress, &view](bool)
	{
		auto value = progress->value() + 0.1;
		if(value >= 1)
		{
			progress->value(0.0);
		}
		else
		{
			progress->value(value);
		}
		view.refresh(progress.operator*());
	};

	return margin(
			{10, 10, 10, 10},
			vtile(
					top_margin(20, momentary),
					top_margin(20, toggle),
					top_margin(20, hold(latching)),
					top_margin(20, reset),
					top_margin(20, note),
					top_margin(20, vsize(25, hold(progress))),
					top_margin(20, progress_controller)
					)
			);
}

auto make_button_control(view& view)
{
	// todo: we should have some way to get which button had been selected, not give every button a trigger

	auto check1 = check_box("Check1");
	auto check2 = check_box("Check2");
	auto check3 = check_box("Check3");

	check1.value(true);
	check2.value(true);
	check3.value(false);

	auto check_group = group(
			"Check Something(Left/Center/Right)",
			margin(
					{10, 10, 10, 10},
					top_margin(25,
							   vtile(
									   top_margin(10, align_left(check1)),
									   top_margin(10, align_center(check2)),
									   top_margin(10, align_right(check3))
									   )
							   )
					)
			);

	auto radio1 = radio_button("Radio1");
	auto radio2 = radio_button("Radio2");
	auto radio3 = radio_button("Radio3");

	radio1.select(true);
	radio2.select(false);
	radio3.select(false);

	auto radio_group = group(
			"Select One(Right/Center/Left)",
			margin(
					{10, 10, 10, 10},
					top_margin(25,
							vtile(
									   top_margin(10, align_right(radio1)),
									   top_margin(10, align_center(radio2)),
									   top_margin(10, align_left(radio3))
									   )
							   )
					)
			);

	auto power = toggle_icon_button(icons::power, 1.2, get_theme().indicator_color);
	auto magnifying_glass = icon_button(icons::magnifying_glass, 1.2);
	auto left_circle = icon_button(icons::left_circled, 1.2);
	auto left_right = toggle_icon_button(icons::left, icons::right, 1.2);

	auto icon_group = group(
			"Icon Buttons(Messy Placement)",
			margin(
					{10, 10, 10, 10},
					vtile(
							top_margin(35,
									   htile(
											   align_center_top(power),
											   align_center_bottom(magnifying_glass)
											   )
									   ),
							top_margin(35,
									   htile(
											   align_left_bottom(left_circle),
											   align_left_top(left_right)
											   )
									   )
							)
					)
			);

	const auto scale = 1.0f / 4;
	auto power_sprite = toggle_button(sprite{"power_180x632.png", 158 * scale, scale});
	auto phase_sprite = toggle_button(sprite{"phase_180x632.png", 158 * scale, scale});
	auto mail_sprite = momentary_button(sprite{"mail_180x632.png", 158 * scale, scale});
	auto transport_sprite = toggle_button(sprite{"transport_180x632.png", 158 * scale, scale});

	auto sprite_group = group(
			"Sprite Buttons(Neat Placement)",
			margin(
					{10, 10, 10, 10},
					vtile(
							top_margin(35,
									   htile(
											   align_center(power_sprite),
											   align_center(phase_sprite)
											   )
									   ),
							top_margin(35,
									   htile(
											   align_center(mail_sprite),
											   align_center(transport_sprite)
											   )
									   )
							)
					)
			);

	return vtile(
			htile(
					make_buttons(view),
					vtile(
							margin({10, 10, 10, 10}, check_group),
							margin({10, 10, 10, 10}, radio_group)
							)
					),
			htile(
					hmin_size(250, margin({10, 10, 10, 10}, icon_group)),
					hmin_size(250, margin({10, 10, 10, 10}, sprite_group))
					)
			);
}

int main(int argc, char* argv[])
{
	// Main window background color
	auto constexpr bkd_color = color::build_color(35, 35, 37, 255);
	auto background = box(bkd_color);

	app _app(argc, argv, "test", "com.cycfi.test");
	window _win(_app.name());
	_win.on_close = [&_app]() { _app.stop(); };

	view view(_win);

	slider_ptr horizontal_sliders[3];
	slider_ptr vertical_sliders[3];
	dial_ptr dials[3];

	auto menu = share(make_menu_controls());
	auto slider_and_knob = share(make_slider_and_knob_control(horizontal_sliders, vertical_sliders, dials));
	auto button = share(make_button_control(view));


	view.content(
//			menu,
//			slider_and_knob,
			button,

			background
	);

	link_slider_and_knob(horizontal_sliders, vertical_sliders, dials, view);

	return _app.run();
}
