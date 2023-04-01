/*!************************************************************************
\file:          FileIOManager.cpp
\author:
\par DP email:
\par Course:    CSD1171B
\par Software Engineering Project
\date:          30-01-2023
\brief


The functions include:
-
**************************************************************************/
///////////////////////////////////////////////////////////////////////////
#include <iostream>
#include <fstream>
#include <FileIOManager.h>
#include <BuildingManager.h>
#include <ScoreManager.h>
#include <DebugManager.h>
///////////////////////////////////////////////////////////////////////////

namespace FileIOManager {

	namespace GM = GridManager;
	namespace AM = AudioManager;
	namespace CM = CardManager;
	///////////////////////////////////////////////////////////////////////////
	//Helper function to save the current building data to file
	void SaveBuildingDataToFile(std::vector<BuildingData>& buildingData) {
		if (buildingData.empty()) {
			Debug::Print("No buildings to save from!\n");
			assert(0);
			return;
		}
		std::ofstream buildingFile("Assets/JSON_Data/buildings2.momodata");
		if (!buildingFile.is_open()) {
			Debug::Print("Unable to write to file!\n");
			assert(0);
		}
		// buildingFile << "Building Count " << BuildingManager::GetBuildingDataVector().size() <<'\n';
		buildingFile << '\n';
		for (BuildingData _data : buildingData) {
			// for(BuildingData _data : BuildingManager::GetBuildingDataVector()){
			buildingFile << "type " << (int)(_data.type) << '\n';
			buildingFile << "size " << _data.size.x << ' ' << _data.size.y << '\n';
			buildingFile << "level " << (int)(_data.level) << '\n';
			buildingFile << "synergy ";
			buildingFile << _data.SynergyResidential << ' ';
			buildingFile << _data.SynergyCommercial << ' ';
			buildingFile << _data.SynergyIndustrial << ' ';
			buildingFile << _data.SynergyNature << '\n';
			buildingFile << _data.name << '\n';
			buildingFile << _data.desc << '\n';
			buildingFile << "texture " << (int)(_data.TextureID) << '\n';
			buildingFile << '\n';
		}
		buildingFile.close();
	}
	///////////////////////////////////////////////////////////////////////////
	//
	void LoadBuildingDataFromFile(std::vector<BuildingData>& buildingsData) {
		std::ifstream buildingFile("Assets/JSON_Data/buildings.momodata");
		if (!buildingFile.is_open()) {
			Debug::Print("Unable to open Assets/JSON_Data/buildings.momodata!\n");
			assert(0);
		}
		std::string buffer{};
		int lineCount{ 0 };
		BuildingData _data{};
		while (std::getline(buildingFile, buffer)) {
			//Loop through each char 
			if (lineCount == 5) _data.name = buffer;
			if (lineCount == 6) _data.desc = buffer;
			for (int i{ 0 }; i < static_cast<int>(buffer.length()); ++i) {
				if (isdigit(buffer[i])) {
					//This is to explicitly tell the compiler, hey! I know what I'm doing with the indexes
					int _residential = i - 1;
					int _commercial = i + 2;
					int _industrial = i + 4;
					int _nature = i + 6;
					switch (lineCount)
					{
					case 1://TYPE
						_data.type = (BuildingEnum::TYPE)(std::atoi(&buffer[i]));
						break;
					case 2://SIZE
						if (!_data.size.x)_data.size.x = std::atoi(&buffer[i]);
						if (_data.size.x)_data.size.y = std::atoi(&buffer[i]);
						break;
					case 3://LEVEL
						_data.level = (BuildingEnum::LEVEL)(std::atoi(&buffer[i]));
						break;
					case 4://SYNERGY

						if (!_data.SynergyResidential) {
							_data.SynergyResidential = std::atoi(&buffer[_residential]);
							_data.SynergyCommercial = std::atoi(&buffer[_commercial]);
							_data.SynergyIndustrial = std::atoi(&buffer[_industrial]);
							_data.SynergyNature = std::atoi(&buffer[_nature]);
							break;
						}
						break;
					case 5://NAME	
						// _data.name = buffer;						
						break;
					case 6://DESC							
						// _data.desc = buffer;						
						break;
					case 7://TEXTURE							
						_data.TextureID = (TextureManager::TEX_TYPE)(std::atoi(&buffer[i]));
						buildingsData.push_back(_data);
						_data = BuildingData{};
						lineCount = -1;
						break;
					}
				}
			}
			lineCount++;
		}
		buildingFile.close();
	}
	///////////////////////////////////////////////////////////////////////////
	//Saves the map to a custom file (used by gridmanager)
	void SaveGridToFile(std::string filePath) {
		std::ofstream mapFile(filePath);
		if (!mapFile.is_open()) {
			Debug::Print("Unable to write to file!\n");
			assert(0);
		}
		
		mapFile << "Width " << GridManager::gridX << '\n';
		mapFile << "Height " << GridManager::gridY << '\n';
		mapFile << "Score " << ScoreManager::GetHighScore() << ",";
		mapFile << ScoreManager::GetCurrScore() << ",";
		mapFile << ScoreManager::GetLevel() << '\n';
		mapFile << "Joe" << '\n';
		for (int y{ 0 }; y < GridManager::gridY; ++y) {
			for (int x{ 0 }; x < GridManager::gridX; ++x) {
				//If the tile is a placeable tile, we meed to check if it has a nature tile and store that
				const GridManager::cell* _temp = (GridManager::GetGrid() + GridManager::GetIndex(Vec2<int>{x, y}));
				if(_temp->isRenderable){
				//We have to increment the ID by 1 to represent that it is renderable.
					mapFile << (_temp->ID+1);
					if(_temp->ID>0){
						mapFile << "," << (int)(_temp->_building.data.type);	//[0,4]
						mapFile << "," << (int)(_temp->_building.data.level);	//[0,2]
					}
					// if((GM::GetGrid()+GM::GetIndex(Vec2<int>{x,y}))->_building.data.type != BuildingEnum::NATURE){
					// 	mapFile << 1;
					// } else mapFile << 2;
				} else mapFile << 0;	//if not, the tile is probably not renderable anyway
				if (x < GridManager::gridX - 1) mapFile << ' ';
			}
			mapFile << '\n';
		}
		mapFile.close();
	}
	void ConvertMapFile(){
		std::ifstream inputFile("Assets/JSON_Data/Maps/tutorial2.momomaps");
		std::ofstream outputFile("Assets/JSON_Data/Maps/tutorial_2.momomaps");
		std::string buffer;
		int id{1};
		while(std::getline(inputFile,buffer)){
			for(int i{0}; i< (int)(buffer.length()); ++i){
				if(buffer[i] == '2'){
					outputFile << ++id << ',';
					outputFile << (int)(BuildingEnum::NATURE) << ',';
					outputFile << (int)(BuildingEnum::L1) << ' ';
				} else outputFile << buffer[i];
				if(i == buffer.length()-1) outputFile << '\n';
			}
		}
		inputFile.close();
		outputFile.close();
		// std::ofstream outputFile(fileName);
	}

