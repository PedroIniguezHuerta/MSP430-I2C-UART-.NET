// MSP430Controller.cpp : main project file.

#include "stdafx.h"
#include "Form1.h"

using namespace MSP430Controller;

[STAThreadAttribute]
int main(array<System::String ^> ^args)
{
	// Enabling Windows XP visual effects before any controls are created
	Application::EnableVisualStyles();
	Application::SetCompatibleTextRenderingDefault(false); 

	Form1 ^f = gcnew Form1(args);
	// Create the main window and run it
	Application::Run(f);

	return 0;
}
