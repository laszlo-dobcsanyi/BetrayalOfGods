#include "Macro.h"
#include "Window_NewWorld.h"

#include <vector>
#include <fstream>

#include <direct.h>
#include <Commdlg.h>

#include <boost\lexical_cast.hpp>

#include "ResourceManager.h"

#include "Button.h"
#include "TextBox.h"

extern LPSTR path;

Window_NewWorld::Window_NewWorld()
	: Window()
{
	D3DCOLOR fontColor = D3DCOLOR_ARGB(255, 0, 64, 0);
	TextBox_Name = new TextBox(this, 0, "New World", 8, 8, 256, 32, fontColor, false);
	TextBox_FileName = new TextBox(this, 1, "NewWorldFile", 8, 8 + 40, 256, 32, fontColor, false);

	Button_Create = new Button(this, 2, "Create", 8 + 256 + 8, 8 + 40, 64, 32);

	active = TextBox_Name;
}

void Window_NewWorld::NextActive()
{
	active_index++;
	if (active_index == 3) active_index = 0;
	SetActive(active_index);
}

void Window_NewWorld::SetActive(const int& _index)
{
	switch (_index)
	{
		case 0: active = TextBox_Name; break;
		case 1: active = TextBox_FileName; break;

		case 2: active = Button_Create; break;
	}
}

void Window_NewWorld::Render()
{
	device->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(0, 0, 0), 1.0f, 0);
    device->BeginScene();

	LPD3DXSPRITE sprite;
	D3DXCreateSprite(device, &sprite);
	sprite->Begin(0);

	TextBox_Name->Render(resources->font);
	TextBox_FileName->Render(resources->font);

	Button_Create->Render(sprite);

	sprite->End();

	device->EndScene();
	device->Present(NULL, NULL, NULL, NULL);
	sprite->Release();

}

void Window_NewWorld::Handle_KeyUp(const UINT_PTR& _key)
{
	switch (_key)
	{
		case VK_TAB: NextActive(); break;
		case VK_RETURN:
			if (active_index == 2) { OnCreate(); break; }
			
			NextActive();
			break;
		default: active->Handle_KeyUp(_key); break;
	}
}

void Window_NewWorld::Handle_MouseUp(const int& _x, const int& _y, const int& button)
{
	if (TextBox_Name->Handle_MouseUp(_x, _y)) { SetActive(0); return; }
	if (TextBox_FileName->Handle_MouseUp(_x, _y)) { SetActive(1); return; }

	if (Button_Create->Handle_MouseUp(_x, _y)) { SetActive(2); OnCreate(); return; }
}

void Window_NewWorld::OnCreate()
{
	char bitmap[128];

	OPENFILENAME bitmap_dialog;

	ZeroMemory(&bitmap_dialog , sizeof(bitmap_dialog));
	bitmap_dialog.lStructSize = sizeof (bitmap_dialog);
	bitmap_dialog.hwndOwner = NULL;
	bitmap_dialog.lpstrFile = bitmap;
	bitmap_dialog.lpstrFile[0] = '\0';
	bitmap_dialog.nMaxFile = sizeof(bitmap);
	bitmap_dialog.lpstrFilter = "Bitmap\0*.bmp";
	bitmap_dialog.nFilterIndex = 1;
	bitmap_dialog.lpstrFileTitle = NULL;
	bitmap_dialog.nMaxFileTitle = 0;
	bitmap_dialog.lpstrInitialDir=NULL;
	bitmap_dialog.Flags = OFN_PATHMUSTEXIST|OFN_FILEMUSTEXIST;
	GetOpenFileName(&bitmap_dialog);

	FILE* bitmap_file;
	fopen_s(&bitmap_file, bitmap, "rb");

	if(bitmap_file)
	{
		unsigned char info[54];
		//Read the 54-byte bmp header
		fread(info, sizeof(unsigned char), 54, bitmap_file); 

		//Extract image height and width
		int width = *(int*)&info[18];
		int height = *(int*)&info[22];

		int row_padded = (width*3 + 3) & (~3);
		unsigned char* bitmap_data = new unsigned char[row_padded];
		unsigned char tmp;

		int* data = new int[width * height];

		//Read and convert data read
		for(int i = 0; i < height; i++)
		{
			fread(bitmap_data, sizeof(unsigned char), row_padded, bitmap_file);
			for(int j = 0; j < width; ++j)
			{
				// Convert (B, G, R) to (R, G, B)
				tmp = bitmap_data[3*j];
				bitmap_data[3*j] = bitmap_data[3*j+2];
				bitmap_data[3*j+2] = tmp;

				data[j * width + i] = ((int)bitmap_data[3*j + 0] /16) * 16 * 16 + ((int)bitmap_data[3*j + 1] /16) * 16 + ((int)bitmap_data[3*j + 2] /16);
			}

		}
		
		//Create data file
		std::ofstream data_file(boost::lexical_cast<std::string>(path) + '\\' + TextBox_FileName->GetText() + ".data");
		if (data_file)
		{
			data_file << TextBox_Name->GetText() << "\n" << width << "\t" << height;
			for(int row = height - 1; 0 <= row; --row)
			{
				for (int column = 0; column < width; ++column)
					data_file << "\n1\t\t1\t\t" << data[column * width + row] << "\t\t0";
			}
		} else { MessageBox(NULL, "Error while opening data file!", "Error" , MB_ICONERROR | MB_OK); return; }

		data_file.close();

		//Create resource file
		std::ofstream resource_file(boost::lexical_cast<std::string>(path) + '\\' + TextBox_FileName->GetText() + ".resource");
		if (resource_file)
		{
			std::vector<int> tile_data;

			for(int row = 0; row < height; ++row)
				for(int column = 0; column < width; ++column)
				{
					bool found = false;
					for(unsigned int current = 0; current < tile_data.size(); ++current)
						if (tile_data[current] == data[row * width + column]) { found = true; break; }

					if (!found) tile_data.push_back(data[row * width + column]);
				}

			resource_file << tile_data.size() << '\n';
			for(int current = 0; current < tile_data.size(); ++current)
				resource_file << tile_data[current] << '\t';
		} else { MessageBox(NULL, "Error while opening resource file!", "Error" , MB_ICONERROR | MB_OK); return; }

		resource_file.close();

	} else { MessageBox(NULL, "Error while opening bitmap file!", "Error" , MB_ICONERROR | MB_OK); return; }
	
	fclose(bitmap_file);

	return_value = 2;
}


void Window_NewWorld::Dispose()
{
	TextBox_Name->Dispose();
	TextBox_FileName->Dispose();

	Button_Create->Dispose();
}
