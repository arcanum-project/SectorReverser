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

/**
 Read up to 4 bytes starting from a specified position in a file.
 
 Note: this method moves the file pointer by a number of bytes read
 
 @param file - file to read from
 @param start - position in a file to read from inclusively
 @param length - number of bytes to read.
 @returns 4-byte unsigned integer
 */
const uint32_t readUpTo4Bytes(std::ifstream & file, const uint32_t start, const uint32_t length) {
  file.seekg(start, std::ios::beg);
  std::vector<uint8_t> container = std::vector<uint8_t>(length);
  file.read(reinterpret_cast<char*>(container.data()), length);
  uint32_t result {};
  memcpy(& result, container.data(), length);
  return result;
}

const bool isOutdoor(const uint32_t & tile) {
  return tile >> 8 & 1;
}

const bool isFlippable(const uint32_t & tile) {
  return tile >> 7 & 1;
}

const uint32_t getTileId(const uint32_t & tile) {
  return tile >> 22 & 0x3F;
}

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

const std::string getArtPrefix(const uint32_t & tile) {
  const uint32_t tileId = getTileId(tile);
  const bool isTileOutdoor = isOutdoor(tile);
  const bool isTileFlippable = isFlippable(tile);
  // Take the last byte of tile to check if it is an edge tile. C0 = normal, C1 = edge.
  const uint8_t tileType = (uint8_t) tile;
  std::unordered_map<uint32_t, std::string> tilesOfProperType {};
  if (tileType == 0xC0) {
	std::cout << "it's a normal tile. Tile: " << std::hex << tile << std::endl;
	if (isTileOutdoor) {
	  if (isTileFlippable) {
		tilesOfProperType = outdoorFlippable;
	  } else {
		tilesOfProperType = outdoorNonFlippable;
	  }
	} else {
	  if (isTileFlippable) {
		tilesOfProperType = indoorFlippable;
	  } else {
		tilesOfProperType = indoorNonFlippable;
	  }
	}
  } else if (tileType == 0xC1) {
	std::cout << "it's an edge tile. Tile: " << std::hex << tile << std::endl;
	tilesOfProperType = edges;
  } else {
	std::cout << "it's a tile of unknown type. Tile: " << std::hex << tile << std::endl;
	return std::string();
  }
  std::unordered_map<uint32_t, std::string>::const_iterator pGot = tilesOfProperType.find(tileId);
  if (pGot == tilesOfProperType.end()) {
//	throw std::runtime_error("Couldn't map tileId to art prefix. tileId = " + std::to_string(tileId));
	std::cout << "Couldn't map tileId to art prefix. tileId = " << tileId;
	return std::string();
  }
  std::string artPrefix = tileType == 0xC0 ? pGot->second + "bse" : pGot->second;
  boost::algorithm::to_lower(artPrefix);
  return artPrefix;
}

int main(int argc, const char * argv[]) {
  std::ifstream file("Resources/86570436012.sec", std::ifstream::in);
  const uint16_t numTilesInSector = 4096;
  boost::json::array tiles;
  tiles.reserve(numTilesInSector);
  if (file.is_open()) {
	try {
	  const std::vector<uint8_t> char2Array = std::vector<uint8_t> { 0x0, 0x1, 0x2, 0x9, 0x4, 0x5, 0xC, 0xD, 0x2, 0x9, 0xA, 0xB, 0xC, 0xD };
	  const std::string char2Values = std::string("06b489237ea5dc10");
	  const std::vector<uint8_t> char1Array = std::vector<uint8_t> { 0x0, 0x1, 0x8, 0x3, 0x4, 0x5, 0x6, 0x7, 0x8, 0x3, 0xA, 0xB, 0x6, 0x7, 0xE, 0xF };
	  const std::string char1Values = std::string("abcdefgh");
	  uint16_t currentTile {1};
	  // Read all 4096 tiles from sector
	  while (currentTile <= numTilesInSector) {
		const uint32_t tile = readUpTo4Bytes(file, currentTile * 4, 4);
		const uint32_t tileId = getTileId(tile);
		const bool isLeastSignificantBitSet = tileId & 1;
		
		const uint32_t char2Number = isLeastSignificantBitSet ? char2Array.at(tile >> 12 & 0xF) : tile >> 12 & 0xF;
		uint32_t char1Number {};
		if (char2Number >= char2Array.size()) {
		  char1Number = tile >> 9 & 7;
		}
		else {
		  char1Number = char2Array.at(char2Number) == char1Array.at(char2Number) && isLeastSignificantBitSet ? (tile >> 9 & 7) + 8 : tile >> 9 & 7;
		}
		char1Number = char1Number >= 8 ? char1Number - 8 : char1Number;
		const char char1Value = char1Values.at(char1Number);
		const std::string artPrefix = getArtPrefix(tile);
		// A hack to address some inconsistencies
		const char char2Value = artPrefix.find("bse") == std::string::npos ? char2Values.at(char2Number) : '0';
		const std::string art = artPrefix == "" ? "unknown" : artPrefix + char2Value + char1Value + "_0.bmp";
		
		boost::json::object tileObj;
		tileObj["instanceId"] = currentTile - 1;
		tileObj["textureName"] = art;
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
