#ifndef OBJ_TO_H_INCLUDED
#define OBJ_TO_H_INCLUDED

#include <glm/glm.hpp>
#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <filesystem>
#include <string>

#define RESET   "\033[0m"
#define RED     "\033[31m"
#define GREEN   "\033[32m"
#define YELLOW  "\033[33m"

/*
//Jak Korzystaæ? $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
generateHeaderFile("nazwa pliku typu .obj, "Jak chcesz nazwaæ plik header? Zostanie utworzony plik o nazwie: my<TWOJA_NAZWA>.h")
toTrianglesOrder("nazwa pliku typu.obj", "jak chcesz nazwaæ swój plik?", my<TWOJA_NAZWA>Vertices, my<TWOJA_NAZWA>TexCoords, my<TWOJA_NAZWA>Normals, 3 (ta funkcja przekszta³ca utworzony poprzednio .h tak aby main by³ w stanie rysowaæ model iteruj¹c po kolejnych wartoœciach macierzy z .h - 
                                                                                                                                                      uk³ada wierzcho³ki w kolejnoœci kolejnych rysowanych trójk¹tow, standardowo trójk¹t sk³ada siê z 3. wierzcho³ków, ale istniej¹ pliki .obj
                                                                                                                                                      posiadaj¹ce "trójk¹ty" zdefiniowane przez 4 wierzcho³ki - odczyt takich plików bêdzie zaimplementowany w przysz³oœci poprzez manipulacjê t¹
                                                                                                                                                      ostatni¹ wartoœci¹ - zaimplementowane))
calculateTBN("nazwa pliku typu.obj", "jak chcesz nazwaæ swój plik?", my<TWOJA_NAZWA>TexCoords, my<TWOJA_NAZWA>Normals, my<TWOJA_NAZWA>VertexCount) Tworzy plik o nazwie my<TWOJA_NAZWA>_TBN.h zawieraj¹cy macierze my<TWOJA_NAZWA>C1, my<TWOJA_NAZWA>C2, my<TWOJA_NAZWA>C3
                                                                                                                                                   W przysz³oœci macierze te bêd¹ dodawane do pliku my<TWOJA_NAZWA>.h - zaimplementowane
Przyk³ad:
(wywo³ywane w main() w pliku main_file.cpp)
//generateHeaderFile("rock.obj", "Rock");
//toTrianglesOrder("rock.obj", "Rock", myRockVertices, myRockTexCoords, myRockNormals, 3);
//calculateTBN("Rock", myRockVertices, myRockTexCoords, myRockNormals, myRockVertexCount);
$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
*/ 

