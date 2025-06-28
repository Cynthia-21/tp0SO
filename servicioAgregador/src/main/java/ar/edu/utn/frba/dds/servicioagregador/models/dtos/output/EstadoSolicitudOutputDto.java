package ar.edu.utn.frba.dds.servicioagregador.models.dtos.output;

import lombok.Data;

import java.time.LocalDateTime;

@Data
public class EstadoSolicitudOutputDto {
  private String estado;
  private String administrador;
  private LocalDateTime fechaDeRevision;
}
