package ar.edu.utn.frba.dds.servicioagregador.services.hechoBuilder;

import ar.edu.utn.frba.dds.servicioagregador.models.domain.Hecho;
import ar.edu.utn.frba.dds.servicioagregador.models.domain.fuentes.Fuente;
import ar.edu.utn.frba.dds.servicioagregador.models.dtos.input.HechoInputDTO;

import java.util.List;

public interface HechoBuilderStrategy {
  List<Hecho> construirHechos(List<HechoInputDTO> hechosInputDto);
  boolean soporta(Fuente fuente);
}
