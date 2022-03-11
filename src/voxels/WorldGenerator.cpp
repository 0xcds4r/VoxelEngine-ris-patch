#include "WorldGenerator.h"
#include "voxel.h"
#include "Chunk.h"

#include <math.h>
#include <glm/glm.hpp>
#include <glm/gtc/noise.hpp>
#define FNL_IMPL
#include "../maths/FastNoiseLite.h"
#include <time.h>
#include <iostream>

class PseudoRandom {
	unsigned seed;
public:
	PseudoRandom(){
		seed = (unsigned)time(0);
	}

	int rand(){
		seed = (8253729 * seed + 2396403);
	    return seed % 32768;
	}

	void setSeed(int number){
		seed = (unsigned)number+8253729;
		rand();
	}
};

float calc_height(fnl_state *noise, fnl_state *noiser, fnl_state *noiseb, int real_x, int real_z){
	const float s = 0.2f;
	const float j = 40.0f;
	const float h = -0.2f;
	float a = abs(fnlGetNoise3D(noiser, real_x*0.005f*s*32,real_z*0.005f*s*32, 0.0f));
	// float height = glm::perlin(glm::vec3(real_x*0.025f*s,real_z*0.025f*s, 0.0f))*0.5f;
	// height += glm::perlin(glm::vec3(real_x*0.05f*s,real_z*0.05f*s, 0.0f))*0.25f;
	// height += glm::perlin(glm::vec3(real_x*0.1f*s,real_z*0.1f*s, 0.0f))*0.225f;
	// height += glm::perlin(glm::vec3(real_x*0.2f*s,real_z*0.2f*s, 0.0f))*0.15f;
	// height += glm::perlin(glm::vec3(real_x*0.4f*s,real_z*0.4f*s, 0.0f))*0.08f;
	// height += glm::perlin(glm::vec3(real_x*0.8f*s,real_z*0.8f*s, 0.0f))*0.04f;
	// height *= glm::perlin(glm::vec3(real_x*0.0125f*s,real_z*0.0125f*s, 0.0f));

	//height = fnlGetNoise3D(noise, real_x*0.05f*s*32,real_z*0.05f*s*32, 0.0f)*0.25f;
	double height = fnlGetNoise3D(noise, real_x*0.8f*s*32,real_z*0.8f*s*32, 0.0f)*0.04f*
					fnlGetNoise3D(noiseb, real_x*0.2f*s*32,real_z*0.2f*s*32, 0.0f);
					
	height += fnlGetNoise3D(noise, real_x*0.4f*s*32,real_z*0.4f*s*32, 0.0f)*0.08f*
			  fnlGetNoise3D(noiseb, real_x*0.1f*s*32,real_z*0.1f*s*32, 0.0f);

	height += fnlGetNoise3D(noise, real_x*0.2f*s*32,real_z*0.2f*s*32, 0.0f)*0.15f*
			  fnlGetNoise3D(noiseb, real_x*0.05f*s*32,real_z*0.05f*s*32, 0.0f);

	height += fnlGetNoise3D(noise, real_x*0.1f*s*32,real_z*0.1f*s*32, 0.0f)*0.225f*
			  fnlGetNoise3D(noiseb, real_x*0.025f*s*32,real_z*0.025f*s*32, 0.0f);

	height += fnlGetNoise3D(noise, real_x*0.05f*s*32,real_z*0.05f*s*32, 0.0f)*0.5f*
			  fnlGetNoise3D(noiseb, real_x*0.0125f*s*32,real_z*0.0125f*s*32, 0.0f);

	height += fnlGetNoise3D(noise, real_x*0.025f*s*32,real_z*0.025f*s*32, 0.0f)*0.5f*
			  fnlGetNoise3D(noiseb, real_x*0.00625f*s*32,real_z*0.00625f*s*32, 0.0f);

	height += fnlGetNoise3D(noise, real_x*0.0125f*s*32,real_z*0.0125f*s*32, 0.0f)*
			  fnlGetNoise3D(noiseb, real_x*0.003f*s*32,real_z*0.003f*s*32, 0.0f);
	// height = (height + h) * pow(a, 2);
	// float a = abs(sin(real_x * 0.02));
	double zpre = -pow(pow(fmin(j / 8.0f * a, 1), 2) - 1, 4);
	height += fmax(height - h / 5.0f, 0) * zpre;
	double z = -pow(pow(fmin(j * a, 1), 2) - 1, 4);
	height += fmax(height - h, 0) * z;
	// height = abs(fnlGetNoise3D(noise, real_x*0.05f*s*32,real_z*0.05f*s*32, 0.0f));
	//height += std::fmin(sqrt(abs(fnlGetNoise3D(noise, real_x*0.05f*s*32,real_z*0.05f*s*32, 0.0f))) - height, 0.2);
	height = height * 1.0f + 1.0f;
	// height *= height;
	// height *= (140.0f)*0.12f/s;
	// height += (42)*0.12f/s;
	height *= 256;
	std::cout << height << std::endl;
	// height *= 30;
	return height;
}