void calculateTBN(const std::string& figureName, std::vector<float>& Vertexes, std::vector<float>& TexCoords, std::vector<float>& Normals, int VertexCount, std::vector<std::string>& verticesS, std::vector<std::string>& texCoordsS, std::vector<std::string>& normalsS) {

    std::ofstream headerFile("my" + figureName + ".h");
    if (!headerFile.is_open()) {
        std::cerr << RED << "Error: Unable to create header file " << figureName << ".h" << RESET << std::endl;
        return;
    }

    std::vector<std::string> TB1;
    std::vector<std::string> TB2;
    std::vector<std::string> TB3;
    float f = 0;

    int i = 0;
    while (i < VertexCount){
        // Vertexes
        glm::vec3 pos1(Vertexes[4*i], Vertexes[4*i+1], Vertexes[4*i+2]);
        glm::vec3 pos2(Vertexes[4*i+4], Vertexes[4*i+5], Vertexes[4*i+6]);
        glm::vec3 pos3(Vertexes[4*i+8], Vertexes[4*i+9], Vertexes[4*i+10]);
        // Texture Coordinates
        glm::vec2 uv1(TexCoords[2*i], TexCoords[2*i+1]);
        glm::vec2 uv2(TexCoords[2*i+2], TexCoords[2*i+3]);
        glm::vec2 uv3(TexCoords[2*i+4], TexCoords[2*i+5]);

        glm::vec3 nm1(Normals[4 * i], Normals[4 * i + 1], Normals[4 * i + 2]);
        glm::vec3 nm2(Normals[4 * i + 4], Normals[4 * i + 5], Normals[4 * i + 6]);
        glm::vec3 nm3(Normals[4 * i + 8], Normals[4 * i + 9], Normals[4 * i + 10]);

        // Calculate tangent/bitangent vectors of both triangles
        glm::vec3 tangent1, bitangent1, nm;
        // Triangle 1
        // ----------
        glm::vec3 edge1 = pos2 - pos1;
        glm::vec3 edge2 = pos3 - pos1;
        glm::vec2 deltaUV1 = uv2 - uv1;
        glm::vec2 deltaUV2 = uv3 - uv1;

        // Check if deltaUV1 and deltaUV2 are not equal to zero vector
        if ((deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y) > 0 || (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y) < 0) {
            float f = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);

            tangent1.x = f * (deltaUV2.y * edge1.x - deltaUV1.y * edge2.x);
            tangent1.y = f * (deltaUV2.y * edge1.y - deltaUV1.y * edge2.y);
            tangent1.z = f * (deltaUV2.y * edge1.z - deltaUV1.y * edge2.z);

            bitangent1.x = f * (-deltaUV2.x * edge1.x + deltaUV1.x * edge2.x);
            bitangent1.y = f * (-deltaUV2.x * edge1.y + deltaUV1.x * edge2.y);
            bitangent1.z = f * (-deltaUV2.x * edge1.z + deltaUV1.x * edge2.z);

            nm = glm::normalize(cross(edge1, edge2));
            //nm = normalize(nm1);

            glm::normalize(nm1);
            glm::normalize(nm2);
            glm::normalize(nm3);

            // Normalize tangent and bitangent vectors
            tangent1 = glm::normalize(tangent1);
            bitangent1 = glm::normalize(bitangent1);

        }
        else {
            tangent1 = glm::vec3(0.0f);
            bitangent1 = glm::vec3(0.0f);
        
            nm = glm::vec3(0.0f);
        }

        TB1.push_back(std::to_string(tangent1.x) + "f, " + std::to_string(tangent1.y) + "f, " + std::to_string(tangent1.z) + "f, 0.0f,");
        TB1.push_back(std::to_string(tangent1.x) + "f, " + std::to_string(tangent1.y) + "f, " + std::to_string(tangent1.z) + "f, 0.0f,");
        TB1.push_back(std::to_string(tangent1.x) + "f, " + std::to_string(tangent1.y) + "f, " + std::to_string(tangent1.z) + "f, 0.0f," + "\n");

        TB2.push_back(std::to_string(bitangent1.x) + "f, " + std::to_string(bitangent1.y) + "f, " + std::to_string(bitangent1.z) + "f, 0.0f,");
        TB2.push_back(std::to_string(bitangent1.x) + "f, " + std::to_string(bitangent1.y) + "f, " + std::to_string(bitangent1.z) + "f, 0.0f,");
        TB2.push_back(std::to_string(bitangent1.x) + "f, " + std::to_string(bitangent1.y) + "f, " + std::to_string(bitangent1.z) + "f, 0.0f," + "\n");

        TB3.push_back(std::to_string(nm1.x) + "f, " + std::to_string(nm1.y) + "f, " + std::to_string(nm1.z) + "f, 0.0f,");
        TB3.push_back(std::to_string(nm2.x) + "f, " + std::to_string(nm2.y) + "f, " + std::to_string(nm2.z) + "f, 0.0f,");
        TB3.push_back(std::to_string(nm3.x) + "f, " + std::to_string(nm3.y) + "f, " + std::to_string(nm3.z) + "f, 0.0f," + "\n");


        i = i + 3;
        
    }


    headerFile << "#ifndef MY" << figureName << "_H_INCLUDED" << std::endl;
    headerFile << "#define MY" << figureName << "_H_INCLUDED" << std::endl;
    headerFile << std::endl;
    headerFile << std::endl;
    headerFile << "//my" << figureName << "VerticesTriangles - homogenous vertex coordinates in model space" << std::endl;
    headerFile << "//my" << figureName << "TexCoordsTriangles - texture coordinates" << std::endl;
    headerFile << "//my" << figureName << "NormalsTriangles - homogenous vertex normals of walls (per vertex) in model space" << std::endl;
    headerFile << "//my" << figureName << "C1 - t.x b.x n.x 0.0f" << std::endl;
    headerFile << "//my" << figureName << "C2 - t.y b.y n.y 0.0f" << std::endl;
    headerFile << "//my" << figureName << "C3 - t.z b.z n.z 0.0f" << std::endl;
    headerFile << std::endl;
    headerFile << std::endl;

    headerFile << "// Vertex count" << std::endl;
    headerFile << "int my" << figureName << "VertexCount = " << VertexCount << ";" << std::endl;
    headerFile << std::endl;

    headerFile << "float my" << figureName << "Vertices[] = {" << std::endl;
    for (const std::string& vertex : verticesS) {
        headerFile << vertex << std::endl;
    }
    headerFile << "};" << std::endl;
    headerFile << std::endl;
    headerFile << "float my" << figureName << "TexCoords[] = {" << std::endl;
    for (const std::string& texCoord : texCoordsS) {
        headerFile << texCoord << std::endl;
    }
    headerFile << "};" << std::endl;
    headerFile << std::endl;
    headerFile << "float my" << figureName << "Normals[] = {" << std::endl;
    for (const std::string& normals : normalsS) {
        headerFile << normals << std::endl;
    }
    headerFile << "};" << std::endl;
    headerFile << std::endl;

    headerFile << "float my" << figureName << "C1[] = {" << std::endl;
    for (const std::string& vertex : TB1) {
        headerFile << vertex << std::endl;
    }
    headerFile << "};" << std::endl;
    headerFile << std::endl;
    headerFile << "float my" << figureName << "C2[] = {" << std::endl;
    for (const std::string& texCoord : TB2) {
        headerFile << texCoord << std::endl;
    }
    headerFile << "};" << std::endl;
    headerFile << std::endl;
    headerFile << "float my" << figureName << "C3[] = {" << std::endl;
    for (const std::string& normals : TB3) {
        headerFile << normals << std::endl;
    }
    headerFile << "};" << std::endl;
    headerFile << std::endl;
    headerFile << "#endif // MY" << figureName << "_H_INCLUDED" << std::endl;

    std::cout << GREEN << "TBN matrixes created succesfully." << RESET << std::endl;

    headerFile.close();

}


