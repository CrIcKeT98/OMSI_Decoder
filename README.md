# OMSI 2 Model decoder

_**All the work done was done solely for the sake of obtaining new knowledge and experience.**_

## Improved Skills:
- **Reverse engineering**
- **IDA Pro**
- **C++ Boost**
- **File serialization**

This utility allows you to decode the official models of the game OMSI 2, which were previously unavailable. The decoded model can then be imported into 3D editors (3DS Max or Blender) using custom plugins.
# Usage
Put the file path via the command line:
```
OMSI_Decoder.exe --p <some_path/filename>.o3d
```
Output decoded model will appear with the name `filename_decoded.o3d` next to the input file.