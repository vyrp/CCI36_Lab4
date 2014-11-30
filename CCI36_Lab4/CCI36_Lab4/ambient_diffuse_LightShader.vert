struct gl_LightSourceParameters {
	vec4 ambient; 
	vec4 diffuse; 
	vec4 specular; 
	vec4 position; 
	vec4 halfVector; 
	vec3 spotDirection; 
	float spotExponent; 
	float spotCutoff; // (range: [0.0,90.0], 180.0)
	float spotCosCutoff; // (range: [1.0,0.0],-1.0)
	float constantAttenuation; 
	float linearAttenuation; 
	float quadraticAttenuation;	
};

uniform gl_LightSourceParameters gl_LightSource[gl_MaxLights];

struct gl_LightModelParameters {
	vec4 ambient; 
};

uniform gl_LightModelParameters gl_LightModel;

struct gl_MaterialParameters {
	vec4 emission;   
	vec4 ambient;    
	vec4 diffuse;    
	vec4 specular;   
	float shininess; 
};

uniform gl_MaterialParameters gl_FrontMaterial;
uniform gl_MaterialParameters gl_BackMaterial;
	
void main()
{
	vec3 normal, lightDir;
	vec4 diffuse, ambient, globalAmbient;
	float NdotL;
	
	normal = normalize(gl_NormalMatrix * gl_Normal);
	lightDir = normalize(vec3(gl_LightSource[0].position));
	NdotL = max(dot(normal, lightDir), 0.0);
	diffuse = gl_FrontMaterial.diffuse * gl_LightSource[0].diffuse;
	
	/* Compute the ambient and globalAmbient terms */
	ambient = gl_FrontMaterial.ambient * gl_LightSource[0].ambient;
	globalAmbient = gl_LightModel.ambient * gl_FrontMaterial.ambient;
	
	gl_FrontColor =  NdotL * diffuse + globalAmbient + ambient;
	
	gl_Position = ftransform();
} 

	
	