#include "Window_Game.h"

#include <boost\foreach.hpp>

#include "Tile.h"
#include "Character.h"
#include "ResourceManager.h"

extern Game* game;
extern ResourceManager* resources;

void GameWindow::DrawFields(LPDIRECT3DDEVICE9 _device)
{
	LPD3DXSPRITE sprite;
	D3DXCreateSprite(_device, &sprite);
	sprite->Begin(0);

	int camera_x = game->hero->location.x / 512;
	int camera_y = game->hero->location.y / 512;

	double offset_x = game->hero->location.x - camera_x * 512;
	double offset_y = game->hero->location.y - camera_y * 512;

	for(int row = -1; row < 2; ++row)
		for(int column = -1; column < 2; ++column)
		{
			int tileid = resources->GetTileID(camera_x + column, camera_y + row);
			const Tile* const tile = resources->GetTile(tileid);
			sprite->Draw(tile->GetTexture(), NULL, NULL, &D3DXVECTOR3(row * 512 - offset_x + 1024 / 2, column * 512 - offset_y + 768 / 2, 0), 0xFFFFFFFF);
		}

	sprite->End();
	sprite->Release();
}

void GameWindow::DrawUnits(LPDIRECT3DDEVICE9 _device)
{
	LPD3DXSPRITE sprite;
	D3DXCreateSprite(_device, &sprite);
	sprite->Begin(0);

	D3DXVECTOR3 position(100 - game->hero->location.x + 512 - 32, 100 - game->hero->location.y + 384 - 32, 0);
	sprite->Draw(resources->character_icons[0], NULL, NULL, &position, 0xFFFFFFFF);
}

void GameWindow::DrawLoading(LPDIRECT3DDEVICE9 _device)
{

}

