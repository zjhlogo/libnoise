/*!
 * \file helloworld.cpp
 *
 * \author zjhlogo
 * \date 2019/12/05
 *
 * 
 */
#pragma once
#include <noise/noise.h>
#include <noiseutils.h>

using namespace noise::module;

int main(int argc, char** argv)
{
    Perlin perlin;

    Const low(-1.0);
    Const high(1.0);
    Select select(low, high, perlin, 0.0);

    utils::NoiseMap heightMap;

    utils::NoiseMapBuilderPlane heightMapBuilder;
    heightMapBuilder.SetSourceModule(select);
    heightMapBuilder.SetDestNoiseMap(heightMap);
    heightMapBuilder.SetDestSize(512, 128);
    heightMapBuilder.SetBounds(0.0, 4.0, 0.0, 1.0);
    heightMapBuilder.Build();

    utils::RendererImage renderer;
    utils::Image image;
    renderer.SetSourceNoiseMap(heightMap);
    renderer.SetDestImage(image);
    renderer.Render();

    utils::WriterBMP writer;
    writer.SetSourceImage(image);
    writer.SetDestFilename("helloworld.bmp");
    writer.WriteDestFile();

    return 0;
}
