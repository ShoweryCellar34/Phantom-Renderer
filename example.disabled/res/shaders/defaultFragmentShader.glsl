#version 460 core
layout(location = 0) out vec4 fragmentColor;
layout(location = 1) out vec4 brightColor;

uniform vec3 cameraPosition;
uniform mat4 translation;

in GEOMETRY_OUT {
    vec3 fragmentPosition;
    vec2 textureCoordinates;
    mat3 TBN;
    vec4 fragmentPositionLightSpace;
} geometryOut;

struct Material {
    sampler2D ambient;
    sampler2D diffuse;
    sampler2D specular;
    sampler2D normal;
    float shininess;
};
uniform Material material;

uniform sampler2D shadowMap;

struct directionalLight {
    vec3 direction;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    sampler2D shadowMap;
};
#define NR_DIRECTIONAL_LIGHTS 1
uniform directionalLight directionalLights[NR_DIRECTIONAL_LIGHTS];

struct pointLight {
    vec3 position;

    float constant;
    float linear;
    float quadratic;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    samplerCube shadowMap;
    float farPlane;
};
#define NR_POINT_LIGHTS 1
uniform pointLight pointLights[NR_POINT_LIGHTS];

float directonalShadowCalculation(directionalLight light, vec4 fragmentPositionLightSpace, float bias) {
    vec3 projectedCoordinates = fragmentPositionLightSpace.xyz / fragmentPositionLightSpace.w;
    projectedCoordinates = projectedCoordinates * 0.5 + 0.5;

    float currentDepth = projectedCoordinates.z;

    float shadow = 0.0;
    vec2 texelSize = 1.0 / textureSize(light.shadowMap, 0);
    for(int x = -1; x <= 1; ++x) {
        for(int y = -1; y <= 1; ++y) {
            float pcfDepth = texture(light.shadowMap, projectedCoordinates.xy + vec2(x, y) * texelSize).r;
            shadow += currentDepth - bias > pcfDepth ? 1.0 : 0.0;
        }
    }
    shadow /= 9;

    if(projectedCoordinates.z > 1.0) {
        shadow = 0.0;
    }

    return shadow;
}

float pointShadowCalculation(pointLight light, vec3 fragmentPosition, float bias) {
    vec3 fragmentToLight = fragmentPosition - light.position;

    float currentDepth = length(fragmentToLight);

    vec3 sampleOffsetDirections[20] = vec3[] (
        vec3(1, 1, 1), vec3(1, -1, 1), vec3(-1, -1, 1), vec3(-1, 1, 1),
        vec3(1, 1,-1), vec3(1, -1, -1), vec3(-1, -1, -1), vec3(-1, 1, -1),
        vec3(1, 1, 0), vec3(1, -1, 0), vec3(-1, -1, 0), vec3(-1, 1, 0),
        vec3(1, 0, 1), vec3(-1, 0, 1), vec3(1, 0, -1), vec3(-1, 0, -1),
        vec3(0, 1, 1), vec3(0, -1, 1), vec3(0, -1, -1), vec3(0, 1, -1)
    );
    int samples = 20;
    float viewDistance = length(cameraPosition - fragmentPosition);
    float diskRadius = (1.0 + (viewDistance / light.farPlane)) / 25.0;

    float shadow = 0.0;
    for(int i = 0; i < samples; ++i) {
        float closestDepth = texture(light.shadowMap, fragmentToLight + sampleOffsetDirections[i] * diskRadius).r; 
        closestDepth *= light.farPlane;
        shadow += currentDepth - bias > closestDepth ? 1.0 : 0.0;
    }
    shadow /= float(samples);

    return shadow;
}

vec3 calculateDirectionalLight(directionalLight light, vec3 viewDirection, vec3 ambient, vec3 diffuse, vec3 specular, vec3 normal, float shadow) {
    vec3 lightDir = normalize(-light.direction);

    float difference = max(dot(normal, lightDir), 0.0);

    vec3 halfwayDirection = normalize(lightDir + viewDirection);
    float spec = pow(max(dot(normal, halfwayDirection), 0.0), material.shininess);

    vec3 ambientOutput  = light.ambient * ambient;
    vec3 diffuseOutput  = light.diffuse * difference * diffuse * (1.0 - shadow);
    vec3 specularOutput = light.specular * spec * specular * (1.0 - shadow);

    return ambientOutput + diffuseOutput + specularOutput;
}

vec3 calculatePointLight(pointLight light, vec3 viewDirection, vec3 fragmentPosition, vec3 ambient, vec3 diffuse, vec3 specular, vec3 normal, float shadow) {
    vec3 lightDir = normalize(light.position - fragmentPosition);

    float difference = max(dot(normal, lightDir), 0.0);

    vec3 halfwayDirection = normalize(lightDir + viewDirection);
    float spec = pow(max(dot(normal, halfwayDirection), 0.0), material.shininess);

    float distance = length(light.position - fragmentPosition);
    float attenuation = 1.0 / (light.constant + light.linear * distance + 
                light.quadratic * (distance * distance));    

    vec3 ambientOutput = light.ambient * ambient;
    vec3 diffuseOutput = light.diffuse * difference * diffuse * (1.0 - shadow);
    vec3 specularOutput = light.specular * spec * specular * (1.0 - shadow);
    ambientOutput *= attenuation;
    diffuseOutput *= attenuation;
    specularOutput *= attenuation;

    return ambientOutput + diffuseOutput + specularOutput;
}

vec3 calculateShadedResult(vec3 ambient, vec3 diffuse, vec3 specular, vec3 normal) {
    vec3 viewDirection = normalize(cameraPosition - geometryOut.fragmentPosition);

    vec3 result = vec3(0.0, 0.0, 0.0);

    for(int i = 0; i < NR_DIRECTIONAL_LIGHTS; i++) {
        float bias = max(0.01 * (1.0 - dot(normal, normalize(-directionalLights[i].direction))), 0.005);
        float shadow = directonalShadowCalculation(directionalLights[i], geometryOut.fragmentPositionLightSpace, bias);
        result += calculateDirectionalLight(directionalLights[i], viewDirection, ambient, diffuse, specular, normal, shadow);
    }

    for(int i = 0; i < NR_POINT_LIGHTS; i++) {
        float bias = 0.005;
        float shadow = pointShadowCalculation(pointLights[i], geometryOut.fragmentPosition, bias);
        result += calculatePointLight(pointLights[i], viewDirection, geometryOut.fragmentPosition, ambient, diffuse, specular, normal, shadow);
    }

    return result;
}

void main() {
    vec3 ambient = texture(material.ambient, geometryOut.textureCoordinates).rgb;
    vec3 diffuse = texture(material.diffuse, geometryOut.textureCoordinates).rgb;
    vec3 specular = texture(material.specular, geometryOut.textureCoordinates).rgb;
    vec3 normal = texture(material.normal, geometryOut.textureCoordinates).rgb;
    normal = normal * 2.0 - 1.0;
    normal = normalize(geometryOut.TBN * normal);

    vec3 result = calculateShadedResult(ambient, diffuse, specular, normal);
    float brightness = dot(result.rgb, vec3(0.2126, 0.7152, 0.0722));
    if(brightness > 1.0) {
        brightColor = vec4(result.rgb, 1.0);
    } else {
        brightColor = vec4(0.0, 0.0, 0.0, 1.0);
    }

    fragmentColor = vec4(result, 1.0);
}