float calc_height_faster(fnl_state *noise, int real_x, int real_z){
	const float s = 0.2f;
	float height = fnlGetNoise3D(noise, real_x*0.05f*s*32,real_z*0.05f*s*32, 0.0f)*0.25f;
	height += fnlGetNoise3D(noise, real_x*0.1f*s*32,real_z*0.1f*s*32, 0.0f)*0.225f;
	height += fnlGetNoise3D(noise, real_x*0.2f*s*32,real_z*0.2f*s*32, 0.0f)*0.15f;
	// height += fnlGetNoise3D(noise, real_x*0.4f*s*32,real_z*0.4f*s*32, 0.0f)*0.08f;
	// height += fnlGetNoise3D(noise, real_x*0.8f*s*32,real_z*0.8f*s*32, 0.0f)*0.04f;
	height *= fnlGetNoise3D(noise, real_x*0.0125f*s*32,real_z*0.0125f*s*32, 0.0f);
	height += fnlGetNoise3D(noise, real_x*0.025f*s*32,real_z*0.025f*s*32, 0.0f)*0.5f;
	height = height * 1.0f + 0.5f;
	height *= height;
	height *= (140.0f)*0.12f/s;
	height += (42)*0.12f/s;
	return height;
}
#include <iostream>
int generate_tree(fnl_state *noise, PseudoRandom* random, const float* heights, int real_x, int real_y, int real_z, int tileSize){//, int height){
	const int tileX = floor((double)real_x/(double)tileSize);
	const int tileY = floor((double)real_z/(double)tileSize);
	random->setSeed(tileX*4325261+tileY*12160951+tileSize*9431111);

	bool gentree = fnlGetNoise3D(noise, tileX*3.0f+633, 0.0, tileY*3.0f) > -0.1f && (random->rand() % 10) < 7;
	if (!gentree)
		return 0;

	const int randomX = (random->rand() % (tileSize/2)) - tileSize/4;
	const int randomZ = (random->rand() % (tileSize/2)) - tileSize/4;
	int centerX = tileX * tileSize + tileSize/2 + randomX;
	int centerY = tileY * tileSize + tileSize/2 + randomZ;
	int height = (int)calc_height_faster(noise, centerX, centerY);
	if (height < 65)
		return 0;
	int lx = real_x - centerX;
	int radius = random->rand() % 4 + 3;
	int ly = real_y - height - 3 * radius;
	int lz = real_z - centerY;
	if (lx == 0 && lz == 0 && real_y - height < 4*radius)
		return 6;
	if (lx*lx+ly*ly/2+lz*lz < radius*radius)
		return 7;
	return 0;
}

void WorldGenerator::generate(voxel* voxels, int cx, int cy, int cz){
	fnl_state noise = fnlCreateState();
	fnl_state noiser = fnlCreateState();
	fnl_state noiseb = fnlCreateState();
	noise.seed = 1;
	noiser.seed = 2;
	noiseb.seed = 3;
	// noise.noise_type = FNL_NOISE_PERLIN;
	// noise.noise_type = FNL_NOISE_CELLULAR;
	noise.noise_type = FNL_NOISE_OPENSIMPLEX2;
	noiser.noise_type = FNL_NOISE_OPENSIMPLEX2S;
	noiseb.noise_type = FNL_NOISE_OPENSIMPLEX2;

	PseudoRandom random;

	float heights[CHUNK_VOL];

	for (int z = 0; z < CHUNK_D; z++){
		for (int x = 0; x < CHUNK_W; x++){
			int real_x = x + cx * CHUNK_W;
			int real_z = z + cz * CHUNK_D;
			float height = calc_height(&noise, &noiser, &noiseb, real_x, real_z);
			heights[z*CHUNK_W+x] = height;
		}
	}

	for (int z = 0; z < CHUNK_D; z++){
		for (int x = 0; x < CHUNK_W; x++){
			int real_x = x + cx * CHUNK_W;
			int real_z = z + cz * CHUNK_D;
			float height = heights[z*CHUNK_W+x];

			for (int y = 0; y < CHUNK_H; y++){
				int real_y = y + cy * CHUNK_H;
				int id = real_y < 55 ? 0 : 0;
				if (real_y == (int)height && height >= 56)
					id = 2;
				else if (real_y == (int)height && height >= 52)
					id = 10;
				else if (real_y < height) {
					if (real_y < height-6 || height <= 53)
						id = 8;
					else
						id = 1;
				} else {
					// int tree = generate_tree(&noise, &random, heights, real_x, real_y, real_z, 16);
					// if (tree)
					// 	id = tree;
					// else if ((tree = generate_tree(&noise, &random, heights, real_x, real_y, real_z, 19))){
					// 	id = tree;
					// }else if ((tree = generate_tree(&noise, &random, heights, real_x, real_y, real_z, 23))){
					// 	id = tree;
					// }
				}

				// if (real_y <= 2)
				// 	id = 8;
				voxels[(y * CHUNK_D + z) * CHUNK_W + x].id = id;
			}
		}
	}
}
