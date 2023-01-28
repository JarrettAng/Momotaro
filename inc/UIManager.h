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
		Vec4<float> color;
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

	enum FONT_SIZE {
		SMALL,
		MIDIUM,
		LARGE
	};

	enum FONT_TYPE {
		NONE,
		ROBOTO
	};

	struct FONT {
		FONT_TYPE type;
		s8 S;
		s8 M;
		s8 L;

		s8 size(FONT_SIZE size) {
			switch (size)
			{
			case SMALL:
				return S;
				break;
			case MIDIUM:
				return M;
				break;
			case LARGE:
				return L;
				break;
			default:
				break;
			}
		}
	};

	void Initialize();
	void PrepareUIRenderBatch();
	void UIPause();
	void RenderText(const s8& font, const float& x, const float& y, std::string text, const Vec3<float>& color = { 1.0f,1.0f,1.0f });
	FONT GetFont(const FONT_TYPE& type);

	/*!***********************************************************************
	* BUTTON
	*************************************************************************/
	// Button with TEXTURE + NO text.
	void RenderButton(const float& x, const float& y, const float& xPadding, const float& yPadding, const int& layer, AEGfxTexture* tex);
	// Button with COLOR + NO text.
	void RenderButton(const float& x, const float& y, const float& xPadding, const float& yPadding, const int& layer, const Vec4<float>& btnColor = { 1.0f,1.0f,1.0f,1.0f });

	// Button with TEXTURE + text.
	void RenderButton(const float& x, const float& y, const float& xPadding, const float& yPadding, const int& layer, const s8& font, const std::string& text, AEGfxTexture* tex, const Vec3<float>& txtColor = { 1.0f,1.0f,1.0f });
	// Button with COLOR + text.
	void RenderButton(const float& x, const float& y, const float& xPadding, const float& yPadding, const int& layer, const s8& font, const std::string& text, const Vec4<float>& btnColor = { 1.0f,1.0f,1.0f,1.0f }, const Vec3<float>& txtColor = { 1.0f,1.0f,1.0f });

}

