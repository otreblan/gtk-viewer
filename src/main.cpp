#include "main.hpp"

#include <glibmm/iochannel.h>
#include <glibmm/main.h>

#include <iostream>

HelloWorld::HelloWorld(): m_button("Hello World")
{
	picture.set_margin(10);
	picture.set_alternative_text("Preview");

	//picture.set_filename("/dev/stdin");

	loader = Gdk::PixbufLoader::create();
	if(!loader)
		return;

	loader->signal_area_prepared().connect(
		[&]()
		{
			if(!loader)
				return;

			auto pixbuf = loader->get_pixbuf();

			if(!pixbuf)
				return;

			pixbuf->fill(0);
			picture.set_pixbuf(pixbuf);
		}
	);

	loader->signal_area_updated().connect(
		[&](int, int, int, int)
		{
			if(!loader)
				return;

			picture.set_pixbuf(nullptr);
			picture.set_pixbuf(loader->get_pixbuf());
		}
	);

	Glib::signal_io()
		.connect(
			[this](Glib::IOCondition io_condition) -> bool
			{
				return read_file(io_condition);
			},
			STDIN_FILENO,
			Glib::IOCondition::IO_IN
		);

	io_channel = Glib::IOChannel::create_from_fd(STDIN_FILENO);
	io_channel->set_encoding("");
	io_channel->set_flags(Glib::IOFlags::NONBLOCK);

	set_child(picture);
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
		std::cerr << "End file\n";
		return false;
	}
	else
	{
		gsize size;
		char buffer[PIPE_BUF];

		if(io_channel->read(buffer, sizeof(buffer), size) == Glib::IOStatus::ENDOFFILE)
			return false;

		loader->write((guint8*)buffer, size);
	}

	return true;
}

void HelloWorld::on_button_clicked()
{
	std::cout << "Hello World" << std::endl;
}

int main(int argc, char* argv[])
{
	auto app = Gtk::Application::create("org.gtkmm.example");

	//Shows the window and returns when it is closed.
	return app->make_window_and_run<HelloWorld>(argc, argv);
}
