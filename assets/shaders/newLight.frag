#version 430 core

in vec3 FragPos; // Pozice fragmentu ve světových souřadnicích
in vec3 Normal; // Normála k povrchu fragmentu

out vec4 FragColor; // Výsledná barva fragmentu: MUSÍ být napsáno

uniform vec3 lightPos; // Pozice světla

void main()
{
    // Vypočtení vektoru směřujícího od pozice fragmentu k pozici světla
    vec3 lightDir = normalize(lightPos - FragPos);

    // Vypočtení úrovně osvětlení pomocí dot produktu mezi normálou a vektorem směřujícím ke světlu
    float diff = max(dot(Normal, lightDir), 0.0);

    // Nastavení barvy fragmentu s ohledem na osvětlení
    vec3 diffuse = vec3(1.0, 1.0, 1.0); // Bílá difúzní barva
    vec3 ambient = vec3(0.2, 0.2, 0.2); // Alespoň malá ambientní složka

    vec3 result = diffuse * diff + ambient;

    FragColor = vec4(result, 1.0f); // Barva fragmentu
}
