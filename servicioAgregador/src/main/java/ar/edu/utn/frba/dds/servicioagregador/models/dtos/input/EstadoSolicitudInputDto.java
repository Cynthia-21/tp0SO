package ar.edu.utn.frba.dds.servicioagregador.models.dtos.input;

import lombok.Data;

import java.time.LocalDateTime;

@Data
public class EstadoSolicitudInputDto {
  private String estado;
  private String administrador;
  private LocalDateTime fechaDeRevision;
}
