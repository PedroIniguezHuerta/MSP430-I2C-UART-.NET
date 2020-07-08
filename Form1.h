#pragma once


namespace MSP430Controller {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;
	using namespace System::IO;
	using namespace System::IO::Ports;
	using namespace System::Threading;
	using namespace System::Data;
	using namespace System::Text;

	/// <summary>
	/// Summary for Form1
	///
	/// WARNING: If you change the name of this class, you will need to change the
	///          'Resource File Name' property for the managed resource compiler tool
	///          associated with all .resx files this class depends on.  Otherwise,
	///          the designers will not be able to interact properly with localized
	///          resources associated with this form.
	/// </summary>
	public ref class Form1 : public System::Windows::Forms::Form
	{
	public:
		Form1(array<System::String ^> ^args)
		{
			InitializeComponent();
			//
			//TODO: Add the constructor code here
			//
			if(args->Length > 3)
			{
				mRunningWithParameters = true;

				array<Byte>^bytes = gcnew array<Byte>(4);
				bytes[0] = Convert::ToByte(args[0],10);
				bytes[1] = Convert::ToByte(args[1],10);
				bytes[2] = Convert::ToByte(args[2],10);
				bytes[3] = Convert::ToByte(args[3],10);

				Encoding^ ascii = Encoding::ASCII;


				if(args->Length > 4 )
					textBoxCommand->Text = "*" + ascii->GetString( bytes,0,4);

				if(args->Length > 5)
					textBoxPort->Text = args[4];

				if(args->Length > 6)
					textBoxBaudRate->Text = args[5];

				buttonPort_Click(nullptr,nullptr);

				Thread::Sleep(2000);  // sleep in order to allow receiving any pending data

				textBoxCommand_KeyUp(nullptr,nullptr);
			}
			else
				mRunningWithParameters = false;
		}

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~Form1()
		{
			if (components)
			{
				delete components;
			}
		}
	private: System::Windows::Forms::GroupBox^  groupBoxInput;
	protected: 

	private: System::Windows::Forms::RichTextBox^  richTextBoxInput;
	protected: 

	private: System::IO::Ports::SerialPort^  serialPort1;
	private: System::Windows::Forms::Timer^  timer1;
	private: System::Windows::Forms::GroupBox^  groupBoxCommands;
	private: System::Windows::Forms::Label^  label1;
	private: System::Windows::Forms::TextBox^  textBoxCommand;
	private: System::Windows::Forms::Button^  buttonAbout;




	private: System::Windows::Forms::TextBox^  textBoxPort;
	private: System::Windows::Forms::Label^  label2;
	private: System::Windows::Forms::Button^  buttonPort;
	private: System::Windows::Forms::GroupBox^  groupBox1;
	private: System::Windows::Forms::RichTextBox^  richTextBoxDebugOutput;
	private: System::Windows::Forms::TextBox^  textBoxBaudRate;

	private: System::Windows::Forms::Label^  label3;
	private: System::Windows::Forms::Button^  buttonClose;
	private: System::Windows::Forms::Button^  buttonClearData;
	private: System::Windows::Forms::Button^  buttonClearDebug;



	private: System::ComponentModel::IContainer^  components;

	private:
		/// <summary>
		/// Required designer variable.
		bool mRunningWithParameters;
		/// </summary>


#pragma region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		void InitializeComponent(void)
		{
			this->components = (gcnew System::ComponentModel::Container());
			this->groupBoxInput = (gcnew System::Windows::Forms::GroupBox());
			this->richTextBoxInput = (gcnew System::Windows::Forms::RichTextBox());
			this->serialPort1 = (gcnew System::IO::Ports::SerialPort(this->components));
			this->timer1 = (gcnew System::Windows::Forms::Timer(this->components));
			this->groupBoxCommands = (gcnew System::Windows::Forms::GroupBox());
			this->textBoxBaudRate = (gcnew System::Windows::Forms::TextBox());
			this->label3 = (gcnew System::Windows::Forms::Label());
			this->buttonPort = (gcnew System::Windows::Forms::Button());
			this->textBoxPort = (gcnew System::Windows::Forms::TextBox());
			this->label2 = (gcnew System::Windows::Forms::Label());
			this->label1 = (gcnew System::Windows::Forms::Label());
			this->textBoxCommand = (gcnew System::Windows::Forms::TextBox());
			this->buttonAbout = (gcnew System::Windows::Forms::Button());
			this->groupBox1 = (gcnew System::Windows::Forms::GroupBox());
			this->richTextBoxDebugOutput = (gcnew System::Windows::Forms::RichTextBox());
			this->buttonClose = (gcnew System::Windows::Forms::Button());
			this->buttonClearData = (gcnew System::Windows::Forms::Button());
			this->buttonClearDebug = (gcnew System::Windows::Forms::Button());
			this->groupBoxInput->SuspendLayout();
			this->groupBoxCommands->SuspendLayout();
			this->groupBox1->SuspendLayout();
			this->SuspendLayout();
			// 
			// groupBoxInput
			// 
			this->groupBoxInput->AutoSize = true;
			this->groupBoxInput->Controls->Add(this->richTextBoxInput);
			this->groupBoxInput->Location = System::Drawing::Point(12, 103);
			this->groupBoxInput->Name = L"groupBoxInput";
			this->groupBoxInput->Size = System::Drawing::Size(545, 293);
			this->groupBoxInput->TabIndex = 0;
			this->groupBoxInput->TabStop = false;
			this->groupBoxInput->Text = L"Data From Controller";
			// 
			// richTextBoxInput
			// 
			this->richTextBoxInput->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom) 
				| System::Windows::Forms::AnchorStyles::Left) 
				| System::Windows::Forms::AnchorStyles::Right));
			this->richTextBoxInput->Location = System::Drawing::Point(6, 15);
			this->richTextBoxInput->Name = L"richTextBoxInput";
			this->richTextBoxInput->ReadOnly = true;
			this->richTextBoxInput->Size = System::Drawing::Size(532, 272);
			this->richTextBoxInput->TabIndex = 0;
			this->richTextBoxInput->Text = L"";
			// 
			// timer1
			// 
			this->timer1->Enabled = true;
			this->timer1->Interval = 1000;
			this->timer1->Tick += gcnew System::EventHandler(this, &Form1::timer1_Tick);
			// 
			// groupBoxCommands
			// 
			this->groupBoxCommands->AutoSize = true;
			this->groupBoxCommands->Controls->Add(this->textBoxBaudRate);
			this->groupBoxCommands->Controls->Add(this->label3);
			this->groupBoxCommands->Controls->Add(this->buttonPort);
			this->groupBoxCommands->Controls->Add(this->textBoxPort);
			this->groupBoxCommands->Controls->Add(this->label2);
			this->groupBoxCommands->Controls->Add(this->label1);
			this->groupBoxCommands->Controls->Add(this->textBoxCommand);
			this->groupBoxCommands->Location = System::Drawing::Point(12, 12);
			this->groupBoxCommands->Name = L"groupBoxCommands";
			this->groupBoxCommands->Size = System::Drawing::Size(545, 85);
			this->groupBoxCommands->TabIndex = 1;
			this->groupBoxCommands->TabStop = false;
			this->groupBoxCommands->Text = L"Commands";
			// 
			// textBoxBaudRate
			// 
			this->textBoxBaudRate->Location = System::Drawing::Point(283, 13);
			this->textBoxBaudRate->Name = L"textBoxBaudRate";
			this->textBoxBaudRate->Size = System::Drawing::Size(133, 20);
			this->textBoxBaudRate->TabIndex = 5;
			this->textBoxBaudRate->Text = L"9600";
			// 
			// label3
			// 
			this->label3->AutoSize = true;
			this->label3->Location = System::Drawing::Point(216, 16);
			this->label3->Name = L"label3";
			this->label3->Size = System::Drawing::Size(61, 13);
			this->label3->TabIndex = 4;
			this->label3->Text = L"Baud Rate:";
			// 
			// buttonPort
			// 
			this->buttonPort->Location = System::Drawing::Point(463, 13);
			this->buttonPort->Name = L"buttonPort";
			this->buttonPort->Size = System::Drawing::Size(75, 23);
			this->buttonPort->TabIndex = 3;
			this->buttonPort->Text = L"&Open";
			this->buttonPort->UseVisualStyleBackColor = true;
			this->buttonPort->Click += gcnew System::EventHandler(this, &Form1::buttonPort_Click);
			// 
			// textBoxPort
			// 
			this->textBoxPort->Location = System::Drawing::Point(67, 13);
			this->textBoxPort->Name = L"textBoxPort";
			this->textBoxPort->Size = System::Drawing::Size(133, 20);
			this->textBoxPort->TabIndex = 3;
			this->textBoxPort->Text = L"COM38";
			// 
			// label2
			// 
			this->label2->AutoSize = true;
			this->label2->Location = System::Drawing::Point(6, 16);
			this->label2->Name = L"label2";
			this->label2->Size = System::Drawing::Size(29, 13);
			this->label2->TabIndex = 2;
			this->label2->Text = L"Port:";
			// 
			// label1
			// 
			this->label1->AutoSize = true;
			this->label1->Location = System::Drawing::Point(6, 49);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(54, 13);
			this->label1->TabIndex = 1;
			this->label1->Text = L"Command";
			// 
			// textBoxCommand
			// 
			this->textBoxCommand->Location = System::Drawing::Point(68, 46);
			this->textBoxCommand->Name = L"textBoxCommand";
			this->textBoxCommand->ReadOnly = true;
			this->textBoxCommand->Size = System::Drawing::Size(470, 20);
			this->textBoxCommand->TabIndex = 0;
			this->textBoxCommand->KeyUp += gcnew System::Windows::Forms::KeyEventHandler(this, &Form1::textBoxCommand_KeyUp);
			// 
			// buttonAbout
			// 
			this->buttonAbout->Location = System::Drawing::Point(176, 542);
			this->buttonAbout->Name = L"buttonAbout";
			this->buttonAbout->Size = System::Drawing::Size(75, 23);
			this->buttonAbout->TabIndex = 2;
			this->buttonAbout->Text = L"&About";
			this->buttonAbout->UseVisualStyleBackColor = true;
			this->buttonAbout->Click += gcnew System::EventHandler(this, &Form1::buttonAbout_Click);
			// 
			// groupBox1
			// 
			this->groupBox1->AutoSize = true;
			this->groupBox1->Controls->Add(this->richTextBoxDebugOutput);
			this->groupBox1->Location = System::Drawing::Point(12, 402);
			this->groupBox1->Name = L"groupBox1";
			this->groupBox1->Size = System::Drawing::Size(545, 134);
			this->groupBox1->TabIndex = 1;
			this->groupBox1->TabStop = false;
			this->groupBox1->Text = L"Debug Output";
			// 
			// richTextBoxDebugOutput
			// 
			this->richTextBoxDebugOutput->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom) 
				| System::Windows::Forms::AnchorStyles::Left) 
				| System::Windows::Forms::AnchorStyles::Right));
			this->richTextBoxDebugOutput->Location = System::Drawing::Point(6, 15);
			this->richTextBoxDebugOutput->Name = L"richTextBoxDebugOutput";
			this->richTextBoxDebugOutput->ReadOnly = true;
			this->richTextBoxDebugOutput->Size = System::Drawing::Size(532, 113);
			this->richTextBoxDebugOutput->TabIndex = 0;
			this->richTextBoxDebugOutput->Text = L"";
			// 
			// buttonClose
			// 
			this->buttonClose->Location = System::Drawing::Point(257, 542);
			this->buttonClose->Name = L"buttonClose";
			this->buttonClose->Size = System::Drawing::Size(75, 23);
			this->buttonClose->TabIndex = 3;
			this->buttonClose->Text = L"&Close";
			this->buttonClose->UseVisualStyleBackColor = true;
			this->buttonClose->Click += gcnew System::EventHandler(this, &Form1::buttonClose_Click);
			// 
			// buttonClearData
			// 
			this->buttonClearData->Location = System::Drawing::Point(14, 542);
			this->buttonClearData->Name = L"buttonClearData";
			this->buttonClearData->Size = System::Drawing::Size(75, 23);
			this->buttonClearData->TabIndex = 4;
			this->buttonClearData->Text = L"Clea&r Data";
			this->buttonClearData->UseVisualStyleBackColor = true;
			this->buttonClearData->Click += gcnew System::EventHandler(this, &Form1::buttonClearData_Click);
			// 
			// buttonClearDebug
			// 
			this->buttonClearDebug->Location = System::Drawing::Point(95, 542);
			this->buttonClearDebug->Name = L"buttonClearDebug";
			this->buttonClearDebug->Size = System::Drawing::Size(75, 23);
			this->buttonClearDebug->TabIndex = 5;
			this->buttonClearDebug->Text = L"Clear &Debug";
			this->buttonClearDebug->UseVisualStyleBackColor = true;
			this->buttonClearDebug->Click += gcnew System::EventHandler(this, &Form1::buttonClearDebug_Click);
			// 
			// Form1
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(570, 576);
			this->Controls->Add(this->buttonClearDebug);
			this->Controls->Add(this->buttonClearData);
			this->Controls->Add(this->buttonClose);
			this->Controls->Add(this->groupBox1);
			this->Controls->Add(this->buttonAbout);
			this->Controls->Add(this->groupBoxCommands);
			this->Controls->Add(this->groupBoxInput);
			this->Name = L"Form1";
			this->Text = L"MSP430 Controller";
			this->groupBoxInput->ResumeLayout(false);
			this->groupBoxCommands->ResumeLayout(false);
			this->groupBoxCommands->PerformLayout();
			this->groupBox1->ResumeLayout(false);
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion

