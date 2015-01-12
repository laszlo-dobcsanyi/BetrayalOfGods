#include "Macro.h"
#include "Window_Launcher.h"

#include "Message.hpp"
#include "Connection.h"

LauncherWindow::LauncherWindow(LPDIRECT3DDEVICE9 _device)
	: Window()
{
	D3DXCreateTextureFromFile(_device, "data\\interface\\launcher.png", &background);

	D3DXCreateFont(_device, 32, 0, FW_NORMAL, 0, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, TEXT("Arial Black"), &font_input);
	D3DXCreateFont(_device, 60, 0, FW_BOLD, 0, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, TEXT("Arial"), &font_title);

	D3DCOLOR fontColor = D3DCOLOR_ARGB(255, 0, 64, 0);
	TextBox_Username = new TextBox(this, 0, "kuku", 168, 152, 256, 32, fontColor, false);
	TextBox_Password = new TextBox(this, 1, "muku", 168, 208, 256, 32, fontColor, true);

	Button_Login = new Button(this, 2, "button_login", 318, 320, 170, 40, _device);
	Button_Registrate = new Button(this, 3, "button_registrate", 24, 320, 170, 40, _device);

	active = TextBox_Username;
}

void LauncherWindow::NextActive()
{
	active_index++;
	if (active_index == 4) active_index = 0;
	SetActive(active_index);
}

void LauncherWindow::SetActive(const int& _index)
{
	switch (_index)
	{
		case 0: active = TextBox_Username; break;
		case 1: active = TextBox_Password; break;
		case 2: active = Button_Login; break;
		case 3: active = Button_Registrate; break;
	}
}


void LauncherWindow::Render(LPDIRECT3DDEVICE9 _device)
{
	_device->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(0, 0, 0), 1.0f, 0);
    _device->BeginScene();

	LPD3DXSPRITE sprite;
	D3DXCreateSprite(_device, &sprite);

	sprite->Begin(0);
	sprite->Draw(background, NULL, NULL, NULL, 0xFFFFFFFF);
	sprite->Flush();

	DrawString("Betrayal", font_title,		160, 8, 256, 64,	255, 255, 255 - 32, 0);
	DrawString("of Gods", font_title,		166, 72, 256, 64,	255, 255, 255 - 32, 0);
	DrawString("Username:", font_input,		24, 148, 128, 32,	255, 255 - 112, 255 - 128, 0);
	DrawString("Password:", font_input,		24, 204, 128, 32,	255, 255 - 112, 255 - 160, 0);

	TextBox_Username->Render(font_input);
	TextBox_Password->Render(font_input);

	Button_Login->Render(sprite);
	Button_Registrate->Render(sprite);

	sprite->End();
 
    _device->EndScene();
    _device->Present(NULL, NULL, NULL, NULL);

	sprite->Release();
}

void LauncherWindow::Handle_KeyUp(const UINT_PTR& _key)
{
	switch (_key)
	{
		case VK_TAB: NextActive(); break;
		case VK_RETURN:
			if (active_index == 2) { OnLogin(); break; }
			if (active_index == 3) { OnRegistration(); break; }
			
			NextActive();
			break;
		default: active->Handle_KeyUp(_key); break;
	}
}

void LauncherWindow::Handle_MouseUp(const int& _x, const int& _y)
{
	if(TextBox_Username->Handle_MouseUp(_x, _y))
	{
		SetActive(0);
		return;
	}
	if(TextBox_Password->Handle_MouseUp(_x, _y))
	{
		SetActive(1);
		return;
	}

	if(Button_Login->Handle_MouseUp(_x, _y))
	{
		SetActive(2);
		OnLogin();
		return;
	}
	if(Button_Registrate->Handle_MouseUp(_x, _y))
	{
		SetActive(3);
		OnRegistration();
		return;
	}
}


void LauncherWindow::OnLogin()
{
	std::string username = TextBox_Username->GetText();
	std::string password = TextBox_Password->GetText();

	connection->Send(Connection::LOGIN, username + ';' + password);
}

void LauncherWindow::OnRegistration()
{

}


void LauncherWindow::Dispose()
{
	if (disposed) return; disposed = true;

	background->Release();

	font_input->Release();
	font_title->Release();

	TextBox_Username->Dispose();
	delete TextBox_Username;

	TextBox_Password->Dispose();
	delete TextBox_Password;

	Button_Login->Dispose();
	delete Button_Login;

	Button_Registrate->Dispose();
	delete Button_Registrate;

	delete this;
}


