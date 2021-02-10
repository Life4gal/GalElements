#include <elements.hpp>
#include <infra/support.hpp>

using namespace cycfi::elements;

///////////////////////////////////////////////////////////////////////////////
// There are many ways to structure an application using elements. The user is
// not forced to do it one way or another. However, this specific examplar is
// the recommended way and can be considered as good practice.
//
// This example demonstrates:
//
//    1. How to make shared elements that connect to each other via
//       callbacks.
//    2. How to make custom elements that can interact with other elements.
//    3. How to create element class member variables that are either held by
//       value or by shared_ptr.
//    4. How to hold or link the elements in a composition.
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
// A Custom control element
///////////////////////////////////////////////////////////////////////////////
class custom_control : public tracker<>, public receiver<float> {
public:
	using point_array = std::array<point, 4>;
	constexpr static float default_value = 100;

	void draw(const context& context) override {
		auto& canvas = context.canvas;
		auto& bounds = context.bounds;

		canvas.fill_style({0.8, 0.8, 0.8});
		canvas.fill_rect(bounds);

		auto cursor = context.cursor_pos();

		// Draw a circle
		canvas.line_width(1.0);
		canvas.stroke_style({0, 0, 0.5});
		auto out_ring = circle(bounds.center_point(), radius);
		canvas.circle(out_ring);
		canvas.stroke();

		// Center points for four knobs.
		auto circumscribed_rect = out_ring.get_circumscribed_rect();
		knobs =
				{
						point(circumscribed_rect.left, bounds.center_point().y),
						point(circumscribed_rect.right, bounds.center_point().y),
						point(bounds.center_point().x, circumscribed_rect.top),
						point(bounds.center_point().x, circumscribed_rect.bottom)};

		for (auto& knob: knobs) {
			canvas.line_width(1.0);
			canvas.stroke_style({0, 0, 0.6});
			canvas.fill_style({1.0, 1.0, 1.0});
			canvas.circle({knob, 5.0});
			canvas.fill_preserve();
			canvas.stroke();

			// Highlight the knob which is near cursor.
			if (mouse_over && knob.get_distance(cursor) < 10) {
				canvas.line_width(2.0);
				canvas.stroke_style({0, 0, 0.8});
				canvas.circle({knob, 7.0});
				canvas.stroke();
			}
		}

		// Highlight choose knob with green circle
		if (chosen_knob != -1) {
			canvas.line_width(2.0);
			canvas.stroke_style({0, 0.8, 0});
			canvas.circle({knobs[chosen_knob], 7.0});
			canvas.stroke();
		}

		if (mouse_over) {
			canvas.line_width(1.0);
			canvas.stroke_style({0, 0.5, 0.5, 0.2});
			canvas.circle({cursor.x, cursor.y, 10});
			canvas.stroke();
		}
	}

	bool cursor(const context& context, point p, cursor_tracking status) override {
		switch (status) {
			case cursor_tracking::hovering:
			case cursor_tracking::entering:
				mouse_over = true;
				context.view.refresh(context.bounds);
				break;
			case cursor_tracking::leaving:
				mouse_over = false;
		}

		return false;
	}

	bool click(const context& context, mouse_button button) override {
		tracker::click(context, button);

		if (button.state == mouse_button::what::left) {
			// lock the knob clicked over if any
			for (auto index = 0; index < knobs.size(); ++index) {
				if (knobs[index].get_distance(button.pos) < 10) {
					chosen_knob = index;
				}
			}
		}
		return true;
	}

	void keep_tracking(const context& context, tracker_info& info) override {
		auto center = context.bounds.center_point();
		radius = center.get_distance(info.current);
		cycfi::clamp(radius, 50, 150);
		if (on_change) {
			on_change(radius);
		}
		context.view.refresh(context.bounds);
	}

	float value() const override { return radius; }
	void value(float _radius) override { radius = _radius; }

	// Provide an on_change callback
	using on_change_f = std::function<void(float)>;
	on_change_f on_change;

private:
	bool mouse_over = false;
	int chosen_knob = -1;
	point_array knobs;
	float radius = default_value;
};

class custom_app : public app {
public:
	custom_app(int argc, char* argv[]);

private:
	constexpr static auto bkd_color = color::build_color(35, 35, 37, 255);
	constexpr static float default_value = custom_control::default_value;
	using dial_ptr = std::shared_ptr<dial_base>;
	using label_ptr = decltype(share(label("")));

	auto make_control();
	auto make_button();
	auto make_dial();
	auto make_label();
	void set_value(float value);
	auto make_control_panel();

	window win;
	view _view;
	custom_control control;
	dial_ptr _dial;
	label_ptr _label;
	box_element background;
};

auto custom_app::make_control() {
	return layer(
			align_center_middle(
					fixed_size(
							{400, 400},
							link(control))),
			image{"wall.jpg"});
}

auto custom_app::make_button() {
	// Make a button for sharing (making it a shared_ptr)
	auto reset_button = share(button("Reset Size"));
	// Attach an on_click function
	reset_button->on_click = [this](bool) {
		this->set_value(default_value);// Reset default value
	};

	return margin({50, 10, 50, 10}, hold(reset_button));// Hold our shared button
}

auto custom_app::make_dial() {
	const auto scale = 1.0f;
	auto knob = sprite{"knob_sprites_150x150_darker.png", 150 * scale, scale};

	_dial = share(dial(knob, 0.5));// Share a dial
	// Attach an on_change function
	_dial->on_change = [this](double value) {
		// Un-normalize from 0.0...1.0 to 50...150
		this->set_value(static_cast<float>(50 + (value * 100)));
	};

	// Connect our control via control.on_change
	control.on_change = [this](double value) {
		this->set_value(static_cast<float>(value));
	};

	return align_center_middle(hold(_dial));// Hold the dial
}

auto custom_app::make_label() {
	_label = share(label(""));				 // share a label
	return align_center_middle(hold(_label));// hold the label
}

void custom_app::set_value(float value) {
	control.value(value);
	_dial->value((value - 50) / 100);// Set _dial value (normalized to 0.0...1.0)

	// set the label text
	_label->set_text("Radius: " + std::to_string(std::round(value)));
	// refresh
	_view.refresh();
}

auto custom_app::make_control_panel() {
	return margin(
			{50, 20, 50, 20},
			layer(
					vgrid(
							span(5, make_dial()),
							span(1, make_label()),
							span(1, make_button())),
					panel{},
					background));
}

custom_app::custom_app(int argc, char** argv)
	: app(argc, argv, "Custom Control", "com.cycfi.custom_control"),
	  win(name()),
	  _view(win),
	  background(box(bkd_color)) {
	win.on_close = [this]() { this->stop(); };

	_view.content(
			htile(
					make_control_panel(),
					make_control()),
			background);

	set_value(default_value);
}

///////////////////////////////////////////////////////////////////////////////
// main function
///////////////////////////////////////////////////////////////////////////////
int main(int argc, char* argv[]) {
	custom_app _app(argc, argv);

	return _app.run();
}
