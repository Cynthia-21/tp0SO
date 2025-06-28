package ar.edu.utn.frba.dds.servicioagregador.models.dtos;

import lombok.Data;

@Data
public class FuenteDTO {
  private String tipo;
  private String archivo;
  private String proxy;
}
