#version 330 core // Adjust the version as needed

// Input variables
in vec2 v_texcoord0;
in vec4 v_posTime;

// Uniforms
uniform sampler2D s_MatTexture;

// Include required GLSL functions (assuming they are defined in these headers)
#include "bgfx_shader.glsl"
#include "newb/main.glsl"

vec3 getRainbowColor(float t) {
    float r = abs(sin(t * 6.2831853 + 0.0)); // 2 * PI for a full cycle
    float g = abs(sin(t * 6.2831853 + 2.094393)); // 2 * PI / 3 phase shift
    float b = abs(sin(t * 6.2831853 + 4.188787)); // 4 * PI / 3 phase shift
    return vec3(r, g, b);
}

// Function to generate nebula clouds
vec3 generateNebulaClouds(vec2 uv, float time) {
    float cloud1 = abs(sin(uv.x * 12.0 + time * 0.1));
    float cloud2 = abs(sin(uv.y * 8.0 + time * 0.07));
    float cloud3 = abs(sin((uv.x + uv.y) * 16.0 + time * 0.05));
    float cloudIntensity = (cloud1 + cloud2 + cloud3) / 3.0;
    return vec3(0.5, 0.2, 0.8) * cloudIntensity; // Adjust color and intensity as needed
}

// Function to create a 2D rotation matrix
mat2 getRotationMatrix(float angle) {
    float c = cos(angle);
    float s = sin(angle);
    return mat2(c, -s, s, c);
}

out vec4 FragColor;

void main() {
    // Calculate rotation angle based on time
    float rotationAngle = v_posTime.w * 0.5; // Adjust speed by changing the multiplier

    // Apply rotation to the texture coordinates
    vec2 rotatedTexCoords = getRotationMatrix(rotationAngle) * (v_texcoord0 - 0.5) + 0.5;

    vec4 diffuse = texture(s_MatTexture, rotatedTexCoords);

    // Generate rainbow color based on texture coordinates
    vec3 rainbowColor = getRainbowColor(rotatedTexCoords.x + rotatedTexCoords.y);

    // End sky gradient
    vec3 color = renderEndSky(getEndHorizonCol(), getEndZenithCol(), normalize(v_posTime.xyz), v_posTime.w);

    // Add nebula clouds
    vec3 nebulaClouds = generateNebulaClouds(rotatedTexCoords, v_posTime.w);
    color += nebulaClouds;

    // Stars with rainbow color
    color += 89.8 * diffuse.rgb * rainbowColor;

    color = colorCorrection(color);

    FragColor = vec4(color, 1.0);
}
