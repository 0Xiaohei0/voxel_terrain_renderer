#version 330 core
out vec4 FragColor;

struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;    
    float shininess;
}; 

struct Light {
    vec3 position;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

in vec3 FragPos;  
in vec3 Normal;  
  
uniform vec3 viewPos;
uniform Material material;
uniform Light light;

void main()
{
    
    //vec3 ambient = vec3(0, 0, 0);
    //if (FragPos.y > 14){
    //    ambient = vec3(0.439, 0.318, 0.161);
    //} else {
    //    ambient = vec3(0.49, 0.824, 0.949);
    //}
    // ambient
    vec3 ambient = light.ambient * material.ambient;
  	
    // diffuse 
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(light.position - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = light.diffuse * (diff * material.diffuse);
    
    // specular
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = light.specular * (spec * material.specular);  

    // fog
    float dis = length(viewPos - FragPos);
    float blendFactor = min(((dis * dis)/72000),1);
    vec3 fog = vec3(0.816, 0.933, 0.98) * blendFactor + vec3(1,1, 1) * (1 - blendFactor);
        
    vec3 result = (ambient + diffuse + specular) * (1 - blendFactor) + fog * blendFactor;
    FragColor = vec4(result, 1.0);
} 
