#include <PR/shader.h>

#include <PR/defines.h>

#include <stddef.h>
#include <string.h>
#include <stdio.h>
#include <PR/error.h>

unsigned int prShaderGenerateDefaultProgram(GladGLContext* context) {
    const char* vertexShaderSource = "\n\
            #version 330 core\n\
            layout (location = 0) in vec3 inputPosition;\n\
            layout (location = 1) in vec2 inputTextureCoordinates;\n\
            \n\
            out vec3 fragmentPosition;\n\
            out vec2 textureCoordinates;\n\
            \n\
            uniform mat4 translation;\n\
            uniform mat4 view;\n\
            uniform mat4 projection;\n\
            \n\
            void main() {\n\
                gl_Position = projection * view * translation * vec4(inputPosition, 1.0);\n\
                fragmentPosition = vec3(translation * vec4(inputPosition, 1.0));\n\
                textureCoordinates = inputTextureCoordinates;\n\
            }\n\
        ";

    const char* fragmentShaderSource = "\n\
            #version 330 core\n\
            out vec4 fragmentColor;\n\
            \n\
            uniform vec3 cameraPosition;\n\
            uniform mat4 translation;\n\
            \n\
            in vec3 fragmentPosition;\n\
            in vec2 textureCoordinates;\n\
            in mat3 TBN;\n\
            \n\
            struct Material {\n\
                sampler2D ambient;\n\
                sampler2D diffuse;\n\
                sampler2D specular;\n\
                sampler2D normal;\n\
                float shininess;\n\
            };\n\
            uniform Material material;\n\
            \n\
            struct directionalLight {\n\
                vec3 direction;\n\
                \n\
                vec3 ambient;\n\
                vec3 diffuse;\n\
                vec3 specular;\n\
            };\n\
            #define NR_DIRECTIONAL_LIGHTS 1\n\
            uniform directionalLight directionalLights[NR_DIRECTIONAL_LIGHTS];\n\
            \n\
            struct pointLight {\n\
                vec3 position;\n\
                \n\
                float constant;\n\
                float linear;\n\
                float quadratic;\n\
                \n\
                vec3 ambient;\n\
                vec3 diffuse;\n\
                vec3 specular;\n\
            };\n\
            #define NR_POINT_LIGHTS 1\n\
            uniform pointLight pointLights[NR_POINT_LIGHTS];\n\
            \n\
            vec3 calculateDirectionalLight(directionalLight light, vec3 viewDirection, vec3 ambient, vec3 diffuse, vec3 specular, vec3 normal) {\n\
                vec3 lightDir = normalize(-light.direction);\n\
                \n\
                float difference = max(dot(normal, lightDir), 0.0);\n\
                \n\
                vec3 reflectDir = reflect(-lightDir, normal);\n\
                float spec = pow(max(dot(viewDirection, reflectDir), 0.0), material.shininess);\n\
                \n\
                vec3 ambientOutput  = light.ambient * ambient;\n\
                vec3 diffuseOutput  = light.diffuse * difference * diffuse;\n\
                vec3 specularOutput = light.specular * spec * specular;\n\
                \n\
                return (ambientOutput + diffuseOutput + specularOutput);\n\
            }\n\
            \n\
            vec3 calculatePointLight(pointLight light, vec3 viewDirection, vec3 fragmentPosition, vec3 ambient, vec3 diffuse, vec3 specular, vec3 normal) {\n\
                vec3 lightDir = normalize(light.position - fragmentPosition);\n\
                \n\
                float difference = max(dot(normal, lightDir), 0.0);\n\
                \n\
                vec3 reflectDirection = reflect(-lightDir, normal);\n\
                float spec = pow(max(dot(viewDirection, reflectDirection), 0.0), material.shininess);\n\
                \n\
                float distance = length(light.position - fragmentPosition);\n\
                float attenuation = 1.0 / (light.constant + light.linear * distance + \n\
                            light.quadratic * (distance * distance));    \n\
                \n\
                vec3 ambientOutput = light.ambient * ambient;\n\
                vec3 diffuseOutput = light.diffuse * difference * diffuse;\n\
                vec3 specularOutput = light.specular * spec * specular;\n\
                ambientOutput *= attenuation;\n\
                diffuseOutput *= attenuation;\n\
                specularOutput *= attenuation;\n\
                \n\
                return (ambientOutput + diffuseOutput + specularOutput);\n\
            }\n\
            \n\
            void main() {\n\
                vec3 ambient = texture(material.ambient, textureCoordinates).xyz;\n\
                vec3 diffuse = texture(material.diffuse, textureCoordinates).xyz;\n\
                vec3 specular = texture(material.specular, textureCoordinates).xxx;\n\
                vec3 normal = normalize(mat3(transpose(inverse(translation))) * texture(material.normal, textureCoordinates).xyz);\n\
                normal = normal * 2.0 - 1.0;\n\
                normal = normalize(TBN * normal);\n\
                \n\
                vec3 viewDirection = normalize(cameraPosition - fragmentPosition);\n\
                \n\
                vec3 result = vec3(0.0, 0.0, 0.0);\n\
                \n\
                for(int i = 0; i < NR_DIRECTIONAL_LIGHTS; i++) {\n\
                    result += calculateDirectionalLight(directionalLights[i], viewDirection, ambient, diffuse, specular, normal);\n\
                }\n\
                \n\
                for(int i = 0; i < NR_POINT_LIGHTS; i++) {\n\
                    result += calculatePointLight(pointLights[i], viewDirection, fragmentPosition, ambient, diffuse, specular, normal);\n\
                }\n\
                \n\
                fragmentColor = vec4(result, 1.0);\n\
            }\n\
        ";

    int success;
    char infoLog[512];
    char failMessage[768];

    unsigned int vertexShader = context->CreateShader(GL_VERTEX_SHADER);
    context->ShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    context->CompileShader(vertexShader);

    context->GetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if(!success) {
        context->GetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        snprintf(failMessage, 768, "Vertex shader 0 failed to compile. Aborting operation, nothing was modified: %s", infoLog);
        context->DeleteShader(vertexShader);
        prError(PR_GL_ERROR, failMessage);
        return 0;
    }

    unsigned int fragmentShader = context->CreateShader(GL_FRAGMENT_SHADER);
    context->ShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    context->CompileShader(fragmentShader);

    context->GetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if(!success) {
        context->GetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        snprintf(failMessage, 768, "Fragment shader failed to compile. Aborting operation, nothing was modified: %s", infoLog);
        context->DeleteShader(vertexShader);
        context->DeleteShader(fragmentShader);
        prError(PR_GL_ERROR, failMessage);
        return 0;
    }

    unsigned int shaderProgram = context->CreateProgram();
    context->AttachShader(shaderProgram, vertexShader);
    context->AttachShader(shaderProgram, fragmentShader);
    context->LinkProgram(shaderProgram);

    context->GetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if(!success) {
        context->GetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        snprintf(failMessage, 768, "Shader program failed to link. Aborting operation, nothing was modified: %s", infoLog);
        context->DeleteShader(vertexShader);
        context->DeleteShader(fragmentShader);
        context->DeleteProgram(shaderProgram);
        prError(PR_GL_ERROR, failMessage);
        return 0;
    }

    return shaderProgram;
}
