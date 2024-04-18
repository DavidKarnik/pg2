#version 430 core

in vec3 Normal;      // Normálka předaná z vertex shaderu
in vec3 FragPos;     // Pozice fragmentu předaná z vertex shaderu

out vec4 FragColor;  // Výstupní barva aktuálního fragmentu

uniform vec3 lightPos;    // Pozice světelného zdroje
uniform vec3 viewPos;     // Pozice kamery/pozorovatele
uniform vec3 lightColor;  // Barva světla
uniform vec3 objectColor; // Základní barva objektu

void main()
{
    // Ambientní osvětlení
    float ambientStrength = 0.5;
    vec3 ambient = ambientStrength * lightColor;

    // Difúzní osvětlení
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;

    // Speculární osvětlení
    float specularStrength = 0.5;
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = specularStrength * spec * lightColor;

    // Výsledná barva fragmentu
    vec3 result = (ambient + diffuse + specular) * objectColor;
    FragColor = vec4(result, 1.0); // Výstup výsledku
}
