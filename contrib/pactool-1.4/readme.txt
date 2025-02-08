Conversion mode:
"-c [PAC file] [PAB file]"

Converts PAC model files to DAE files (one DAE per mesh in PAC file). PAB file is not necessary, but DAE file will not contain any weight/joint data if it's not provided.

Mesh replace mode:
"-r [DAE file] [PAC file] [PAB file]"

Replaces one mesh in a PAC file with a mesh stored in a DAE file. PAB file not required, but no new weights data will be applied unless it's provided.

Mesh merge mode:
"-m [DAE file] [PAC file] [PAB file]"

Merges one mesh in PAC file with a mesh stored in a DAE file. PAB file not required, but no new weights data will be applied unless it's provided.

Merge mode uses vertex colors as guideline. Vertices with any colour (RGB value higher than 0,0,0) are seen as new vertices, and these will replace original vertices covering the same area. This mode is intended for updating only a specific part of multiple meshes which are extremely similar, so it is VERY important that the vertex colors stretch out to a part of the mesh which is identical to the original.

For instance, if you're updating a sword model and you only change the hilt, you want the vertex colors to cover the entire hilt and also move up to a part of the mesh which is identical (in this case, that would be the bottom part of the blade).

Since the mode relies on vertex colors for the merge process, it'll use vertex colors from the original PAC mesh which are closest so the new vertices.

Optional command line arguments:
"-replaceAllLOD" (for replace and merge modes)

Each mesh has 3 LOD versions in PAC files. If you use this option (and you're replacing with a LOD0 mesh), LOD1 and LOD2 will be replaced with same mesh.

"-colorCoding" (for replace and merge modes)

This will reference vertex colors to determine what data to replace and what to keep from original mesh. White (1,1,1) will be seen as an indicator to copy all types of data. Red (1,0,0) is for vertex positions and vertex normals. Green (0,1,0) is for weight information. Blue (0,0,1) is for UV data.

For instance, if you color vertices with red and blue (1,0,1) the tool will apply new vertex positions, normals, and UV positions, but for weights it will use data from the original mesh which are closest to the new vertices.

The code checks if values are higher than 0, so any hint of blue in the RGB value will been as blue.

Enabling this option will make it use vertex colors from original mesh (based on vertices which are closest).

Notes:

Only use DAE files containing one mesh.

Do not rename the DAE files. PACtool relies on the filename to determine which specific mesh and LOD to replace.

Some 3d modelling programs might handle DAE files in a way which is not compatible with PACtool. If things aren't working, try importing/exporting DAE files with another program.

Vertex normal generation in this tool is not as good as whatever solution the developer is using, so certain geometry will not look as good as the original meshes. Unless you're importing a whole new mesh, I recommend relying on merge mode or color coding to ensure untouched parts of the mesh are not modified in any way.