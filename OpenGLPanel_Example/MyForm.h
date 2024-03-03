#pragma once

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <windows.h>

#include "Param.h"
#include "LoadShaders.h"

namespace OpenGLPanel_Example
{
	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;
	using namespace std;

	/// <summary>
	/// MyForm 的摘要
	/// </summary>
	public ref class MyForm : public System::Windows::Forms::Form
	{
	public:
		MyForm(void)
		{
			InitializeComponent();
		}

	protected:
		/// <summary>
		/// 清除任何使用中的資源。
		/// </summary>
		~MyForm()
		{
			if (components)
			{
				delete components;
			}
		}
	private: HKOGLPanel::HKOGLPanelControl^ hkoglPanelControl1;
	private: System::Windows::Forms::Timer^ timer1;

	protected:

		/// <summary>
		/// 設計工具所需的變數。
		/// </summary>
	private: System::ComponentModel::IContainer^ components;

#pragma region Windows Form Designer generated code

	private:
		/// <summary>
		/// 此為設計工具支援所需的方法 - 請勿使用程式碼編輯器修改
		/// 這個方法的內容。
		/// </summary>
		void InitializeComponent(void)
		{
			this->components = (gcnew System::ComponentModel::Container());
			HKOGLPanel::HKCOGLPanelCameraSetting^ hkcoglPanelCameraSetting2 = (gcnew HKOGLPanel::HKCOGLPanelCameraSetting());
			HKOGLPanel::HKCOGLPanelPixelFormat^ hkcoglPanelPixelFormat2 = (gcnew HKOGLPanel::HKCOGLPanelPixelFormat());
			this->hkoglPanelControl1 = (gcnew HKOGLPanel::HKOGLPanelControl());
			this->timer1 = (gcnew System::Windows::Forms::Timer(this->components));
			this->SuspendLayout();
			// 
			// hkoglPanelControl1
			// 
			hkcoglPanelCameraSetting2->Far = 1000;
			hkcoglPanelCameraSetting2->Fov = 45;
			hkcoglPanelCameraSetting2->Near = -1000;
			hkcoglPanelCameraSetting2->Type = HKOGLPanel::HKCOGLPanelCameraSetting::CAMERATYPE::ORTHOGRAPHIC;
			this->hkoglPanelControl1->Camera_Setting = hkcoglPanelCameraSetting2;
			this->hkoglPanelControl1->Location = System::Drawing::Point(12, 13);
			this->hkoglPanelControl1->Name = L"hkoglPanelControl1";
			hkcoglPanelPixelFormat2->Accumu_Buffer_Bits = HKOGLPanel::HKCOGLPanelPixelFormat::PIXELBITS::BITS_0;
			hkcoglPanelPixelFormat2->Alpha_Buffer_Bits = HKOGLPanel::HKCOGLPanelPixelFormat::PIXELBITS::BITS_0;
			hkcoglPanelPixelFormat2->Stencil_Buffer_Bits = HKOGLPanel::HKCOGLPanelPixelFormat::PIXELBITS::BITS_0;
			this->hkoglPanelControl1->Pixel_Format = hkcoglPanelPixelFormat2;
			this->hkoglPanelControl1->Size = System::Drawing::Size(360, 365);
			this->hkoglPanelControl1->TabIndex = 0;
			this->hkoglPanelControl1->Load += gcnew System::EventHandler(this, &MyForm::hkoglPanelControl1_Load);
			this->hkoglPanelControl1->Paint += gcnew System::Windows::Forms::PaintEventHandler(this, &MyForm::hkoglPanelControl1_Paint);
			// 
			// timer1
			// 
			this->timer1->Enabled = true;
			this->timer1->Interval = 17;
			this->timer1->Tick += gcnew System::EventHandler(this, &MyForm::timer1_Tick);
			// 
			// MyForm
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(384, 391);
			this->Controls->Add(this->hkoglPanelControl1);
			this->Name = L"MyForm";
			this->Text = L"OpenGL Panel";
			this->ResumeLayout(false);

		}

#pragma endregion
	private:
		Vec3f* vertices;

		int angle = 0;

		Vec3f setVertice(float degree, float len)
		{
			return { len * cos(degree),len * sin(degree), 0.0f };
		}

	private: System::Void hkoglPanelControl1_Load(System::Object^ sender, System::EventArgs^ e)
	{
		// Initialize GLEW
		GLenum err = glewInit();
		if (GLEW_OK != err)
		{
			cout << "GLEW is not initialized!\n";
			return;
		}

		// Create native parameters
		paramGL = new ParamGL();

		// Load shader from files
		ShaderInfo shaders[] =
		{
			{ GL_VERTEX_SHADER,  "../Shader/shader.vs" },
			{ GL_FRAGMENT_SHADER, "../Shader/shader.fs" },
			{ GL_NONE, NULL }
		};
		paramGL->Program = LoadShaders(shaders);

		// Create and bind VAO
		glGenVertexArrays(1, &paramGL->VAO);
		glBindVertexArray(paramGL->VAO);

		//Assign 3 vertices for triangles
		vertices = new Vec3f[9]();

		const float H = 2 * Math::PI / 5, len = cos(H) * (1 / cos(H / 2)); // 0.38f

		vertices[0] = setVertice(Math::PI / 2, 1);
		vertices[1] = setVertice(1.5 * Math::PI - H / 2, 1);
		vertices[2] = setVertice(1.5 * Math::PI + H, len);

		vertices[3] = setVertice(Math::PI / 2 - H, 1);
		vertices[4] = setVertice(Math::PI / 2 + H, 1);
		vertices[5] = setVertice(1.5 * Math::PI, len);

		vertices[6] = setVertice(1.5 * Math::PI + H / 2, 1);
		vertices[7] = setVertice(Math::PI / 2, 1);
		vertices[8] = setVertice(1.5 * Math::PI - H, len);

		// Assign colors for each vertex
		Vec3f color[9];

		for (int i = 0; i < 9; i++)
			color[i] = { 148, 0, 211 };

		glGenBuffers(1, &paramGL->VBOv);
		glBindBuffer(GL_ARRAY_BUFFER, paramGL->VBOv);
		glBufferData(GL_ARRAY_BUFFER, 9 * sizeof(Vec3f), vertices, GL_STATIC_DRAW);

		glGenBuffers(1, &paramGL->VBOc);
		glBindBuffer(GL_ARRAY_BUFFER, paramGL->VBOc);
		glBufferData(GL_ARRAY_BUFFER, 9 * sizeof(Vec3f), &color[0], GL_STATIC_DRAW);

		// Get and print used OpenGL version
		int ver[2] = { 0, 0 };
		glGetIntegerv(GL_MAJOR_VERSION, &ver[0]);
		glGetIntegerv(GL_MINOR_VERSION, &ver[1]);
		cout << "OpenGL Version: " << ver[0] << "." << ver[1] << endl;
	}

	private: System::Void hkoglPanelControl1_Paint(System::Object^ sender, System::Windows::Forms::PaintEventArgs^ e)
	{
		// Clear color and depth buffer
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Set the color buffer
		glClearColor(0, 0.5, 0.5, 0);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

		// Use the shader
		glUseProgram(paramGL->Program);

		// 2D Rotation
		float degree = angle * 2 * Math::PI / 360;
		float rotationMatrix[] = {
			cos(degree), -sin(degree), 0.0f, 0.0f,
			sin(degree), cos(degree),  0.0f, 0.0f,
			0.0f,       0.0f,        1.0f, 0.0f,
			0.0f,       0.0f,        0.0f, 1.0f
		};
		glUniformMatrix4fv(glGetUniformLocation(paramGL->Program, "rotationMatrix"), 1, GL_FALSE, rotationMatrix);

		// Enable 'position' attribute and assign data
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, paramGL->VBOv);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

		// Enable 'color' attribute and assign data
		glEnableVertexAttribArray(1);
		glBindBuffer(GL_ARRAY_BUFFER, paramGL->VBOc);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);


		// Draw the triangle
		glDrawArrays(GL_TRIANGLES, 0, 9);

		// Disable vertex attribute arrays
		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);
	}

	private: System::Void timer1_Tick(System::Object^ sender, System::EventArgs^ e)
	{
		angle = (angle + 1) % 360;
		// Redraw the OpenGL Panel
		hkoglPanelControl1->Invalidate();
	}

	private:
		ParamGL* paramGL = NULL;
	};
}
