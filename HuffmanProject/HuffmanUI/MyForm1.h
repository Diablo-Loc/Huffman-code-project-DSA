#pragma once
#ifndef __INTELLISENSE__
#include "HuffmanWrapper.h"
#include "Node.h"
#include "Visualize.h"
#include "Metrics.h"
#include "ActivityLogForm.h"
#include <msclr/marshal_cppstd.h>

using msclr::interop::marshal_as;
namespace HuffmanUI {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

	/// <summary>
	/// Summary for MyForm1
	/// </summary>
	public ref class MyForm1 : public System::Windows::Forms::Form
	{
	public:
		MyForm1(void)
		{
			InitializeComponent();
			InitMetrics();
			// Khởi tạo visualizer
			visualizer = gcnew HuffmanUI::Visualize();
			visualizer->TopLevel = false;
			visualizer->FormBorderStyle = System::Windows::Forms::FormBorderStyle::None;
			visualizer->Dock = DockStyle::Fill;

			// Thêm vào pnlGraphic
			this->pnlGraphic->Controls->Add(visualizer);
			visualizer->Show();
			this->pnlGraphic->MouseWheel += gcnew MouseEventHandler(this, &MyForm1::pnlGraphic_MouseWheel);
			// Khởi tạo Form Log
			logUI = gcnew ActivityLogForm();
			logUI->TopLevel = false;
			logUI->FormBorderStyle = System::Windows::Forms::FormBorderStyle::None;

			// Ép Dock Fill để nó luôn bám sát 4 cạnh của Panel
			logUI->Dock = System::Windows::Forms::DockStyle::Fill;

			this->pnlLogContainer->Controls->Add(logUI);
			logUI->Show();
			//
			//TODO: Add the constructor code here
			//
		}
		void UpdateUI_Tree(IntPtr rootPtr) {
			Node* root = (Node*)rootPtr.ToPointer();
			if (visualizer != nullptr && root != nullptr) {
				visualizer->SetTreeAndDraw(root);
			}
		}

		// Cập nhật Thông số (Metrics)
		void UpdateUI_Metrics(IntPtr metricsPtr) {
			CompressionMetrics* m = (CompressionMetrics*)metricsPtr.ToPointer();
			UpdateMetric("Space Saving", m->spaceSaving.ToString("F1") + "%");
			UpdateMetric("Entropy", m->entropy.ToString("F2"));
			UpdateMetric("Max Tree Depth", m->maxTreeDepth.ToString());
			UpdateMetric("Processing Speed", m->processingTimeMs.ToString("F2") + " ms");
		}
		void LogActivity(String^ action, Color textColor) {
			if (logUI != nullptr) {
				logUI->UpdateStep(action, textColor); 
			}
		}
		void PerformCompression() {
			try {
				String^ inputPath = txtSource->Text;
				String^ outputPath = System::IO::Path::Combine(txtDest->Text, System::IO::Path::GetFileName(inputPath) + ".huf");

				std::string input = marshal_as<std::string>(inputPath);
				std::string output = marshal_as<std::string>(outputPath);

				logUI->UpdateStep("INIT: Starting Huffman Compression Engine...", Color::Yellow);
				System::Threading::Thread::Sleep(1000);

				logUI->UpdateStep("STEP 1: Analyzing frequencies...", Color::White);
				CompressionMetrics metrics;
				bool ok = HuffmanWrapper::compressFile(input, output, metrics);
				System::Threading::Thread::Sleep(1000);

				if (ok) {
					logUI->UpdateStep("STEP 2: Building Binary Tree...", Color::Cyan);
					this->Invoke(gcnew Action<IntPtr>(this, &MyForm1::UpdateUI_Tree), (IntPtr)metrics.root);
					System::Threading::Thread::Sleep(1000);

					logUI->UpdateStep("STEP 3: Encoding & Saving file...", Color::Yellow);
					this->Invoke(gcnew Action<IntPtr>(this, &MyForm1::UpdateUI_Metrics), (IntPtr)&metrics);
					System::Threading::Thread::Sleep(1000);

					logUI->UpdateStep("SUCCESS: Compression completed!", Color::Lime);

					// GỌI HÀM HIỆN THÔNG BÁO (KHÔNG DÙNG LAMBDA)
					this->Invoke(gcnew Action<String^>(this, &MyForm1::ShowFinishMessage), outputPath);
				}
			}
			catch (Exception^ ex) {
				if (logUI != nullptr) logUI->UpdateStep("ERROR: " + ex->Message, Color::Red);
			}
		}
		void PerformDecompression() {
			try {
				String^ inputPath = txtSource->Text;
				String^ restoredName = System::IO::Path::GetFileName(inputPath)->Replace(".huf", "");
				String^ outputPath = System::IO::Path::Combine(txtDest->Text, restoredName);

				std::string input = marshal_as<std::string>(inputPath);
				std::string output = marshal_as<std::string>(outputPath);

				logUI->UpdateStep("INIT: Starting Decompression...", Color::Yellow);
				System::Threading::Thread::Sleep(1000);

				CompressionMetrics metrics;
				bool ok = HuffmanWrapper::decompressFile(input, output, metrics);

				if (ok) {
					logUI->UpdateStep("STEP 1: Reconstructing Tree...", Color::Cyan);
					this->Invoke(gcnew Action<IntPtr>(this, &MyForm1::UpdateUI_Tree), (IntPtr)metrics.root);
					System::Threading::Thread::Sleep(1000);

					logUI->UpdateStep("STEP 2: Decoding bitstream...", Color::Yellow);
					this->Invoke(gcnew Action<IntPtr>(this, &MyForm1::UpdateUI_Metrics), (IntPtr)&metrics);
					System::Threading::Thread::Sleep(1000);

					logUI->UpdateStep("SUCCESS: File restored!", Color::Lime);

					// GỌI HÀM HIỆN THÔNG BÁO
					this->Invoke(gcnew Action<String^>(this, &MyForm1::ShowFinishMessage), outputPath);
				}
			}
			catch (Exception^ ex) {
				if (logUI != nullptr) logUI->UpdateStep("ERROR: " + ex->Message, Color::Red);
			}
		}
	protected:
		float zoomLevel = 1.0f; // 1.0 là 100%

			
	private: 
		System::Windows::Forms::Panel^ pnlLogContainer;
		HuffmanUI::ActivityLogForm^ logUI;
	protected:
		const float zoomStep = 0.1f; // Mỗi lần cuộn chuột tăng/giảm 10%
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~MyForm1() {
			if (huffmanRoot) {
				DeleteTree(huffmanRoot); 
				huffmanRoot = nullptr;
			}
			if (components) delete components;
		}
		void ShowFinishMessage(String^ outputPath) {
			System::Windows::Forms::DialogResult result = MessageBox::Show(
				"Xử lý file thành công!\nBạn có muốn mở thư mục chứa file không?",
				"Thông báo",
				MessageBoxButtons::YesNo,
				MessageBoxIcon::Information
			);

			if (result == System::Windows::Forms::DialogResult::Yes) {
				// Mở thư mục và chọn file
				System::Diagnostics::Process::Start("explorer.exe", "/select,\"" + outputPath + "\"");
			}
		}
		void InitMetrics()
		{
			lvMetrics->Scrollable = true;
			lvMetrics->Clear();

			lvMetrics->OwnerDraw = true;

			lvMetrics->DrawColumnHeader +=
				gcnew DrawListViewColumnHeaderEventHandler(
					this, &MyForm1::lvMetrics_DrawColumnHeader);

			lvMetrics->DrawItem +=
				gcnew DrawListViewItemEventHandler(
					this, &MyForm1::lvMetrics_DrawItem);

			lvMetrics->DrawSubItem +=
				gcnew DrawListViewSubItemEventHandler(
					this, &MyForm1::lvMetrics_DrawSubItem);

			// Hack tăng chiều cao dòng
			ImageList^ img = gcnew ImageList();
			img->ImageSize = Drawing::Size(1, 48);
			lvMetrics->SmallImageList = img;

			lvMetrics->View = View::Details;
			lvMetrics->FullRowSelect = true;
			lvMetrics->GridLines = true;

			int totalWidth = lvMetrics->ClientSize.Width;

			lvMetrics->Columns->Add("Metric", totalWidth * 20 / 100);
			lvMetrics->Columns->Add("Description", totalWidth * 55 / 100);
			lvMetrics->Columns->Add("Value", totalWidth * 25 / 100);


			AddMetric("Space Saving",
				"Compression ratio improvement when using Huffman coding",
				"-");

			AddMetric("Entropy",
				"Source entropy (bits per symbol)",
				"-");

			AddMetric("Max Tree Depth",
				"Height of the generated Huffman tree",
				"-");

			AddMetric("Processing Speed",
				"Total compression execution time (ms)",
				"-");
		}

		void AddMetric(String^ name,
			String^ description,
			String^ value)
		{
			ListViewItem^ item = gcnew ListViewItem(name);
			item->SubItems->Add(description);
			item->SubItems->Add(value);
			lvMetrics->Items->Add(item);
		}

		void UpdateMetric(String^ metricName,
			String^ value)
		{
			for each (ListViewItem ^ item in lvMetrics->Items)
			{
				if (item->Text == metricName)
				{
					item->SubItems[2]->Text = value;
					return;
				}
			}
		}
		void lvMetrics_DrawColumnHeader(
			Object^ sender,
			DrawListViewColumnHeaderEventArgs^ e)
		{
			e->DrawDefault = true;
		}
		void lvMetrics_DrawItem(
			Object^ sender,
			DrawListViewItemEventArgs^ e)
		{
		}

		void lvMetrics_DrawSubItem(
			Object^ sender,
			DrawListViewSubItemEventArgs^ e)
		{
			TextFormatFlags flags =
				TextFormatFlags::WordBreak |
				TextFormatFlags::Left |
				TextFormatFlags::VerticalCenter;

			// Tô nền khi được chọn
			if (e->Item->Selected)
			{
				e->Graphics->FillRectangle(
					SystemBrushes::Highlight,
					e->Bounds
				);
			}

			TextRenderer::DrawText(
				e->Graphics,
				e->SubItem->Text,
				lvMetrics->Font,
				e->Bounds,
				e->Item->Selected
				? SystemColors::HighlightText
				: SystemColors::ControlText,
				flags
			);
		}
		void DeleteTree(Node* node) {
			if (!node) return;
			DeleteTree(node->left);
			DeleteTree(node->right);
			delete node; // Giải phóng vùng nhớ của Node
		}

	private: System::Windows::Forms::Label^ label1;
	private: System::Windows::Forms::Label^ lblFileManagement;
	private: System::Windows::Forms::Label^ lblAlgorithmVisualization;
	private: System::Windows::Forms::Label^ lblPerformanceMetrics;
	private: System::Windows::Forms::Panel^ pnlTop;

	private: System::Windows::Forms::Panel^ pnlGraphic;
	private: System::Windows::Forms::Button^ btnCompress;
	private: System::Windows::Forms::Button^ btnDecompress;
	private: System::Windows::Forms::GroupBox^ groupboxFileOperations;
	private: System::Windows::Forms::TextBox^ txtDest;
	private: System::Windows::Forms::TextBox^ txtSource;
	private: System::Windows::Forms::Label^ lblDestinationFolder;

	private: System::Windows::Forms::Label^ lblSourcePath;
	private: System::Windows::Forms::Button^ btnSelectFolder;
	private: System::Windows::Forms::Button^ btnBrowse;



	private: System::Windows::Forms::ListView^ lvMetrics;

	private: System::Windows::Forms::ColumnHeader^ Metric;
	private: System::Windows::Forms::ColumnHeader^ Description;
	private: System::Windows::Forms::ColumnHeader^ Value;
	private: System::Windows::Forms::Label^ lblProFeatures;
	private: System::Windows::Forms::Label^ lblLabelmultiline;
	private: System::Windows::Forms::Label^ lblMemoryclean;
	private: System::Windows::Forms::PictureBox^ pictureBox1;

	protected:

	private:
		/// <summary>
		/// Required designer variable.
		/// </summary>
		/// 
		HuffmanUI::Visualize^ visualizer;
		System::ComponentModel::Container^ components;
	private: System::Windows::Forms::Label^ lblSize;

	private: System::Windows::Forms::Label^ lblFormat;
	private: System::Windows::Forms::Label^ lblFileName;
	private: System::Windows::Forms::PictureBox^ picFileIcon;

		   Node* huffmanRoot = nullptr;
#pragma region Windows Form Designer generated code
		   /// <summary>
		   /// Required method for Designer support - do not modify
		   /// the contents of this method with the code editor.
		   /// </summary>
		   void InitializeComponent(void)
		   {
			   this->label1 = (gcnew System::Windows::Forms::Label());
			   this->lblFileManagement = (gcnew System::Windows::Forms::Label());
			   this->lblAlgorithmVisualization = (gcnew System::Windows::Forms::Label());
			   this->lblPerformanceMetrics = (gcnew System::Windows::Forms::Label());
			   this->pnlTop = (gcnew System::Windows::Forms::Panel());
			   this->pnlGraphic = (gcnew System::Windows::Forms::Panel());
			   this->btnCompress = (gcnew System::Windows::Forms::Button());
			   this->btnDecompress = (gcnew System::Windows::Forms::Button());
			   this->groupboxFileOperations = (gcnew System::Windows::Forms::GroupBox());
			   this->picFileIcon = (gcnew System::Windows::Forms::PictureBox());
			   this->lblSize = (gcnew System::Windows::Forms::Label());
			   this->lblFormat = (gcnew System::Windows::Forms::Label());
			   this->lblFileName = (gcnew System::Windows::Forms::Label());
			   this->btnSelectFolder = (gcnew System::Windows::Forms::Button());
			   this->btnBrowse = (gcnew System::Windows::Forms::Button());
			   this->txtDest = (gcnew System::Windows::Forms::TextBox());
			   this->txtSource = (gcnew System::Windows::Forms::TextBox());
			   this->lblDestinationFolder = (gcnew System::Windows::Forms::Label());
			   this->lblSourcePath = (gcnew System::Windows::Forms::Label());
			   this->lvMetrics = (gcnew System::Windows::Forms::ListView());
			   this->Metric = (gcnew System::Windows::Forms::ColumnHeader());
			   this->Description = (gcnew System::Windows::Forms::ColumnHeader());
			   this->Value = (gcnew System::Windows::Forms::ColumnHeader());
			   this->lblProFeatures = (gcnew System::Windows::Forms::Label());
			   this->lblLabelmultiline = (gcnew System::Windows::Forms::Label());
			   this->lblMemoryclean = (gcnew System::Windows::Forms::Label());
			   this->pictureBox1 = (gcnew System::Windows::Forms::PictureBox());
			   this->pnlLogContainer = (gcnew System::Windows::Forms::Panel());
			   this->pnlTop->SuspendLayout();
			   this->groupboxFileOperations->SuspendLayout();
			   (cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->picFileIcon))->BeginInit();
			   (cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->pictureBox1))->BeginInit();
			   this->SuspendLayout();
			   // 
			   // label1
			   // 
			   this->label1->AutoSize = true;
			   this->label1->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 13.8F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				   static_cast<System::Byte>(0)));
			   this->label1->Location = System::Drawing::Point(375, 21);
			   this->label1->Name = L"label1";
			   this->label1->Size = System::Drawing::Size(203, 29);
			   this->label1->TabIndex = 0;
			   this->label1->Text = L"HUFFMAN TOOL";
			   // 
			   // lblFileManagement
			   // 
			   this->lblFileManagement->AutoSize = true;
			   this->lblFileManagement->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 13.8F, System::Drawing::FontStyle::Regular,
				   System::Drawing::GraphicsUnit::Point, static_cast<System::Byte>(0)));
			   this->lblFileManagement->Location = System::Drawing::Point(12, 101);
			   this->lblFileManagement->Name = L"lblFileManagement";
			   this->lblFileManagement->Size = System::Drawing::Size(200, 29);
			   this->lblFileManagement->TabIndex = 1;
			   this->lblFileManagement->Text = L"File Management";
			   // 
			   // lblAlgorithmVisualization
			   // 
			   this->lblAlgorithmVisualization->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left)
				   | System::Windows::Forms::AnchorStyles::Right));
			   this->lblAlgorithmVisualization->AutoSize = true;
			   this->lblAlgorithmVisualization->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 13.8F, System::Drawing::FontStyle::Regular,
				   System::Drawing::GraphicsUnit::Point, static_cast<System::Byte>(0)));
			   this->lblAlgorithmVisualization->Location = System::Drawing::Point(286, 101);
			   this->lblAlgorithmVisualization->Name = L"lblAlgorithmVisualization";
			   this->lblAlgorithmVisualization->Size = System::Drawing::Size(255, 29);
			   this->lblAlgorithmVisualization->TabIndex = 2;
			   this->lblAlgorithmVisualization->Text = L"Algorithm Visualization";
			   // 
			   // lblPerformanceMetrics
			   // 
			   this->lblPerformanceMetrics->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Right));
			   this->lblPerformanceMetrics->AutoSize = true;
			   this->lblPerformanceMetrics->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 13.8F, System::Drawing::FontStyle::Regular,
				   System::Drawing::GraphicsUnit::Point, static_cast<System::Byte>(0)));
			   this->lblPerformanceMetrics->Location = System::Drawing::Point(805, 101);
			   this->lblPerformanceMetrics->Name = L"lblPerformanceMetrics";
			   this->lblPerformanceMetrics->Size = System::Drawing::Size(235, 29);
			   this->lblPerformanceMetrics->TabIndex = 3;
			   this->lblPerformanceMetrics->Text = L"Performance Metrics";
			   // 
			   // pnlTop
			   // 
			   this->pnlTop->Controls->Add(this->label1);
			   this->pnlTop->Dock = System::Windows::Forms::DockStyle::Top;
			   this->pnlTop->Location = System::Drawing::Point(0, 0);
			   this->pnlTop->Name = L"pnlTop";
			   this->pnlTop->Size = System::Drawing::Size(1132, 78);
			   this->pnlTop->TabIndex = 4;
			   // 
			   // pnlGraphic
			   // 
			   this->pnlGraphic->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom)
				   | System::Windows::Forms::AnchorStyles::Left)
				   | System::Windows::Forms::AnchorStyles::Right));
			   this->pnlGraphic->Location = System::Drawing::Point(291, 136);
			   this->pnlGraphic->Name = L"pnlGraphic";
			   this->pnlGraphic->Size = System::Drawing::Size(494, 420);
			   this->pnlGraphic->TabIndex = 5;
			   // 
			   // btnCompress
			   // 
			   this->btnCompress->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Bottom | System::Windows::Forms::AnchorStyles::Left));
			   this->btnCompress->Location = System::Drawing::Point(318, 627);
			   this->btnCompress->Name = L"btnCompress";
			   this->btnCompress->Size = System::Drawing::Size(154, 42);
			   this->btnCompress->TabIndex = 6;
			   this->btnCompress->Text = L"Compress File";
			   this->btnCompress->UseVisualStyleBackColor = true;
			   this->btnCompress->Click += gcnew System::EventHandler(this, &MyForm1::btnCompress_Click);
			   // 
			   // btnDecompress
			   // 
			   this->btnDecompress->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Bottom | System::Windows::Forms::AnchorStyles::Right));
			   this->btnDecompress->Location = System::Drawing::Point(586, 627);
			   this->btnDecompress->Name = L"btnDecompress";
			   this->btnDecompress->Size = System::Drawing::Size(154, 42);
			   this->btnDecompress->TabIndex = 7;
			   this->btnDecompress->Text = L"Decompress File";
			   this->btnDecompress->UseVisualStyleBackColor = true;
			   this->btnDecompress->Click += gcnew System::EventHandler(this, &MyForm1::btnDecompress_Click);
			   // 
			   // groupboxFileOperations
			   // 
			   this->groupboxFileOperations->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom)
				   | System::Windows::Forms::AnchorStyles::Left));
			   this->groupboxFileOperations->Controls->Add(this->picFileIcon);
			   this->groupboxFileOperations->Controls->Add(this->lblSize);
			   this->groupboxFileOperations->Controls->Add(this->lblFormat);
			   this->groupboxFileOperations->Controls->Add(this->lblFileName);
			   this->groupboxFileOperations->Controls->Add(this->btnSelectFolder);
			   this->groupboxFileOperations->Controls->Add(this->btnBrowse);
			   this->groupboxFileOperations->Controls->Add(this->txtDest);
			   this->groupboxFileOperations->Controls->Add(this->txtSource);
			   this->groupboxFileOperations->Controls->Add(this->lblDestinationFolder);
			   this->groupboxFileOperations->Controls->Add(this->lblSourcePath);
			   this->groupboxFileOperations->Location = System::Drawing::Point(13, 136);
			   this->groupboxFileOperations->Name = L"groupboxFileOperations";
			   this->groupboxFileOperations->Size = System::Drawing::Size(262, 308);
			   this->groupboxFileOperations->TabIndex = 8;
			   this->groupboxFileOperations->TabStop = false;
			   this->groupboxFileOperations->Text = L"File Operations";
			   // 
			   // picFileIcon
			   // 
			   this->picFileIcon->Location = System::Drawing::Point(16, 190);
			   this->picFileIcon->Name = L"picFileIcon";
			   this->picFileIcon->Size = System::Drawing::Size(61, 47);
			   this->picFileIcon->TabIndex = 9;
			   this->picFileIcon->TabStop = false;
			   // 
			   // lblSize
			   // 
			   this->lblSize->AutoSize = true;
			   this->lblSize->Location = System::Drawing::Point(13, 171);
			   this->lblSize->Name = L"lblSize";
			   this->lblSize->Size = System::Drawing::Size(36, 16);
			   this->lblSize->TabIndex = 8;
			   this->lblSize->Text = L"Size:";
			   // 
			   // lblFormat
			   // 
			   this->lblFormat->AutoSize = true;
			   this->lblFormat->Location = System::Drawing::Point(13, 145);
			   this->lblFormat->Name = L"lblFormat";
			   this->lblFormat->Size = System::Drawing::Size(52, 16);
			   this->lblFormat->TabIndex = 7;
			   this->lblFormat->Text = L"Format:";
			   // 
			   // lblFileName
			   // 
			   this->lblFileName->AutoSize = true;
			   this->lblFileName->Location = System::Drawing::Point(13, 120);
			   this->lblFileName->Name = L"lblFileName";
			   this->lblFileName->Size = System::Drawing::Size(47, 16);
			   this->lblFileName->TabIndex = 6;
			   this->lblFileName->Text = L"Name:";
			   // 
			   // btnSelectFolder
			   // 
			   this->btnSelectFolder->Location = System::Drawing::Point(29, 279);
			   this->btnSelectFolder->Name = L"btnSelectFolder";
			   this->btnSelectFolder->Size = System::Drawing::Size(75, 23);
			   this->btnSelectFolder->TabIndex = 5;
			   this->btnSelectFolder->Text = L"Select";
			   this->btnSelectFolder->UseVisualStyleBackColor = true;
			   this->btnSelectFolder->Click += gcnew System::EventHandler(this, &MyForm1::btnSelectFolder_Click);
			   // 
			   // btnBrowse
			   // 
			   this->btnBrowse->Location = System::Drawing::Point(29, 85);
			   this->btnBrowse->Name = L"btnBrowse";
			   this->btnBrowse->Size = System::Drawing::Size(75, 23);
			   this->btnBrowse->TabIndex = 4;
			   this->btnBrowse->Text = L"Add";
			   this->btnBrowse->UseVisualStyleBackColor = true;
			   this->btnBrowse->Click += gcnew System::EventHandler(this, &MyForm1::btnBrowse_Click);
			   // 
			   // txtDest
			   // 
			   this->txtDest->Location = System::Drawing::Point(138, 243);
			   this->txtDest->Name = L"txtDest";
			   this->txtDest->ReadOnly = true;
			   this->txtDest->Size = System::Drawing::Size(118, 22);
			   this->txtDest->TabIndex = 3;
			   // 
			   // txtSource
			   // 
			   this->txtSource->Location = System::Drawing::Point(102, 36);
			   this->txtSource->Name = L"txtSource";
			   this->txtSource->ReadOnly = true;
			   this->txtSource->Size = System::Drawing::Size(154, 22);
			   this->txtSource->TabIndex = 2;
			   // 
			   // lblDestinationFolder
			   // 
			   this->lblDestinationFolder->AutoSize = true;
			   this->lblDestinationFolder->Location = System::Drawing::Point(13, 249);
			   this->lblDestinationFolder->Name = L"lblDestinationFolder";
			   this->lblDestinationFolder->Size = System::Drawing::Size(119, 16);
			   this->lblDestinationFolder->TabIndex = 1;
			   this->lblDestinationFolder->Text = L"Destination Folder:";
			   // 
			   // lblSourcePath
			   // 
			   this->lblSourcePath->AutoSize = true;
			   this->lblSourcePath->Location = System::Drawing::Point(13, 36);
			   this->lblSourcePath->Name = L"lblSourcePath";
			   this->lblSourcePath->Size = System::Drawing::Size(83, 16);
			   this->lblSourcePath->TabIndex = 0;
			   this->lblSourcePath->Text = L"Source Path:";
			   // 
			   // lvMetrics
			   // 
			   this->lvMetrics->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom)
				   | System::Windows::Forms::AnchorStyles::Right));
			   this->lvMetrics->BackColor = System::Drawing::SystemColors::InactiveCaption;
			   this->lvMetrics->Columns->AddRange(gcnew cli::array< System::Windows::Forms::ColumnHeader^  >(3) {
				   this->Metric, this->Description,
					   this->Value
			   });
			   this->lvMetrics->FullRowSelect = true;
			   this->lvMetrics->HeaderStyle = System::Windows::Forms::ColumnHeaderStyle::Nonclickable;
			   this->lvMetrics->HideSelection = false;
			   this->lvMetrics->Location = System::Drawing::Point(809, 136);
			   this->lvMetrics->Name = L"lvMetrics";
			   this->lvMetrics->Size = System::Drawing::Size(297, 286);
			   this->lvMetrics->TabIndex = 12;
			   this->lvMetrics->UseCompatibleStateImageBehavior = false;
			   this->lvMetrics->View = System::Windows::Forms::View::Details;
			   // 
			   // Metric
			   // 
			   this->Metric->Text = L"Metric";
			   // 
			   // Description
			   // 
			   this->Description->Text = L"Description";
			   // 
			   // Value
			   // 
			   this->Value->Text = L"Value";
			   // 
			   // lblProFeatures
			   // 
			   this->lblProFeatures->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Bottom | System::Windows::Forms::AnchorStyles::Right));
			   this->lblProFeatures->AutoSize = true;
			   this->lblProFeatures->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 13.8F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				   static_cast<System::Byte>(0)));
			   this->lblProFeatures->Location = System::Drawing::Point(804, 444);
			   this->lblProFeatures->Name = L"lblProFeatures";
			   this->lblProFeatures->Size = System::Drawing::Size(152, 29);
			   this->lblProFeatures->TabIndex = 13;
			   this->lblProFeatures->Text = L"Pro Features";
			   // 
			   // lblLabelmultiline
			   // 
			   this->lblLabelmultiline->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Bottom | System::Windows::Forms::AnchorStyles::Right));
			   this->lblLabelmultiline->AutoSize = true;
			   this->lblLabelmultiline->Location = System::Drawing::Point(812, 486);
			   this->lblLabelmultiline->Name = L"lblLabelmultiline";
			   this->lblLabelmultiline->Size = System::Drawing::Size(205, 16);
			   this->lblLabelmultiline->TabIndex = 14;
			   this->lblLabelmultiline->Text = L"Support: .doc, .txt, .exe, .png, .mp4";
			   // 
			   // lblMemoryclean
			   // 
			   this->lblMemoryclean->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Bottom | System::Windows::Forms::AnchorStyles::Right));
			   this->lblMemoryclean->AutoSize = true;
			   this->lblMemoryclean->Location = System::Drawing::Point(845, 517);
			   this->lblMemoryclean->Name = L"lblMemoryclean";
			   this->lblMemoryclean->Size = System::Drawing::Size(94, 16);
			   this->lblMemoryclean->TabIndex = 15;
			   this->lblMemoryclean->Text = L"Memory Clean";
			   // 
			   // pictureBox1
			   // 
			   this->pictureBox1->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Bottom | System::Windows::Forms::AnchorStyles::Right));
			   this->pictureBox1->BackColor = System::Drawing::Color::LimeGreen;
			   this->pictureBox1->Location = System::Drawing::Point(815, 513);
			   this->pictureBox1->Name = L"pictureBox1";
			   this->pictureBox1->Size = System::Drawing::Size(20, 20);
			   this->pictureBox1->TabIndex = 16;
			   this->pictureBox1->TabStop = false;
			   // 
			   // pnlLogContainer
			   // 
			   this->pnlLogContainer->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Bottom | System::Windows::Forms::AnchorStyles::Left)
				   | System::Windows::Forms::AnchorStyles::Right));
			   this->pnlLogContainer->Location = System::Drawing::Point(322, 562);
			   this->pnlLogContainer->Name = L"pnlLogContainer";
			   this->pnlLogContainer->Size = System::Drawing::Size(434, 50);
			   this->pnlLogContainer->TabIndex = 17;
			   // 
			   // MyForm1
			   // 
			   this->AutoScaleDimensions = System::Drawing::SizeF(8, 16);
			   this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			   this->ClientSize = System::Drawing::Size(1132, 722);
			   this->Controls->Add(this->pnlLogContainer);
			   this->Controls->Add(this->pictureBox1);
			   this->Controls->Add(this->lblMemoryclean);
			   this->Controls->Add(this->lblLabelmultiline);
			   this->Controls->Add(this->lblProFeatures);
			   this->Controls->Add(this->lvMetrics);
			   this->Controls->Add(this->groupboxFileOperations);
			   this->Controls->Add(this->btnDecompress);
			   this->Controls->Add(this->btnCompress);
			   this->Controls->Add(this->pnlGraphic);
			   this->Controls->Add(this->pnlTop);
			   this->Controls->Add(this->lblPerformanceMetrics);
			   this->Controls->Add(this->lblAlgorithmVisualization);
			   this->Controls->Add(this->lblFileManagement);
			   this->DoubleBuffered = true;
			   this->Name = L"MyForm1";
			   this->StartPosition = System::Windows::Forms::FormStartPosition::CenterScreen;
			   this->Text = L"MyForm1";
			   this->WindowState = System::Windows::Forms::FormWindowState::Maximized;
			   this->pnlTop->ResumeLayout(false);
			   this->pnlTop->PerformLayout();
			   this->groupboxFileOperations->ResumeLayout(false);
			   this->groupboxFileOperations->PerformLayout();
			   (cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->picFileIcon))->EndInit();
			   (cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->pictureBox1))->EndInit();
			   this->ResumeLayout(false);
			   this->PerformLayout();

		   }
