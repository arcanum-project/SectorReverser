//
//  main.cpp
//  SectorReverser
//
//  Created by Dmitrii Belousov on 8/8/22.
//

#include <stdio.h>
#include <fstream>
#include <vector>
#include <iostream>
#include <unordered_map>
#include "boost/algorithm/string.hpp"
#include "boost/json/src.hpp"
#include <string>

const std::unordered_map<uint32_t, std::string> outdoorFlippable = std::unordered_map<uint32_t, std::string> {
  // Dirt
  std::make_pair(0, std::string("drt")),
  // Grass
  std::make_pair(1, std::string("grs")),
  std::make_pair(2, std::string("rck")),
  std::make_pair(3, std::string("snw")),
  std::make_pair(4, std::string("dg1")),
  std::make_pair(5, std::string("bg1")),
  std::make_pair(6, std::string("bg2")),
  std::make_pair(7, std::string("bog")),
  std::make_pair(8, std::string("sw1")),
  std::make_pair(9, std::string("sw2")),
  std::make_pair(10, std::string("sw3")),
  std::make_pair(11, std::string("swr")),
  std::make_pair(12, std::string("dwr")),
  std::make_pair(13, std::string("rok")),
  std::make_pair(14, std::string("WK1")),
  std::make_pair(15, std::string("WK2")),
  std::make_pair(16, std::string("ST1")),
  std::make_pair(17, std::string("WD3")),
  std::make_pair(18, std::string("Dst")),
  std::make_pair(19, std::string("ssd")),
  std::make_pair(20, std::string("dIs")),
  std::make_pair(21, std::string("Jdt")),
  std::make_pair(22, std::string("Jgr")),
  std::make_pair(23, std::string("SxW")),
  std::make_pair(24, std::string("sod")),
  std::make_pair(25, std::string("TS1")),
  std::make_pair(26, std::string("TS2")),
  std::make_pair(27, std::string("TDt")),
  std::make_pair(28, std::string("Blk")),
  std::make_pair(29, std::string("VDR")),
  std::make_pair(30, std::string("FOG"))
};

const std::unordered_map<uint32_t, std::string> outdoorNonFlippable = std::unordered_map<uint32_t, std::string> {
  std::make_pair(0, std::string("TST")),
  std::make_pair(1, std::string("Prh")),
  std::make_pair(2, std::string("Dks")),
  std::make_pair(3, std::string("TLa")),
  std::make_pair(4, std::string("VWK")),
  std::make_pair(5, std::string("ICE"))
};

const std::unordered_map<uint32_t, std::string> indoorFlippable = std::unordered_map<uint32_t, std::string> {
  std::make_pair(0, std::string("fl1")),
  std::make_pair(1, std::string("wod"))
};

const std::unordered_map<uint32_t, std::string> indoorNonFlippable = std::unordered_map<uint32_t, std::string> {
  std::make_pair(0, std::string("Dn1")),
  std::make_pair(1, std::string("WI1")),
  std::make_pair(2, std::string("Dn2")),
  std::make_pair(3, std::string("CF1")),
  std::make_pair(4, std::string("WD1")),
  std::make_pair(5, std::string("Sew")),
  std::make_pair(6, std::string("WD2")),
  std::make_pair(7, std::string("ILL")),
  std::make_pair(8, std::string("Dir")),
  std::make_pair(9, std::string("cf2")),
  std::make_pair(10, std::string("STD")),
  std::make_pair(11, std::string("VFL")),
  std::make_pair(12, std::string("JIN"))
};

