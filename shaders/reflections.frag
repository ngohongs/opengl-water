#version 330 core
out vec4 FragColor;


in vec2 fTexCoord;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;



uniform sampler2D positionTexture;
uniform sampler2D normalTexture;



void main()
{    
    float maxDistance = 15;
    float resolution  = 0.3;
    int   steps       = 10;
    float thickness   = 0.5;
    

    vec2 texSize  = textureSize(positionTexture, 0).xy;
    vec2 texCoord = gl_FragCoord.xy / texSize;

    vec4 positionFrom     = texture(positionTexture, fTexCoord);

    vec3 uv;

    if (positionFrom.w <= 0.0) {
        FragColor = vec4(1.0);
        return; 
    }

    vec3 unitPositionFrom = normalize(positionFrom.xyz);
    vec3 normal           = normalize(texture(normalTexture, fTexCoord).xyz);
    vec3 pivot            = normalize(reflect(unitPositionFrom, normal));

vec4 positionTo = positionFrom;

    vec4 startView = vec4(positionFrom.xyz + (pivot *         0.0), 1.0);
    vec4 endView   = vec4(positionFrom.xyz + (pivot * maxDistance), 1.0);

    vec4 startFrag      = startView;
    startFrag      = projection * startFrag;
    startFrag.xyz /= startFrag.w;
    startFrag.xy   = startFrag.xy * 0.5 + 0.5;
    startFrag.xy  *= texSize;

    vec4 endFrag      = endView;
    endFrag      = projection * endFrag;
    endFrag.xyz /= endFrag.w;
    endFrag.xy   = endFrag.xy * 0.5 + 0.5;
    endFrag.xy  *= texSize;

    vec2 frag  = startFrag.xy;
    uv.xy = frag / texSize;

    float deltaX    = endFrag.x - startFrag.x;
    float deltaY    = endFrag.y - startFrag.y;
    float useX      = abs(deltaX) >= abs(deltaY) ? 1.0 : 0.0;
    float delta     = mix(abs(deltaY), abs(deltaX), useX) * clamp(resolution, 0.0, 1.0);
    vec2  increment = vec2(deltaX, deltaY) / max(delta, 0.001); // possible chnage vec2(deltaX, deltaY) * res / delta where delta is the larger of two deltas

    float search0 = 0;
    float search1 = 0;

    int hit0 = 0;
    int hit1 = 0;

    float viewDistance = startView.z;
    float depth        = thickness;

    float i = 0;

    for (i = 0; i < int(delta); ++i) {
        frag      += increment;
        uv.xy      = frag / texSize;
        positionTo = texture(positionTexture, uv.xy);

        search1 =
            mix
            ( (frag.y - startFrag.y) / deltaY
            , (frag.x - startFrag.x) / deltaX
            , useX
            );

        search1 = clamp(search1, 0.0, 1.0);

        viewDistance = (startView.z * endView.z) / mix(endView.z, startView.z, search1); // TODO
        depth        = viewDistance - positionTo.z;

        if (depth > 0 && depth < thickness) {
            hit0 = 1;
            break;
        } 
        else {
            search0 = search1;
        }
    }

    search1 = search0 + ((search1 - search0) / 2.0);

    steps *= hit0;

    for (i = 0; i < steps; ++i) {
        frag       = mix(startFrag.xy, endFrag.xy, search1);
        uv.xy      = frag / texSize;
        positionTo = texture(positionTexture, uv.xy);

        viewDistance = (startView.z * endView.z) / mix(endView.z, startView.z, search1);
        depth        = viewDistance - positionTo.z;

        if (depth > 0 && depth < thickness) {
            hit1 = 1;
            search1 = search0 + ((search1 - search0) / 2);
        } 
        else {
            float temp = search1;
            search1 = search1 + ((search1 - search0) / 2);
            search0 = temp;
        }
  }


    FragColor = vec4(fNormal, 1);
}