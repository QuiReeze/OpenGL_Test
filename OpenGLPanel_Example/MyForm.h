#pragma once

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <windows.h>
#include <algorithm>
#include <bitset>
#include <vector>

#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>

#include "Param.h"
#include "LoadShaders.h"
#include "moveControl.h"

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
	private: System::Windows::Forms::Timer^ moveControl;

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
			this->moveControl = (gcnew System::Windows::Forms::Timer(this->components));
			this->SuspendLayout();
			// 
			// hkoglPanelControl1
			// 
			hkcoglPanelCameraSetting1->Far = 1000;
			hkcoglPanelCameraSetting1->Fov = 45;
			hkcoglPanelCameraSetting1->Near = -1000;
			hkcoglPanelCameraSetting1->Type = HKOGLPanel::HKCOGLPanelCameraSetting::CAMERATYPE::ORTHOGRAPHIC;
			this->hkoglPanelControl1->Camera_Setting = hkcoglPanelCameraSetting1;
			this->hkoglPanelControl1->Cursor = System::Windows::Forms::Cursors::Default;
			this->hkoglPanelControl1->Dock = System::Windows::Forms::DockStyle::Fill;
			this->hkoglPanelControl1->Location = System::Drawing::Point(0, 0);
			this->hkoglPanelControl1->Name = L"hkoglPanelControl1";
			hkcoglPanelPixelFormat1->Accumu_Buffer_Bits = HKOGLPanel::HKCOGLPanelPixelFormat::PIXELBITS::BITS_0;
			hkcoglPanelPixelFormat1->Alpha_Buffer_Bits = HKOGLPanel::HKCOGLPanelPixelFormat::PIXELBITS::BITS_0;
			hkcoglPanelPixelFormat1->Stencil_Buffer_Bits = HKOGLPanel::HKCOGLPanelPixelFormat::PIXELBITS::BITS_0;
			this->hkoglPanelControl1->Pixel_Format = hkcoglPanelPixelFormat1;
			this->hkoglPanelControl1->Size = System::Drawing::Size(1396, 675);
			this->hkoglPanelControl1->TabIndex = 0;
			this->hkoglPanelControl1->Load += gcnew System::EventHandler(this, &MyForm::hkoglPanelControl1_Load);
			this->hkoglPanelControl1->Paint += gcnew System::Windows::Forms::PaintEventHandler(this, &MyForm::hkoglPanelControl1_Paint);
			this->hkoglPanelControl1->KeyDown += gcnew System::Windows::Forms::KeyEventHandler(this, &MyForm::hkoglPanelControl1_KeyDown);
			this->hkoglPanelControl1->KeyUp += gcnew System::Windows::Forms::KeyEventHandler(this, &MyForm::hkoglPanelControl1_KeyUp);
			this->hkoglPanelControl1->MouseDown += gcnew System::Windows::Forms::MouseEventHandler(this, &MyForm::hkoglPanelControl1_MouseDown);
			this->hkoglPanelControl1->MouseMove += gcnew System::Windows::Forms::MouseEventHandler(this, &MyForm::hkoglPanelControl1_MouseMove);
			this->hkoglPanelControl1->MouseUp += gcnew System::Windows::Forms::MouseEventHandler(this, &MyForm::hkoglPanelControl1_MouseUp);
			this->hkoglPanelControl1->MouseWheel += gcnew System::Windows::Forms::MouseEventHandler(this, &MyForm::hkoglPanelControl1_MouseWheel);
			// 
			// timer1
			// 
			this->timer1->Enabled = true;
			this->timer1->Interval = 12;
			this->timer1->Tick += gcnew System::EventHandler(this, &MyForm::timer1_Tick);
			// 
			// moveControl
			// 
			this->moveControl->Enabled = true;
			this->moveControl->Interval = 6;
			this->moveControl->Tick += gcnew System::EventHandler(this, &MyForm::moveControl_Tick);
			// 
			// MyForm
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(1396, 675);
			this->Controls->Add(this->hkoglPanelControl1);
			this->Name = L"MyForm";
			this->Text = L"OpenGL Panel";
			this->ResumeLayout(false);

		}

