#include "Macro.h"
#include "Window_Main.h"

#include <direct.h>
#include <Commdlg.h>

#include "World.h"
#include "Button.h"
#include "TextBox.h"

Window_Main::Window_Main()
	: Window(),
	  camera(0, 0, 0),
	  scale(1.0)
{
	Button_NewWorld = new Button(this, 0, "World_New", 0, 0, 32, 32);
	Button_LoadWorld = new Button(this, 1, "World_Add", 32, 0, 32, 32);

	active = Button_NewWorld;
}

void Window_Main::NextActive()
{
	active_index++;
	if (active_index == 2) active_index = 0;
	SetActive(active_index);
}

void Window_Main::SetActive(const int& _index)
{
	switch (_index)
	{
		case 0: active = Button_NewWorld; break;
		case 1: active = Button_LoadWorld; break;
	}
}

void Window_Main::Render()
{	
	device->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(229, 229, 229), 1.0f, 0);
    device->BeginScene();

	LPD3DXSPRITE sprite;
	D3DXCreateSprite(device, &sprite);
	sprite->Begin(0);

	if (world)
	{
		float rotation = 0;
		D3DXVECTOR2 spriteCentre(512.0f, 512.0f);
		D3DXVECTOR2 trans(50.0f,80.0f);
		D3DXVECTOR2 scaling(scale, scale);

		D3DXMATRIX mat;

		//D3DXMatrixTransformation2D(&mat, NULL, 0.0, &scaling, &spriteCentre, rotation, &trans);
		D3DXMatrixTransformation2D(&mat, NULL, 0.0, &scaling, NULL, NULL, NULL);
		sprite->SetTransform(&mat);

		world->Render(sprite, camera, scale); 
		sprite->End();
		sprite->Release();

		D3DXCreateSprite(device, &sprite);
		sprite->Begin(0);


		sprite->Draw(resources->panel, NULL, NULL,  &D3DXVECTOR3(1024, 32, 0), 0xFFFFFFFF);
		sprite->Flush();
		RECT position; position.left = 1024; position.top = 32; position.right = position.left + 256; position.bottom = position.top + 32;
		resources->font->DrawText(NULL, ("camera : " + boost::lexical_cast<std::string>((int)camera.x) + ":" + boost::lexical_cast<std::string>((int)camera.y)).c_str(), -1, &position, 0, D3DCOLOR_ARGB(255, 0, 128,1280));
		position.left = 1024; position.top = 64; position.right = position.left + 256; position.bottom = position.top + 32;
		resources->font->DrawText(NULL, ("scale : " + boost::lexical_cast<std::string>(scale)).c_str(), -1, &position, 0, D3DCOLOR_ARGB(255, 0, 128,1280));
	}

	Button_NewWorld->Render(sprite);
	Button_LoadWorld->Render(sprite);

	sprite->End();

	device->EndScene();
	device->Present(NULL, NULL, NULL, NULL);
	sprite->Release();
}

void Window_Main::Handle_KeyUp(const UINT_PTR& _key)
{
	switch (_key)
	{
		case VK_LEFT : if ((world) && (0 < camera.x)) --camera.x; break;
		case VK_RIGHT : if ((world) && (camera.x < world->width - 1)) ++camera.x; break;
		case VK_UP : if ((world) && (0 < camera.y)) --camera.y; break;
		case VK_DOWN : if ((world) && (camera.y < world->height - 1)) ++camera.y; break;

		case VK_HOME: if ((world) && (0.015625 < scale)) scale /= 2; break;
		case VK_END: if ((world) && (scale < 1)) scale *= 2; break;

		case VK_INSERT: if (world) world->SwitchField((int)camera.x, (int)camera.y); break;

		case VK_TAB: NextActive(); break;

		case VK_RETURN:
			if (active_index == 0) { return_value = 3; break; }
			if (active_index == 1) { LoadWorld(); break; }
			
			NextActive();
			break;

		default: active->Handle_KeyUp(_key); break;
	}
}

void Window_Main::Handle_MouseUp(const int& _x, const int& _y, const int& button)
{
	if (Button_NewWorld->Handle_MouseUp(_x, _y)) { return_value = 3; return; }
	if (Button_LoadWorld->Handle_MouseUp(_x, _y)) { LoadWorld(); return; }

	if (world)
		if (scale == 1.0)
		{
			if (button == 0)
			{
			}
			else
			{
				//Modify field @ current camera location
			}
		}
		else
		{
			float multiplier = 1 / scale;
			int offset_x = (_x * multiplier - 256) / 512;
			int offset_y = (_y * multiplier - 256 + 128 - 36) / 512;

			if ((0 <= camera.x + offset_x && camera.x + offset_x < world->width) && (0 <= camera.y + offset_y && camera.y + offset_y < world->height))
			{ camera.x += offset_x; camera.y += offset_y; }
		}
}

void Window_Main::LoadWorld()
{
	char world_name[128];

	OPENFILENAME world_open_dialog;

	ZeroMemory(&world_open_dialog , sizeof(world_open_dialog));
	world_open_dialog.lStructSize = sizeof (world_open_dialog);
	world_open_dialog.hwndOwner = NULL;
	world_open_dialog.lpstrFile = world_name;
	world_open_dialog.lpstrFile[0] = '\0';
	world_open_dialog.nMaxFile = sizeof(world_name);
	world_open_dialog.lpstrFilter = "World File\0*.data";
	world_open_dialog.nFilterIndex = 1;
	world_open_dialog.lpstrFileTitle = NULL;
	world_open_dialog.nMaxFileTitle = 0;
	world_open_dialog.lpstrInitialDir=NULL;
	world_open_dialog.Flags = OFN_PATHMUSTEXIST|OFN_FILEMUSTEXIST;
	GetOpenFileName(&world_open_dialog);

	world = new World(world_open_dialog.lpstrFile);
}

void Window_Main::Dispose()
{
	if (disposed) return; disposed = true;

	Button_NewWorld->Dispose();
	Button_LoadWorld->Dispose();
}