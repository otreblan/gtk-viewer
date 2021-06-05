#pragma once

#include <gtkmm/box.h>
#include <gtkmm/window.h>
#include <gtkmm/picture.h>
#include <gtkmm/progressbar.h>
#include <gtkmm/headerbar.h>

#include <giomm/inputstream.h>

#include <gdkmm/pixbufloader.h>

class HelloWorld : public Gtk::Window
{

public:
  HelloWorld();
  ~HelloWorld() override;

protected:
  //Signal handlers:
  bool read_file(Glib::IOCondition io_condition);

  Gtk::HeaderBar header_bar;

  //Member widgets:
  Gtk::Box vbox;
  Gtk::Picture picture;
  Gtk::ProgressBar bar;

  Glib::RefPtr<Glib::IOChannel> io_channel;
  Glib::RefPtr<Gdk::PixbufLoader> loader;
};
