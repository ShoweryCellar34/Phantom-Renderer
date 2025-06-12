#version 460 core
out vec4 fragmentColor;

uniform vec3 cameraPosition;
uniform mat4 translation;
uniform vec2 screenSize;

in GEOMETRY_OUT {
    vec3 fragmentPosition;
    vec3 normals;
    vec2 textureCoordinates;
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
};
#define NR_POINT_LIGHTS 1
uniform pointLight pointLights[NR_POINT_LIGHTS];

float shadowCalculation(vec4 fragmentPositionLightSpace, float bias) {
    vec3 projectedCoordinates = fragmentPositionLightSpace.xyz / fragmentPositionLightSpace.w;
    projectedCoordinates = projectedCoordinates * 0.5 + 0.5;

    float currentDepth = projectedCoordinates.z;

    float shadow = 0.0;
    vec2 texelSize = 1.0 / textureSize(shadowMap, 0);
    for(int x = -1; x <= 1; ++x) {
        for(int y = -1; y <= 1; ++y) {
            float pcfDepth = texture(shadowMap, projectedCoordinates.xy + vec2(x, y) * texelSize).r;
            shadow += currentDepth - bias > pcfDepth ? 1.0 : 0.0;
        }
    }
    shadow /= 9;

    if(projectedCoordinates.z > 1.0) {
        shadow = 0.0;
    }

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

vec3 calculatePointLight(pointLight light, vec3 viewDirection, vec3 fragmentPosition, vec3 ambient, vec3 diffuse, vec3 specular, vec3 normal) {
    vec3 lightDir = normalize(light.position - fragmentPosition);

    float difference = max(dot(normal, lightDir), 0.0);

    vec3 halfwayDirection = normalize(lightDir + viewDirection);
    float spec = pow(max(dot(normal, halfwayDirection), 0.0), material.shininess);

    float distance = length(light.position - fragmentPosition);
    float attenuation = 1.0 / (light.constant + light.linear * distance + 
                light.quadratic * (distance * distance));    

    vec3 ambientOutput = light.ambient * ambient;
    vec3 diffuseOutput = light.diffuse * difference * diffuse;
    vec3 specularOutput = light.specular * spec * specular;
    ambientOutput *= attenuation;
    diffuseOutput *= attenuation;
    specularOutput *= attenuation;

    return ambientOutput + diffuseOutput + specularOutput;
}

vec3 calculateShadedResult(vec3 ambient, vec3 diffuse, vec3 specular, vec3 normal) {
    vec3 viewDirection = normalize(cameraPosition - geometryOut.fragmentPosition);

    vec3 result = vec3(0.0, 0.0, 0.0);

    for(int i = 0; i < NR_DIRECTIONAL_LIGHTS; i++) {
        float bias = max(0.05 * (1.0 - dot(normal, normalize(-directionalLights[i].direction))), 0.005);
        float shadow = shadowCalculation(geometryOut.fragmentPositionLightSpace, bias);
        result += calculateDirectionalLight(directionalLights[i], viewDirection, ambient, diffuse, specular, normal, shadow);
    }

    for(int i = 0; i < NR_POINT_LIGHTS; i++) {
        result += calculatePointLight(pointLights[i], viewDirection, geometryOut.fragmentPosition, ambient, diffuse, specular, normal);
    }

    return result;
}

void main() {
    vec3 ambient = texture(material.ambient, geometryOut.textureCoordinates).rgb;
    vec3 diffuse = texture(material.diffuse, geometryOut.textureCoordinates).rgb;
    vec3 specular = texture(material.specular, geometryOut.textureCoordinates).rgb;
    vec3 normal = normalize(texture(material.normal, geometryOut.textureCoordinates).rgb);
    normal = normalize(geometryOut.normals);

    vec4 result = vec4(0.0, 0.0, 0.0, 1.0);
    if(gl_FragCoord.x > screenSize.x / 2 && gl_FragCoord.y > screenSize.y / 2) {
        result = vec4(calculateShadedResult(ambient, diffuse, specular, normal), 1.0);
    } else if(gl_FragCoord.y <= screenSize.y / 2) {
        if(gl_FragCoord.x < screenSize.x / 3) {
            result = vec4(specular, 1.0);
        } else if(gl_FragCoord.x < screenSize.x / 3 * 2) {
            result = vec4(diffuse, 1.0);
        } else {
            result = vec4(ambient, 1.0);
        }
    } else if(gl_FragCoord.x <= screenSize.x / 2 && gl_FragCoord.y > screenSize.y / 2) {
        result = vec4((normal + 1.0) * 0.5, 1.0);
    }

    fragmentColor = result;
}
