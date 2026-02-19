//
// Created by jay on 16/02/2026.
//

#include "stb_image.h"
#define STB_RECT_PACK_IMPLEMENTATION
#include "stb_rect_pack.h"
#include <spdlog/spdlog.h>
#include <SFML/Graphics.hpp>

#include "TextureManager.h"
#include "ManagerUtilities.h"
#include "../Types/Texture.h"


bool TextureManager::initialised;
std::filesystem::path TextureManager::fullPath;

std::map<std::filesystem::path, Texture> TextureManager::textures;
sf::Texture TextureManager::atlas;

void TextureManager::initialise() {
    fullPath = std::filesystem::path("assets/textures");
    spdlog::info("Initialising Texture Manager...");
    const auto& files = ManagerUtilities::getFilesFromPath(fullPath, {".png"});
    for (const auto& file : files) {
        spdlog::info("Found texture: {}", file.string());
    }

    spdlog::info("Creating texture atlas...");
    atlas = createAtlas(files);
    spdlog::info("Created texture atlas!");

    initialised = true;
    spdlog::info("Initialised Texture Manager!");
}

bool TextureManager::isTexture(const std::filesystem::path& path) {
    if (!isInitialised()) {
        spdlog::error("Tried to check if Texture \"{}\" exists in uninitialised Texture Manager!", path.string());
        throw std::runtime_error("Texture Manager is uninitialised!");
    }
    return textures.contains(path);
}

bool TextureManager::isInitialised() {
    return initialised;
}

sf::Texture* TextureManager::getAtlas() {
    if (!isInitialised()) {
        spdlog::error("Tried to get texture atlas, but Texture Manager is uninitialised!");
        throw std::runtime_error("Texture Manager is uninitialised!");
    }
    return &atlas;
}

Texture* TextureManager::getTexture(const std::filesystem::path& path) {
    if (!isInitialised()) {
        spdlog::error("Tried to get AtlasRect \"{}\" in uninitialised Texture Manager!", path.string());
        throw std::runtime_error("Texture Manager is uninitialised!");
    }
    return &textures.at(path);
}

sf::Texture TextureManager::createAtlas(const std::vector<std::filesystem::path>& files) {
    constexpr int atlasWidth = 256;
    constexpr int atlasHeight = 256;

    int numImages = files.size();
    std::vector<stbrp_rect> rects(numImages);
    std::vector<unsigned char*> imageData(numImages);
    std::vector<sf::Vector2i> imageSizes(numImages);

    // Load Images and Initialise Rectangles
    for (int i = 0; i < numImages; ++i) {
        int w, h, channels;
        imageData[i] = stbi_load(
            (fullPath / files[i]).string().c_str(),
            &w, &h, &channels,
            4);

        if (!imageData[i]) {
            spdlog::error("Failed to load image: {}", files[i].string());
            for (int j = 0; j < i; ++j) {
                stbi_image_free(imageData[j]);
            }
            throw std::runtime_error("Failed to load image");
        }

        imageSizes[i] = { w, h };
        rects[i].id = i;
        rects[i].w = w;
        rects[i].h = h;
    }

    // Pack Rectangles
    stbrp_context context;
    std::vector<stbrp_node> nodes(atlasWidth);
    stbrp_init_target(&context, atlasWidth, atlasHeight, nodes.data(), nodes.size());
    if (!stbrp_pack_rects(&context, rects.data(), numImages)) {
        spdlog::error("Failed to pack all textures into atlas as atlas is too small!");
        for (auto* data : imageData) {
            stbi_image_free(data);
        }
        throw std::runtime_error("Failed to pack textures into atlas");
    }

    for (int i = 0; i < numImages; ++i) {
        if (!rects[i].was_packed) {
            spdlog::error("Texture '{}' failed to fit in atlas", files[i].filename().string());
            for (auto* data : imageData) {
                stbi_image_free(data);
            }
            throw std::runtime_error("Not all textures could fit in atlas!");
        }
    }

    // Create final atlas
    std::vector<uint8_t> atlasPixels(atlasWidth * atlasHeight * 4, 0);

    for (int i = 0; i < numImages; ++i) {
        const auto& rect = rects[i];
        const int width = imageSizes[i].x;
        const int height = imageSizes[i].y;

        for (int row = 0; row < height; ++row) {
            size_t srcOffset = row * width * 4;
            size_t dstOffset = ((rect.y + row) * atlasWidth + rect.x) * 4;
            std::copy_n(
                imageData[i] + srcOffset,
                width * 4,
                atlasPixels.data() + dstOffset
                );
        }
        const std::filesystem::path& filepath{files[i]};
        const sf::IntRect atlasRect{{rect.x, rect.y}, {width, height}};
        Texture texture;
        texture.path = filepath;
        texture.rect = atlasRect;
        textures.emplace(filepath, texture);

        stbi_image_free(imageData[i]);
    }

    sf::Image atlasImage(sf::Vector2u{atlasWidth, atlasHeight}, atlasPixels.data());

    return sf::Texture(atlasImage);
}