void toTrianglesOrder(const std::string& objFilename, const std::string& figureName, std::vector<float>& Vertexes, std::vector<float>& Textures, std::vector<float>& Normals, int Trojkaty, int& VertexCountf, std::vector<std::string>& verticesS, std::vector<std::string>& texCoordsS, std::vector<std::string>& normalsS) {
    std::ifstream objFile(objFilename);
    if (!objFile.is_open()) {
        std::cerr << RED << "Error: Unable to open file " << objFilename << RESET << std::endl;
        return;
    }

    std::ofstream headerFile("my" + figureName + ".h");
    if (!headerFile.is_open()) {
        std::cerr << RED << "Error: Unable to create header file " << figureName << ".h" << RESET << std::endl;
        return;
    }

    std::vector<std::string> faces;

    std::string line;

    int vertexCount = 0;
    int zliczajka = 0;

    std::vector<std::string> Vertexes_Triangles;
    std::vector<std::string> Textures_Triangles;
    std::vector<std::string> Normals_Triangles;

    std::vector<float> Vertexesf_Triangles;
    std::vector<float> Texturesf_Triangles;
    std::vector<float> Normalsf_Triangles;

    while (std::getline(objFile, line)) {
        if (line.substr(0, 2) == "f ") {
            std::stringstream ss(line.substr(2)); // Usuñ "f " z pocz¹tku linii
            int vertice, texture, normal;
            std::string vertice_s, texture_s, normal_s;

            zliczajka = 0;

            for (int i = 0; i < Trojkaty; i++) {

                std::getline(ss, vertice_s, '/');
                if (ss.fail()) {
                    break;
                }
                std::getline(ss, texture_s, '/');
                std::getline(ss, normal_s, ' ');

                vertice = std::stoi(vertice_s);
                texture = std::stoi(texture_s);
                normal = std::stoi(normal_s);

                if (zliczajka == 3) {
                    Vertexes_Triangles.push_back(Vertexes_Triangles[Vertexes_Triangles.size() - 3]);
                    Vertexes_Triangles.push_back(Vertexes_Triangles[Vertexes_Triangles.size() - 2]);
                    Vertexesf_Triangles.push_back(Vertexesf_Triangles[Vertexesf_Triangles.size() - 12]);
                    Vertexesf_Triangles.push_back(Vertexesf_Triangles[Vertexesf_Triangles.size() - 12]);
                    Vertexesf_Triangles.push_back(Vertexesf_Triangles[Vertexesf_Triangles.size() - 12]);
                    Vertexesf_Triangles.push_back(Vertexesf_Triangles[Vertexesf_Triangles.size() - 12]);
                    Vertexesf_Triangles.push_back(Vertexesf_Triangles[Vertexesf_Triangles.size() - 8]);
                    Vertexesf_Triangles.push_back(Vertexesf_Triangles[Vertexesf_Triangles.size() - 8]);
                    Vertexesf_Triangles.push_back(Vertexesf_Triangles[Vertexesf_Triangles.size() - 8]);
                    Vertexesf_Triangles.push_back(Vertexesf_Triangles[Vertexesf_Triangles.size() - 8]);


                    Textures_Triangles.push_back(Textures_Triangles[Textures_Triangles.size() - 3]);
                    Textures_Triangles.push_back(Textures_Triangles[Textures_Triangles.size() - 2]);
                    Texturesf_Triangles.push_back(Texturesf_Triangles[Texturesf_Triangles.size() - 6]);
                    Texturesf_Triangles.push_back(Texturesf_Triangles[Texturesf_Triangles.size() - 6]);
                    Texturesf_Triangles.push_back(Texturesf_Triangles[Texturesf_Triangles.size() - 4]);
                    Texturesf_Triangles.push_back(Texturesf_Triangles[Texturesf_Triangles.size() - 4]);

                    Normals_Triangles.push_back(Normals_Triangles[Normals_Triangles.size() - 3]);
                    Normals_Triangles.push_back(Normals_Triangles[Normals_Triangles.size() - 2]);
                    Normalsf_Triangles.push_back(Normalsf_Triangles[Normalsf_Triangles.size() - 12]);
                    Normalsf_Triangles.push_back(Normalsf_Triangles[Normalsf_Triangles.size() - 12]);
                    Normalsf_Triangles.push_back(Normalsf_Triangles[Normalsf_Triangles.size() - 12]);
                    Normalsf_Triangles.push_back(Normalsf_Triangles[Normalsf_Triangles.size() - 12]);
                    Normalsf_Triangles.push_back(Normalsf_Triangles[Normalsf_Triangles.size() - 8]);
                    Normalsf_Triangles.push_back(Normalsf_Triangles[Normalsf_Triangles.size() - 8]);
                    Normalsf_Triangles.push_back(Normalsf_Triangles[Normalsf_Triangles.size() - 8]);
                    Normalsf_Triangles.push_back(Normalsf_Triangles[Normalsf_Triangles.size() - 8]);


                    zliczajka--;

                    vertexCount++;
                    vertexCount++;
                }

                vertice = vertice * 4;
                Vertexes_Triangles.push_back(std::to_string(Vertexes[vertice - 4]) + "f, " +
                                             std::to_string(Vertexes[vertice - 3]) + "f, " +
                                             std::to_string(Vertexes[vertice - 2]) + "f, " +
                                             std::to_string(Vertexes[vertice - 1]) + "f,"  );

                Vertexesf_Triangles.push_back(Vertexes[vertice - 4]);
                Vertexesf_Triangles.push_back(Vertexes[vertice - 3]);
                Vertexesf_Triangles.push_back(Vertexes[vertice - 2]);
                Vertexesf_Triangles.push_back(Vertexes[vertice - 1]);




                texture = texture * 2;
                Textures_Triangles.push_back(std::to_string(Textures[texture - 2]) + "f, " + 
                                             std::to_string(Textures[texture - 1]) + "f,"  );

                Texturesf_Triangles.push_back(Textures[texture - 2]);
                Texturesf_Triangles.push_back(Textures[texture - 1]);




                normal = normal * 4;
                Normals_Triangles.push_back(std::to_string(Normals[normal - 4]) + "f, " +
                                            std::to_string(Normals[normal - 3]) + "f, " +
                                            std::to_string(Normals[normal - 2]) + "f, " +
                                            std::to_string(Normals[normal - 1]) + "f,");

                Normalsf_Triangles.push_back(Normals[normal - 4]);
                Normalsf_Triangles.push_back(Normals[normal - 3]);
                Normalsf_Triangles.push_back(Normals[normal - 2]);
                Normalsf_Triangles.push_back(Normals[normal - 1]);

                vertexCount++;
                
                
                zliczajka++;
            }
        }
    }

    headerFile << "#ifndef MY" << figureName << "_H_INCLUDED" << std::endl;
    headerFile << "#define MY" << figureName << "_H_INCLUDED" << std::endl;
    headerFile << std::endl;
    headerFile << std::endl;
    headerFile << "//my" << figureName << "VerticesTriangles - homogenous vertex coordinates in model space" << std::endl;
    headerFile << "//my" << figureName << "TexCoordsTriangles - texture coordinates" << std::endl;
    headerFile << "//my" << figureName << "NormalsTriangles - homogenous vertex normals of walls (per vertex) in model space" << std::endl;
    headerFile << std::endl;

    headerFile << "// Vertex count" << std::endl;
    headerFile << "int my" << figureName << "VertexCount = " << vertexCount << ";" << std::endl;
    headerFile << std::endl;

    headerFile << "float my" << figureName << "Vertices[] = {" << std::endl;
    for (const std::string& vertex : Vertexes_Triangles) {
        headerFile << vertex << std::endl;
    }
    headerFile << "};" << std::endl;
    headerFile << std::endl;
    headerFile << "float my" << figureName << "TexCoords[] = {" << std::endl;
    for (const std::string& texCoord : Textures_Triangles) {
        headerFile << texCoord << std::endl;
    }
    headerFile << "};" << std::endl;
    headerFile << std::endl;
    headerFile << "float my" << figureName << "Normals[] = {" << std::endl;
    for (const std::string& normals : Normals_Triangles) {
        headerFile << normals << std::endl;
    }
    headerFile << "};" << std::endl;
    headerFile << std::endl;
    headerFile << "#endif // MY" << figureName << "_H_INCLUDED" << std::endl;

    std::cout << GREEN << "Triangles created succesfully." << RESET << std::endl;


    Vertexes = Vertexesf_Triangles;
    Textures = Texturesf_Triangles;
    Normals = Normalsf_Triangles;
    VertexCountf = vertexCount;

    verticesS = Vertexes_Triangles;
    texCoordsS = Textures_Triangles;
    normalsS = Normals_Triangles;


    objFile.close();
    headerFile.close();
}


