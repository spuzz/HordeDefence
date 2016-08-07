#pragma once

#include <iostream>
#include "Vect.h"
#include <vector>
#include <memory>
#include "Point.h"

#include "textureLoader.h"



class Sprite
{
public:
	Sprite();
	virtual ~Sprite();

	virtual Vect& getLocation() { return mLocation; }
	virtual void setLocation(Vect nLocation) { mLocation = nLocation; }

	virtual Vect& getIsoLocation() { return mIsoLocation; }
	virtual void setIsoLocation(Vect nIsoLocation) { mIsoLocation = nIsoLocation; }

	virtual Vect& getMin() { return mMin;  }
	virtual Vect& getMax() { return mMax; }

	virtual void setAABBModelSpace(const Vect& nMin, const Vect& nMax) { mMinRelative = nMin; mMaxRelative = nMax; }
	virtual void setAABBWorldSpace(const Vect& nMin, const Vect& nMax) { mMin = nMin; mMax = nMax; }
	virtual void calcAABBWorldSpace();

	virtual bool& getIsoVisitedFlag() { return mIsoVisitedFlag;  }
	virtual void setIsoVisitedFlag(const bool& nIsoVisitedFlag) { mIsoVisitedFlag = nIsoVisitedFlag; }

	virtual void setIsoDepth(const int& nIsoDepth) { mIsoDepth = nIsoDepth; }
	virtual int  getIsoDepth() const { return mIsoDepth; }

	virtual void draw(const float& xScreenLoc, const float& yScreenLoc, const float& zScreenLoc, textureLoader& txtLoader) = 0;

	std::vector<GLuint>& const GetTexture(void) { return texture; }
	void SetTexture(std::vector<GLuint> n_texture) { texture = n_texture; }

	virtual void addSpriteBehind(std::shared_ptr<Sprite>& nSprite) { mIsoSpritesBehind.push_back(std::shared_ptr<Sprite>(nSprite)); }
	virtual void clearSpriteBehind() { mIsoSpritesBehind.clear(); }
	virtual void visitNode(int& sortDepth);

	bool operator<(const Sprite& rhs) const;

protected:

	Vect mLocation;
	Vect mIsoLocation;

	// AABB in iso world space
	Vect mMin;
	Vect mMax;

	// AABB in iso model space 
	Vect mMinRelative;
	Vect mMaxRelative;

	bool mIsoVisitedFlag;
	int mIsoDepth;
	std::vector<std::shared_ptr<Sprite>> mIsoSpritesBehind;
	std::vector<GLuint>	texture;
};

