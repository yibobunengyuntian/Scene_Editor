#version 330 core

struct Material {
    sampler2D texture_diffuse1;
    sampler2D texture_specular1;
    float shininess;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct Light {
    vec3 direction;
    vec3 color;
};
uniform Light light;
uniform Material material;
uniform bool isMesh;
uniform bool isUseColor;
uniform bool NormalIsColor;
uniform bool isInWater;
uniform bool isDepthMap;
uniform vec3 color;

out vec4 FragColor;

in vec2 TexCoords;
in vec3 Normal;
in vec3 FragPos;
in vec4 FragPosLightSpace;

uniform vec3 viewPos;
uniform sampler2D depthMap;

float ShadowCalculation(vec4 fragPosLightSpace) {
    // perform perspective divide
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    // transform to [0,1] range
    projCoords = projCoords * 0.5 + 0.5;
    // get closest depth value from light's perspective (using [0,1] range fragPosLight as coords)
    float closestDepth = texture(depthMap, projCoords.xy).r;
    // get depth of current fragment from light's perspective
    float currentDepth = projCoords.z;
    // check whether current frag pos is in shadow
    float bias = max(0.01 * (1.0 - dot(normalize(Normal), -light.direction)), 0.005);
    float shadow = 0.0;
    vec2 texelSize = 1.0 / textureSize(depthMap, 0);
    for(int x = -1; x <= 1; ++x)
    {
        for(int y = -1; y <= 1; ++y)
        {
            float pcfDepth = texture(depthMap, projCoords.xy + vec2(x, y) * texelSize).r;
            shadow += currentDepth - bias > pcfDepth ? 1.0 : 0.0;
        }
    }
    shadow /= 9.0;
    if(projCoords.z > 1.0)
        shadow = 0.0;
    return shadow;
}


void main() {
    vec3 result;
    if(isMesh == true)
    {
        result=vec3(0, 1, 0);
    }
    else if(NormalIsColor == true)
    {
        result = Normal;
    }
    else{
        vec3 diffuseTexColor;
        vec3 specularTexColor;
        if(isUseColor == true)
        {
            diffuseTexColor=color;
            specularTexColor=color;
        }else{
            diffuseTexColor=vec3(texture(material.texture_diffuse1,TexCoords));
            specularTexColor=vec3(texture(material.texture_specular1,TexCoords));
        }

        // ambient
        vec3 ambient = diffuseTexColor*material.ambient;
        // diffuse
        vec3 norm = normalize(Normal);
        vec3 lightDir = normalize(-light.direction);
        float diff = max(dot(norm, lightDir), 0.0);
        vec3 diffuse = diff *diffuseTexColor*material.diffuse;
        // specular
        vec3 viewDir = normalize(viewPos - FragPos);
        vec3 reflectDir = reflect(-lightDir, norm);
        float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
        vec3 specular =  spec*specularTexColor*material.specular;
        if(isDepthMap)
        {
            float shadow =ShadowCalculation(FragPosLightSpace);
            result = (ambient + ((diffuse + specular) * (1.0 - shadow)));
        }
        else
        {
            result = (ambient + diffuse + specular);
        }
    }
    FragColor = vec4(result, 1.0);
}