const std::unordered_map<uint32_t, std::string> edges = std::unordered_map<uint32_t, std::string> {
  std::make_pair(0, std::string("drtgrs")),
  std::make_pair(1, std::string("drtrck")),
  std::make_pair(2, std::string("rcksnw")),
  std::make_pair(3, std::string("drtdg1")),
  std::make_pair(4, std::string("drtbg1")),
  std::make_pair(5, std::string("bg1bg2")),
  std::make_pair(6, std::string("bg2bog")),
  std::make_pair(7, std::string("drtsw1")),
  std::make_pair(8, std::string("sw1sw2")),
  std::make_pair(9, std::string("sw2sw3")),
  std::make_pair(10, std::string("sw3swr")),
  std::make_pair(11, std::string("swrdwr")),
  std::make_pair(12, std::string("drtrok")),
  std::make_pair(13, std::string("drtWK1")),
  std::make_pair(14, std::string("drtWK2")),
  std::make_pair(15, std::string("drtST1")),
  std::make_pair(16, std::string("drtWD3")),
  std::make_pair(17, std::string("drtDst")),
  std::make_pair(18, std::string("swrssd")),
  std::make_pair(19, std::string("ssddIs")),
  std::make_pair(20, std::string("dIsJdt")),
  std::make_pair(21, std::string("JdtJgr")),
  std::make_pair(22, std::string("drtsod")),
  std::make_pair(23, std::string("drtSxW")),
  std::make_pair(24, std::string("drtTS1")),
  std::make_pair(25, std::string("TS1TS2")),
  std::make_pair(26, std::string("drtTDt")),
  std::make_pair(27, std::string("drtBlk")),
  std::make_pair(28, std::string("drtTST")),
  std::make_pair(29, std::string("drtPrh")),
  std::make_pair(30, std::string("dwrDks")),
  std::make_pair(31, std::string("dIsTLa")),
  std::make_pair(32, std::string("DrtFOG")),
  std::make_pair(33, std::string("DrtVDR")),
  std::make_pair(34, std::string("VDRVWK")),
  std::make_pair(35, std::string("SnwICE"))
};

/**
 Read up to 4 bytes starting from a specified position in a file.
 
 Note: this method moves the file pointer by a number of bytes read
 
 @param file - file to read from
 @param start - position in a file to read from inclusively
 @param length - number of bytes to read.
 @returns 4-byte unsigned integer
 */
uint32_t readUpTo4Bytes(std::ifstream & file, const uint32_t start, const uint32_t length) {
  file.seekg(start, std::ios::beg);
  std::vector<uint8_t> container = std::vector<uint8_t>(length);
  file.read(reinterpret_cast<char*>(container.data()), length);
  uint32_t result {};
  memcpy(& result, container.data(), length);
  return result;
}

const std::string getOutdoorFlippableArtPrefix(const uint32_t & tileId) {
  const std::unordered_map<uint32_t, std::string>::const_iterator pIter = outdoorFlippable.find(tileId);
  if (pIter != outdoorFlippable.end()) {
	std::string artPrefix = pIter->second + "bse";
	boost::algorithm::to_lower(artPrefix);
	return artPrefix;
  } else {
	throw std::runtime_error("Couldn't map tileId1 to art prefix.");
  }
}

const std::string getOutdoorFlippableEdgeArtPrefix(const uint32_t & tileId1, const uint32_t & tileId2) {
  std::string artPrefix1 {};
  std::string artPrefix2 {};
  const std::unordered_map<uint32_t, std::string>::const_iterator pIter1 = outdoorFlippable.find(tileId1);
  if (pIter1 != outdoorFlippable.end()) {
	artPrefix1 = pIter1->second;
	boost::algorithm::to_lower(artPrefix1);
  } else {
	throw std::runtime_error("Couldn't map tileId1 to art prefix.");
  }
  
  const std::unordered_map<uint32_t, std::string>::const_iterator pIter2 = outdoorFlippable.find(tileId2);
  if (pIter2 != outdoorFlippable.end()) {
	artPrefix2 = pIter2->second;
	boost::algorithm::to_lower(artPrefix2);
  } else {
	throw std::runtime_error("Couldn't map tileId2 to art prefix.");
  }
  
  const std::string artPrefix = artPrefix1 + artPrefix2;
  return artPrefix;
}

uint32_t shr1C(uint32_t val) {
  return val >> 0x1C;
}

