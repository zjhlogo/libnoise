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

int main(int argc, char** argv)
{
    noise::module::Perlin myModule;
    utils::NoiseMap heightMap;

    utils::NoiseMapBuilderPlane heightMapBuilder;
    heightMapBuilder.SetSourceModule(myModule);
    heightMapBuilder.SetDestNoiseMap(heightMap);
    heightMapBuilder.SetDestSize(512, 512);
    heightMapBuilder.SetBounds(2.0, 6.0, 1.0, 5.0);
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
