#ifndef FIELD_GENERATED_H
#define FIELD_GENERATED_H

#include <vector>

#include "Field.h"

class Field_Generated
	: public Field
{
public:
	inline Field_Generated() : Field() { tile_ids.push_back(0); };
	Field_Generated(std::ifstream& _stream);

	void Save(std::ofstream& _stream);

	void Render(LPD3DXSPRITE _sprite, D3DXVECTOR3 _offset);

	void Dispose();

private:
	std::vector<int> tile_ids;
	std::vector<int> object_ids;
};

#endif