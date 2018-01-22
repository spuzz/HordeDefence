#pragma once

#include <iostream>
#include <vector>
#include <memory>
#include "GameMath.h"

#include "textureLoader.h"

using GameMath::Vector3D;

class GameObject
{
public:
	GameObject();
	virtual ~GameObject();


	virtual Vector3D& getIsoLocation() { return mIsoLocation; }
	virtual void setIsoLocation(Vector3D nIsoLocation) { mIsoLocation = nIsoLocation; }

	virtual void setLookDirection(const Vector3D& direction) {};
	virtual Vector3D& getScreenLocation() { return mScreenLocation; }
	virtual void calcScreenLocation();
	virtual float& getZHeight() { return mZHeight; }
	virtual void setZHeight(float nZHeight) { mZHeight = nZHeight; }

	virtual bool& isTargetable() { return mIsTargetable; }
	virtual void setTargetable(bool nIsTargetable) { mIsTargetable = nIsTargetable; }

	virtual Vector3D& getMin() { return mMin; }
	virtual Vector3D& getMax() { return mMax; }

	virtual void setAABBModelSpace(const Vector3D& nMin, const Vector3D& nMax) { mMinRelative = nMin; mMaxRelative = nMax; }
	virtual void setAABBWorldSpace(const Vector3D& nMin, const Vector3D& nMax) { mMin = nMin; mMax = nMax; }
	virtual void calcAABBWorldSpace();

	virtual bool& getIsoVisitedFlag() { return mIsoVisitedFlag;  }
	virtual void setIsoVisitedFlag(const bool& nIsoVisitedFlag) { mIsoVisitedFlag = nIsoVisitedFlag; }

	virtual void setIsoDepth(const int& nIsoDepth) { mIsoDepth = nIsoDepth; }
	virtual int  getIsoDepth() const { return mIsoDepth; }

	virtual Vector3D getMapLocation() { return Vector3D(int(mIsoLocation.x), int(mIsoLocation.y), 0); }
	virtual void draw(const float& xScreenLoc, const float& yScreenLoc, const float& zScreenLoc, textureLoader& txtLoader) = 0;

	virtual Vector3D findDirection() { return Vector3D(0, 0, 0); }

	std::vector<GLuint>& const GetTexture(void) { return texture; }
	void SetTexture(std::vector<GLuint> n_texture) { texture = n_texture; }

	virtual void addGameObjectBehind(std::shared_ptr<GameObject>& nGameObject) { mIsoGameObjectsBehind.push_back(std::shared_ptr<GameObject>(nGameObject)); }
	virtual void clearGameObjectBehind() { mIsoGameObjectsBehind.clear(); }
	virtual void visitNode(int& sortDepth);

	bool operator<(const GameObject& rhs) const;

	virtual bool collide(const GameMath::Circle& circle) = 0;
	virtual bool collide(const GameMath::Rectangle& circle) = 0;
	virtual bool collide(const GameMath::Vector3D& point) = 0;

	virtual void update(float nSeconds) = 0;

	virtual void hit() {};

	virtual bool isDead() { return mDead; }
	virtual void setDead(bool nDead) { mDead = nDead; }

	virtual int getObjectID() { return mGameObjectID; }
protected:

	GameMath::Vector3D mIsoLocation;
	GameMath::Vector3D mScreenLocation;
	float mZHeight;
	// AABB in iso world space
	GameMath::Vector3D mMin;
	GameMath::Vector3D mMax;

	// AABB in iso model space 
	GameMath::Vector3D mMinRelative;
	GameMath::Vector3D mMaxRelative;

	bool mIsTargetable;
	bool mIsoVisitedFlag;
	int mIsoDepth;
	std::vector<std::shared_ptr<GameObject>> mIsoGameObjectsBehind;
	std::vector<GLuint>	texture;

	bool mDead;

	int mGameObjectID;
};

