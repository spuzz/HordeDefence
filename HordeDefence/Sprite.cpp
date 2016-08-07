#include "Sprite.h"



Sprite::Sprite()
{
	mLocation = Vect(-1, -1, 0);
}

void Sprite::visitNode(int& sortDepth)
{
	if (mIsoVisitedFlag == 0)
	{
		mIsoVisitedFlag = 1;

		const int spritesBehindLength = mIsoSpritesBehind.size();
		for (int i = 0; i < spritesBehindLength; ++i)
		{
			if (mIsoSpritesBehind[i] == NULL)
			{
				break;
			}
			else
			{
				mIsoSpritesBehind[i]->visitNode(sortDepth);
				mIsoSpritesBehind[i] = NULL;
			}
		}

		mIsoDepth = sortDepth++;
	}
}

void Sprite::calcAABBWorldSpace()
{
	mMin = mIsoLocation + mMinRelative;
	mMax = mIsoLocation + mMaxRelative;
}

Sprite::~Sprite()
{
	int test = 0;
}

bool Sprite::operator<(const Sprite& rhs) const
{
	return this->getIsoDepth() < rhs.getIsoDepth();
}
