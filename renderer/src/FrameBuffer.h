#pragma once


class FrameBuffer {
public:
    unsigned int fbo{};
    unsigned int texture{};
    unsigned int rbo{};

public:
    FrameBuffer() = default;

    void create(int width, int height);
    void resize(int width, int height) const;
    void bind() const;
};