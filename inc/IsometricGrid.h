#pragma once
namespace IsometricGrid
{
    struct vec2i{
        int x{0};
        int y{0};
    };
    struct cell{
		vec2i pos;
        int ID{ 0 };
		enum TileType{
            NONE = 0,
			RESIDENTIAL = 1,
			INDUSTRIAL = 2,
			COMMERCIAL = 3,
			NATURE = 99,
		};
	};

    class GridManager{
    private:
	vec2i gridSize;

    public:
	GridManager(int width,int height){gridSize.x = width; gridSize.y = height;}
    vec2i GetGridSize();
    void Update(int mouseX,int mouseY);
    

   };
    /// @brief Return 
    /// @param x1 
    /// @param y1 
    /// @param x2 
    /// @param y2 
    /// @param x3 
    /// @param y3 
    /// @return 
    float area(int x1, int y1, int x2, int y2, int x3, int y3);

    /// @brief 
    /// @param _mouseX 
    /// @param _mouseY 
    /// @param x1 
    /// @param y1 
    /// @param x2 
    /// @param y2 
    /// @param x3 
    /// @param y3 
    /// @return 
    bool isInside(int _mouseX, int _mouseY, int x1, int y1, int x2, int y2, int x3, int y3);


    /// @brief Converts World Cell Index to Screen Position.
    /// @param x x index of the world cell
    /// @param y y index of the world cell
    /// @return Screen position in the form of vec2i
    vec2i WorldIndexToScreenPos(int x, int y);
    vec2i ScreenPosToIso(int x, int y);

    
    vec2i MouseToCell(int mouseX, int mouseY);
} // namespace IsometricGrid