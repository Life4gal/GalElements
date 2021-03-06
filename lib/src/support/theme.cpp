/*=============================================================================
   Copyright (c) 2016-2020 Joel de Guzman

   Distributed under the MIT License [ https://opensource.org/licenses/MIT ]
=============================================================================*/
#include <elements/support/theme.hpp>
#include <elements/support/enum_operator.hpp>
#include <elements/element/dial.hpp>
#include <elements/view.hpp>

namespace cycfi { namespace elements
{
   theme::theme()
    : panel_color                { color::build_color(28, 30, 34, 192) }
    , frame_color                { color::build_color(220, 220, 220, 80) }
    , frame_hilite_color         { color::build_color(220, 220, 220, 160) }
    , frame_corner_radius        { 3.0 }
    , frame_stroke_width         { 1.0 }
    , scrollbar_color            { color::build_color(80, 80, 80, 80) }
    , default_button_color       { color::build_color(0, 0, 0, 0) }
    , button_margin              { 10, 5, 10, 5 }

    , controls_color             { color::build_color(18, 49, 85, 200) }
    , controls_frame_stroke_width{ 1.5 }
    , indicator_color            { color::build_color(0, 127, 255, 200) }
    , indicator_bright_color     { indicator_color.level(1.5) }
    , indicator_hilite_color     { indicator_color.level(2.0) }
    , basic_font_color           { color::build_color(220, 220, 220, 200) }

// The symbols_font font is the OS supplied font that includes unicode symbols
// such as Miscellaneous Technical : Unicode U+2300 – U+23FF (8960–9215)
#if defined(__APPLE__)
    , system_font                { font_descriptor{"Lucida Grande" } }
#elif defined(_WIN32)
    , system_font                { font_descriptor{ "Segoe UI Symbol" } }
#elif defined(__linux__)
    , system_font                { font_descriptor{ "DejaVu Sans" } }
#endif

    , element_background_opacity { 32.0f / 255.0f }

    , heading_font_color         { basic_font_color }
    , heading_font               { font_descriptor{ "Roboto" }.get_weight_medium() }
    , heading_font_size          { 14.0 }
    , heading_text_align         { canvas::text_alignment::middle | canvas::text_alignment::center }

    , label_font_color           { basic_font_color }
    , label_font                 { font_descriptor{ "Open Sans" } }
    , label_font_size            { 14.0 }
    , label_text_align           { canvas::text_alignment::middle | canvas::text_alignment::center }

    , icon_color                 { basic_font_color }
    , icon_font                  { font_descriptor{ "elements_basic" } }
    , icon_font_size             { 16.0 }
    , icon_button_color          { default_button_color }

    , text_box_font_color        { basic_font_color }
    , text_box_font              { font_descriptor{ "Open Sans" } }
    , text_box_font_size         { 14.0 }
    , text_box_hilite_color      { color::build_color(0, 127, 255, 100) }
    , text_box_caret_color       { color::build_color(0, 190, 255, 255) }
    , text_box_caret_width       { 1.2 }
    , inactive_font_color        { color::build_color(127, 127, 127, 150) }
    , input_box_text_limit       { 1024 }

    , ticks_color                { color::build_color(127, 127, 127, 150) }
    , major_ticks_level          { 0.5 }
    , major_ticks_width          { 1.5 }
    , minor_ticks_level          { 0.4 }
    , minor_ticks_width          { 0.7 }

    , major_grid_color           { frame_color }
    , major_grid_width           { 0.5 }
    , minor_grid_color           { indicator_color }
    , minor_grid_width           { 0.4 }

    , dialog_button_size         { 100 }
    , message_textbox_size       { 300, 120 }

    , dial_mode                  { dial_mode_enum::linear }
    , dial_linear_range          { 200 }
   {
   }

   // The global theme
   theme& global_theme::_theme()
   {
      static theme thm;
      return thm;
   }

   theme const& get_theme()
   {
      return global_theme::_theme();
   }

   void set_theme(theme const& thm)
   {
      global_theme::_theme() = thm;
   }
}}
