#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <iterator>
#include <cstring>

#include <thread>
#include <chrono>

void clearScreen() {
	#ifdef _WIN32
		system("cls");
	#elif __linux__
		system("clear");
	#elif __APPLE__
		system("clear");
	#endif
}

void printGame(bool *image, const unsigned WIDTH, const unsigned HEIGHT) {
	std::stringstream stream;

	for (int idx = 0; idx < HEIGHT * WIDTH; idx++) {
		stream << (image[idx] ? '*' : ' ');
		
		if ((idx + 1) % WIDTH == 0)
			stream << std::endl;
	}

	std::cout << stream.str();
}

void updateGen(bool *inImage, bool *outImage, const unsigned WIDTH, const unsigned HEIGHT) {
	for (int idx = 0; idx < HEIGHT * WIDTH; idx++) {
		unsigned cellsAlive = 0;

		for (int i = -1; i < 2; i++) {
			int col = (idx % WIDTH) + i;
			
			if (col < 0 || col >= WIDTH)
				continue;
			
			for (int j = -1; j < 2; j++) {				
				int row = (idx / WIDTH) + j;

				if ((i == 0 && j == 0))
					continue;

				int rowIdx = ((HEIGHT + row % HEIGHT) % HEIGHT) * WIDTH;
				int colIdx = (WIDTH + col % WIDTH) % WIDTH;
				cellsAlive += inImage[rowIdx + colIdx];
			}
		}

		if (inImage[idx] && (cellsAlive < 2 || cellsAlive > 3)) {
			outImage[idx] = 0;
		} else if (!inImage[idx] && cellsAlive == 3) {
			outImage[idx] = 1;
		} else {
			outImage[idx] = inImage[idx];
		}
	}
}

void saveGame(const std::string &path, const bool *IMG, const unsigned WIDTH, const unsigned HEIGHT) {
	std::ofstream outputFile(path);

	if (outputFile.is_open()) {
		std::stringstream out;
		
		out << WIDTH << ' ' << HEIGHT << std::endl;

		for (int i = 0; i < HEIGHT * WIDTH; i++) {
			out << (IMG[i] ? '*' : '_');

			if ((i + 1) % WIDTH == 0 && i - 1 != WIDTH * HEIGHT)
				out << std::endl;
		}
		
		outputFile << out.str();
	}

	outputFile.close();
	
	std::cout << "Pattern saved in file: " << path << std::endl;
}

int main(int argc, char* argv[]) {
	unsigned width, height;
	bool  *inImage = nullptr;
	bool *outImage = nullptr;

	const unsigned GENS = std::atoi(argv[1]), SLEEP_MILLI = std::atoi(argv[2]);
	const std::string INPUT_PATH = argv[3];
	
	// > ./cgol 100 100 r-pentomino.txt output.txt

	std::ifstream inputFile(INPUT_PATH);

	if (inputFile.is_open()) {
		inputFile >> width >> height;

		inImage = new bool[height * width];
		outImage = new bool[height * width];

		for (int row = 0; row < height; row++) {
			std::string inputRow;
			inputFile >> inputRow;
			
			for (int col = 0; col < width; col++)
				inImage[row * width + col] = inputRow[col] == '*';
		}
	}

	inputFile.close();

	for (int i = 0; i < GENS + 1; ) {
		std::this_thread::sleep_for(std::chrono::milliseconds(SLEEP_MILLI));
		clearScreen();

		updateGen(inImage, outImage, width, height);
		printGame(outImage, width, height);
		std::swap(inImage, outImage);
		//std::cout << "Generation: " << i << '/' << GENS;
	}

	if (argc == 5 && (!strcmp(argv[4], "--output") || !strcmp(argv[4], "-o"))) {
		const std::string OUTPUT_PATH = INPUT_PATH + "_output";
		saveGame(OUTPUT_PATH, inImage, width, height);
	}

	if (inImage != nullptr) {
		delete [] inImage;
		inImage = nullptr;
	}

	if (outImage != nullptr) {
		delete [] outImage;
		outImage = nullptr;
	}

	return 0;
}