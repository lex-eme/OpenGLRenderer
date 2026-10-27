#version 410 core

in vec3 worldSpacePos;
out vec4 FragColor;

uniform vec3 cameraPos;
uniform float gridSpacing = 1.0; // 1 unit grid
uniform vec4 gridColor = vec4(0.5, 0.5, 0.5, 1.0);
uniform vec4 xAxisColor = vec4(1.0, 0.0, 0.0, 1.0); // Red
uniform vec4 zAxisColor = vec4(0.0, 0.0, 1.0, 1.0); // Blue

void main() {
    // 1. Calculate screen-space derivative for anti-aliasing and constant line width
    vec2 coord = worldSpacePos.xz / gridSpacing;
    vec2 derivative = fwidth(coord);

    // 2. Draw lines using absolute value and stepping
    vec2 grid = abs(fract(coord - 0.5) - 0.5) / derivative;
    float line = min(grid.x, grid.y);

    // Justify minimum pixel width (1.0 means 1 pixel wide line)
    float gridAlpha = 1.0 - min(line, 1.0);

    // 3. Highlight Main Axes (X and Z)
    // Check how close the pixel is to world 0.0 on X and Z
    vec2 targetAxes = abs(worldSpacePos.xz) / fwidth(worldSpacePos.xz);
    float xAxisAlpha = 1.0 - min(targetAxes.y, 1.0); // Z=0 is the X-axis
    float zAxisAlpha = 1.0 - min(targetAxes.x, 1.0); // X=0 is the Z-axis

    // 4. Distance Fading (so the grid doesn't turn into a solid color at the horizon)
    float distance = length(worldSpacePos - cameraPos);
    float fade = exp(-distance * 0.08); // Adjust 0.01 to change how far the grid views

    // 5. Blend Colors
    vec4 finalColor = gridColor * gridAlpha;
    if (xAxisAlpha > 0.0) finalColor = mix(finalColor, xAxisColor, xAxisAlpha);
    if (zAxisAlpha > 0.0) finalColor = mix(finalColor, zAxisColor, zAxisAlpha);

    finalColor.a *= fade;

    if (finalColor.a < 0.05) discard; // Performance optimization

    FragColor = finalColor;
}