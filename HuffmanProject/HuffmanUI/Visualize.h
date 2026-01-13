#pragma once
#include "HuffmanWrapper.h"
#include "Node.h"
#include <string>
#include <msclr/marshal_cppstd.h>
#include <algorithm>

namespace HuffmanUI {

    using namespace System;
    using namespace System::ComponentModel;
    using namespace System::Collections;
    using namespace System::Windows::Forms;
    using namespace System::Drawing;
    using namespace System::Drawing::Drawing2D;

    public ref class Visualize : public System::Windows::Forms::Form
    {
    public:
        Visualize(void)
        {
            InitializeComponent();
            huffmanRoot = nullptr;
            this->DoubleBuffered = true;

            this->pnlCanvas->MouseDown += gcnew MouseEventHandler(this, &Visualize::OnMouseDown);
            this->pnlCanvas->MouseMove += gcnew MouseEventHandler(this, &Visualize::OnMouseMove);
            this->pnlCanvas->MouseUp += gcnew MouseEventHandler(this, &Visualize::OnMouseUp);
            this->pnlCanvas->MouseWheel += gcnew MouseEventHandler(this, &Visualize::OnMouseWheel);
        }

        void ApplyZoom(float scale) {
            this->currentScale = scale;
            this->pnlCanvas->Invalidate();
        }

        void SetTreeAndDraw(Node* root) {
            if (this->huffmanRoot) DeleteTree(this->huffmanRoot);
            if (root) {
                this->huffmanRoot = root->cloneTree(root);
                offsetX = 0;
                offsetY = 0;
                currentScale = 0.6f; // Thu nhỏ hơn một chút để thấy toàn cảnh
            }
            this->pnlCanvas->Invalidate();
        }

    protected:
        ~Visualize() {
            if (_components) delete _components;
            if (huffmanRoot) DeleteTree(huffmanRoot);
        }

    private:
        Node* huffmanRoot;
        float currentScale = 1.0f;
        float offsetX = 0.0f;
        float offsetY = 0.0f;
        bool isDragging = false;
        Drawing::Point lastMousePos;
        int levelSpacing = 100; // Khoảng cách tầng rộng rãi

        void DeleteTree(Node* node) {
            if (!node) return;
            DeleteTree(node->left);
            DeleteTree(node->right);
            delete node;
        }

        // --- THUẬT TOÁN QUAN TRỌNG: ĐẾM LÁ ---
        int GetLeafCount(Node* node) {
            if (!node) return 0;
            if (node->isLeaf()) return 1;
            return GetLeafCount(node->left) + GetLeafCount(node->right);
        }

        int GetTreeHeight(Node* node) {
            if (!node) return 0;
            return 1 + System::Math::Max(GetTreeHeight(node->left), GetTreeHeight(node->right));
        }

        void DrawHuffmanNode(Graphics^ g, Node* node, int x, int y, float availableWidth, StringFormat^ sf, int r) {
            if (!node) return;

            if (node->left || node->right) {
                int leftLeaves = GetLeafCount(node->left);
                int rightLeaves = GetLeafCount(node->right);
                int totalLeaves = leftLeaves + rightLeaves;

                // Tính toán tọa độ X dựa trên tỉ lệ số lá của từng nhánh
                float leftAreaWidth = (availableWidth * leftLeaves) / totalLeaves;
                float rightAreaWidth = (availableWidth * rightLeaves) / totalLeaves;

                // Tọa độ X của các con là trung tâm của vùng diện tích được cấp
                int leftX = x - (availableWidth / 2) + (leftAreaWidth / 2);
                int rightX = x + (availableWidth / 2) - (rightAreaWidth / 2);

                Pen^ edgePen = gcnew Pen(Color::FromArgb(180, 0, 191, 255), 1.5f);

                // Vẽ đường nối (Bezier mượt mà)
                if (node->left) {
                    GraphicsPath^ p = gcnew GraphicsPath();
                    p->AddBezier(x, y, x, y + levelSpacing / 2, leftX, y + levelSpacing / 2, leftX, y + levelSpacing);
                    g->DrawPath(edgePen, p);
                    DrawHuffmanNode(g, node->left, leftX, y + levelSpacing, leftAreaWidth, sf, r);
                }
                if (node->right) {
                    GraphicsPath^ p = gcnew GraphicsPath();
                    p->AddBezier(x, y, x, y + levelSpacing / 2, rightX, y + levelSpacing / 2, rightX, y + levelSpacing);
                    g->DrawPath(edgePen, p);
                    DrawHuffmanNode(g, node->right, rightX, y + levelSpacing, rightAreaWidth, sf, r);
                }
            }

            // Vẽ Node (Hình khối và Chữ)
            System::Drawing::Rectangle rect(x - r, y - r, r * 2, r * 2);
            if (node->isLeaf()) {
                LinearGradientBrush^ br = gcnew LinearGradientBrush(rect, Color::MediumSpringGreen, Color::Teal, 45.0f);
                g->FillEllipse(br, rect);
                g->DrawEllipse(gcnew Pen(Color::White, 1.2f), rect);

                String^ ch = (node->ch == '\n') ? "\\n" : (node->ch == ' ') ? "SP" : gcnew String((wchar_t)node->ch, 1);
                Drawing::Font^ f = gcnew Drawing::Font("Segoe UI", (float)r / 2.2f, FontStyle::Bold);
                g->DrawString(ch + "\n" + node->freq, f, Brushes::White, rect, sf);
            }
            else {
                LinearGradientBrush^ br = gcnew LinearGradientBrush(rect, Color::DeepSkyBlue, Color::MidnightBlue, 45.0f);
                g->FillRectangle(br, rect);
                g->DrawRectangle(gcnew Pen(Color::Cyan, 1.0f), rect);

                Drawing::Font^ f = gcnew Drawing::Font("Segoe UI", (float)r / 2.8f, FontStyle::Regular);
                g->DrawString(node->freq.ToString(), f, Brushes::White, rect, sf);
            }
        }

    private: System::Void pnlCanvas_Paint(System::Object^ sender, System::Windows::Forms::PaintEventArgs^ e) {
        if (this->huffmanRoot == nullptr) return;
        Graphics^ g = e->Graphics;
        g->SmoothingMode = SmoothingMode::HighQuality;
        g->TextRenderingHint = Drawing::Text::TextRenderingHint::ClearTypeGridFit;

        // Vẽ nền Gradient tối
        System::Drawing::Rectangle fullRect(0, 0, pnlCanvas->Width, pnlCanvas->Height);
        LinearGradientBrush^ bgBr = gcnew LinearGradientBrush(fullRect, Color::FromArgb(15, 15, 25), Color::Black, 90.0f);
        g->FillRectangle(bgBr, fullRect);

        g->TranslateTransform(offsetX + (float)pnlCanvas->Width / 2.0f, 60.0f + offsetY);
        g->ScaleTransform(currentScale, currentScale);

        int height = GetTreeHeight(huffmanRoot);
        int nodeR = (height > 10) ? 18 : 25;

        // Chiều rộng tổng thể ban đầu (Tự động scale theo số lượng nút lá)
        float totalTreeWidth = GetLeafCount(huffmanRoot) * (nodeR * 3.5f);

        StringFormat^ sf = gcnew StringFormat();
        sf->Alignment = StringAlignment::Center;
        sf->LineAlignment = StringAlignment::Center;

        DrawHuffmanNode(g, huffmanRoot, 0, 0, totalTreeWidth, sf, nodeR);
    }

           // --- Mouse Events (Pan & Zoom) ---
           void OnMouseWheel(Object^ sender, MouseEventArgs^ e) {
               if (e->Delta > 0) currentScale *= 1.1f;
               else if (currentScale > 0.05f) currentScale /= 1.1f;
               pnlCanvas->Invalidate();
           }
           void OnMouseDown(Object^ sender, MouseEventArgs^ e) {
               if (e->Button == System::Windows::Forms::MouseButtons::Left) { isDragging = true; lastMousePos = e->Location; }
           }
           void OnMouseMove(Object^ sender, MouseEventArgs^ e) {
               if (isDragging) {
                   offsetX += (e->X - lastMousePos.X);
                   offsetY += (e->Y - lastMousePos.Y);
                   lastMousePos = e->Location;
                   pnlCanvas->Invalidate();
               }
           }
           void OnMouseUp(Object^ sender, MouseEventArgs^ e) { isDragging = false; }

    private:
        System::Windows::Forms::Panel^ pnlCanvas;
        System::ComponentModel::Container^ _components;

        void InitializeComponent(void) {
            this->pnlCanvas = (gcnew System::Windows::Forms::Panel());
            this->SuspendLayout();
            this->pnlCanvas->Dock = System::Windows::Forms::DockStyle::Fill;
            this->pnlCanvas->BackColor = Color::Black;
            this->pnlCanvas->Name = L"pnlCanvas";
            this->pnlCanvas->Paint += gcnew System::Windows::Forms::PaintEventHandler(this, &Visualize::pnlCanvas_Paint);
            this->ClientSize = System::Drawing::Size(1000, 700);
            this->Controls->Add(this->pnlCanvas);
            this->Name = L"Visualize";
            this->Text = L"Huffman Tree Pro Visualizer";
            this->ResumeLayout(false);
        }
    };
}