	std::vector<BuildingData> LoadHandFromFile(){
		std:: fstream inputFile("Assets/JSON_Data/Maps/Save.momohand");
		std::vector<BuildingData> _temp;
		if(!inputFile.is_open()){
			Debug::Print("Unable to read Assets/JSON_Data/Maps/Save.momohand\n");
		}
		std::string buffer;
		while(std::getline(inputFile,buffer)){
			//First we get each line of the hand file
			_temp.push_back(BuildingManager::GetBuildingData(
				(BuildingEnum::TYPE)(std::stoi(buffer)),
				Vec2<int>{1,1},
				(BuildingEnum::LEVEL)(std::atoi(&buffer[2]))
			));
		}
		inputFile.close();
		if(_temp.empty()) return _temp;
		return _temp;
	}
	void SaveHandToFile(std::vector<Card> const& _hand){
		std:: ofstream outputFile("Assets/JSON_Data/Maps/Save.momohand");
		if(!outputFile.is_open()){
			Debug::Print("Unable to read Assets/JSON_Data/Maps/Save.momohand!\n");
		}
		if(_hand.empty()){
			outputFile.close();
			Debug::Print("File IO Saved Hand is empty! Check if game is over!\n");
			return;
		}
		for(Card _card : _hand){
			for(int i{}; i< _card.count; ++i){
				outputFile << (int)(_card.bData.type) <<",";
				outputFile << (int)(_card.bData.level) << '\n';
			}
		}
		outputFile.close();
	}
	///////////////////////////////////////////////////////////////////////////
	//Loads a map from the specified filename (used in gridmanager)
	GM::cell* LoadGridFromFile(std::string fileName)
	{
		// ConvertMapFile();
		GM::cell* newMap{};
		std::fstream inputFile(fileName);
		if (!inputFile.is_open()) {
			Debug::Print("Unable to read" + fileName + "\n");
			// assert(0);
		}
		std::string buffer; 		//where each line of text will be stored
		GM::gridX = GM::gridY = 0;

		int lineCount{};
		int maxID{};
		while (std::getline(inputFile, buffer)) {
			int xIndex = 0;	//Since each line has a spacing, we only want to count the xIndex when we get a number
			//First we loop through each line and grab the numbers
			for (int i{ 0 }; i < static_cast<int>(buffer.length()); ++i) {
				int _level = i+2;
				if (isdigit(buffer[i]))
				{
					//The first 2 lines will be the width and the height
					if (lineCount == 0 && GM::gridX == 0) GM::gridX = std::atoi(&buffer[i]);
					if (lineCount == 1 && GM::gridY == 0) GM::gridY = std::atoi(&buffer[i]);
					// 	//since we got the width and height of the map, we can generate it
					if (lineCount == 1 && (GM::gridY > 0 && GM::gridX > 0)) {
						if (!newMap) {
							newMap = { new GM::cell[GM::gridX * GM::gridY]{} };
						}
					}
					if(lineCount == 2) {
						ScoreManager::SetHighScore(std::stoi(&buffer[i]));
						if(std::stoi(&buffer[i]) == 0){
							i+=2;
						} else i += (int)(log10f(std::stoi(&buffer[i]))+2);
						ScoreManager::SetScore(std::stoi(&buffer[i]));
						if(std::stoi(&buffer[i]) == 0){
							i+=2;
						} else i += (int)(log10f(std::stoi(&buffer[i]))+2);
						ScoreManager::SetLevel(std::stoi(&buffer[i]));
					}
					//Once the line count exceeds 3, meaning we already have the width&height, we can start to add to array
					if (lineCount > 3) {
						//If the value in the mapdata is NOT 1, it shall be treated as a 0 (which means it's renderable)
						newMap[GM::GetIndex(xIndex, lineCount - 4)].isRenderable = std::atoi(&buffer[i]);
						//Now that we know it's renderable, if it's > 1 it's either probabilisitic OR Nature.
							if(std::atoi(&buffer[i])>1){
								//First we get the ID
								maxID = max(std::stoi(&buffer[i])-1,maxID);
								newMap[GM::GetIndex(xIndex, lineCount - 4)].ID = (std::stoi(&buffer[i])-1);
								//Then we need to move the index by however many multiples of 10 the ID is
								i+=(int)(log10f(std::stoi(&buffer[i])));
								//After getting to the end of the number, we should hit the ','
								if(buffer[i+=1] == ','){
									//Then we store the type and level
									switch((BuildingEnum::TYPE)(std::stoi(&buffer[i+=1]))){
										case BuildingEnum::NATURE:
										newMap[GM::GetIndex(xIndex, lineCount - 4)] = GM::NatureCell();
										i+=2;
										break;
										default:
										newMap[GM::GetIndex(xIndex, lineCount - 4)]._building.data =
										BuildingManager::GetBuildingData(
											(BuildingEnum::TYPE)(std::stoi(&buffer[i])),
											Vec2<int>{1,1},
											(BuildingEnum::LEVEL)(std::atoi(&buffer[i+=2]))
										);
										newMap[GM::GetIndex(xIndex, lineCount - 4)]._building.buildingCells = std::vector<Vec2<int>>{ Vec2<int>{xIndex,lineCount-4}};
										// std::cout << std::atoi(&buffer[i+2]) << '\n';
										break;
									}

								}

							}
							//assuming right now we only have nature tiles and not probabilistic. 
							//gotta do some switch case here in future!
							//TODO Future implementation for rando stuff
						}
						xIndex++;
				}
			}
			lineCount++;		//Linecount-2 is basically our yIndex
		}
		GridManager::SetBuildingID(maxID);
		inputFile.close();
		return newMap;
	}

