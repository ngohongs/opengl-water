#version 330 core
uniform sampler2D tex;
uniform vec2 texelSize;
in vec2 fTexCoord;
out vec4 color;


vec4 GaussianBlur(vec2 pos) {
    vec2 pxpy = vec2(texelSize.x, texelSize.y);
    vec2 nxpy = vec2(-texelSize.x, texelSize.y);
    vec2 pxny = vec2(texelSize.x, -texelSize.y);
    vec2 nxny = vec2(-texelSize.x, -texelSize.y);
    vec2 px = vec2(texelSize.x, 0);
    vec2 nx = vec2(-texelSize.x, 0);
    vec2 py = vec2(0, texelSize.y);
    vec2 ny = vec2(0, -texelSize.y);
  
    vec4 vpxpy = texture(tex, pos + pxpy);
    vec4 vpxny = texture(tex, pos + pxny);
    vec4 vnxpy = texture(tex, pos + nxpy);
    vec4 vnxny = texture(tex, pos + nxny);
    vec4 vpx = texture(tex, pos + px);
    vec4 vpy = texture(tex, pos + py);
    vec4 vnx = texture(tex, pos + nx);
    vec4 vny = texture(tex, pos + ny);

    vec4 c = texture(tex, pos);

    vec4 weightedSum = 4 * c + 2 * (vpx + vpy + vnx + vny) + (vpxpy + vpxny + vnxpy + vnxny);
    weightedSum = weightedSum / 16.0f;

    return weightedSum;
}

vec4 BoxBlur(vec2 pos) {
    vec2 pxpy = vec2(texelSize.x, texelSize.y);
    vec2 nxpy = vec2(-texelSize.x, texelSize.y);
    vec2 pxny = vec2(texelSize.x, -texelSize.y);
    vec2 nxny = vec2(-texelSize.x, -texelSize.y);
    vec2 px = vec2(texelSize.x, 0);
    vec2 nx = vec2(-texelSize.x, 0);
    vec2 py = vec2(0, texelSize.y);
    vec2 ny = vec2(0, -texelSize.y);

    vec4 vpxpy = texture(tex, pos + pxpy);
    vec4 vpxny = texture(tex, pos + pxny);
    vec4 vnxpy = texture(tex, pos + nxpy);
    vec4 vnxny = texture(tex, pos + nxny);
    vec4 vpx = texture(tex, pos + px);
    vec4 vpy = texture(tex, pos + py);
    vec4 vnx = texture(tex, pos + nx);
    vec4 vny = texture(tex, pos + ny);

    vec4 c = texture(tex, pos);

    vec4 sum =  c + (vpx + vpy + vnx + vny) + (vpxpy + vpxny + vnxpy + vnxny);
    sum = sum / 9.0f;
    return sum;
}


void main()
{
    color = GaussianBlur(fTexCoord);
}  