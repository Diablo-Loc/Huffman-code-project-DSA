#include "MyForm1.h"

using namespace System;
using namespace System::Windows::Forms;
using namespace HuffmanUI;

[STAThreadAttribute]
int main(array<System::String^>^ args)
{
    Application::EnableVisualStyles();
    Application::SetCompatibleTextRenderingDefault(false);

    Application::Run(gcnew HuffmanUI::MyForm1());
    return 0;
}
