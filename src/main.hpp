#pragma once

#include <gtkmm/button.h>
#include <gtkmm/window.h>
#include <gtkmm/picture.h>
#include <gtkmm/progressbar.h>

#include <giomm/inputstream.h>

#include <gdkmm/pixbufloader.h>

class HelloWorld : public Gtk::Window
{

public:
  HelloWorld();
  ~HelloWorld() override;

protected:
  //Signal handlers:
  void on_button_clicked();
  bool read_file(Glib::IOCondition io_condition);

  //Member widgets:
  Gtk::Button m_button;
  Gtk::Picture picture;
  //Gtk::ProgressBar bar;

  Glib::RefPtr<Glib::IOChannel> io_channel;
  Glib::RefPtr<Gdk::PixbufLoader> loader;
};