int main(int argc, const char * argv[]) {
  std::ifstream file("Resources/86570436012.sec", std::ifstream::in);
  const uint16_t numTilesInSector = 4096;
  boost::json::array tiles;
  tiles.reserve(numTilesInSector);
  if (file.is_open()) {
	try {
	  uint16_t currentTile {1};
	  // Read all 4096 tiles from sector
	  while (currentTile <= numTilesInSector) {
		uint32_t tile = readUpTo4Bytes(file, currentTile * 4, 4);
		const uint32_t originalTile = tile;
		uint32_t leastSignificantBit {0};
		uint32_t shiftedTile = shr1C(tile);
		// Default tile if unknown
		std::string art {"drttst9a_0"};
		if (shiftedTile <= 0xE) {
		  if (shiftedTile != 0x9) {
			if (shiftedTile != 0xB) {
			  if (shiftedTile == 0x0) {
				tile = tile & 0xFFFFFFF0;
				uint32_t tileId1 = (tile >> 0x16) & 0x3F;
				uint32_t tileId2 = (tile >> 0x10) & 0x3F;
				uint32_t shiftedTile3 = (tile >> 0xC) & 0xF;
				uint32_t char2Value = (tile >> 0x9) & 0x7;
				uint32_t isOutdoor = (tile >> 0x8) & 0x1;
				uint32_t isFlippable = (tile >> 0x7) & 0x1;
				uint32_t shiftedTile5 = (tile >> 0x6) & 0x1;
				if ((isFlippable & isFlippable) != 0x0) {
				  leastSignificantBit = originalTile & 0x0000000F;
				  if ((isOutdoor & isOutdoor) != 0x0) {
					if ((shiftedTile5 & shiftedTile5) != 0x0) {
					  if (tileId2 < 0x1F) {
						char2Value = char2Value < 0x8 ? char2Value : char2Value - 0x8;
						if (shiftedTile3 != 0xF) {
						  const std::string char1Str = std::string("06b489237ea5dc10");
						  uint32_t char2 = char2Value + 0x61;
						  uint32_t char1 = char1Str.at(shiftedTile3);
						  char char1Ascii = static_cast<char>(char1);
						  char char2Ascii = static_cast<char>(char2);
						  if (tileId1 == tileId2) {
							try {
							  // For this type of a tile tileId1 == tileId2, so it doesnt matter which one we use
							  const std::string artPrefix = getOutdoorFlippableArtPrefix(tileId1);
							  art = artPrefix + char1Ascii + char2Ascii + "_0";
							} catch (std::system_error & e) {
							  std::cout << e.code().message() << " tileId1 = " << tileId1 << ". Original tile = " << std::hex << originalTile << std::endl;
							}
						  } else {
							try {
							  const std::string artPrefix = getOutdoorFlippableEdgeArtPrefix(tileId1, tileId2);
							  art = artPrefix + char1Ascii + char2Ascii + "_0";
							} catch (std::system_error & e) {
							  std::cout << e.code().message() << " tileId1 = " << tileId1 << ". tileId2 = " << tileId2 << ". Original tile = " << std::hex << originalTile << std::endl;
							}
						  }
						} else {
						  std::cout << "shiftedTile3 is = 0xF. Original tile = " << std::hex << originalTile << std::endl;
						}
					  } else {
						std::cout << "tileId2 is >= 0x1F. Original tile = " << std::hex << originalTile << std::endl;
					  }
					} else {
					  std::cout << "shiftedTile5 == 0x0. Original tile = " << std::hex << originalTile << std::endl;
					}
				  } else {
					std::cout << "Tile is not outdoor. Original tile = " << std::hex << originalTile << std::endl;
				  }
				} else {
				  std::cout << "Tile is not flippable. Original tile = " << std::hex << originalTile << std::endl;
				}
			  } else {
				std::cout << "Shifted tile is != 0x0. Original tile = " << std::hex << originalTile << std::endl;
			  }
			} else {
			  std::cout << "Shifted tile is = 0xB. Original tile = " << std::hex << originalTile << std::endl;
			}
		  } else {
			std::cout << "Shifted tile is = 0x9. Original tile = " << std::hex << originalTile << std::endl;
		  }
		} else {
		  std::cout << "Shifted tile is > than 0xE. Original tile = " << std::hex << originalTile << std::endl;
		}
		
		
		boost::json::object tileObj;
		tileObj["instanceId"] = currentTile - 1;
		tileObj["textureName"] = art;
		tileObj["shouldFlip"] = (leastSignificantBit & leastSignificantBit) != 0x0 ? "1" : "0";
		tiles.push_back(tileObj);
		
		++currentTile;
	  }
	} catch (std::system_error & e) {
	  if (file.is_open())
		file.close();
	  std::cerr << e.code().message() << std::endl;
	  throw;
	}
  }
  file.close();
  
  boost::json::object json;
  json["tiles"] = tiles;
  std::ofstream outFile("/opt/projects/SectorReverser/SectorReverser/Resources/86570436012", std::ofstream::out);
  if (outFile.is_open()) {
	try {
	  outFile << json << std::endl;
	  outFile.flush();
	} catch (std::system_error & e) {
	  if (outFile.is_open())
		outFile.close();
	  std::cerr << e.code().message() << std::endl;
	  throw;
	}
	outFile.close();
  }
  
  return 0;
}
