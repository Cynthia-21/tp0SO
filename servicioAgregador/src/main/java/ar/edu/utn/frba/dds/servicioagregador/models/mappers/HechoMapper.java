package ar.edu.utn.frba.dds.servicioagregador.models.mappers;

import ar.edu.utn.frba.dds.servicioagregador.models.domain.Hecho;
import ar.edu.utn.frba.dds.servicioagregador.models.dtos.input.HechoInputDTO;

public interface HechoMapper {
  Hecho mapearHecho(HechoInputDTO hechoDto);
}
