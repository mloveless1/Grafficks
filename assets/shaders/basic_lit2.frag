#version 330 core

struct Material{
	vec3 Ambient;
	vec3 Diffuse;
	vec3 Specular;
	float Shininess;
};

struct DirLight{
	vec3 Direction;
	vec3 Ambient;
	vec3 Diffuse;
	vec3 Specular;

};

struct PointLight {
	vec3 Position;
	vec3 Ambient;
	vec3 Diffuse;
	vec3 Specular;

	float Constant;
	float Linear;
	float Quadratic;
};

out vec4 FragColor;
in vec4 vertexColor;
in vec2 texCoord;
in vec3 fragNormal;
in vec3 fragPosition;

uniform sampler2D tex0;
uniform sampler2D tex1;
uniform sampler2D tex2;
uniform sampler2D tex3;
uniform sampler2D tex4;
uniform sampler2D tex5;
uniform sampler2D tex6;

uniform vec3 eyePos;
#define MAX_POINT_LIGHTS 4

uniform PointLight pointLights[MAX_POINT_LIGHTS];
uniform DirLight dirLight;
uniform Material material;

vec3 CalcDirLight(vec3 normal, vec3 viewDir){
	float ambientStrength = 0.2;

	vec3 ambient = material.Ambient * dirLight.Ambient;

	//diffuse
	vec3 lightDir = dirLight.Direction;
	float diff = max(dot(normal, lightDir), 0.0);
	vec3 diffuse = (material.Diffuse * diff) * dirLight.Diffuse;

	//specular
	float specularStrength = 0.5;
	vec3 reflectDir = reflect(-lightDir, normal);

	float shininess = 32;
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.Shininess);
	vec3 specular = (material.Specular * spec) * dirLight.Specular;

	vec3 dirLightColor = (diffuse + ambient + specular);
	
	return dirLightColor;
}
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 viewDir){
	float ambientStrength = 0.5;
	vec3 ambient = material.Ambient * light.Ambient;

	//diffuse
	vec3 lightDir = normalize(light.Position - fragPosition);
	float diff = max(dot(normal, lightDir), 0.0);
	vec3 diffuse = (material.Diffuse * diff) * light.Diffuse;

	//specular
	float specularStrength = 0.5;
	vec3 reflectDir = reflect(-lightDir, normal);

	float shininess = 32;
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.Shininess);
	vec3 specular = (material.Specular * spec) * light.Specular;

	float distance = length(light.Position - fragPosition);
	float attenuation = 1.0 / (light.Constant + light.Linear * distance + light.Quadratic * (distance * distance)); 
	
	return (diffuse + ambient + specular) * attenuation;

}

void main(){
	vec3 objectColor = vec3(vertexColor * mix(texture(tex0, texCoord), texture(tex1, texCoord), 0.45));

	vec3 norm = normalize(fragNormal);
	vec3 viewDir = normalize(eyePos - fragPosition);

	vec3 result = CalcDirLight(norm, viewDir);

	for (int i = 0; i < MAX_POINT_LIGHTS; i++){
		result += CalcPointLight(pointLights[i], norm, viewDir); 
	}
	vec3 finalColor = result * objectColor;
	FragColor = vec4(finalColor, 1.0);
}