#pragma endregion
	private:
		glm::vec3* eye, * look;
		float yzDegree = 0, xzDegree = 0, objAngle = 0, fovy = 45;
		ParamGL* star = NULL;
		ParamGL* worldPlane = NULL;
		ParamGL* block = NULL;

		glm::vec2* mouseClickPos;

		Vec3f setVertice(float degree, float len)
		{
			return { len * cos(degree), 0.0f, len * sin(degree) };
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
		star = new ParamGL();
		worldPlane = new ParamGL();
		block = new ParamGL();
		mouseMoveDelta = new glm::vec2(0, 0);
		mouseClickPos = new glm::vec2(0, 0);
		keyEn = new keyEnable();
		moveCam = new bool(0);
		// init eye
		eye = new glm::vec3(0, 0.5, 2);
		look = new glm::vec3(0, 0, 0);

		// Load shader from files
		ShaderInfo shaders[] =
		{
			{ GL_VERTEX_SHADER,  "../Shader/shader.vs" },
			{ GL_FRAGMENT_SHADER, "../Shader/shader.fs" },
			{ GL_NONE, NULL }
		};
		star->Program = LoadShaders(shaders);
		worldPlane->Program = LoadShaders(shaders);
		block->Program = LoadShaders(shaders);

		// Create and bind VAO star
		glGenVertexArrays(1, &star->VAO);
		glBindVertexArray(star->VAO);

		// Assign vertices and index for triangles
		int pointAmount = 10;
		const float H = 2 * Math::PI / 10, len = cos(2 * H) * (1 / cos(H)); // 0.38f

		// set vertice into VBO
		Vec3f* vertices = new Vec3f[pointAmount + 2]();
		vertices[pointAmount] = { 0,0,0.1f };
		vertices[pointAmount + 1] = { 0,0,-0.1f };
		for (int i = 0; i < pointAmount; i++)
			vertices[i] = setVertice(i * H, (i % 2 == 1 ? 1 : len));

		// set index into EBO
		unsigned int* indice = new unsigned int[2 * 3 * pointAmount]();
		for (int i = 0; i < pointAmount; i++)
		{
			indice[i * 3] = pointAmount;
			indice[i * 3 + 1] = i;
			indice[i * 3 + 2] = (i + 1) % pointAmount;

			indice[(3 * pointAmount) + i * 3] = pointAmount + 1;
			indice[(3 * pointAmount) + i * 3 + 1] = (i + 1) % pointAmount;
			indice[(3 * pointAmount) + i * 3 + 2] = i;
		}

		// Assign colors for each vertex
		Vec3f* color = new Vec3f[pointAmount + 1]();
		color[pointAmount] = { 0.8f,0.8f,0.8f };
		color[pointAmount + 1] = { 0.8f,0.8f,0.8f };
		for (int i = 0; i < pointAmount; i++)
			color[i] = { 0,0.8f,0.8f };

		glGenBuffers(1, &star->VBOv);
		glBindBuffer(GL_ARRAY_BUFFER, star->VBOv);
		glBufferData(GL_ARRAY_BUFFER, (pointAmount + 2) * sizeof(Vec3f), vertices, GL_STATIC_DRAW);

		glGenBuffers(1, &star->VBOc);
		glBindBuffer(GL_ARRAY_BUFFER, star->VBOc);
		glBufferData(GL_ARRAY_BUFFER, (pointAmount + 2) * sizeof(Vec3f), color, GL_STATIC_DRAW);

		glGenBuffers(1, &star->EBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, star->EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, 2 * 3 * pointAmount * sizeof(unsigned int), indice, GL_STATIC_DRAW);

		// clear data
		delete vertices;
		delete color;
		delete indice;


		// set up worldPlane

		glGenVertexArrays(1, &worldPlane->VAO);
		glBindVertexArray(worldPlane->VAO);

		vertices = new Vec3f[4]();
		vertices[0] = { -100,	0,	-100 };
		vertices[1] = { 100,	0,	-100 };
		vertices[2] = { 100,	0,	100 };
		vertices[3] = { -100,	0,	100 };

		color = new Vec3f[4]();
		for (int i = 0; i < 4; i++)
			color[i] = { 0.5f,0.5f,0.5f };

		indice = new unsigned int[12]{ 2, 1, 0, 3, 2, 0, 0, 1, 2, 0, 2, 3 };

		glGenBuffers(1, &worldPlane->VBOv);
		glBindBuffer(GL_ARRAY_BUFFER, worldPlane->VBOv);
		glBufferData(GL_ARRAY_BUFFER, 4 * sizeof(Vec3f), vertices, GL_STATIC_DRAW);

		glGenBuffers(1, &worldPlane->VBOc);
		glBindBuffer(GL_ARRAY_BUFFER, worldPlane->VBOc);
		glBufferData(GL_ARRAY_BUFFER, 4 * sizeof(Vec3f), color, GL_STATIC_DRAW);

		glGenBuffers(1, &worldPlane->EBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, worldPlane->EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, 12 * sizeof(unsigned int), indice, GL_STATIC_DRAW);



		// set up worldPlane

		glGenVertexArrays(1, &block->VAO);
		glBindVertexArray(block->VAO);

		pointAmount = 8;
		vertices = new Vec3f[pointAmount]();
		for (int i = 0; i < pointAmount; i++)
		{
			string bin = std::bitset<3>(i).to_string();
			vertices[i] = { 0.3,0.3,0.3 };
			vertices[i].x *= (bin[0] == '0' ? -1 : 1);
			vertices[i].y *= (bin[1] == '0' ? -1 : 1);
			vertices[i].z *= (bin[2] == '0' ? -1 : 1);
			cout << vertices[i].x << " " << vertices[i].y << " " << vertices[i].z << endl;
		}

		color = new Vec3f[pointAmount]();
		for (int i = 0; i < pointAmount; i++)
			color[i] = { 0.01f,0.01f,0.01f };

		indice = new unsigned int[36]{
			0,1,2,
			3,2,1,
			6,5,4,
			5,6,7,
			5,3,1,
			3,5,7,
			0,2,4,
			6,4,2,
			2,3,6,
			7,6,3,
			1,0,4,
			4,5,1
		};

		glGenBuffers(1, &block->VBOv);
		glBindBuffer(GL_ARRAY_BUFFER, block->VBOv);
		glBufferData(GL_ARRAY_BUFFER, 8 * sizeof(Vec3f), vertices, GL_STATIC_DRAW);

		glGenBuffers(1, &block->VBOc);
		glBindBuffer(GL_ARRAY_BUFFER, block->VBOc);
		glBufferData(GL_ARRAY_BUFFER, 8 * sizeof(Vec3f), color, GL_STATIC_DRAW);

		glGenBuffers(1, &block->EBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, block->EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, 36 * sizeof(unsigned int), indice, GL_STATIC_DRAW);

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
		glClearColor(173.0f / 255, 216.0f / 255, 230.0f / 255, 0);
		glPolygonMode(GL_DOUBLE, GL_FILL);
		glEnable(GL_DEPTH_TEST);

		// Set up look degree
		float yzTemp = yzDegree - mouseMoveDelta->y, xzTemp = xzDegree + mouseMoveDelta->x;
		glm::vec3 vecXZ, lookVec;
		vecXZ =
			glm::rotate(glm::mat4(1.0f), glm::radians(xzTemp), glm::vec3(0, 1, 0)) *
			glm::vec4(0.0f, 0.0f, -1.0f, 0.0f);
		lookVec =
			glm::rotate(glm::mat4(1.0f), glm::radians(yzTemp), glm::cross(glm::vec3(0, 1, 0), vecXZ)) * glm::vec4(vecXZ, 0);
		*look = *eye + glm::vec3(lookVec.x, lookVec.y, lookVec.z);

		// Set matrix
		glm::mat4 rotation = glm::rotate(glm::mat4(1.0f), glm::radians(objAngle), glm::vec3(0, 1, 0));
		glm::mat4 move = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0.1f, 0));
		glm::mat4 view = glm::lookAt(*eye, *look, glm::vec3(0, 1, 0));
		glm::mat4 project = glm::perspective(glm::radians(fovy), (float)hkoglPanelControl1->Width / hkoglPanelControl1->Height, 0.1f, 10.0f);



		// draw star
		// Use the shader
		glUseProgram(star->Program);

		// put matrix
		glUniformMatrix4fv(glGetUniformLocation(star->Program, "rotationMatrix"), 1, GL_FALSE, glm::value_ptr(rotation));
		glUniformMatrix4fv(glGetUniformLocation(star->Program, "moveMatrix"), 1, GL_FALSE, glm::value_ptr(move));
		glUniformMatrix4fv(glGetUniformLocation(star->Program, "lookMatrix"), 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(glGetUniformLocation(star->Program, "projectMatrix"), 1, GL_FALSE, glm::value_ptr(project));

		// Enable 'position' attribute and assign data
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, star->VBOv);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

		// Enable 'color' attribute and assign data
		glEnableVertexAttribArray(1);
		glBindBuffer(GL_ARRAY_BUFFER, star->VBOc);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

		// Draw the triangle
		//glGenVertexArrays(1, &paramGL->VAO);
		glBindVertexArray(star->VAO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, star->EBO);
		glDrawElements(GL_TRIANGLES, 60, GL_UNSIGNED_INT, 0);



		// Set matrix
		rotation = glm::mat4(1.0f);
		move = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0));

		// draw plane
		glUseProgram(worldPlane->Program);

		glUniformMatrix4fv(glGetUniformLocation(worldPlane->Program, "rotationMatrix"), 1, GL_FALSE, glm::value_ptr(rotation));
		glUniformMatrix4fv(glGetUniformLocation(worldPlane->Program, "moveMatrix"), 1, GL_FALSE, glm::value_ptr(move));
		glUniformMatrix4fv(glGetUniformLocation(worldPlane->Program, "lookMatrix"), 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(glGetUniformLocation(worldPlane->Program, "projectMatrix"), 1, GL_FALSE, glm::value_ptr(project));

		// Enable 'position' attribute and assign data
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, worldPlane->VBOv);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

		// Enable 'color' attribute and assign data
		glEnableVertexAttribArray(1);
		glBindBuffer(GL_ARRAY_BUFFER, worldPlane->VBOc);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

		// Draw the triangle
		//glBindVertexArray(worldPlane->VAO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, worldPlane->EBO);
		glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_INT, 0);



		// Set matrix
		rotation = glm::rotate(glm::mat4(1.0f), glm::radians(objAngle), glm::vec3(0, 1, 1));
		move = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0.5 + sin(glm::radians(objAngle * 1.5)) / 8, 0));

		// draw block
		glUseProgram(block->Program);

		glUniformMatrix4fv(glGetUniformLocation(block->Program, "rotationMatrix"), 1, GL_FALSE, glm::value_ptr(rotation));
		glUniformMatrix4fv(glGetUniformLocation(block->Program, "moveMatrix"), 1, GL_FALSE, glm::value_ptr(move));
		glUniformMatrix4fv(glGetUniformLocation(block->Program, "lookMatrix"), 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(glGetUniformLocation(block->Program, "projectMatrix"), 1, GL_FALSE, glm::value_ptr(project));

		// Enable 'position' attribute and assign data
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, block->VBOv);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

		// Enable 'color' attribute and assign data
		glEnableVertexAttribArray(1);
		glBindBuffer(GL_ARRAY_BUFFER, block->VBOc);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

		// Draw the triangle
		//glBindVertexArray(block->VAO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, block->EBO);
		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

		// Disable vertex attribute arrays
		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);
		glDisable(GL_DEPTH_TEST);
	}

	private:
		keyEnable* keyEn;

	private: System::Void hkoglPanelControl1_KeyDown(System::Object^ sender, System::Windows::Forms::KeyEventArgs^ e)
	{
		bool* keyGet = nullptr;
		if (e->KeyCode == Keys::W)
			keyGet = &(keyEn->forword);
		if (e->KeyCode == Keys::S)
			keyGet = &(keyEn->back);
		if (e->KeyCode == Keys::A)
			keyGet = &(keyEn->left);
		if (e->KeyCode == Keys::D)
			keyGet = &(keyEn->right);
		if (e->KeyCode == Keys::Q)
			keyGet = &(keyEn->down);
		if (e->KeyCode == Keys::E)
			keyGet = &(keyEn->up);

		if (e->KeyCode == Keys::I)
			keyGet = &(keyEn->lookUp);
		if (e->KeyCode == Keys::K)
			keyGet = &(keyEn->lookDown);
		if (e->KeyCode == Keys::J)
			keyGet = &(keyEn->lookLeft);
		if (e->KeyCode == Keys::L)
			keyGet = &(keyEn->lookRight);

		if (keyGet)
			*keyGet = 1;
	}
	private: System::Void hkoglPanelControl1_KeyUp(System::Object^ sender, System::Windows::Forms::KeyEventArgs^ e)
	{
		bool* keyGet = nullptr;
		if (e->KeyCode == Keys::W)
			keyGet = &(keyEn->forword);
		if (e->KeyCode == Keys::S)
			keyGet = &(keyEn->back);
		if (e->KeyCode == Keys::A)
			keyGet = &(keyEn->left);
		if (e->KeyCode == Keys::D)
			keyGet = &(keyEn->right);
		if (e->KeyCode == Keys::Q)
			keyGet = &(keyEn->down);
		if (e->KeyCode == Keys::E)
			keyGet = &(keyEn->up);

		if (e->KeyCode == Keys::I)
			keyGet = &(keyEn->lookUp);
		if (e->KeyCode == Keys::K)
			keyGet = &(keyEn->lookDown);
		if (e->KeyCode == Keys::J)
			keyGet = &(keyEn->lookLeft);
		if (e->KeyCode == Keys::L)
			keyGet = &(keyEn->lookRight);

		if (keyGet)
			*keyGet = 0;
	}
	private: System::Void moveControl_Tick(System::Object^ sender, System::EventArgs^ e)
	{
		float speed = 0.05;
		if (keyEn->forword)
		{
			glm::vec3 temp = glm::normalize(*look - *eye);
			*eye += speed * temp;
			*look += speed * temp;
		}
		if (keyEn->back)
		{
			glm::vec3 temp = -glm::normalize(*look - *eye);
			*eye += speed * temp;
			*look += speed * temp;
		}
		if (keyEn->left)
		{
			glm::vec3 temp = -glm::normalize(glm::cross(*look - *eye, glm::vec3(0, 1, 0)));
			*eye += speed * temp;
			*look += speed * temp;
		}
		if (keyEn->right)
		{
			glm::vec3 temp = glm::normalize(glm::cross(*look - *eye, glm::vec3(0, 1, 0)));
			*eye += speed * temp;
			*look += speed * temp;
		}
		if (keyEn->down)
		{
			eye->y -= speed;
			look->y -= speed;
		}
		if (keyEn->up)
		{
			eye->y += speed;
			look->y += speed;
		}
	}

	private:
		glm::vec2* mouseMoveDelta;
		bool* moveCam;

	private: System::Void hkoglPanelControl1_MouseWheel(System::Object^ sender, System::Windows::Forms::MouseEventArgs^ e)
	{
		if (fovy - e->Delta / 50 < 0 || fovy - e->Delta / 50 > 90)
			return;
		fovy -= e->Delta / 50;
	}
	private: System::Void hkoglPanelControl1_MouseDown(System::Object^ sender, System::Windows::Forms::MouseEventArgs^ e)
	{
		*moveCam = 1;
		*mouseClickPos = { e->X,e->Y };
	}
	private: System::Void hkoglPanelControl1_MouseUp(System::Object^ sender, System::Windows::Forms::MouseEventArgs^ e)
	{
		*moveCam = 0;
		yzDegree -= mouseMoveDelta->y;
		xzDegree += mouseMoveDelta->x;
		*mouseMoveDelta = { 0,0 };
	}
	private: System::Void hkoglPanelControl1_MouseMove(System::Object^ sender, System::Windows::Forms::MouseEventArgs^ e)
	{
		if (*moveCam)
		{
			glm::vec2 cur = glm::vec2(e->X, e->Y);
			*mouseMoveDelta = (cur - *mouseClickPos) / 5.0f;
			mouseMoveDelta->x /= 5.0f;
			mouseMoveDelta->y /= 5.0f;
		}
	}

	private: System::Void timer1_Tick(System::Object^ sender, System::EventArgs^ e)
	{
		objAngle = (int)(objAngle + 1) % 360;
		// Redraw the OpenGL Panel
		hkoglPanelControl1->Invalidate();
	}
	};
}
