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
    Gradient gradient(0.0, 0.0, 0.0, 0.0, 1.0, 0.0);

    Perlin perlin(6, 0.3);
    ScaleBias scaleBias(perlin, 0.5, -0.2);
    ScaleDomain scaleDomain(scaleBias, 1.0, 1.0, 0.0);
    TranslateDomain transDomain(gradient, 0.0, 0.0, scaleDomain);
    // Turbulence turbulence(gradient, 1.0, 1.0, 6);
    Select select(-1.0, 1.0, transDomain, 0.0, 0.0);

    Perlin perlin2(3, 14);
    ScaleBias scaleBias2(gradient, 0.6, -0.3);
    Select select2(-1.0, 1.0, perlin2, scaleBias2, 0.0);

    Select select3(select, select2, transDomain, 0.0, 0.0);

    utils::NoiseMap heightMap;

    utils::NoiseMapBuilderPlane heightMapBuilder;
    heightMapBuilder.SetSourceModule(select3);
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
