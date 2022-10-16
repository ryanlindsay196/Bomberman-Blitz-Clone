#include "Engine/Rendering/SpriteSheet.h"

void Animation::InitializeKeyFrames(KeyFrame * inKeyFrames, int keyFrameCount)
{
	//TODO: the keyframes we pass in have their start times at 0. This doesn't matter. It will matter once animations are loaded from files.

	animationPlaybackTimeInSeconds = 0;
	for (unsigned int i = 0; i < keyFrameCount; ++i)
	{
		KeyFrame& keyFrame = inKeyFrames[i];
		keyFrame.startTimeInSeconds = animationPlaybackTimeInSeconds;
		animationPlaybackTimeInSeconds += keyFrame.playbackTimeInSeconds;
		keyFrames.push_back(keyFrame);
	}
}

KeyFrame* Animation::GetKeyFrameAtTime(float keyFrameTimeInSeconds)
{
	if (keyFrameTimeInSeconds < 0)
		return nullptr;

	for (KeyFrame& keyFrame : keyFrames)
	{
		if (keyFrame.startTimeInSeconds + keyFrame.playbackTimeInSeconds >= keyFrameTimeInSeconds)
		{
			return &keyFrame;
		}
	}

	return nullptr;
}

void SpriteSheet::Initialize()
{
	currentAnimationTime = 0.0f;
	currentAnimationIndex = 0;
	animationPlaybackSpeed = 1.0f;
	isPlayingLoopingAnimation = true;
}

void SpriteSheet::Update(float deltaTime)
{
	currentAnimationTime += deltaTime * animationPlaybackSpeed / 1000;

	float animationPlaybackTime = animations[currentAnimationIndex].GetAnimationPlaybackTimeInSeconds();
	if (currentAnimationTime > animationPlaybackTime && isPlayingLoopingAnimation)
	{
		while (currentAnimationTime > animationPlaybackTime)
		{
			currentAnimationTime -= animationPlaybackTime;
		}
	}
}

FrameBounds SpriteSheet::GetDisplayedFrameBounds()
{
	Animation* currentAnimation = currentAnimationIndex < animations.size() ? &animations[currentAnimationIndex] : nullptr;

	if (!currentAnimation)
	{
		return FrameBounds();
	}

	KeyFrame* currentKeyFrame = currentAnimation->GetKeyFrameAtTime(currentAnimationTime);

	if (!currentKeyFrame)
	{
		return FrameBounds();
	}

	unsigned int boundsIndex = currentKeyFrame->boundsIndex;
	return boundsIndex < frameBounds.size() ? frameBounds[boundsIndex] : FrameBounds();
}

bool SpriteSheet::PlayAnimationAtIndex(unsigned int animIndex, bool shouldLoop, float playbackSpeed)
{
	if (animIndex >= animations.size())
	{
		return false;
	}

	currentAnimationTime = 0.0f;

	animationPlaybackSpeed = playbackSpeed;
	currentAnimationIndex = animIndex;
	isPlayingLoopingAnimation = shouldLoop;

	return true;
}

void SpriteSheet::InitializeAnimations()
{
	//TODO: Initialize from file instead of hard-coding frames in
	Animation newAnimation;

	KeyFrame keyFrames[]
	{
		{0, 0.25f, 0},
		{0, 0.25f, 1},
		{0, 0.25f, 2},
		{0, 0.25f, 1},
		{0, 0.25f, 3},
		{0, 0.25f, 4},
		{0, 0.25f, 5},
		{0, 0.25f, 4}
	};

	newAnimation.InitializeKeyFrames(keyFrames, sizeof(keyFrames) / sizeof(KeyFrame));
	animations.push_back(newAnimation);

	Animation newAnimation2;

	KeyFrame keyFrames2[]
	{
		0, 0.25, 6,
		0, 0.25, 7,
		0, 0.25, 8,
		0, 0.25, 9,
		0, 0.25, 10,
		0, 0.25, 11
	};

	newAnimation2.InitializeKeyFrames(keyFrames2, sizeof(keyFrames2) / sizeof(KeyFrame));
	animations.push_back(newAnimation2);
}

void SpriteSheet::InitializeFrameBounds()
{
	//TODO: Load this from file instead of hard coding it.
	frameBounds.push_back({ {2, 62}, {21, 89} });
	frameBounds.push_back({ {24, 62}, {42, 89} });
	frameBounds.push_back({ {45, 62}, {62, 89} });
	frameBounds.push_back({ {65, 62}, {83, 89} });
	frameBounds.push_back({ {86, 62}, {104, 89} });
	frameBounds.push_back({ {107, 62}, {124, 89} });

	frameBounds.push_back({ {134, 62}, {148, 89} });
	frameBounds.push_back({ {152, 62}, {166, 89} });
	frameBounds.push_back({ {169, 62}, {185, 89} });
	frameBounds.push_back({ {189, 62}, {203, 89} });
	frameBounds.push_back({ {207, 62}, {221, 89} });
	frameBounds.push_back({ {225, 62}, {239, 89} });
}
