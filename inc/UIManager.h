#pragma once
#include <AEEngine.h>
#include <MomoMaths.h>
#include <string>

namespace UIManager {
	struct Transform {
		float x, y;
		float width, height;
	};

	struct GraphicsData {
		bool hasGraphics = false;
		Vec3<float> color;
		AEGfxTexture* tex = nullptr;
	};

	struct TextData {
		bool hasText = false;
		s8 fontID;
		float x;
		float y;
		std::string text;
		float scale;
		Vec3<float> color;
	};

	struct UIData {
		Transform transform;
		GraphicsData graphics;
		TextData text;
		int layer;
	};

	enum FONT_TYPE {
		ROBOTO,
	};
	void Initialize();
	void PrepareUIRenderBatch();
	void RenderText(const FONT_TYPE& font, const float& x, const float& y, std::string text, const Vec3<float>& color = { 1.0f,1.0f,1.0f }, const float& scale = 1);
	void RenderButton(const float& x, const float& y, const float& width, const float& height, FONT_TYPE font, std::string text, const int& layer, AEGfxTexture* tex = nullptr, const Vec3<float>& txtColor = { 1.0f,1.0f,1.0f }, const Vec3<float>& btnColor = { 1.0f,1.0f,1.0f });
}

