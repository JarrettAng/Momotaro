/*!************************************************************************
\file			RenderSystem.h
\author			Tan Jun Rong
\par DP email:	t.junrong@digipen.edu
\par Course:	CSD1171B
\par Software Engineering Project
\date			18-01-2023
\brief
This header file contains prototype functions and declarations for RenderSystem.cpp.

All content © 2023 DigiPen Institute of Technology Singapore.
All rights reserved.
**************************************************************************/

#pragma once
///////////////////////////////////////////////////////////////////////////
#include <list>
#include <vector>
#include <AEEngine.h>
#include <MomoMaths.h>
#include <string>
#include <TextureManager.h>
#include <iostream>
#include <algorithm>
#include <FontManager.h>
///////////////////////////////////////////////////////////////////////////

namespace RenderSystem {
	/*!***********************************************************************
	\brief
		Render setting object to affect subsequent object's graphics.
	*************************************************************************/
	struct RenderSetting {
		// Default: Opaque + Allow transperancy. (PNG images)
		static const AEGfxBlendMode BLEND_MODE{ AE_GFX_BM_BLEND };
		static constexpr Vec4<float> TINT{ 1.0f,1.0f,1.0f,1.0f };
		static constexpr Vec4<float> BLEND_COLOR{ 0,0,0,0 };

		AEGfxBlendMode blendMode = BLEND_MODE;
		Vec4<float> tint = TINT;
		Vec4<float> blendColor = BLEND_COLOR;

		/*!***********************************************************************
		\brief
			Reset setting to default.
		*************************************************************************/
		void setDefault() { blendMode = BLEND_MODE; blendColor = BLEND_COLOR; tint = TINT; }

		/*!***********************************************************************
		\brief
			Is setting defaulted?
		\return
			True / false.
		*************************************************************************/
		bool isDefault() { return blendMode == BLEND_MODE && blendColor == BLEND_COLOR && tint == TINT; }
	};

	/*!***********************************************************************
	\brief
		Type of batches in render system.
	*************************************************************************/
	enum BATCH_TYPE {
		TILE_BATCH = 0,
		GAME_PIECES_BATCH,
		UI_BATCH
	};

	/*!***********************************************************************
	\brief
		Render pivot of subsequent drawn object.
	*************************************************************************/
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

	/*!***********************************************************************
	\brief
		Stores transform information.
	*************************************************************************/
	struct Transform {
		Vec2<float> pos{};
		Vec2<float> cachedPos{};

		Vec2<float> size = { 1,1 };
		Vec2<float> cachedSize{};

		float rot = 0;
	};

	/*!***********************************************************************
	\brief
		Store graphics information.
	*************************************************************************/
	struct Graphics {
		Vec4<float> color;
		TextureManager::TEX_TYPE tex;
	};

	/*!***********************************************************************
	\brief
		Rect object that stores transform and graphics information.
	*************************************************************************/
	struct Rect {
		Transform transform{};
		Graphics graphics{};
	};

	/*!***********************************************************************
	\brief
		Text object.
	*************************************************************************/
	struct Text {
		s8 fontID;
		int fontSize;

		Vec2<float> pos{};
		Vec2<float> cachedPos{};

		std::string text;
		Vec3<float> color;
	};

	/*!***********************************************************************
	\brief
		Types of render object.
	*************************************************************************/
	enum RENDER_TYPE {
		RECT,
		TEXT
	};

	/*!***********************************************************************
	\brief
		Renderable object that the system used to renders.
	*************************************************************************/
	struct Renderable {
		RENDER_TYPE type{};
		Rect rect;
		Text text;
		int layer = 0;
	};

	/*!***********************************************************************
	\brief
		Interactable object for interective UI.
	*************************************************************************/
	struct Interactable {
		Renderable render;
		bool isActive = true;
		bool isClickable = true;
		void (*func)();
	};

	/*!***********************************************************************
	\brief
		Initialize RenderSystem.
	*************************************************************************/
	void Initialize();

	/*!***********************************************************************
	\brief
		Free RenderSystem.
	*************************************************************************/
	void Free();

	/*!***********************************************************************
	\brief
		Main rendering func.
			- Loop through all batches and render each renderable in batch.
	*************************************************************************/
	void Render();

	/*!***********************************************************************
	\brief
		Convert position based on current pivot position.
	*************************************************************************/
	Vec2<float> GetPivotPos(const Vec2<float>& pos, const float& width, const float& height);

	/*!***********************************************************************
	\brief
		Update current render setting to use for subsequent object when adding to batch.
	*************************************************************************/
	void SetRenderSetting(Vec4<float> tint, Vec4<float> blendColor = {}, AEGfxBlendMode blendMode = AE_GFX_BM_BLEND);

	/*!***********************************************************************
	* TEXT RENDERING
	*************************************************************************/
	void AddTextToBatch(const BATCH_TYPE& id, const float& x, const float& y, const s8& font, const int& fontSize, std::string text, const int& layer = 0, const Vec3<float>& color = { 1,1,1 });

	/*!***********************************************************************
	* RECT RENDERING
	*************************************************************************/
	// Rect with TEXTURE.
	void AddRectToBatch(const BATCH_TYPE& batch, const float& x, const float& y, const float& width, const float& height, TextureManager::TEX_TYPE tex, const int& layer = 0, const float& rot = 0);
	// Rect with COLOR
	void AddRectToBatch(const BATCH_TYPE& batch, const float& x, const float& y, const float& width, const float& height, const Vec4<float>& btnColor = { 1,1,1 ,1 }, const int& layer = 0, const float& rot = 0);

	/*!***********************************************************************
	* BUTTON RENDERING (RECT + TEXT IN MIDDLE OF RECT)
	*************************************************************************/
	// Button with TEXTURE + text.
	void AddButtonToBatch(const BATCH_TYPE& id, const float& x, const float& y, const float& xPadding, const float& yPadding, const s8& font, const int& fontSize, const std::string& text, TextureManager::TEX_TYPE tex, const int& layer = 0, const Vec3<float>& txtColor = { 1.0f,1.0f,1.0f });
	// Button with COLOR + text.
	void AddButtonToBatch(const BATCH_TYPE& id, const float& x, const float& y, const float& xPadding, const float& yPadding, const s8& font, const int& fontSize, const std::string& text, const int& layer = 0, const Vec4<float>& btnColor = { 1.0f,1.0f,1.0f,1.0f }, const Vec3<float>& txtColor = { 1.0f,1.0f,1.0f });
}
