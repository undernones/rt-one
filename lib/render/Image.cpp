//
// Copyright © 2017 Undernones. All rights reserved.
//

#include "Image.h"

#include <assert.h>

namespace
{

template <class T>
using ChannelPicker = T (*)(const T& t);

ChannelPicker<uint32_t> sRPicker = [] (const uint32_t& c) { return (0xFF000000 & c) >> 24; };
ChannelPicker<uint32_t> sGPicker = [] (const uint32_t& c) { return (0x00FF0000 & c) >> 16; };
ChannelPicker<uint32_t> sBPicker = [] (const uint32_t& c) { return (0x0000FF00 & c) >>  8; };
ChannelPicker<uint32_t> sAPicker = [] (const uint32_t& c) { return (0x000000FF & c) >>  0; };

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
        auto r = static_cast<uint8_t>(rPicker(x));
        auto g = static_cast<uint8_t>(gPicker(x));
        auto b = static_cast<uint8_t>(bPicker(x));
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
Image::clear(int32_t clearColor)
{
    for (auto& value: mValues) {
        value = clearColor;
    }
}

const int32_t&
Image::value(int row, int col) const
{
    assert(row >= 0 && row < mRows);
    assert(col >= 0 && col < mCols);
    row = (mRows - 1) - row;
    return mValues[mCols * row + col];
}

void
Image::setValue(int row, int col, int32_t value)
{
    assert(row >= 0 && row < mRows);
    assert(col >= 0 && col < mCols);
    row = (mRows - 1) - row;
    mValues[mCols * row + col] = value;
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
