/*!************************************************************************
\file RenderSystem.h
\author Tan Jun Rong
\par DP email: t.junrong@digipen.edu
\par Course: CSD1171B
\par Software Engineering Project
\date 18-01-2023
\brief
This header file declares

**************************************************************************/

#pragma once
#include <list>
#include <vector>
#include <AEEngine.h>
#include <MomoMaths.h>
#include <string>
#include <TextureManager.h>
#include <iostream>
#include <algorithm>
#include <FontManager.h>

namespace RenderSystem {

	struct RenderSetting {
		// Default: Opaque + Allow transperancy. (PNG images)
		static const AEGfxBlendMode BLEND_MODE{ AE_GFX_BM_BLEND };
		static constexpr Vec4<float> TINT{ 1.0f,1.0f,1.0f,1.0f };
		static constexpr Vec4<float> BLEND_COLOR{ 0,0,0,0 };

		AEGfxBlendMode blendMode = BLEND_MODE;
		Vec4<float> tint = TINT;
		Vec4<float> blendColor = BLEND_COLOR;

		void setDefault() { blendMode = BLEND_MODE; blendColor = BLEND_COLOR; tint = TINT; }
		bool isDefault() { return blendMode == BLEND_MODE && blendColor == BLEND_COLOR && tint == TINT; }
	};

	enum BATCH_TYPE {
		TILE_BATCH = 0,
		GAME_PIECES_BATCH,
		UI_BATCH
	};

	enum RENDER_PIVOT {
		TOP_LEFT,
		TOP_MID,
		TOP_RIGHT,
		MID_LEFT,
		MID,
		MID_RIGHT,
		BOT_LEFT,
		BOT_MID,
		BOT_RIGHT
	};

	struct Transform {
		Vec2<float> pos{};
		Vec2<float> size = { 1,1 };
		float rot = 0;
	};

	struct Graphics {
		Vec4<float> color;
		TextureManager::TEX_TYPE tex;
	};

	struct Rect {
		Transform transform{};
		Graphics graphics{};
	};

	struct Text {
		s8 fontID;
		int fontSize;
		Vec2<float> pos{};
		std::string text;
		Vec3<float> color;
	};

	enum RENDER_TYPE {
		RECT,
		TEXT
	};

	struct Renderable {
		RENDER_TYPE type{};
		Rect rect;
		Text text;
		int layer = 0;
	};

	struct Interactable {
		Renderable render;
		bool isActive;
		void (*func)();
	};

	void Initialize();
	void Render();

	Vec2<float> GetPivotPos(const Vec2<float>& pos, const float& width, const float& height);
	void SetRenderSetting(Vec4<float> tint, Vec4<float> blendColor = {}, AEGfxBlendMode blendMode = AE_GFX_BM_BLEND);

	/*!***********************************************************************
	* TEXT RENDERING
	*************************************************************************/
	void AddTextToBatch(const BATCH_TYPE& id, const float& x, const float& y, const s8& font, const int& fontSize, std::string text, const int& layer = 0, const Vec3<float>& color = { 1,1,1 });
	/*************************************************************************/

	/*!***********************************************************************
	* RECT RENDERING
	*************************************************************************/
	// Rect with TEXTURE.
	void AddRectToBatch(const BATCH_TYPE& batch, const float& x, const float& y, const float& width, const float& height, TextureManager::TEX_TYPE tex, const int& layer = 0, const float& rot = 0);
	// Rect with COLOR
	void AddRectToBatch(const BATCH_TYPE& batch, const float& x, const float& y, const float& width, const float& height, const Vec4<float>& btnColor = { 1,1,1 ,1 }, const int& layer = 0, const float& rot = 0);
	/*************************************************************************/

	/*!***********************************************************************
	* BUTTON RENDERING (RECT + TEXT IN MIDDLE OF RECT)
	*************************************************************************/
	// Button with TEXTURE + text.
	void AddButtonToBatch(const BATCH_TYPE& id, const float& x, const float& y, const float& xPadding, const float& yPadding, const s8& font, const int& fontSize, const std::string& text, TextureManager::TEX_TYPE tex, const int& layer = 0, const Vec3<float>& txtColor = { 1.0f,1.0f,1.0f });
	// Button with COLOR + text.
	void AddButtonToBatch(const BATCH_TYPE& id, const float& x, const float& y, const float& xPadding, const float& yPadding, const s8& font, const int& fontSize, const std::string& text, const int& layer = 0, const Vec4<float>& btnColor = { 1.0f,1.0f,1.0f,1.0f }, const Vec3<float>& txtColor = { 1.0f,1.0f,1.0f });
	/*************************************************************************/
}

