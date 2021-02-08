#include <elements.hpp>
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
	  p_slider = share(slider(basic_thumb<25 /* size of thumb */>(), make_slider_marker<false>(), init_value));
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
	  p_slider = share(slider(basic_thumb<25 /* size of thumb */>(), make_slider_marker<true>(), init_value));
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

int main(int argc, char* argv[])
{
	// Main window background color
	auto constexpr bkd_color = color::build_color(35, 35, 37, 255);
	auto background = box(bkd_color);

	app app(argc, argv, "Basic Sliders And Knobs", "com.cycfi.basic-sliders-and-knobs");
	window win(app.name());
	win.on_close = [&app]() { app.stop(); };

	view view(win);

	slider_ptr horizontal_sliders[3];
	slider_ptr vertical_sliders[3];
	dial_ptr dials[3];

	auto slider_and_knob = share(make_slider_and_knob_control(horizontal_sliders, vertical_sliders, dials));

	view.content(
			slider_and_knob,
			background
			);

	link_slider_and_knob(horizontal_sliders, vertical_sliders, dials, view);

	return app.run();
}