void generateHeaderFile(const std::string& objFilename, const std::string& figureName, std::vector<float>& verticesf, std::vector<float>& texCoordsf, std::vector<float>& normalsf, int& vertexCountf) {
    std::ifstream objFile(objFilename);
    if (!objFile.is_open()) {
        std::cerr << RED << "Error: Unable to open file " << objFilename << RESET << std::endl;
        return;
    }

    std::ofstream headerFile("my" + figureName + ".h");
    if (!headerFile.is_open()) {
        std::cerr << RED << "Error: Unable to create header file " << figureName << ".h" << RESET << std::endl;
        return;
    }

    std::vector<std::string> vertices;
    std::vector<std::string> texCoords;
    std::vector<std::string> normals;
    int vertexCount = 0;

    std::string line;
    while (std::getline(objFile, line)) {
        if (line.substr(0, 2) == "v ") {
            std::stringstream ss(line.substr(2)); // Usuñ "v " z pocz¹tku linii
            float x, y, z;
            ss >> x >> y >> z;
            verticesf.push_back(x);
            verticesf.push_back(y);
            verticesf.push_back(z);
            verticesf.push_back(1.0f);

            vertices.push_back(std::to_string(x) + "f, " + std::to_string(y) + "f, " + std::to_string(z) + "f, 1.0f,"); // Dodaj "f," po ka¿dej liczbie
            vertexCount++;
        }
        else if (line.substr(0, 3) == "vt ") {
            std::stringstream ss(line.substr(3)); // Usuñ "vt " z pocz¹tku linii
            float u, v;
            ss >> u >> v;
            texCoordsf.push_back(u);
            texCoordsf.push_back(v);

            texCoords.push_back(std::to_string(u) + "f, " + std::to_string(v) + "f,");
        }
        else if (line.substr(0, 3) == "vn ") {
            std::stringstream ss(line.substr(3)); // Usuñ "vn " z pocz¹tku linii
            float x1, y1, z1;
            ss >> x1 >> y1 >> z1 ;
            normalsf.push_back(x1);
            normalsf.push_back(y1);
            normalsf.push_back(z1);
            normalsf.push_back(0.0f);

            normals.push_back(std::to_string(x1) + "f, " + std::to_string(y1) + "f," + std::to_string(z1) + "f, " + "0.0f,");  // Dodaj "f," po ka¿dej liczbie
        }
    }

    headerFile << "#ifndef MY" << figureName << "_H_INCLUDED" << std::endl;
    headerFile << "#define MY" << figureName << "_H_INCLUDED" << std::endl;
    headerFile << std::endl;
    headerFile << "//my" << figureName << "Vertices - homogenous vertex coordinates in model space" << std::endl;
    headerFile << "//my" << figureName << "TexCoords - texture coordinates" << std::endl;
    headerFile << "//my" << figureName << "Normals - homogenous vertex normals of walls (per vertex) in model space" << std::endl;
    headerFile << std::endl;
    headerFile << "// Vertex count" << std::endl;
    headerFile << "int my" << figureName << "VertexCount = " << vertexCount << ";" << std::endl;
    headerFile << std::endl;
    headerFile << "float my" << figureName << "Vertices[] = {" << std::endl;
    for (const std::string& vertex : vertices) {
        headerFile << vertex << std::endl;
    }
    headerFile << "};" << std::endl;
    headerFile << std::endl;
    headerFile << "float my" << figureName << "TexCoords[] = {" << std::endl;
    for (const std::string& texCoord : texCoords) {
        headerFile << texCoord << std::endl;
    }
    headerFile << "};" << std::endl;
    headerFile << std::endl;
    headerFile << "float my" << figureName << "Normals[] = {" << std::endl;
    for (const std::string& normals : normals) {
        headerFile << normals << std::endl;
    }
    headerFile << "};" << std::endl;
    headerFile << std::endl;
    headerFile << "#endif // MY" << figureName << "_H_INCLUDED" << std::endl;

    std::cout << GREEN << "Vertexes created succesfully." << RESET << std::endl;
    
    vertexCountf = vertexCount;

    objFile.close();
    headerFile.close();
}


void delSlashes(const std::string& objFilename) {
    std::ifstream inputFile(objFilename);
    if (!inputFile) {
        std::cerr << RED << "Nie mo¿na otworzyæ pliku wejœciowego: " << RESET << objFilename << std::endl;
        return;
    }

    std::stringstream buffer;
    buffer << inputFile.rdbuf();
    std::string content = buffer.str();
    inputFile.close();

    std::string::size_type pos = 0;
    while ((pos = content.find("//", pos)) != std::string::npos) {
        content.replace(pos, 2, "/");
        pos += 1;
    }
    
    std::ofstream outputFile(objFilename);
    if (!outputFile) {
        std::cerr << RED << "Nie mo¿na otworzyæ pliku do zapisu: " << RESET << objFilename << std::endl;
        return;
    }

    outputFile << content;
    outputFile.close();
}

#endif // OBJ_TO_H_INCLUDED