private: System::Void buttonAbout_Click(System::Object^  sender, System::EventArgs^  e)
{
	richTextBoxDebugOutput->Text = richTextBoxDebugOutput->Text + "MSP430 Controller Tester.\n";
	richTextBoxDebugOutput->Text = richTextBoxDebugOutput->Text + "Author:\n";
	richTextBoxDebugOutput->Text = richTextBoxDebugOutput->Text + "\tPedro Iniguez Huerta\n";
}

private: System::Void timer1_Tick(System::Object^  sender, System::EventArgs^  e)
{
	if(buttonPort->Text == "&Finish")
	{
		const int BYTES_TO_READ = 100;

		serialPort1->ReadTimeout=100;
		serialPort1->DtrEnable=false;
		array<Byte>^bytes = gcnew array<Byte>(BYTES_TO_READ);  //A17C from 500 to MAX_LINE_BUFFER
		int bytes_read=0;

		try
		{
			bytes_read = serialPort1->Read(bytes,0,BYTES_TO_READ);

			Encoding^ ascii = Encoding::ASCII;

			String ^response = "" + ascii->GetString( bytes,0,bytes_read);

			richTextBoxInput->Text = richTextBoxInput->Text + response;
			if(response->Contains("Master"))
				this->Text="MSP430 Controller (Master Controller)";
			else if(response->Contains("Slave"))
				this->Text="MSP430 Controller (Slave Controller)";
//			else
//				this->Text="MSP430 Controller (Device in port not recognized)";

			if(mRunningWithParameters && response == "O")
			{
				buttonPort_Click(nullptr,nullptr);
				Thread::Sleep(500);
				buttonClose_Click(nullptr,nullptr);
			}
		} catch (System::IO::IOException^ e)
		{
			displayError("error:"+e);
			return;
		}
		catch(System::TimeoutException^ e)
		{
			e = e;
			if(bytes_read == 0)
				return;
		}
	}
}

