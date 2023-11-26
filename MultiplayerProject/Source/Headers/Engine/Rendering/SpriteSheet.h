#include <vector>
#include "mathfu/vector.h"

struct KeyFrame
{
	KeyFrame();

	float startTimeInSeconds;
	float playbackTimeInSeconds;
	int boundsIndex;
};

struct Animation
{
	Animation();

	float GetAnimationPlaybackTimeInSeconds() { return animationPlaybackTimeInSeconds; }

	KeyFrame* GetKeyFrameAtTime(float keyFrameTimeInSeconds);

	std::vector<KeyFrame> keyFrames;

	float animationPlaybackTimeInSeconds;
};

struct FrameBounds
{
	FrameBounds();

	mathfu::Vector<int, 2> startBounds, endBounds;
};

class SpriteSheet
{
public:
	void Initialize(const char* uiFile);

	void Update(float deltaTime);

	FrameBounds GetDisplayedFrameBounds();

	bool PlayAnimationAtIndex(unsigned int animIndex, bool shouldLoop, float playbackSpeed = 1.0f);

private:
	float currentAnimationTime;
	float animationPlaybackSpeed;
	unsigned int currentAnimationIndex;
	bool isPlayingLoopingAnimation;

	std::vector<FrameBounds> frameBounds;
	std::vector<Animation> animations;
};

