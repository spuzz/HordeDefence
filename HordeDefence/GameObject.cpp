#include "GameObject.h"



GameObject::GameObject()
{
	mDead = false;
	//mLocation = Vector3D(-1, -1, 0);
}

void GameObject::visitNode(int& sortDepth)
{
	if (mIsoVisitedFlag == 0)
	{
		mIsoVisitedFlag = 1;

		const int GameObjectsBehindLength = mIsoGameObjectsBehind.size();
		for (int i = 0; i < GameObjectsBehindLength; ++i)
		{
			if (mIsoGameObjectsBehind[i] == NULL)
			{
				break;
			}
			else
			{
				mIsoGameObjectsBehind[i]->visitNode(sortDepth);
				mIsoGameObjectsBehind[i] = NULL;
			}
		}

		mIsoDepth = sortDepth++;
	}
}

void GameObject::calcAABBWorldSpace()
{
	mMin = mIsoLocation + mMinRelative;
	mMax = mIsoLocation + mMaxRelative;
}

GameObject::~GameObject()
{
	int test = 0;
}

bool GameObject::operator<(const GameObject& rhs) const
{
	return this->getIsoDepth() < rhs.getIsoDepth();
}