private: System::Void displayError(String ^errors)
{
	String ^text = richTextBoxDebugOutput->Text;

	richTextBoxDebugOutput->Text = text + errors + "\n";
}

private: System::Void buttonPort_Click(System::Object^  sender, System::EventArgs^  e)
{
	if(buttonPort->Text == "&Open")
	{
		try 
		{
			serialPort1->PortName = gcnew String(textBoxPort->Text);
			serialPort1->BaudRate=Convert::ToInt32(textBoxBaudRate->Text);
			serialPort1->DataBits=8;
			serialPort1->Parity=Parity::None;
			serialPort1->StopBits=StopBits::One;

			try
			{
				serialPort1->Open();
				buttonPort->Text = "&Finish";
				textBoxCommand->ReadOnly = false;

//				serialPort1->WriteTimeout=2000;
//				serialPort1->RtsEnable=true;
//				serialPort1->Write("!");
			} catch (System::UnauthorizedAccessException ^e)								
			{
				displayError("error:"+e);
				return;
			}
		} catch (System::IO::IOException^ e )
		{
			displayError("error:"+e);
			return;
		}
			catch (System::InvalidOperationException^ e )
		{
			displayError("error:"+e);
			return;
		}
	}
	else
	{
		buttonPort->Text = "&Open";
		textBoxCommand->ReadOnly = true;

		serialPort1->Close();
	}
}
private: System::Void buttonClose_Click(System::Object^  sender, System::EventArgs^  e)
{
	if(buttonPort->Text == "&Finish")
	{
		serialPort1->Close();
	}
	this->Close();
}

private: System::Void buttonClearData_Click(System::Object^  sender, System::EventArgs^  e)
{
	richTextBoxInput->Text = "";
}

private: System::Void buttonClearDebug_Click(System::Object^  sender, System::EventArgs^  e)
{
	richTextBoxDebugOutput->Text = "";
}

private: System::Void textBoxCommand_KeyUp(System::Object^  sender, System::Windows::Forms::KeyEventArgs^  e)
{
	if(buttonPort->Text == "&Finish")
	{
		// request to send data
		serialPort1->WriteTimeout=2000;
		serialPort1->RtsEnable=true;
		serialPort1->Write(textBoxCommand->Text);
	}
	else
		displayError("COM Port not openned yet");
	
	textBoxCommand->Text="";
}

};
}