    void LoadAudioClipsFromFile(std::array<AM::AudioClip, AM::ClipName::MAX_COUNT> &audioData)
    {
		//Note: to load the sounds, just put in the name of the enum and the volume for it
		audioData[AM::ClipName::BGM_MAIN]=AM::AudioClip{ 
			AEAudioLoadMusic("Assets/Audio/GameMenu.wav"),
			0.25f, AM::BGM_GROUP};
		audioData[AM::ClipName::BGM_GAME]=AM::AudioClip{ 
			AEAudioLoadMusic("Assets/Audio/GameLevel.wav"),
			1.0f,AM::BGM_GROUP};
		audioData[AM::ClipName::BGM_CREDITS]=AM::AudioClip{ 
			AEAudioLoadMusic("Assets/Audio/CreditsLoop.wav"),
			.25f,AM::BGM_GROUP};
		audioData[AM::ClipName::SFX_CLICK]=AM::AudioClip{ 
			AEAudioLoadSound("Assets/Audio/MouseClick.wav"),
			0.10f,AM::SFX_GROUP};
		audioData[AM::ClipName::SFX_MERGE1]=AM::AudioClip{ 
			AEAudioLoadSound("Assets/Audio/MergeSFX1.wav"),
			.30f,AM::SFX_GROUP};
		audioData[AM::ClipName::SFX_MERGE2]=AM::AudioClip{ 
			AEAudioLoadSound("Assets/Audio/MergeSFX2.wav"),
			1.0f,AM::SFX_GROUP};
		audioData[AM::ClipName::SFX_GAMEOVER]=AM::AudioClip{ 
			AEAudioLoadSound("Assets/Audio/BuildFinished.wav"),
			1.0f,AM::SFX_GROUP};
		audioData[AM::ClipName::SFX_GAINPOINT]=AM::AudioClip{ 
			AEAudioLoadSound("Assets/Audio/PointGain.wav"),
			1.0f,AM::SFX_GROUP};
		audioData[AM::ClipName::SFX_LOSEPOINT]=AM::AudioClip{ 
			AEAudioLoadSound("Assets/Audio/GameLose.wav"),
			1.0f,AM::SFX_GROUP};
		audioData[AM::ClipName::SFX_THRESHOLD]=AM::AudioClip{ 
			AEAudioLoadSound("Assets/Audio/ThresholdUp.wav"),
			1.0f,AM::SFX_GROUP};
    }

