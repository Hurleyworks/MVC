/*
	src/example1.cpp -- C++ version of an example application that shows
	how to use the various widget classes. For a Python implementation, see
	'../python/example1.py'.

	NanoGUI was developed by Wenzel Jakob <wenzel.jakob@epfl.ch>.
	The widget drawing code is based on the NanoVG demo application
	by Mikko Mononen.

	All rights reserved. Use of this source code is governed by a
	BSD-style license that can be found in the LICENSE.txt file.
*/

#include <nanogui/opengl.h>
#include <nanogui/screen.h>
#include <nanogui/window.h>
#include <nanogui/layout.h>
#include <nanogui/label.h>
#include <nanogui/checkbox.h>
#include <nanogui/button.h>
#include <nanogui/toolbutton.h>
#include <nanogui/popupbutton.h>
#include <nanogui/combobox.h>
#include <nanogui/progressbar.h>
#include <nanogui/icons.h>
#include <nanogui/messagedialog.h>
#include <nanogui/textbox.h>
#include <nanogui/slider.h>
#include <nanogui/imagepanel.h>
#include <nanogui/imageview.h>
#include <nanogui/vscrollpanel.h>
#include <nanogui/colorwheel.h>
#include <nanogui/colorpicker.h>
#include <nanogui/graph.h>
#include <nanogui/tabwidget.h>
#include <nanogui/texture.h>
#include <nanogui/shader.h>
#include <nanogui/renderpass.h>
#include <iostream>
#include <memory>


#include "Model.h"

#define STB_IMAGE_STATIC
#define STB_IMAGE_IMPLEMENTATION
#if defined(_MSC_VER)
#  pragma warning (disable: 4505) // don't warn about dead code in stb_image.h
#elif defined(__GNUC__)
#   pragma GCC diagnostic ignored "-Wunused-function"
#endif
#include <stb_image.h>

using namespace nanogui;

class View : public Screen, public CsSignal::SignalBase
{
public:
	SIGNAL_1(void sendMsg1(uint32_t id))
	SIGNAL_2(sendMsg1, id)

	SIGNAL_1(void sendMsg2(uint32_t id))
	SIGNAL_2(sendMsg2, id)

	SIGNAL_1(void sendMsg3(uint32_t id))
	SIGNAL_2(sendMsg3, id)

	SIGNAL_1(void sendMsg4(uint32_t id))
	SIGNAL_2(sendMsg4, id)

	SIGNAL_1(void send1())
	SIGNAL_2(send1)

	SIGNAL_1(void send2())
	SIGNAL_2(send2)
	
	SIGNAL_1(void send3())
	SIGNAL_2(send3)
	
	SIGNAL_1(void send4())
	SIGNAL_2(send4)

public:
	View() : Screen(Vector2i(1024, 768), "NanoGUI Test") {
		inc_ref();

		Window* window = new Window(this, "Button demo");
		window->set_position(Vector2i(15, 15));
		window->set_layout(new GroupLayout());

		/* No need to store a pointer, the data structure will be automatically
		   freed when the parent window is deleted */
		new Label(window, "Push buttons", "sans-bold");

		Button* b = new Button(window, "Button 1");
		b->set_callback([this] { sendMsg1(1); });
		
		b = new Button(window, "Button 2");
		b->set_callback([this] { sendMsg2(2); });
		
		b = new Button(window, "Button 3");
		b->set_callback([this] { sendMsg3(3); });
		
		b = new Button(window, "Button 4");
		b->set_callback([this] { sendMsg4(4); });

		b = new Button(window, "Send 1");
		b->set_callback([this] { send1(); });

		b = new Button(window, "Send 2");
		b->set_callback([this] { send2(); });

		b = new Button(window, "Send 3");
		b->set_callback([this] { send3(); });

		b = new Button(window, "Send 4");
		b->set_callback([this] { send4(); });
		

		perform_layout();


		m_render_pass = new RenderPass({ this });
		m_render_pass->set_clear_color(0, Color(0.3f, 0.3f, 0.32f, 1.f));

		m_shader = new Shader(
			m_render_pass,

			/* An identifying name */
			"a_simple_shader",

			R"(/* Vertex shader */
            #version 330
            uniform mat4 mvp;
            in vec3 position;
            void main() {
                gl_Position = mvp * vec4(position, 1.0);
            })",

			/* Fragment shader */
			R"(#version 330
            out vec4 color;
            uniform float intensity;
            void main() {
                color = vec4(vec3(intensity), 1.0);
            })"

		);

		uint32_t indices[3 * 2] = {
			0, 1, 2,
			2, 3, 0
		};

		float positions[3 * 4] = {
			-1.f, -1.f, 0.f,
			1.f, -1.f, 0.f,
			1.f, 1.f, 0.f,
			-1.f, 1.f, 0.f
		};

		m_shader->set_buffer("indices", VariableType::UInt32, { 3 * 2 }, indices);
		m_shader->set_buffer("position", VariableType::Float32, { 4, 3 }, positions);
		m_shader->set_uniform("intensity", 0.5f);
	}

	virtual bool keyboard_event(int key, int scancode, int action, int modifiers) {
		if (Screen::keyboard_event(key, scancode, action, modifiers))
			return true;
		if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
			set_visible(false);
			return true;
		}
		return false;
	}

	virtual void draw(NVGcontext* ctx) {

		Screen::draw(ctx);
	}

	virtual void draw_contents() {
		Matrix4f mvp = Matrix4f::scale(Vector3f(
			(float)m_size.y() / (float)m_size.x() * 0.25f, 0.25f, 0.25f)) *
			Matrix4f::rotate(Vector3f(0, 0, 1), (float)glfwGetTime());

		m_shader->set_uniform("mvp", mvp);

		m_render_pass->resize(framebuffer_size());
		m_render_pass->begin();

		m_shader->begin();
		// m_shader->draw_array(Shader::PrimitiveType::Triangle, 0, 6, true);
		m_shader->end();

		m_render_pass->end();
	}
private:
	ProgressBar* m_progress;
	ref<Shader> m_shader;
	ref<RenderPass> m_render_pass;

};

int main(int /* argc */, char** /* argv */) {
	try {
		nanogui::init();

		{
			Model model;
			ref<View> app = new View();

			// View to Model connections
			// These work fine in Debug mode but not in Release mode
			connect(*app, &View::sendMsg1, model, &Model::printMsg1);
			connect(*app, &View::sendMsg2, model, &Model::printMsg2);
			connect(*app, &View::sendMsg3, model, &Model::printMsg3);
			connect(*app, &View::sendMsg4, model, &Model::printMsg4);
			
			connect(*app, &View::send1, model, &Model::receive1);
			connect(*app, &View::send2, model, &Model::receive2);
			connect(*app, &View::send3, model, &Model::receive3);
			connect(*app, &View::send4, model, &Model::receive4);

			app->dec_ref();
			app->draw_all();
			app->set_visible(true);
			nanogui::mainloop(1 / 60.f * 1000);
		}

		nanogui::shutdown();
	}
	catch (const std::exception& e) {
		std::string error_msg = std::string("Caught a fatal error: ") + std::string(e.what());
#if defined(_WIN32)
		MessageBoxA(nullptr, error_msg.c_str(), NULL, MB_ICONERROR | MB_OK);
#else
		std::cerr << error_msg << std::endl;
#endif
		return -1;
	}
	catch (...) {
		std::cerr << "Caught an unknown error!" << std::endl;
	}

	return 0;
}
