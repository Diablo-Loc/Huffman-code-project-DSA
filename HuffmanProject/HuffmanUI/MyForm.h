#pragma once
#include "HuffmanWrapper.h"
#include <msclr/marshal_cppstd.h>
#include <msclr/marshal.h>

namespace HuffmanUI {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

	/// <summary>
	/// Summary for MyForm
	/// </summary>
	public ref class MyForm : public System::Windows::Forms::Form
	{
	public:
		MyForm(void)
		{
			InitializeComponent();
			//
			//TODO: Add the constructor code here
			//
		}

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~MyForm()
		{
			if (components)
			{
				delete components;
			}
		}
	private: System::Windows::Forms::Button^ btnOpen;
	protected:

	protected:


	private: System::Windows::Forms::Button^ btnEncode;
	private: System::Windows::Forms::Button^ btnDecode;
	private: System::Windows::Forms::TextBox^ txtInput;
	private: System::Windows::Forms::TextBox^ txtOutput;
	private: System::Windows::Forms::TextBox^ txtTable;
	private: System::Windows::Forms::Label^ lblBefore;
	private: System::Windows::Forms::Label^ lblAfter;
	private: System::Windows::Forms::Label^ lblRatio;
	private: System::Windows::Forms::Label^ label1;
	private: System::Windows::Forms::Label^ label2;
	private: System::Windows::Forms::Label^ label3;

	private:
		/// <summary>
		/// Required designer variable.
		/// </summary>
		System::ComponentModel::Container^ components;
		System::String^ selectedInputPath;

#pragma region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		void InitializeComponent(void)
		{
			this->btnOpen = (gcnew System::Windows::Forms::Button());
			this->btnEncode = (gcnew System::Windows::Forms::Button());
			this->btnDecode = (gcnew System::Windows::Forms::Button());
			this->txtInput = (gcnew System::Windows::Forms::TextBox());
			this->txtOutput = (gcnew System::Windows::Forms::TextBox());
			this->txtTable = (gcnew System::Windows::Forms::TextBox());
			this->lblBefore = (gcnew System::Windows::Forms::Label());
			this->lblAfter = (gcnew System::Windows::Forms::Label());
			this->lblRatio = (gcnew System::Windows::Forms::Label());
			this->label1 = (gcnew System::Windows::Forms::Label());
			this->label2 = (gcnew System::Windows::Forms::Label());
			this->label3 = (gcnew System::Windows::Forms::Label());
			this->SuspendLayout();
			// 
			// btnOpen
			// 
			this->btnOpen->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 12, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->btnOpen->Location = System::Drawing::Point(63, 25);
			this->btnOpen->Name = L"btnOpen";
			this->btnOpen->Size = System::Drawing::Size(133, 53);
			this->btnOpen->TabIndex = 0;
			this->btnOpen->Text = L"Open";
			this->btnOpen->UseVisualStyleBackColor = true;
			this->btnOpen->Click += gcnew System::EventHandler(this, &MyForm::btnOpen_Click);
			// 
			// btnEncode
			// 
			this->btnEncode->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 12, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->btnEncode->Location = System::Drawing::Point(358, 25);
			this->btnEncode->Name = L"btnEncode";
			this->btnEncode->Size = System::Drawing::Size(133, 53);
			this->btnEncode->TabIndex = 3;
			this->btnEncode->Text = L"Encode";
			this->btnEncode->UseVisualStyleBackColor = true;
			this->btnEncode->Click += gcnew System::EventHandler(this, &MyForm::btnEncode_Click);
			// 
			// btnDecode
			// 
			this->btnDecode->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 12, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->btnDecode->Location = System::Drawing::Point(606, 25);
			this->btnDecode->Name = L"btnDecode";
			this->btnDecode->Size = System::Drawing::Size(133, 53);
			this->btnDecode->TabIndex = 4;
			this->btnDecode->Text = L"Decode";
			this->btnDecode->UseVisualStyleBackColor = true;
			this->btnDecode->Click += gcnew System::EventHandler(this, &MyForm::btnDecode_Click);
			// 
			// txtInput
			// 
			this->txtInput->Location = System::Drawing::Point(63, 165);
			this->txtInput->Multiline = true;
			this->txtInput->Name = L"txtInput";
			this->txtInput->Size = System::Drawing::Size(352, 38);
			this->txtInput->TabIndex = 5;
			// 
			// txtOutput
			// 
			this->txtOutput->Location = System::Drawing::Point(65, 251);
			this->txtOutput->Multiline = true;
			this->txtOutput->Name = L"txtOutput";
			this->txtOutput->Size = System::Drawing::Size(350, 40);
			this->txtOutput->TabIndex = 6;
			// 
			// txtTable
			// 
			this->txtTable->Location = System::Drawing::Point(66, 347);
			this->txtTable->Multiline = true;
			this->txtTable->Name = L"txtTable";
			this->txtTable->ScrollBars = System::Windows::Forms::ScrollBars::Vertical;
			this->txtTable->Size = System::Drawing::Size(349, 226);
			this->txtTable->TabIndex = 7;
			// 
			// lblBefore
			// 
			this->lblBefore->AutoSize = true;
			this->lblBefore->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 16.2F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->lblBefore->Location = System::Drawing::Point(458, 118);
			this->lblBefore->Name = L"lblBefore";
			this->lblBefore->Size = System::Drawing::Size(158, 32);
			this->lblBefore->TabIndex = 8;
			this->lblBefore->Text = L"Size before";
			// 
			// lblAfter
			// 
			this->lblAfter->AutoSize = true;
			this->lblAfter->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 16.2F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->lblAfter->Location = System::Drawing::Point(458, 168);
			this->lblAfter->Name = L"lblAfter";
			this->lblAfter->Size = System::Drawing::Size(134, 32);
			this->lblAfter->TabIndex = 9;
			this->lblAfter->Text = L"Size after";
			// 
			// lblRatio
			// 
			this->lblRatio->AutoSize = true;
			this->lblRatio->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 16.2F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->lblRatio->Location = System::Drawing::Point(460, 219);
			this->lblRatio->Name = L"lblRatio";
			this->lblRatio->Size = System::Drawing::Size(244, 32);
			this->lblRatio->TabIndex = 10;
			this->lblRatio->Text = L"Compression ratio";
			// 
			// label1
			// 
			this->label1->AutoSize = true;
			this->label1->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 13.8F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->label1->Location = System::Drawing::Point(60, 118);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(167, 29);
			this->label1->TabIndex = 11;
			this->label1->Text = L"Input (original)";
			// 
			// label2
			// 
			this->label2->AutoSize = true;
			this->label2->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 13.8F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->label2->Location = System::Drawing::Point(60, 219);
			this->label2->Name = L"label2";
			this->label2->Size = System::Drawing::Size(355, 29);
			this->label2->TabIndex = 12;
			this->label2->Text = L"Output (compressed / decoded)";
			// 
			// label3
			// 
			this->label3->AutoSize = true;
			this->label3->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 13.8F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->label3->Location = System::Drawing::Point(60, 315);
			this->label3->Name = L"label3";
			this->label3->Size = System::Drawing::Size(170, 29);
			this->label3->TabIndex = 13;
			this->label3->Text = L"Huffman Table";
			// 
			// MyForm
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(8, 16);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(891, 611);
			this->Controls->Add(this->label3);
			this->Controls->Add(this->label2);
			this->Controls->Add(this->label1);
			this->Controls->Add(this->lblRatio);
			this->Controls->Add(this->lblAfter);
			this->Controls->Add(this->lblBefore);
			this->Controls->Add(this->txtTable);
			this->Controls->Add(this->txtOutput);
			this->Controls->Add(this->txtInput);
			this->Controls->Add(this->btnDecode);
			this->Controls->Add(this->btnEncode);
			this->Controls->Add(this->btnOpen);
			this->Name = L"MyForm";
			this->Text = L"MyForm";
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion
	private: System::Void btnOpen_Click(System::Object^ sender, System::EventArgs^ e)
	{
		OpenFileDialog^ ofd = gcnew OpenFileDialog();
		ofd->Filter = "All files (*.*)|*.*";

		if (ofd->ShowDialog() == System::Windows::Forms::DialogResult::OK)
		{
			selectedInputPath = ofd->FileName;
			txtInput->Text = selectedInputPath;
		}
	}


	private: System::Void btnEncode_Click(System::Object^ sender, System::EventArgs^ e)
	{
		if (String::IsNullOrEmpty(selectedInputPath))
		{
			MessageBox::Show("Please open a file first!");
			return;
		}

		SaveFileDialog^ sfd = gcnew SaveFileDialog();
		sfd->Filter = "Huffman File (*.huff)|*.huff";

		if (sfd->ShowDialog() != System::Windows::Forms::DialogResult::OK)
			return;

		msclr::interop::marshal_context ctx;
		std::string inPath = ctx.marshal_as<std::string>(selectedInputPath);
		std::string outPath =
			msclr::interop::marshal_as<std::string>(sfd->FileName);

		if (HuffmanWrapper::compressFile(inPath, outPath))
		{
			txtOutput->Text = sfd->FileName;
			MessageBox::Show("Compress success!");
		}
		else
		{
			MessageBox::Show("Compress failed!");
		}
	}


	private: System::Void btnDecode_Click(System::Object^ sender, System::EventArgs^ e)
	{
		OpenFileDialog^ ofd = gcnew OpenFileDialog();
		ofd->Filter = "Huffman File (*.huff)|*.huff";

		if (ofd->ShowDialog() != System::Windows::Forms::DialogResult::OK)
			return;

		SaveFileDialog^ sfd = gcnew SaveFileDialog();
		sfd->Filter = "All files (*.*)|*.*";

		if (sfd->ShowDialog() != System::Windows::Forms::DialogResult::OK)
			return;

		std::string inPath =
			msclr::interop::marshal_as<std::string>(ofd->FileName);
		std::string outPath =
			msclr::interop::marshal_as<std::string>(sfd->FileName);

		if (HuffmanWrapper::decompressFile(inPath, outPath))
		{
			txtOutput->Text = sfd->FileName;
			MessageBox::Show("Decode success!");
		}
		else
		{
			MessageBox::Show("Decode failed!");
		}
	}


	};
}
