#include "Model.h"
#include <iostream>
#include <fstream>
#include <string>

#include "Texture.h"

#define print(x) //std::cout << x << "\n"

Model::Model(const std::filesystem::path& path_main, const std::filesystem::path& path_tex, bool is_height_map)
{
    if (!is_height_map) {
        LoadOBJFile(path_main);
        GLuint texture_id = TextureInit(path_tex.string().c_str());
        mesh = Mesh(GL_TRIANGLES, mesh_vertices, mesh_vertex_indices, texture_id);
    }
    else {
        HeightMap_Load(path_main);
        GLuint texture_id = TextureInit(path_tex.string().c_str());
        mesh = Mesh(GL_TRIANGLES, mesh_vertices, mesh_vertex_indices, texture_id);
    }
}

void Model::Draw(Shader& shader)
{
    mx_model = glm::identity<glm::mat4>();

    mx_model = glm::translate(mx_model, position);
    mx_model = glm::scale(mx_model, scale);
    rotation_axes = glm::vec3(rotation.x, rotation.y, rotation.z);
    mx_model = glm::rotate(mx_model, glm::radians(rotation.w), rotation_axes);

    //mesh.draw(shader, mx_model);
    mesh.draw(shader);
}

void Model::FillFileLines(const std::filesystem::path& file_name)
{
    file_lines.clear();
    std::ifstream file_reader(file_name);
    while (getline(file_reader, file_line)) {
        file_lines.push_back(file_line);
    }
    file_reader.close();
}

