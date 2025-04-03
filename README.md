# objLoader
## Table of contents
1. [Introduction](#introduction)
2. [File hierarchy](#file-hierarchy)
3. [Functionality](#functionality)
4. [Cautions](#cautions)
5. [Improvement ideas](#improvement-ideas)

<a id="introduction"></a>
## Introduction
A terminal-based application that converts a .obj (3D model file format) into a .h file, marking essential model parameters and calculating its TBN matrix. It simplifies the process of rendering complex models by saving them as static arrays in header files, eliminating the need for an external model loader (once the model is converted, it can be used anytime).

<a id="file-hierarchy"></a>
## File hierarchy
```
├── objLoader
│   └── obj
└── Models
```
The ```obj``` directory is where the .obj files must be placed for the program to process them. Additionally, if you're not interested in using the TBN matrix or don't need it, the plain model will be generated there (only the vertices, texCoords, and normals arrays).
The ```Models``` directory is where all final header files will be generated. Every final model.h file includes:
  1. int vertexCount
  2. float vertices[] (every vertice is a next quartet of values)
  3. float texCoords[] (every texCoord is a next pair of values)
  4. float normals[] (every normal is a next quartet of values)
  5. float C1[] (TBN matrix first column - t.x b.x n.x 0.0f)
  6. float C2[] (TBN matrix second column - t.y b.y n.y 0.0f)
  7. float C3[] (TBN matrix third column - t.z b.z n.z 0.0f)

<a id="functionality"></a>
## Functionality
The application lists the .obj files from the ```obj``` directory and provides selection by number.
- Choosing < 1 will exit the program.
- Choosing any number not specified in the list will cause the list to be reprinted.
- Upon selecting a listed .obj file, the conversion process begins and follows these steps:
    1. Print the name of the selected file
    2. Print the base name of the selected file (without the .obj extension).
    3. Process all vertices, texCoords, and normals (information displayed when the step is completed).
    4. Process all vertices, texCoords, and normals into correct triangles (information displayed when the step is completed).
    5. Calculate the TBN matrix (information displayed when the step is completed).
    6. Finalize the process.
    7. List the available .obj files again.
<br>
<p align="center">
  <img src="https://github.com/Dunnnan/objLoader/blob/main/Images/App.png?raw=true" alt="Functionality">
</p>

<a id="cautions"></a>
## Cautions
Using the application requires taking into account a few aspects of it:
1. If the .obj file contains multiple models, they will all be merged into a single model.
2. Based on point 1: If your .obj model requires multiple textures, the parts should not be in the same file.
3. The application ignores material references, smoothing groups, etc. [See](#file-hierarchy).
4. If any face in the .obj file is missing any of the required components ```(vertice, texCoord, normal)```, the program will exit with an error.
5. Processing models with a large number of vertices is not recommended, as it may lead to performance issues.
6. Final model files are hardcoded to be named ```my + baseName.h```. The ```vertexCount``` and arrays names inside the header are also fixed.

Each of these cautions may be addressed in future updates.

<a id="improvement-ideas"></a>
## Improvement ideas
1. **Correct processing of files without normals or texCoords**  
   If the faces include less than three nodes, the program should handle this situation and not result in an error.

2. **Optimalization**  
   The header writing process should be more automated, allowing the user to specify both the output directory for the header files and the directory for selecting .obj files. Additionally, the model processing and conversion should be reviewed for potential optimizations to improve efficiency.
3. **Splitting multiple parts included in the same file**  
   If the .obj file contains multiple parts with different textures, the program should create a separate header file for each part. This would allow for better organization and modularity when working with complex models.
4. **Error handling improvements**
   Implementing better error handling and providing more detailed error messages would improve the user experience, especially in cases where the .obj file contains unexpected data or format issues.
