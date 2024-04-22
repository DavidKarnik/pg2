#include <random>

#include <opencv2/opencv.hpp>

#include "App.h"

// maze map
// characters in maze: 
// '#' wall
// '.' empty
// 'e' end position (target, goal, gateway, etc.)

// Secure access to map
uchar App::MapGet(cv::Mat& map, int x, int y)
{
	x = std::clamp(x, 0, map.cols);
	y = std::clamp(y, 0, map.rows);

	//at(row,col)!!!
	return map.at<uchar>(y, x);
}

// Random map gen
void App::MazeGenerate(cv::Mat& map)
{
	cv::Point2i start_position, end_position;

	// C++ random numbers
	std::random_device r; // Seed with a real random value, if available
	std::default_random_engine e1(r());
	std::uniform_int_distribution<int> uniform_height(1, map.rows - 2); // uniform distribution between int..int
	std::uniform_int_distribution<int> uniform_width(1, map.cols - 2);
	std::uniform_int_distribution<int> uniform_block(0, 15); // how often are walls generated: 0=wall, anything else=empty

	//inner maze 
	for (int j = 0; j < map.rows; j++) {
		for (int i = 0; i < map.cols; i++) {
			switch (uniform_block(e1))
			{
			case 0:
				map.at<uchar>(cv::Point(i, j)) = '#';
				break;
			default:
				map.at<uchar>(cv::Point(i, j)) = '.';
				break;
			}
		}
	}

	//walls
	for (int i = 0; i < map.cols; i++) {
		map.at<uchar>(cv::Point(i, 0)) = '#';
		map.at<uchar>(cv::Point(i, map.rows - 1)) = '#';
	}
	for (int j = 0; j < map.rows; j++) {
		map.at<uchar>(cv::Point(0, j)) = '#';
		map.at<uchar>(cv::Point(map.cols - 1, j)) = '#';
	}

	//gen start_position inside maze (excluding walls)
	do {
		start_position.x = uniform_width(e1);
		start_position.y = uniform_height(e1);
	} while (MapGet(map, start_position.x, start_position.y) == '#'); //check wall

	//gen end different from start, inside maze (excluding outer walls) 
	do {
		end_position.x = uniform_width(e1);
		end_position.y = uniform_height(e1);
	} while (start_position == end_position); //check overlap
	map.at<uchar>(cv::Point(end_position.x, end_position.y)) = 'e';

	std::cout << "Start: " << start_position << "\n";
	std::cout << "End: " << end_position << "\n";

	// spawn maze
	int wall_counter = 0;
	glm::vec3 position{};
	glm::vec3 scale = glm::vec3(1.0f, 1.0f, 1.0f);
	glm::vec4 rotation = glm::vec4(0.0f, 1.0f, 0.0f, 0.0f);
	for (int j = 0; j < map.rows; j++) {
		for (int i = 0; i < map.cols; i++) {
			auto c = MapGet(map, i, j);
			if (c == '#') {
				//position = glm::vec3(i, 0, j);
				position = glm::vec3(-i, 0, -j);
				CreateModel("obj_maze_wall_" + wall_counter++, "cube_triangles_normals_tex.obj", "box_rgb888.png", true, position, scale, rotation);
			}
			std::cout << c;
		}
		std::cout << "\n";
	}

	//set player position in 3D space (transform X-Y in map to XYZ in GL)
	/*
	camera.position.x = (start_position.x) + 1.0 / 2.0f;
	camera.position.z = (start_position.y) + 1.0 / 2.0f;
	camera.position.y = 0.0f;
	/**/
}