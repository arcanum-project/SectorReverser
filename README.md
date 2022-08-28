# Description

Parse .sec file and identify which .bmp file to use for tiles.

Currently only works for outdoor flippable tiles.

Results of parsing are stored to json file with the following structure:

```
{
  "tiles": [
    {
      "instanceId": 0, // defines position of a tile in a sector. Works similarly to WorldEd where tiles stored as contiguous columns
      "textureName": "drtbse0d_0", // name of bmp file to use
      "shouldFlip": "0" // whether to flip the tile during rendering. 1 - true, 0 - false 
    }
  ]
}
```

Input and output file paths are currently hardcoded.
