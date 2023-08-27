#include "main.hpp"

#include <glibmm/iochannel.h>
#include <glibmm/main.h>

#include <iostream>

HelloWorld::HelloWorld():
	vbox(Gtk::Orientation::VERTICAL)
{
	loader = Gdk::PixbufLoader::create();
	if(!loader)
		return;

	loader->signal_area_prepared().connect(
		[&]()
		{
			if(!loader) return;

			auto pixbuf = loader->get_pixbuf();

			if(!pixbuf) return;

			pixbuf->fill(0);
			picture.set_pixbuf(pixbuf);

			bar.set_fraction(0.0);
		}
	);

	loader->signal_area_updated().connect(
		[&](int, int y, int, int)
		{
			if(!loader) return;

			auto pixbuf = loader->get_pixbuf();

			if(!pixbuf) return;

			bar.set_fraction((double)y/pixbuf->get_height());

			picture.set_pixbuf(nullptr);
			picture.set_pixbuf(pixbuf);
		}
	);

	io_channel = Glib::IOChannel::create_from_fd(STDIN_FILENO);
	io_channel->set_encoding("");
	io_channel->set_flags(Glib::IOFlags::NONBLOCK);

	Glib::signal_io()
		.connect(
			[this](Glib::IOCondition io_condition) -> bool
			{
				return read_file(io_condition);
			},
			io_channel,
			Glib::IOCondition::IO_IN |
			Glib::IOCondition::IO_PRI |
			Glib::IOCondition::IO_ERR |
			Glib::IOCondition::IO_HUP
		);

	set_title("Render view");

	picture.set_margin(10);
	picture.set_alternative_text("Preview");

	bar.set_margin(10);
	bar.set_text("Rendering");
	bar.set_show_text(true);

	vbox.append(picture);
	vbox.append(bar);
	vbox.set_valign(Gtk::Align::CENTER);

	set_titlebar(header_bar);
	set_child(vbox);
}

HelloWorld::~HelloWorld()
{
	if(loader)
		loader->close();
}

bool HelloWorld::read_file(Glib::IOCondition io_condition)
{
	if((io_condition & Glib::IOCondition::IO_IN) != Glib::IOCondition::IO_IN)
	{
		vbox.remove(bar);
		return false;
	}
	else
	{
		gsize size;
		char buffer[PIPE_BUF];

		if(io_channel->read(buffer, sizeof(buffer), size) == Glib::IOStatus::ENDOFFILE)
		{
			return false;
		}

		loader->write((guint8*)buffer, size);
	}

	return true;
}

int main(int argc, char* argv[])
{
	auto app = Gtk::Application::create("org.gtkmm.example", Gio::Application::Flags::NON_UNIQUE);

	//Shows the window and returns when it is closed.
	return app->make_window_and_run<HelloWorld>(argc, argv);
}
