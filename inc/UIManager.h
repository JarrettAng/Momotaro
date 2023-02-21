//#pragma once
//#include <AEEngine.h>
//#include <MomoMaths.h>
//#include <string>
//
//namespace UIManager {
//
//	void Initialize();
//	void Texture();
//	void MakeMesh();
//	void DrawUI();
//	void Draw();

//	struct Transform {
//		float x, y;
//		float width, height;
//	};
//
//	struct GraphicsData {
//		bool hasGraphics = false;
//		Vec4<float> color;
//		AEGfxTexture* tex = nullptr;
//	};
//
//	struct TextData {
//		bool hasText = false;
//		s8 fontID;
//		float x;
//		float y;
//		std::string text;
//		float scale;
//		Vec3<float> color;
//	};
//
//	struct UIData {
//		bool isActive;
//		Transform transform;
//		GraphicsData graphics;
//		TextData text;
//		int layer;
//		void(*func)(void);
//	};
//
//	//enum FONT_SIZE {
//	//	SMALL,
//	//	MIDIUM,
//	//	LARGE
//	//};
//
//	//enum FONT_TYPE {
//	//	NONE,
//	//	ROBOTO
//	//};
//
//	//struct FONT {
//	//	FONT_TYPE type;
//	//	s8 S;
//	//	s8 M;
//	//	s8 L;
//	//};
//
//	void Initialize();
//	/*FONT GetFont(const FONT_TYPE& type);*/
//	void TransformDataToUIData(UIData& data, const float& x, const float& y, const float& width, const float& height);
//	void GraphicsDataToUIData(UIData& data, const float& x, const float& y, const float& width, const float& height);
//	/*!***********************************************************************
//	* TEXT
//	*************************************************************************/
//	void AddTextToBatch(const s8& font, const float& x, const float& y, const int& layer, std::string text, const Vec3<float>& color = { 1.0f,1.0f,1.0f });
//	/*************************************************************************/
//
//
//	/*!***********************************************************************
//	* RECT
//	*************************************************************************/
//	// Button with TEXTURE + NO text.
//	void AddRectToBatch(const float& x, const float& y, const float& width, const float& height, const int& layer, AEGfxTexture* tex);
//	// Button with COLOR + NO text.
//	void AddRectToBatch(const float& x, const float& y, const float& width, const float& height, const int& layer, const Vec4<float>& btnColor = { 1.0f,1.0f,1.0f,1.0f });
//	/*************************************************************************/
//
//
//	/*!***********************************************************************
//	* BUTTON
//	*************************************************************************/
//	// Button with TEXTURE + text.
//	void AddButtonToBatch(const float& x, const float& y, const float& xPadding, const float& yPadding, const int& layer, const s8& font, const std::string& text, AEGfxTexture* tex, const Vec3<float>& txtColor = { 1.0f,1.0f,1.0f });
//	// Button with COLOR + text.
//	void AddButtonToBatch(const float& x, const float& y, const float& xPadding, const float& yPadding, const int& layer, const s8& font, const std::string& text, const Vec4<float>& btnColor = { 1.0f,1.0f,1.0f,1.0f }, const Vec3<float>& txtColor = { 1.0f,1.0f,1.0f });
//	/*************************************************************************/
//}

