#define GLM_FORCE_RADIANS
#define GLM_FORCE_SWIZZLE



#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <glm/glm.hpp>

#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <filesystem>
#include <string>
#include <windows.h>

#include "Obj_Loader.h"

#define RESET   "\033[0m"
#define RED     "\033[31m"
#define GREEN   "\033[32m"
#define YELLOW  "\033[33m"
#define BLUE    "\033[34m"
#define BROWN   "\033[38;5;94m"

namespace fs = std::filesystem;

void selectObjFile(const std::string& directory, std::string& selected_file, std::string& base_name) {
	std::vector<std::string> obj_files;

	//List all .obj in the given directory
	for (const auto& entry : std::filesystem::directory_iterator(directory)) {
		if (entry.path().extension() == ".obj") {
			obj_files.push_back(entry.path().filename().string());
		}
	}

	//Check if no .obj files were found
	if (obj_files.empty()) {
		std::cout << "No .obj files found in the directory!" << std::endl;
		exit(-1);
	}

	//Sort the list of .obj files (LexicoGraphically)
	std::sort(obj_files.begin(), obj_files.end());

	//Display the list of .obj files
	std::cout << "Available .obj files:" << std::endl;
	for (size_t i = 0; i < obj_files.size(); i++) {
		std::cout << i + 1 << ": " << obj_files[i] << std::endl;
	}

	//Get user's choice
	int choice;
	std::cout << "Write > 0 < to EXIT " << std::endl;
	std::cout << "Select a file by number : \n";
	std::cin >> choice;

	//Validate choice
	if (choice < 1) {
		exit(0);
	}
	else if (choice > obj_files.size()) {
		std::cerr << "Invalid choice !\n" << std::endl;
		return;
	}

	//Set selected_file and base_name
	selected_file = obj_files[choice - 1];
	base_name = selected_file.substr(0, selected_file.find_last_of('.'));
}

int main(void)
{
	//Main loop
	while (1) {
		//Strings for .obj Choosement
		std::string directory = "./obj"; // Current directory
		std::string selected_file;
		std::string base_name;

		//Variables for creating model
		std::vector<float> vertices;
		std::vector<float> texCoords;
		std::vector<float> normals;
		std::vector<std::string> verticesS;
		std::vector<std::string> texCoordsS;
		std::vector<std::string> normalsS;
		int vertexCount = 0;
		int triangles = 4;

		//Selecting .obj file
		selectObjFile(directory, selected_file, base_name);

		if (!selected_file.empty()) {
			std::cout << "\nSelected file: " << YELLOW << selected_file << RESET << std::endl;
			std::cout << "Base name: " << YELLOW << base_name << "\n" << RESET << std::endl;
		}
		else {
			continue;
		}

		//Create model .h file
		SetCurrentDirectory(L"obj");

		delSlashes(selected_file);
		generateHeaderFile(selected_file, base_name, vertices, texCoords, normals, vertexCount);
		toTrianglesOrder(selected_file, base_name, vertices, texCoords, normals, triangles, vertexCount, verticesS, texCoordsS, normalsS);
		
		SetCurrentDirectory(L"..");
		SetCurrentDirectory(L"..");
		SetCurrentDirectory(L"Models");

		calculateTBN(base_name, vertices, texCoords, normals, vertexCount, verticesS, texCoordsS, normalsS);
		std::cout << "Vertices: " << vertices.size() << "   " << "TexCoords: " << texCoords.size() << "   " << "Normals: " << normals.size() << std::endl;
		
		SetCurrentDirectory(L"..");
		SetCurrentDirectory(L"objLoader");


		std::cout << GREEN << "\nCongratulations! Model " << YELLOW << base_name <<  ".h " << GREEN << "file created !" << RESET << std::endl;
		std::cout << "Do not forget to #include it in" << BLUE << " main_file.cpp " << RESET << "of your project !" << std::endl;
		std::cout << BROWN << "#include " << "<Models/my" << base_name << ".h\>\n" << RESET << std::endl;
	}

	return 0;
}
