#pragma once
#include "Resource.h"
#include "Globals.h"
#include "MaterialComponent.h"

struct TextureMeta
{
	u32 uid = 0;
	std::string texturePath = " ";

	bool mipMap = false;

	bool alienifying = false;
	bool avgBlurring = false;
	bool gausianBlurring = false;
	bool contrast = false;
	bool equalization = false;
	bool gammaCorrection = false;
	bool negativity = false;
	bool noise = false;
	bool pixelization = false;
	bool sharpering = false;

	float amountContrast = 1.6;
	float sharpenFactor = 1.5;
	float sharpenIters = 5;
	float amountAvBlur = 10;
	float amountGausianBlur = 10;
	float amountGammaCorrection = 1;
	float amountNoise = 1;
	float amountPixelation = 10;
	std::string compression = "IL_DXT5";

	bool isLoaded = false;
};

class ResourceMatertial : public Resource
{
public:
	ResourceMatertial(u32 UID);
	~ResourceMatertial();

	ResourceType GetType() override;
	u32 GetUID() override;
	const char* GetAssetFile() override;
	const char* GetLibraryFile() override;
	uint GetReferenceCount() override;
	bool IsLoadedToMemory() override;
	bool LoadToMemory(int width, int height, GLuint imageId) override;
	bool LoadToMemory() override;
	void UnloadFromMemory() override;
	inline uint GetWidth() override { return width; }
	inline uint GetHeight() override { return height; }
	inline uint GetBytes() override { return bytes; }
	inline GLuint GetTextureId() override { return textureId; }
	void GenerateMeta() override;
	void GenerateMeta(bool mipMap, bool alienifying, bool avgBlurring, bool gausianBlurring, bool contrast, bool equalization, bool gammaCorrection, bool negativity, bool noise, bool pixelization, bool sharpering,
	float contrastAmount, float sharpenFactor, float sharpenIters, float avBlurAmount, float gaussianBlurAmount, float gammaCorrectionAmount, float noiseAmmount, float pixelationAmount, std::string compression);
	void LoadMeta() override;

public:
	uint width = 0;
	uint height = 0;
	uint bytes = 0;
	GLuint textureId = 0;
	bool isLoaded = false;
	TextureMeta metaData;
};