#pragma endregion

	void Form_Resize(Object^ sender, EventArgs^ e)
	{
	   if (lvMetrics->Columns->Count == 3)
	   {
	   int w = lvMetrics->ClientSize.Width;
		   lvMetrics->Columns[0]->Width = w * 20 / 100;
		   lvMetrics->Columns[1]->Width = w * 55 / 100;
		   lvMetrics->Columns[2]->Width = w * 25 / 100;
	   }
	}
	private: System::Void pnlGraphic_MouseWheel(System::Object^ sender, System::Windows::Forms::MouseEventArgs^ e)
	{
		// Kiểm tra nếu phím Ctrl đang được nhấn (giống các phần mềm đồ họa)
		if (Control::ModifierKeys == Keys::Control)
		{
			if (e->Delta > 0) // Cuộn lên
				zoomLevel += zoomStep;
			else if (zoomLevel > 0.2f) // Cuộn xuống (giới hạn tối thiểu 20%)
				zoomLevel -= zoomStep;

			// Gọi hàm zoom trong visualizer (bạn sẽ cần thêm hàm này vào lớp Visualize)
			if (visualizer != nullptr) {
				visualizer->ApplyZoom(zoomLevel);
			}
		}
	}
	private: System::Void btnBrowse_Click(System::Object^ sender, System::EventArgs^ e) {
		OpenFileDialog^ ofd = gcnew OpenFileDialog();
		ofd->Filter = "All Files (*.*)|*.*|Huffman Archives (*.huf)|*.huf";

		if (ofd->ShowDialog() == System::Windows::Forms::DialogResult::OK) {
			String^ filePath = ofd->FileName;
			txtSource->Text = filePath;

			// 1. Tự động gợi ý thư mục đích (Cùng thư mục với file nguồn)
			txtDest->Text = System::IO::Path::GetDirectoryName(filePath);

			// 2. Lấy thông tin file và hiển thị lên các Label bạn đã đặt tên
			System::IO::FileInfo^ fileInfo = gcnew System::IO::FileInfo(filePath);

			lblFileName->Text = "Name: " + fileInfo->Name;

			String^ ext = fileInfo->Extension->ToUpper();
			lblFormat->Text = "Format: " + ext->Replace(".", "");

			// Tính toán dung lượng (Hiển thị KB nếu file nhỏ, MB nếu file lớn)
			double sizeInBytes = (double)fileInfo->Length;
			if (sizeInBytes < 1024 * 1024) {
				lblSize->Text = "Size: " + (sizeInBytes / 1024).ToString("F2") + " KB";
			}
			else {
				lblSize->Text = "Size: " + (sizeInBytes / (1024 * 1024)).ToString("F2") + " MB";
			}

			// 3. Hiển thị Icon tương ứng vào picFileIcon
			// Lưu ý: Bạn cần có file ảnh .png trong thư mục debug/bin của project
			try {
				if (ext == ".PDF") {
					picFileIcon->Image = Image::FromFile("pdf_icon.png");
				}
				else if (ext == ".MP3" || ext == ".WAV") {
					picFileIcon->Image = Image::FromFile("music_icon.png");
				}
				else if (ext == ".HUF") {
					picFileIcon->Image = Image::FromFile("huf_icon.png"); // Icon riêng cho file nén
				}
				else {
					picFileIcon->Image = Image::FromFile("default_icon.png");
				}
				picFileIcon->SizeMode = PictureBoxSizeMode::Zoom; // Giúp ảnh không bị vỡ
			}
			catch (...) {
				// Nếu chưa có file ảnh icon thì bỏ qua không báo lỗi
			}

			// 4. Logic bật/tắt nút bấm (Giữ nguyên logic thông minh của bạn)
			if (filePath->EndsWith(".huf")) {
				btnCompress->Enabled = false;
				btnDecompress->Enabled = true;
			}
			else {
				btnCompress->Enabled = true;
				btnDecompress->Enabled = false;
			}

			// 5. Hiển thị mã Hex vào ô RichTextBox (nếu bạn có đặt tên là rtbHexPreview)
			try {
				System::IO::FileStream^ fs = gcnew System::IO::FileStream(filePath, System::IO::FileMode::Open, System::IO::FileAccess::Read);
				array<Byte>^ buffer = gcnew array<Byte>(16);
				int bytesRead = fs->Read(buffer, 0, 16);
				fs->Close();

				String^ hexStr = "";
				for (int i = 0; i < bytesRead; i++) {
					hexStr += buffer[i].ToString("X2") + " ";
				}
				// Thay 'rtbHexPreview' bằng tên chuẩn của ô RichTextBox của bạn
				// rtbHexPreview->Text = hexStr + "..."; 
			}
			catch (...) {}
		}
	}
	private: System::Void btnSelectFolder_Click(System::Object^ sender, System::EventArgs^ e) {
		FolderBrowserDialog^ fbd = gcnew FolderBrowserDialog();
		if (fbd->ShowDialog() == System::Windows::Forms::DialogResult::OK) {
			txtDest->Text = fbd->SelectedPath;
		}
	}
	private: System::Void btnCompress_Click(System::Object^ sender, System::EventArgs^ e) {
		if (String::IsNullOrEmpty(txtSource->Text)) return;
		// Chạy trên luồng riêng để UpdateStep và Sleep không làm đơ giao diện
		System::Threading::Tasks::Task::Run(gcnew Action(this, &MyForm1::PerformCompression));
	}

	private: System::Void btnDecompress_Click(System::Object^ sender, System::EventArgs^ e) {
		if (String::IsNullOrEmpty(txtSource->Text)) return;
		System::Threading::Tasks::Task::Run(gcnew Action(this, &MyForm1::PerformDecompression));
	}

};
}
#endif
