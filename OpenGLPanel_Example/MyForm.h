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
			HKOGLPanel::HKCOGLPanelCameraSetting^ hkcoglPanelCameraSetting1 = (gcnew HKOGLPanel::HKCOGLPanelCameraSetting());
			HKOGLPanel::HKCOGLPanelPixelFormat^ hkcoglPanelPixelFormat1 = (gcnew HKOGLPanel::HKCOGLPanelPixelFormat());
			this->hkoglPanelControl1 = (gcnew HKOGLPanel::HKOGLPanelControl());
			this->timer1 = (gcnew System::Windows::Forms::Timer(this->components));
			this->SuspendLayout();
			// 
			// hkoglPanelControl1
			// 
			hkcoglPanelCameraSetting1->Far = 1000;
			hkcoglPanelCameraSetting1->Fov = 45;
			hkcoglPanelCameraSetting1->Near = -1000;
			hkcoglPanelCameraSetting1->Type = HKOGLPanel::HKCOGLPanelCameraSetting::CAMERATYPE::ORTHOGRAPHIC;
			this->hkoglPanelControl1->Camera_Setting = hkcoglPanelCameraSetting1;
			this->hkoglPanelControl1->Location = System::Drawing::Point(12, 12);
			this->hkoglPanelControl1->Name = L"hkoglPanelControl1";
			hkcoglPanelPixelFormat1->Accumu_Buffer_Bits = HKOGLPanel::HKCOGLPanelPixelFormat::PIXELBITS::BITS_0;
			hkcoglPanelPixelFormat1->Alpha_Buffer_Bits = HKOGLPanel::HKCOGLPanelPixelFormat::PIXELBITS::BITS_0;
			hkcoglPanelPixelFormat1->Stencil_Buffer_Bits = HKOGLPanel::HKCOGLPanelPixelFormat::PIXELBITS::BITS_0;
			this->hkoglPanelControl1->Pixel_Format = hkcoglPanelPixelFormat1;
			this->hkoglPanelControl1->Size = System::Drawing::Size(360, 337);
			this->hkoglPanelControl1->TabIndex = 0;
			this->hkoglPanelControl1->Load += gcnew System::EventHandler(this, &MyForm::hkoglPanelControl1_Load);
			this->hkoglPanelControl1->Paint += gcnew System::Windows::Forms::PaintEventHandler(this, &MyForm::hkoglPanelControl1_Paint);
			this->hkoglPanelControl1->KeyDown += gcnew System::Windows::Forms::KeyEventHandler(this, &MyForm::MyForm_KeyDown);
			this->hkoglPanelControl1->MouseDown += gcnew System::Windows::Forms::MouseEventHandler(this, &MyForm::hkoglPanelControl1_MouseDown);
			this->hkoglPanelControl1->MouseUp += gcnew System::Windows::Forms::MouseEventHandler(this, &MyForm::hkoglPanelControl1_MouseUp);
			// 
			// timer1
			// 
			this->timer1->Enabled = true;
			this->timer1->Interval = 17;
			this->timer1->Tick += gcnew System::EventHandler(this, &MyForm::timer1_Tick);
			// 
			// MyForm
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 12);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(384, 361);
			this->Controls->Add(this->hkoglPanelControl1);
			this->Name = L"MyForm";
			this->Text = L"OpenGL Panel";
			this->ResumeLayout(false);

		}

#pragma endregion
	private:
		Vec3f* vertices;
		Vec3f* camPos, *lookPos, *mouseKeep;
		bool isMouseDown;
		
		unsigned int* indice;

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



		isMouseDown = 0;

		camPos = new Vec3f();
		camPos->x = 0;
		camPos->y = 0;
		camPos->z = 2;




		// Create and bind VAO
		glGenVertexArrays(1, &paramGL->VAO);
		glBindVertexArray(paramGL->VAO);

		//Assign 3 vertices for triangles
		int pointAmount = 10;

		const float H = 2 * Math::PI / 10, len = cos(2 * H) * (1 / cos(H)); // 0.38f
		indice = new unsigned int[3 * pointAmount]();
		vertices = new Vec3f[pointAmount + 1]();

		vertices[pointAmount] = { 0,0,0 };
		for (int i = 0; i < pointAmount; i++)
		{
			vertices[i] = setVertice(i * H, (i % 2 == 1 ? 1 : len));
		}

		for (int i = 0; i < pointAmount; i++)
		{
			indice[i * 3] = pointAmount;
			indice[i * 3 + 1] = i;
			indice[i * 3 + 2] = (i + 1) % pointAmount;
		}

		// Assign colors for each vertex
		Vec3f* color = new Vec3f[pointAmount + 1]();

		color[pointAmount] = { 1,1,1 };
		for (int i = 0; i < pointAmount; i++)
		{
			color[i] = { 0,1,1 };
		}

		glGenBuffers(1, &paramGL->VBOv);
		glBindBuffer(GL_ARRAY_BUFFER, paramGL->VBOv);
		glBufferData(GL_ARRAY_BUFFER, (pointAmount + 1) * sizeof(Vec3f), vertices, GL_STATIC_DRAW);

		glGenBuffers(1, &paramGL->VBOc);
		glBindBuffer(GL_ARRAY_BUFFER, paramGL->VBOc);
		glBufferData(GL_ARRAY_BUFFER, (pointAmount + 1) * sizeof(Vec3f), color, GL_STATIC_DRAW);

		glGenBuffers(1, &paramGL->EBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, paramGL->EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, 3 * pointAmount * sizeof(unsigned int), indice, GL_STATIC_DRAW);

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
		glClearColor(0, 0, 0, 0);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);



		Vec3f temp = { 0,0,0 };
		if (isMouseDown)
		{
			temp = {mouseKeep->x}
		}

		gluPerspective(60, 1, 1, 10);
		gluLookAt(
			camPos->x, camPos->y, camPos->z,
			0, 0, 0,
			0, 1, 0
			);




		// Use the shader
		//glUseProgram(paramGL->Program);

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
		glBindVertexArray(paramGL->VAO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, paramGL->EBO);
		//glDrawElements(GL_TRIANGLES, 30, GL_UNSIGNED_INT, 0);




		glBegin(GL_TRIANGLES);
		for (int i = 0; i < 10; i++)
		{
			glColor3f(1, 1, 0);
			glVertex3f(vertices[10].x / 3, vertices[10].y / 3, 0);
			glVertex3f(vertices[i].x / 3, vertices[i].y / 3, 0);
			glVertex3f(vertices[(i + 1) % 10].x / 3, vertices[(i + 1) % 10].y / 3, 0);
		}
		glEnd();




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
	private: System::Void MyForm_KeyDown(System::Object^ sender, System::Windows::Forms::KeyEventArgs^ e) {
		float distance = 0.1;
		//MessageBox::Show("");
		if (e->KeyCode == Keys::W)
		{
			camPos->z -= distance;
		}
		if (e->KeyCode == Keys::A)
		{
			camPos->x -= distance;
		}
		if (e->KeyCode == Keys::S)
		{
			camPos->z += distance;
		}
		if (e->KeyCode == Keys::D)
		{
			camPos->x += distance;
		}
	}

private: System::Void hkoglPanelControl1_MouseDown(System::Object^ sender, System::Windows::Forms::MouseEventArgs^ e) {
		isMouseDown = 1;
	}
private: 
	System::Void hkoglPanelControl1_MouseUp(System::Object^ sender, System::Windows::Forms::MouseEventArgs^ e) {
		
		isMouseDown = 0;
	}
};
}
