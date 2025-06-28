package ar.edu.utn.frba.dds.servicioagregador.services.hechoBuilder;

import ar.edu.utn.frba.dds.servicioagregador.models.domain.Hecho;
import ar.edu.utn.frba.dds.servicioagregador.models.domain.fuentes.Fuente;
import ar.edu.utn.frba.dds.servicioagregador.models.dtos.input.HechoInputDTO;
import org.springframework.stereotype.Component;

import java.util.List;

@Component
public class HechoBuilderSelector {
  private final List<HechoBuilderStrategy> estrategias;

  public HechoBuilderSelector(List<HechoBuilderStrategy> estrategias) {
    this.estrategias = estrategias;
  }

  public List<Hecho> construirHechosDesdeFuente(Fuente fuente, List<HechoInputDTO> hechosInputDto) {
    return estrategias.stream()
        .filter(builder -> builder.soporta(fuente))
        .findFirst()
        .orElseThrow(() -> new IllegalArgumentException("No hay estrategia para la fuente"))
        .construirHechos(hechosInputDto);
  }
}

