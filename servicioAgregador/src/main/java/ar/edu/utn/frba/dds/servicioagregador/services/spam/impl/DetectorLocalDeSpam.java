package ar.edu.utn.frba.dds.servicioagregador.services.spam.impl;

import ar.edu.utn.frba.dds.servicioagregador.services.spam.DetectorDeSpam;
import jakarta.annotation.PostConstruct;
import org.springframework.core.io.ClassPathResource;
import org.springframework.stereotype.Component;

import java.io.BufferedReader;
import java.io.InputStreamReader;
import java.nio.charset.StandardCharsets;
import java.util.HashSet;
import java.util.Set;

@Component
public class DetectorLocalDeSpam implements DetectorDeSpam {
  private Set<String> palabrasProhibidas;

  @PostConstruct
  public void cargarPalabrasProhibidas() {
    this.palabrasProhibidas = new HashSet<>();
    try {
      BufferedReader reader = new BufferedReader(
          new InputStreamReader(
              new ClassPathResource("static/palabras-prohibidas.txt").getInputStream(),
              StandardCharsets.UTF_8
          )
      );
      String linea;
      while ((linea = reader.readLine()) != null) {
        this.palabrasProhibidas.add(linea);
      }
    } catch (Exception e) {
      throw new RuntimeException(e);
    }
  }

  @Override
  public boolean esSpam(String texto) {
    String textoPlano = texto.toLowerCase();
    return palabrasProhibidas.stream().anyMatch(textoPlano::contains);
  }
}
