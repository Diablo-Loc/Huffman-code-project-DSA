#include "MyForm1.h"

using namespace System;
using namespace System::Windows::Forms;
using namespace HuffmanUI;

[STAThread]
int main()
{
    Application::EnableVisualStyles();
    Application::SetCompatibleTextRenderingDefault(false);
    Application::Run(gcnew MyForm1());
    return 0;
}

