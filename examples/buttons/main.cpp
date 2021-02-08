#include <elements.hpp>

using namespace cycfi::elements;

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

	app app(argc, argv, "Buttons", "com.cycfi.buttons");
	window win(app.name());
	win.on_close = [&app]() { app.stop(); };

	view view(win);

	view.content(
			make_button_control(view),
			background
			);

	return app.run();
}