void Model::LoadOBJFile(const std::filesystem::path& file_name)
{
    FillFileLines(file_name);

    mesh_vertices.clear();
    mesh_vertex_indices.clear();

    std::vector<glm::vec3> vertices;
    std::vector<glm::vec2> texture_coordinates;
    std::vector<glm::vec3> vertex_normals;
    std::vector<GLuint> indices_vertex, indices_texture_coordinate, indices_vertex_normal;

    std::string first_two_chars, first_three_chars;
    glm::vec2 uv;
    glm::vec3 vertex_or_normal;

    bool line_success;

    for (const std::string& line : file_lines) {
        if (!line.empty()) {
            line_success = true;
            first_two_chars = line.substr(0, 2);
            first_three_chars = line.substr(0, 3);
            // v -1.183220029 4.784470081 47.4618988
            if (first_two_chars == "v ") {
                vertex_or_normal = {};
                (void)sscanf_s(line.c_str(), "v %f %f %f", &vertex_or_normal.x, &vertex_or_normal.y, &vertex_or_normal.z);
                vertices.push_back(vertex_or_normal);
            }
            // vt 0.5000 0.7500
            else if (first_three_chars == "vt ") {
                uv = {};
                (void)sscanf_s(line.c_str(), "vt %f %f", &uv.y, &uv.x);
                texture_coordinates.push_back(uv);
            }
            // vn 0.7235898972 -0.6894102097 -0.03363365307
            else if (first_three_chars == "vn ") {
                vertex_or_normal = {};
                (void)sscanf_s(line.c_str(), "vn %f %f %f", &vertex_or_normal.x, &vertex_or_normal.y, &vertex_or_normal.z);
                vertex_normals.push_back(vertex_or_normal);
            }
            else if (first_two_chars == "f ") {
                auto n = std::count(line.begin(), line.end(), '/');
                // f 1 2 3
                if (n == 0) {
                    unsigned int indices_temp[3]{};
                    (void)sscanf_s(line.c_str(), "f %d %d %d", &indices_temp[0], &indices_temp[1], &indices_temp[2]);
                    indices_vertex.insert(indices_vertex.end(), { indices_temp[0], indices_temp[1], indices_temp[2] });
                }
                // f 3/1 4/2 5/3
                else if (n == 3) {
                    unsigned int indices_temp[6]{};
                    (void)sscanf_s(line.c_str(), "f %d/%d %d/%d %d/%d", &indices_temp[0], &indices_temp[3], &indices_temp[1], &indices_temp[4], &indices_temp[2], &indices_temp[5]);
                    indices_vertex.insert(indices_vertex.end(), { indices_temp[0], indices_temp[1], indices_temp[2] });
                    indices_texture_coordinate.insert(indices_texture_coordinate.end(), { indices_temp[3], indices_temp[4], indices_temp[5] });
                }
                else if (n == 6) {
                    // f 7//1 8//2 9//3
                    if (line.find("//") != std::string::npos) {
                        unsigned int indices_temp[6]{};
                        (void)sscanf_s(line.c_str(), "f %d//%d %d//%d %d//%d", &indices_temp[0], &indices_temp[3], &indices_temp[1], &indices_temp[4], &indices_temp[2], &indices_temp[5]);
                        indices_vertex.insert(indices_vertex.end(), { indices_temp[0], indices_temp[1], indices_temp[2] });
                        indices_vertex_normal.insert(indices_vertex_normal.end(), { indices_temp[3], indices_temp[4], indices_temp[5] });
                    }
                    // f 6/4/1 3/5/3 7/6/5
                    else {
                        unsigned int indices_temp[9]{};
                        (void)sscanf_s(line.c_str(), "f %d/%d/%d %d/%d/%d %d/%d/%d", &indices_temp[0], &indices_temp[3], &indices_temp[6], &indices_temp[1], &indices_temp[4], &indices_temp[7], &indices_temp[2], &indices_temp[5], &indices_temp[8]);
                        indices_vertex.insert(indices_vertex.end(), { indices_temp[0], indices_temp[1], indices_temp[2] });
                        indices_texture_coordinate.insert(indices_texture_coordinate.end(), { indices_temp[3], indices_temp[4], indices_temp[5] });
                        indices_vertex_normal.insert(indices_vertex_normal.end(), { indices_temp[6], indices_temp[7], indices_temp[8] });
                    }
                }
                // f 1/1/1 2/2/2 22/23/3 21/22/4
                else if (n == 8) {
                    unsigned int v[4]{};
                    unsigned int vt[4]{};
                    unsigned int vn[4]{};
                    (void)sscanf_s(line.c_str(), "f %d/%d/%d %d/%d/%d %d/%d/%d %d/%d/%d", &v[0], &vt[0], &vn[0], &v[1], &vt[1], &vn[1], &v[2], &vt[2], &vn[2], &v[3], &vt[3], &vn[3]);
                    indices_vertex.insert(indices_vertex.end(), { v[0], v[1], v[2], v[0], v[2], v[3] });
                    indices_texture_coordinate.insert(indices_texture_coordinate.end(), { vt[0], vt[1], vt[2], vt[0], vt[2], vt[3] });
                    indices_vertex_normal.insert(indices_vertex_normal.end(), { vn[0], vn[1], vn[2], vn[0], vn[2], vn[3] });
                }
                else {
                    line_success = false;
                }
            }
            else {
                line_success = false;
            }

            if (!line_success && first_two_chars != "# ") {
                print("LoadOBJFile: Ignoring line '" << line << "' in file '" << file_name << "'");
            }
        }
    }

    // RETARDED DRAW ? 2.0
    std::vector<glm::vec3> vertices_direct;
    std::vector<glm::vec2> texture_coordinates_direct;
    std::vector<glm::vec3> vertex_normals_direct;

    for (unsigned int u = 0; u < indices_vertex.size(); u++) {
        vertices_direct.push_back(vertices[indices_vertex[u] - 1]);
    }
    for (unsigned int u = 0; u < indices_texture_coordinate.size(); u++) {
        texture_coordinates_direct.push_back(texture_coordinates[indices_texture_coordinate[u] - 1]);
    }
    for (unsigned int u = 0; u < indices_vertex_normal.size(); u++) {
        vertex_normals_direct.push_back(vertex_normals[indices_vertex_normal[u] - 1]);
    }

    ///* Uncomment these if you don't like to live dangerously
    auto n_direct_uvs = texture_coordinates_direct.size();
    auto n_direct_normals = vertex_normals_direct.size();
    /**/

    for (unsigned int u = 0; u < vertices_direct.size(); u++) {
        Vertex vertex{};
        vertex.Position = vertices_direct[u];
        if (u < n_direct_uvs) vertex.UVs = texture_coordinates_direct[u];
        if (u < n_direct_normals) vertex.Normal = vertex_normals_direct[u];
        mesh_vertices.push_back(vertex);
        mesh_vertex_indices.push_back(u);
    }

    // What's said is said, what's done is done.
    print("LoadOBJFile: Loaded OBJ file " << file_name << "\n");
}

void Model::LoadMTLFile(const std::filesystem::path& file_name)
{
    FillFileLines(file_name);
    //TODO: Model::LoadMTLFile
}

