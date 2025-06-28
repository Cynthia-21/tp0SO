package ar.edu.utn.frba.dds.fuenteestatica.models.domain.dtos.output;

import java.time.LocalDateTime;
import lombok.Data;

@Data
public class HechoOutputDTO {
  private Long id;
  private String titulo;
  private String descripcion;
  private String categoria;
  private Long latitud;
  private Long longitud;
  private LocalDateTime fechaAcontecimiento;
  private LocalDateTime fechaDeCarga;
}
