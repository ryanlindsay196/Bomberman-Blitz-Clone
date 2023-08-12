#include <vector>
#include "mathfu/vector.h"

struct KeyFrame
{
	float startTimeInSeconds;
	float playbackTimeInSeconds;
	int boundsIndex;
};

struct Animation
{
	//TODO: Remove hard coding of animation data. Maybe replace with a file format of some kind.
	void InitializeKeyFrames(KeyFrame* inKeyFrames, unsigned int keyFrameCount);

	float GetAnimationPlaybackTimeInSeconds() { return animationPlaybackTimeInSeconds; }

	KeyFrame* GetKeyFrameAtTime(float keyFrameTimeInSeconds);

private:
	std::vector<KeyFrame> keyFrames;
	float animationPlaybackTimeInSeconds;
};

struct FrameBounds
{
	mathfu::Vector<int, 2> startBounds, endBounds;
};

class SpriteSheet
{
public:
	void Initialize();

	void Update(float deltaTime);

	FrameBounds GetDisplayedFrameBounds();

	bool PlayAnimationAtIndex(unsigned int animIndex, bool shouldLoop, float playbackSpeed = 1.0f);

	//TODO: Load this from file instead of hard coding
	void InitializeAnimations();

	void InitializeFrameBounds();

private:
	float currentAnimationTime;
	float animationPlaybackSpeed;
	unsigned int currentAnimationIndex;
	bool isPlayingLoopingAnimation;

	std::vector<FrameBounds> frameBounds;
	std::vector<Animation> animations;
};

