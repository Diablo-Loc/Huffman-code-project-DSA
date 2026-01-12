#pragma once
#include "HuffmanWrapper.h"
#include "Node.h"
#include "MyForm1.h"
#include <string>
#include <msclr/marshal_cppstd.h> // Thư viện để chuyển đổi string

namespace HuffmanUI {

    using namespace System;
    using namespace System::ComponentModel;
    using namespace System::Collections;
    using namespace System::Windows::Forms;
    using namespace System::Drawing;

    public ref class Visualize : public System::Windows::Forms::Form
    {
    public:
        Visualize(void)
        {
            InitializeComponent();
            huffmanRoot = nullptr;
        }
        void SetTreeAndDraw(Node* root) {
            if (this->huffmanRoot) {
                DeleteTree(this->huffmanRoot);
            }
            if (root) {
                // Clone để an toàn bộ nhớ
                this->huffmanRoot = root->cloneTree(root);
            }
            this->pnlCanvas->Invalidate(); // Yêu cầu vẽ lại
        }
    protected:
        ~Visualize()
        {
            if (components) delete components;
            // Lưu ý: Việc giải phóng huffmanRoot nên để HuffmanTree đảm nhận
        }

    private:
        Node* huffmanRoot;
        int nodeRadius = 25;      // Tăng kích thước để dễ nhìn text
        int levelSpacing = 70;    // Khoảng cách giữa các tầng
        void DeleteTree(Node* node) {
            if (!node) return;
            DeleteTree(node->left);
            DeleteTree(node->right);
            delete node;
        }
        // Hàm vẽ Node đệ quy
        void DrawHuffmanNode(Graphics^ g, Node* node, int x, int y, int xOffset) {
            if (!node) return;

            Pen^ linePen = gcnew Pen(Color::DeepSkyBlue, 2);
            if (node->left) {
                g->DrawLine(linePen, x, y, x - xOffset, y + levelSpacing);
                DrawHuffmanNode(g, node->left, x - xOffset, y + levelSpacing, xOffset / 1.8);
            }
            if (node->right) {
                g->DrawLine(linePen, x, y, x + xOffset, y + levelSpacing);
                DrawHuffmanNode(g, node->right, x + xOffset, y + levelSpacing, xOffset / 1.8);
            }

            Drawing::Rectangle rect(x - nodeRadius, y - nodeRadius, nodeRadius * 2, nodeRadius * 2);
            Drawing::Font^ font = gcnew Drawing::Font("Arial", 8, FontStyle::Bold);

            if (node->isLeaf()) {
                g->FillEllipse(Brushes::Black, rect);
                g->DrawEllipse(gcnew Pen(Color::Cyan, 2), rect);

                // --- ĐÃ SỬA LỖI Ở ĐÂY ---
                String^ charStr;
                if (node->ch == '\n') charStr = "\\n";
                else if (node->ch == ' ') charStr = "Space";
                else charStr = gcnew String((char)node->ch, 1);

                String^ info = "'" + charStr + "'\nf:" + node->freq;
                g->DrawString(info, font, Brushes::White, x - 18, y - 10);
            }
            else {
                g->FillRectangle(Brushes::Black, rect);
                g->DrawRectangle(gcnew Pen(Color::DeepSkyBlue, 2), rect);
                g->DrawString(node->freq.ToString(), font, Brushes::Aqua, x - 10, y - 7);
            }
        }

    private: System::Void pnlCanvas_Paint(System::Object^ sender, System::Windows::Forms::PaintEventArgs^ e) {
        if (this->huffmanRoot != nullptr) {
            e->Graphics->SmoothingMode = Drawing2D::SmoothingMode::AntiAlias;
            e->Graphics->Clear(Color::Black); // Xóa nền cũ
            // Vẽ từ giữa Panel, tầng 1 cách đỉnh 40px
            DrawHuffmanNode(e->Graphics, huffmanRoot, pnlCanvas->Width / 2, 40, pnlCanvas->Width / 4);
        }
    }
  
    /*private: System::Void btnBuild_Click(System::Object^ sender, System::EventArgs^ e) {
        // TEST NHANH: Tạo cây giả lập thay vì gọi wrapper.compressFile
        // Node gốc (freq: 22)
        this->huffmanRoot = new Node(0, 44);

        // Nhánh trái (Node lá 'c', freq: 12)
        this->huffmanRoot->left = new Node('c', 12);

        // Nhánh phải (Node trung gian, freq: 10)
        this->huffmanRoot->right = new Node(0, 10);
        this->huffmanRoot->right->left = new Node('s', 5);
        this->huffmanRoot->right->right = new Node('a', 5);

        // Yêu cầu vẽ lại
        this->pnlCanvas->Invalidate();
    }*/
    private: System::Void btnBuild_Click(System::Object^ sender, System::EventArgs^ e) {
        OpenFileDialog^ ofd = gcnew OpenFileDialog();
        ofd->Filter = "Text Files (*.txt)|*.txt|All Files (*.*)|*.*";

        if (ofd->ShowDialog() == System::Windows::Forms::DialogResult::OK) {
            std::string inputPath = msclr::interop::marshal_as<std::string>(ofd->FileName);
            std::string outputPath = inputPath + ".huf";

            HuffmanWrapper wrapper;
            CompressionMetrics metrics;
            metrics.root = nullptr; // Khởi tạo an toàn

            if (wrapper.compressFile(inputPath, outputPath, metrics)) {
                // 1. Dọn dẹp cây hiện tại trên giao diện nếu có
                if (this->huffmanRoot != nullptr) {
                    DeleteTree(this->huffmanRoot);
                    this->huffmanRoot = nullptr;
                }

                // 2. Clone cây từ metrics sang UI (Lúc này metrics.root chắc chắn còn sống)
                if (metrics.root != nullptr) {
                    this->huffmanRoot = metrics.root->cloneTree(metrics.root);
                }

                // 3. Yêu cầu vẽ lại
                this->pnlCanvas->Invalidate();
                MessageBox::Show("Build thành công!");
            }
        }
    }
    private:
        System::Windows::Forms::Panel^ pnlCanvas;
        System::Windows::Forms::Button^ btnBuild;
        System::ComponentModel::Container^ components;

        void InitializeComponent(void)
        {
            this->pnlCanvas = (gcnew System::Windows::Forms::Panel());
            this->btnBuild = (gcnew System::Windows::Forms::Button());
            this->pnlCanvas->SuspendLayout();
            this->SuspendLayout();
            // 
            // pnlCanvas
            // 
            this->pnlCanvas->BackColor = System::Drawing::Color::Black;
            this->pnlCanvas->Controls->Add(this->btnBuild);
            this->pnlCanvas->Dock = System::Windows::Forms::DockStyle::Fill;
            this->pnlCanvas->Location = System::Drawing::Point(0, 0);
            this->pnlCanvas->Name = L"pnlCanvas";
            this->pnlCanvas->Size = System::Drawing::Size(825, 627);
            this->pnlCanvas->TabIndex = 1;
            this->pnlCanvas->Paint += gcnew System::Windows::Forms::PaintEventHandler(this, &Visualize::pnlCanvas_Paint);
            // 
            // btnBuild
            // 
            this->btnBuild->Location = System::Drawing::Point(0, 3);
            this->btnBuild->Name = L"btnBuild";
            this->btnBuild->Size = System::Drawing::Size(120, 30);
            this->btnBuild->TabIndex = 0;
            this->btnBuild->Text = L"Build Huffman Tree";
            this->btnBuild->Visible = false;
            this->btnBuild->Click += gcnew System::EventHandler(this, &Visualize::btnBuild_Click);
            // 
            // Visualize
            // 
            this->ClientSize = System::Drawing::Size(825, 627);
            this->Controls->Add(this->pnlCanvas);
            this->Name = L"Visualize";
            this->Text = L"Huffman Tree Visualizer";
            this->pnlCanvas->ResumeLayout(false);
            this->ResumeLayout(false);

        }
    };
}