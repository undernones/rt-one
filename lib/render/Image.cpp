//
// Copyright © 2017 Undernones. All rights reserved.
//

#include "Image.h"

#include <assert.h>
#include <cmath>

namespace
{

const simd::float3 MIN_RGB = { 0.f, 0.f, 0.f };
const simd::float3 MAX_RGB = { 255.f, 255.f, 255.f };

template <class T>
using ChannelPicker = float (*)(const T& t);

ChannelPicker<geom::Vec3> sRPicker = [] (const geom::Vec3& c) { return c.r(); };
ChannelPicker<geom::Vec3> sGPicker = [] (const geom::Vec3& c) { return c.g(); };
ChannelPicker<geom::Vec3> sBPicker = [] (const geom::Vec3& c) { return c.b(); };

template <class T>
std::vector<uint32_t>
combineChannels(const std::vector<T>& collection,
                const ChannelPicker<T>& rPicker,
                const ChannelPicker<T>& gPicker,
                const ChannelPicker<T>& bPicker)
{
    std::vector<uint32_t> result;
    result.reserve(collection.size());
    for (const auto& x : collection) {
        simd::float3 rgb = { 255.99f * sqrt(rPicker(x)), 255.99f * sqrt(gPicker(x)), 255.99f * sqrt(bPicker(x)) };
        rgb = simd::clamp(rgb, MIN_RGB, MAX_RGB);
        auto r = static_cast<uint8_t>(rgb[0]);
        auto g = static_cast<uint8_t>(rgb[1]);
        auto b = static_cast<uint8_t>(rgb[2]);
        auto a = static_cast<uint8_t>(255);
        uint32_t val = (r << 24) | (g << 16) | (b << 8) | a;
        result.push_back(val);
    }
    return result;
}

}

namespace render
{

Image::Image()
    : Image(0, 0)
{
}
Image::Image(int rows, int cols)
    : mRows(rows)
    , mCols(cols)
    , mAspectRatio(static_cast<float>(cols) / rows)
    , mValues(rows * cols)
{
}

void
Image::clear(geom::Vec3 clearColor)
{
    for (auto& value: mValues) {
        value = clearColor;
    }
}

const geom::Vec3&
Image::value(int row, int col) const
{
    assert(row >= 0 && row < mRows);
    assert(col >= 0 && col < mCols);
    row = (mRows - 1) - row;
    return mValues[mCols * row + col];
}

void
Image::setValue(int row, int col, const geom::Vec3& value)
{
    assert(row >= 0 && row < mRows);
    assert(col >= 0 && col < mCols);
    row = (mRows - 1) - row;
    mValues[mCols * row + col] = value;
}

std::vector<uint32_t>
Image::getRgba() const
{
    return combineChannels(mValues, sRPicker, sGPicker, sBPicker);
}

std::vector<uint32_t>
Image::rToRgba() const
{
    return combineChannels(mValues, sRPicker, sRPicker, sRPicker);
}

std::vector<uint32_t>
Image::gToRgba() const
{
    return combineChannels(mValues, sGPicker, sGPicker, sGPicker);
}

std::vector<uint32_t>
Image::bToRgba() const
{
    return combineChannels(mValues, sBPicker, sBPicker, sBPicker);
}

}
