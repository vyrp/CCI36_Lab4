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
	
	void main() {
	
		vec3 normal, lightDir;
		vec4 diffuse;
		float NdotL;
		
		/* first transform the normal into eye space and normalize the result */
		normal = normalize(gl_NormalMatrix * gl_Normal);
		
		/* now normalize the light's direction. Note that according to the
		OpenGL specification, the light is stored in eye space. Also since 
		we're talking about a directional light, the position field is actually 
		direction */
		lightDir = normalize(vec3(gl_LightSource[0].position));
		
		/* compute the cos of the angle between the normal and lights direction. 
		The light is directional so the direction is constant for every vertex.
		Since these two are normalized the cosine is the dot product. We also 
		need to clamp the result to the [0,1] range. */
		NdotL = max(dot(normal, lightDir), 0.0);
		
		/* Compute the diffuse term */
		diffuse = gl_FrontMaterial.diffuse * gl_LightSource[0].diffuse;
		
		gl_FrontColor =  NdotL * diffuse;
		
		gl_Position = ftransform();
	}
	
	
	