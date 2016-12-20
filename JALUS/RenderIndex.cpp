#include "RenderIndex.h"

void RenderIndex::writeToBitStream(BitStream* bitStream, bool isConstruction)
{
	if (isConstruction)
	{
		bitStream->Write((unsigned long)effects.size());
		for (int i = 0; i < effects.size(); i++)
		{
			FXEffect effect = effects.at(i);

			bitStream->Write((unsigned char)effect.name.length());
			for (int k = 0; k < effect.name.length(); k++)
			{
				bitStream->Write(effect.name[k]);
			}

			bitStream->Write(effect.id);

			bitStream->Write((unsigned char)effect.type.length());
			for (int k = 0; k < effect.type.length(); k++)
			{
				bitStream->Write(effect.type[k]);
			}

			bitStream->Write(effect.scale);
			bitStream->Write(effect.secondary);
		}
	}
}
