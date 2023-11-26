#include "Engine/Rendering/SpriteSheet.h"
#include "Engine/JsonDataPopulator.h"

#include "rapidjson/filereadstream.h"
#include "rapidjson/document.h"

Animation::Animation()
{
	CreateVariableMetadata(Animation, keyFrames);
	animationPlaybackTimeInSeconds = 0.0f;
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

	return &keyFrames[keyFrames.size() - 1];
}

void SpriteSheet::Initialize(const char* uiFile)
{
	currentAnimationTime = 0.0f;
	currentAnimationIndex = 0;
	animationPlaybackSpeed = 1.0f;
	isPlayingLoopingAnimation = true;

	FILE* fp = nullptr;
	fopen_s(&fp, uiFile, "rb"); // non-Windows use "r"

	char readBuffer[65536];
	rapidjson::FileReadStream is(fp, readBuffer, sizeof(readBuffer));

	rapidjson::Document document;
	document.ParseStream(is);

	fclose(fp);

	CreateVariableMetadata(SpriteSheet, frameBounds);
	rapidjson::GenericArray<false, rapidjson::Value::ValueType> jsonBoundsArray = document["frameBounds"].GetArray();
	frameBounds.reserve(jsonBoundsArray.Size());
	for (unsigned int i = 0; i < jsonBoundsArray.Size(); ++i)
	{
		frameBounds.push_back(FrameBounds());

		rapidjson::GenericArray<false, rapidjson::Value::ValueType>::PlainType* jsonBoundsElement = &jsonBoundsArray[i];

		ClassMetaData<FrameBounds>::MetaVariable& mv_startBounds = *ClassMetaData<FrameBounds>::Get()->GetMetaVariableByName("startBounds");
		JsonDataPopulator::PopulateVarData(mv_startBounds, jsonBoundsElement,
			frameBounds[i].startBounds.x,
			frameBounds[i].startBounds.y);

		ClassMetaData<FrameBounds>::MetaVariable& mv_endBounds = *ClassMetaData<FrameBounds>::Get()->GetMetaVariableByName("endBounds");
		JsonDataPopulator::PopulateVarData(mv_endBounds, jsonBoundsElement,
			frameBounds[i].endBounds.x,
			frameBounds[i].endBounds.y);
	}
	
	CreateVariableMetadata(SpriteSheet, animations);
	rapidjson::GenericArray<false, rapidjson::Value::ValueType> jsonAnimationsArray = document["animations"].GetArray();
	animations.reserve(jsonAnimationsArray.Size());
	for (unsigned int i = 0; i < jsonAnimationsArray.Size(); ++i)
	{
		rapidjson::GenericArray<false, rapidjson::Value::ValueType>::PlainType* jsonAnim = &jsonAnimationsArray[i];
		animations.push_back(Animation());
		
		auto jsonKeyFrameArray = jsonAnim->FindMember("keyFrames");
		unsigned int keyFrameCount = jsonKeyFrameArray->value.GetArray().Size();
		for (unsigned int j = 0; j < keyFrameCount; ++j)
		{			
			animations[i].keyFrames.push_back(KeyFrame());
			
			ClassMetaData<KeyFrame>::MetaVariable& mv_boundsIndex = *ClassMetaData<KeyFrame>::Get()->GetMetaVariableByName("boundsIndex");
			JsonDataPopulator::PopulateVarData(mv_boundsIndex, &jsonKeyFrameArray->value[j], animations[i].keyFrames[j].boundsIndex);
			
			ClassMetaData<KeyFrame>::MetaVariable& mv_playbackTimeInSeconds = *ClassMetaData<KeyFrame>::Get()->GetMetaVariableByName("playbackTimeInSeconds");
			JsonDataPopulator::PopulateVarData(mv_playbackTimeInSeconds, &jsonKeyFrameArray->value[j], animations[i].keyFrames[j].playbackTimeInSeconds);

			animations[i].keyFrames[j].startTimeInSeconds = animations[i].animationPlaybackTimeInSeconds;
			animations[i].animationPlaybackTimeInSeconds += animations[i].keyFrames[j].playbackTimeInSeconds;
		}
	}
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

FrameBounds::FrameBounds()
{
	CreateVariableMetadata(FrameBounds, startBounds);
	CreateVariableMetadata(FrameBounds, endBounds);
}

KeyFrame::KeyFrame()
{
	CreateVariableMetadata(KeyFrame, playbackTimeInSeconds);
	CreateVariableMetadata(KeyFrame, boundsIndex);

	startTimeInSeconds = 0.0f; 
	playbackTimeInSeconds = 0.0f;
	boundsIndex = 0;

}
