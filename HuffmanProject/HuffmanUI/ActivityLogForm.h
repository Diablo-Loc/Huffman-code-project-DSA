#pragma once

namespace HuffmanUI {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

	/// <summary>
	/// Summary for ActivityLogForm
	/// </summary>
	public ref class ActivityLogForm : public System::Windows::Forms::Form
	{
	public:
		ActivityLogForm(void)
		{
			InitializeComponent();
			//
			//TODO: Add the constructor code here
			//
		}
		void UpdateStep(String^ text, Color color) {
			if (this->rtbLogContent->InvokeRequired) {
				this->rtbLogContent->Invoke(gcnew Action<String^, Color>(this, &ActivityLogForm::UpdateStep), text, color);
			}
			else {
				rtbLogContent->Clear();

				rtbLogContent->SelectionAlignment = HorizontalAlignment::Center;
				rtbLogContent->SelectionFont = (gcnew System::Drawing::Font(L"Consolas", 11, System::Drawing::FontStyle::Bold));
				rtbLogContent->SelectionColor = color;
				rtbLogContent->AppendText(text);
			}
		}
	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~ActivityLogForm()
		{
			if (components)
			{
				delete components;
			}
		}
	private: System::Windows::Forms::RichTextBox^ rtbLogContent;
	protected:

	private:
		/// <summary>
		/// Required designer variable.
		/// </summary>
		System::ComponentModel::Container ^components;

#pragma region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		void InitializeComponent(void)
		{
			this->rtbLogContent = (gcnew System::Windows::Forms::RichTextBox());
			this->SuspendLayout();
			// 
			// rtbLogContent
			// 
			this->rtbLogContent->BackColor = System::Drawing::Color::Black;
			this->rtbLogContent->Dock = System::Windows::Forms::DockStyle::Fill;
			this->rtbLogContent->Font = (gcnew System::Drawing::Font(L"Consolas", 10.2F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->rtbLogContent->ForeColor = System::Drawing::Color::Lime;
			this->rtbLogContent->Location = System::Drawing::Point(0, 0);
			this->rtbLogContent->Name = L"rtbLogContent";
			this->rtbLogContent->ReadOnly = true;
			this->rtbLogContent->Size = System::Drawing::Size(644, 121);
			this->rtbLogContent->TabIndex = 0;
			this->rtbLogContent->Text = L"";
			// 
			// ActivityLogForm
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(8, 16);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(644, 121);
			this->Controls->Add(this->rtbLogContent);
			this->Name = L"ActivityLogForm";
			this->Text = L"ActivityLogForm";
			this->ResumeLayout(false);

		}
#pragma endregion
	};
}