void Model::HeightMap_Load(const std::filesystem::path& file_name)
{
    mesh_vertices.clear();
    mesh_vertex_indices.clear();

    cv::Mat hmap = cv::imread(file_name.u8string(), cv::IMREAD_GRAYSCALE);
    if (hmap.empty()) std::cerr << "HeightMap: [!] Height map empty? File: " << file_name << "\n";

    const unsigned int mesh_step_size = 10;

    print("HeightMap: heightmap size: " << hmap.size << ", channels: " << hmap.channels());

    if (hmap.channels() != 1) std::cerr << "HeightMap: [!] requested 1 channel, got: " << hmap.channels() << "\n";

    // Create heightmap mesh from TRIANGLES in XZ plane, Y is UP (right hand rule)
    //
    //   3-----2
    //   |    /|
    //   |  /  |
    //   |/    |
    //   0-----1
    //
    //   012,023
    //

    glm::vec3 normal{};
    unsigned int indices_counter = 0;

    for (unsigned int x_coord = 0; x_coord < (hmap.cols - mesh_step_size); x_coord += mesh_step_size) {
        for (unsigned int z_coord = 0; z_coord < (hmap.rows - mesh_step_size); z_coord += mesh_step_size) {
            // Get The (X, Y, Z) Value For The Bottom Left Vertex = 0
            glm::vec3 p0(x_coord, hmap.at<uchar>(cv::Point(x_coord, z_coord)), z_coord);
            // Get The (X, Y, Z) Value For The Bottom Right Vertex = 1
            glm::vec3 p1(x_coord + mesh_step_size, hmap.at<uchar>(cv::Point(x_coord + mesh_step_size, z_coord)), z_coord);
            // Get The (X, Y, Z) Value For The Top Right Vertex = 2
            glm::vec3 p2(x_coord + mesh_step_size, hmap.at<uchar>(cv::Point(x_coord + mesh_step_size, z_coord + mesh_step_size)), z_coord + mesh_step_size);
            // Get The (X, Y, Z) Value For The Top Left Vertex = 3
            glm::vec3 p3(x_coord, hmap.at<uchar>(cv::Point(x_coord, z_coord + mesh_step_size)), z_coord + mesh_step_size);

            // Get max normalized height for tile, set texture accordingly
            // Grayscale image returns 0..256, normalize to 0.0f..1.0f by dividing by 256 (255 ?)
            float max_h = std::max(hmap.at<uchar>(cv::Point(x_coord, z_coord)) / 255.0f,
                std::max(hmap.at<uchar>(cv::Point(x_coord, z_coord + mesh_step_size)) / 255.0f,
                    std::max(hmap.at<uchar>(cv::Point(x_coord + mesh_step_size, z_coord + mesh_step_size)) / 255.0f,
                        hmap.at<uchar>(cv::Point(x_coord + mesh_step_size, z_coord)) / 255.0f
                    )));

            // Get texture coords in vertices, bottom left of geometry == bottom left of texture
            glm::vec2 tc0 = HeightMap_GetSubtexByHeight(max_h);
            glm::vec2 tc1 = tc0 + glm::vec2(1.0f / 16, 0.0f);		// add offset for bottom right corner
            glm::vec2 tc2 = tc0 + glm::vec2(1.0f / 16, 1.0f / 16);  // add offset for top right corner
            glm::vec2 tc3 = tc0 + glm::vec2(0.0f, 1.0f / 16);       // add offset for bottom left corner

            // RETARDED HEIGHT MAP ? 1.1
            // calculate normal vector            
            normal = glm::normalize(glm::cross(p1 - p0, p2 - p0));

            // place vertices and ST to mesh
            mesh_vertices.emplace_back(Vertex{ p0, normal, tc0 });
            mesh_vertices.emplace_back(Vertex{ p1, normal, tc1 });
            mesh_vertices.emplace_back(Vertex{ p2, normal, tc2 });
            mesh_vertices.emplace_back(Vertex{ p3, normal, tc3 });

            // place indices
            indices_counter += 4;
            mesh_vertex_indices.emplace_back(indices_counter - 4);
            mesh_vertex_indices.emplace_back(indices_counter - 2);
            mesh_vertex_indices.emplace_back(indices_counter - 3);
            mesh_vertex_indices.emplace_back(indices_counter - 4);
            mesh_vertex_indices.emplace_back(indices_counter - 1);
            mesh_vertex_indices.emplace_back(indices_counter - 2);
        }
    }

    print("HeightMap: height map vertices: " << mesh_vertices.size());
}

glm::vec2 Model::HeightMap_GetSubtexST(const int x, const int y)
{
    return glm::vec2(x * 1.0f / 16, y * 1.0f / 16);
}

glm::vec2 Model::HeightMap_GetSubtexByHeight(float height)
{
    if (height > 0.9) return HeightMap_GetSubtexST(2, 4); //snow
    else if (height > 0.8) return HeightMap_GetSubtexST(3, 4); //ice
    else if (height > 0.5) return HeightMap_GetSubtexST(5, 0); //rock
    else if (height > 0.3) return HeightMap_GetSubtexST(2, 0); //soil
    else return HeightMap_GetSubtexST(0, 0); //grass
}