#pragma once



float lerp(const float& start, const float& end, float delta)
{
	return start * (1.0f - delta) + end * (delta);
}