#include "Perlin.h"

#include <algorithm>
#include <numeric>
#include <random>
#include <vector>

namespace
{

const auto PERLIN_COUNT = 256;

std::vector<geom::Vec3>
perlin_generate()
{
    auto result = std::vector<geom::Vec3>(PERLIN_COUNT);
    for (auto i = 0; i < PERLIN_COUNT; ++i) {
        result[i] = geom::Vec3(-1 + 2*drand48(), -1 + 2*drand48(), -1 + 2*drand48()).normalized();
    }
    return result;
}

std::vector<int>
perlin_generate_perm()
{
    auto result = std::vector<int>(PERLIN_COUNT);
    std::iota(result.begin(), result.end(), 0);

    auto engine = std::default_random_engine();
    std::shuffle(std::begin(result), std::end(result), engine);

    return result;
}

float
perlin_interp(geom::Vec3 c[2][2][2], float u, float v, float w)
{
    auto uu = u*u*(3-2*u);
    auto vv = v*v*(3-2*v);
    auto ww = w*w*(3-2*w);

    auto result = 0.f;
    for (auto i = 0; i < 2; ++i) {
        for (auto j = 0; j < 2; ++j) {
            for (auto k = 0; k < 2; ++k) {
                auto weight_v = geom::Vec3(u-i, v-j, w-k);
                result += (i*uu + (1-i)*(1-uu))*
                          (j*vv + (1-j)*(1-vv))*
                          (k*ww + (1-k)*(1-ww))* c[i][j][k].dot(weight_v);
            }
        }
    }
    return fabs(result);
}

const auto randomVectors = perlin_generate();
const auto permX = perlin_generate_perm();
const auto permY = perlin_generate_perm();
const auto permZ = perlin_generate_perm();

}

namespace render
{

float
Perlin::noise(const geom::Vec3& p)
{
    auto i = int(floor(p.x()));
    auto j = int(floor(p.y()));
    auto k = int(floor(p.z()));

    auto u = p.x() - i;
    auto v = p.y() - j;
    auto w = p.z() - k;

    geom::Vec3 c[2][2][2];
    for (auto di = 0; di < 2; ++di) {
        for (auto dj = 0; dj < 2; ++dj) {
            for (auto dk = 0; dk < 2; ++dk) {
                auto index = permX[(i+di) & 255] ^ permY[(j+dj) & 255] ^ permZ[(k+dk) & 255];
                c[di][dj][dk] = randomVectors[index];
            }
        }
    }
    return perlin_interp(c, u, v, w);
}

float
Perlin::turbulence(const geom::Vec3& p, int depth)
{
    auto result = 0.f;
    auto temp_p = p;
    auto weight = 1.f;
    for (auto i = 0; i < depth; ++i) {
        result += weight * noise(temp_p);
        weight *= 0.5;
        temp_p *= 2;
    }

    return fabs(result);
}

}