    ///////////////////////////////////////////////////////////////////////////
    //*TODO : SERIALISATION FOR BUILDINGS WITH CUSTOM FILETYPES!
    void ReadBuildingsData(std::vector<BuildingData>& buildingsData) {
		std::ifstream dataFile{ "Assets/JSON_Data/buildingsData.json" };
		// json data = json::parse(dataFile);
		if (!dataFile) {
			std::string debugLog;
			Debug::Print(debugLog + "Error " + __FILE__ + "ln" + std::to_string(__LINE__) + ": Unable to read buildings data JSON file!\n");
		}

		// BuildingData testBuilding{};
		// from_json(data,testBuilding);
		// std::cout << "=====TEST BUILDING=====\n";
		// std::cout << testBuilding << '\n';
		// dataFile.close();

// return;

		// Store data in the buildingsData vector (array)
		// Temp only!! To be replaced with actual reading of the JSON file on top ^^
		BuildingData newBuilding{
			BuildingEnum::RESIDENTIAL,
			Vec2<int>{1,1},
			BuildingEnum::L1,
			BuildingEnum::RIGHT,
			1, 3, -5, 1,
			"Small house", "Starter red house!",
			TextureManager::RESIDENTIAL_1X1_L1 };
		buildingsData.push_back(newBuilding);
		BuildingData newBuilding2{
			 BuildingEnum::RESIDENTIAL,
			 Vec2<int>{1,1},
			 BuildingEnum::L2,
			 BuildingEnum::RIGHT,
			 1, 3, -5, 1,
			 "Terrace house", "Bigger blue house with some lovely flowers.",
			 TextureManager::RESIDENTIAL_1X1_L2 };
		buildingsData.push_back(newBuilding2);
		BuildingData newBuilding3{
			BuildingEnum::RESIDENTIAL,
			Vec2<int>{1,1},
			BuildingEnum::L3,
			BuildingEnum::RIGHT,
			1, 3, -5, 1,
			"Shiny house", "For the rich people, we made your roof shine too.",
			TextureManager::RESIDENTIAL_1X1_L3 };
		buildingsData.push_back(newBuilding3);

		//BuildingData newBuilding4{ RESIDENTIAL, _2X1, L1, -1, 5, -8, 3, "Small house", "Small lovely house", 010, 000 };
		//buildingsData.push_back(newBuilding4);
		//BuildingData newBuilding5{ RESIDENTIAL, _2X1, L2, -1, 5, -8, 3, "Small house", "Small lovely house", 011, 000 };
		//buildingsData.push_back(newBuilding5);
		//BuildingData newBuilding6{ RESIDENTIAL, _2X1, L3, -1, 5, -8, 3, "Small house", "Small lovely house", 012, 000 };
		//buildingsData.push_back(newBuilding6);

		BuildingData newBuilding7{
			BuildingEnum::COMMERCIAL,
			Vec2<int>{1,1},
			BuildingEnum::L1,
			BuildingEnum::RIGHT,
			3, -1, 1, 1,
			"Street Stall","Can use your CDC vouchers here",
			TextureManager::COMMERCIAL_1X1_L1 };
		buildingsData.push_back(newBuilding7);
		BuildingData newBuilding8{
			BuildingEnum::COMMERCIAL,
			Vec2<int>{1,1},
			BuildingEnum::L2,
			BuildingEnum::RIGHT,
			3, -1, 1, 1,
			"Small Shop", "Lovely shop with lovely goods",
			TextureManager::COMMERCIAL_1X1_L2 };
		buildingsData.push_back(newBuilding8);
		BuildingData newBuilding9{
			BuildingEnum::COMMERCIAL,
			Vec2<int>{1,1},
			BuildingEnum::L3,
			BuildingEnum::RIGHT,
			3, -1, 1, 1,
			"Big Shophouse", "Big shophouse which sells many goods",
			TextureManager::COMMERCIAL_1X1_L3 };
		buildingsData.push_back(newBuilding9);

		//BuildingData newBuilding10{ COMMERCIAL, _2X1, L1, 5, 0, -5, 1, "Small house", "Small lovely house", 110, 000 };
		//buildingsData.push_back(newBuilding10);
		//BuildingData newBuilding11{ COMMERCIAL, _2X1, L2, 5, 0, -5, 1, "Small house", "Small lovely house", 111, 000 };
		//buildingsData.push_back(newBuilding11);
		//BuildingData newBuilding12{ COMMERCIAL, _2X1, L3, 5, 0, -5, 1, "Small house", "Small lovely house", 112, 000 };
		//buildingsData.push_back(newBuilding12);

		BuildingData newBuilding13{
			BuildingEnum::INDUSTRIAL,
			Vec2<int>{1,1},
			BuildingEnum::L1,
			BuildingEnum::RIGHT,
			-5, 1, 5, 0,
			"Storage Box", "A small boxy building to store boxes.",
			TextureManager::INDUSTRIAL_1X1_L1 };
		buildingsData.push_back(newBuilding13);
		BuildingData newBuilding14{
			BuildingEnum::INDUSTRIAL,
			Vec2<int>{1,1},
			BuildingEnum::L2,
			BuildingEnum::RIGHT,
			-5, 1, 5, 0,
			"Big Lab", "Discover the secrets of life here. Hint: It's ligma.",
			TextureManager::INDUSTRIAL_1X1_L2 };
		buildingsData.push_back(newBuilding14);
		BuildingData newBuilding15{
			BuildingEnum::INDUSTRIAL,
			Vec2<int>{1,1},
			BuildingEnum::L3,
			BuildingEnum::RIGHT,
			-5, 1, 5, 0,
			"Power Plant", "Powered by the dead.",
			TextureManager::INDUSTRIAL_1X1_L3 };
		buildingsData.push_back(newBuilding15);

		//BuildingData newBuilding16{ INDUSTRIAL, _2X2, L1, -8, -5, 8, 0, "Small house", "Small lovely house", 210, 000 };
		//buildingsData.push_back(newBuilding16);
		//BuildingData newBuilding17{ INDUSTRIAL, _2X2, L2, -8, -5, 8, 0, "Small house", "Small lovely house", 211, 000 };
		//buildingsData.push_back(newBuilding17);
		//BuildingData newBuilding18{ INDUSTRIAL, _2X2, L3, -8, -5, 8, 0, "Small house", "Small lovely house", 212, 000 };
		//buildingsData.push_back(newBuilding18);

		dataFile.close();
	}
}
