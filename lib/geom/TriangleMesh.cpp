//
// Copyright © 2017 Undernones. All rights reserved.
//

#include "TriangleMesh.h"

#include <array>
#include <fstream>
#include <sstream>

namespace
{

template<typename Out>
void
split(const std::string &s, char delim, Out result)
{
    auto stream = std::stringstream(s);
    auto line = std::string();
    while (std::getline(stream, line, delim)) {
        *(result++) = line;
    }
}

std::vector<std::string>
split(const std::string &s, char delim)
{
    std::vector<std::string> elems;
    split(s, delim, std::back_inserter(elems));
    return elems;
}

std::vector<std::string>
split(const std::string& s, const std::string& delimiters)
{
    auto result = std::vector<std::string>();

    auto stream = std::stringstream(s);
    auto line = std::string();
    while (std::getline(stream, line)) {
        std::size_t prev = 0, pos;
        while ((pos = line.find_first_of(delimiters, prev)) != std::string::npos) {
            if (pos > prev) {
                result.push_back(line.substr(prev, pos-prev));
            }
            prev = pos+1;
        }
        if (prev < line.length()) {
            result.push_back(line.substr(prev, std::string::npos));
        }
    }

    return result;
}

// trim from start (in place)
void
ltrim(std::string &s)
{
    s.erase(s.begin(), std::find_if(s.begin(), s.end(),
                                    std::not1(std::ptr_fun<int, int>(std::isspace))));
}

// trim from end (in place)
void
rtrim(std::string &s)
{
    s.erase(std::find_if(s.rbegin(), s.rend(),
                         std::not1(std::ptr_fun<int, int>(std::isspace))).base(), s.end());
}

void
trim(std::string &s)
{
    ltrim(s);
    rtrim(s);
}

std::string
trimmed(std::string s)
{
    trim(s);
    return s;
}

// True if the string is empty or starts with '#'. False otherwise.
bool
isComment(const std::string& str)
{
    return str.length() == 0 || str[0] == '#';
}

geom::Vec3
makeVector(const std::vector<std::string>& pieces)
{
    if (pieces.size() < 3 || pieces.size() > 5) {
        throw std::domain_error("A vector must have 2, 3, or 4 coordinates");
    }
    auto x = std::stof(pieces[1]);
    auto y = std::stof(pieces[2]);
    auto z = pieces.size() > 3 ? std::stof(pieces[3]) : 0.f;
    return geom::Vec3(x, y, z);
}

std::vector<geom::Triangle>
makeFaces(const std::vector<std::string>& pieces)
{
    if (pieces.size() != 4 && pieces.size() != 5) {
        throw std::domain_error("Only triangles and quadrilaterals are supported");
    }

    std::array<int, 4> verts = {{ 0, 0, 0, 0 }};
    std::array<int, 4> norms = {{ 0, 0, 0, 0 }};
    std::array<int, 4> uvs = {{ -1, -1, -1, -1 }};
    for (auto i = 1; i < pieces.size(); ++i) {
        auto components = split(pieces[i], '/');

        // Subtract 1 at the end because OBJ files index vertices starting at 1.
        verts[i-1] = std::stof(components[0]) - 1;

        if (components.size() > 1 && components[1].length() > 0) {
            uvs[i-1] = std::stof(components[1]) - 1;
        }

        if (components.size() > 2 && components[2].length() > 0) {
            norms[i-1] = std::stof(components[2]) - 1;
        }
    }
    auto vertCount = static_cast<int>(pieces.size()) - 1;

    auto result = std::vector<geom::Triangle>();
    auto triangle = geom::Triangle({ verts[0], verts[1], verts[2] });
    result.push_back(triangle);
    if (vertCount == 4) {
        triangle = geom::Triangle({ verts[0], verts[2], verts[3] });
        result.push_back(triangle);
    }

    return result;
}

}

namespace geom
{

TriangleMesh::TriangleMesh(const std::string& fileName)
{
    std::ifstream stream(fileName);
    if (!stream.is_open()) {
        throw std::invalid_argument("Mesh file \"" + fileName + "\" does not exist.");
    }

    load(stream);
}

TriangleMesh::TriangleMesh(std::istream& stream)
{
    load(stream);
}

void
TriangleMesh::load(std::istream& stream)
{
    try {
        for (std::string line; std::getline(stream, line); ) {
            trim(line);
            if (isComment(line)) continue;

            auto pieces = split(line, "\t ");
            if (pieces.empty()) continue;

            if (pieces[0] == "v") { // Vertex
                auto vert = makeVector(pieces);
                mVerts.push_back(vert);
            } else if (pieces[0] == "vn") { // Normal
                auto norm = makeVector(pieces);
                mNormals.push_back(norm);
            } else if (pieces[0] == "vt") { // Texture
                auto uv = makeVector(pieces);
                mUvs.push_back(uv.xy());
            } else if (pieces[0] == "f") { // Face
                auto faces = makeFaces(pieces);
                mTriangles.insert(std::end(mTriangles), std::begin(faces), std::end(faces));
            }
        }
        std::cerr << "Successfully loaded mesh" << std::endl
                  << "  Vertices:  " << mVerts.size() << std::endl
                  << "  Triangles: " << mTriangles.size() << std::endl;
    } catch (const std::domain_error& ex) {
        std::cerr << "Parsing domain error: " << ex.what() << std::endl;
    }
}

}
