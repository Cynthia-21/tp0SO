package ar.edu.utn.frba.dds.servicioagregador.models.dtos.output;

import ar.edu.utn.frba.dds.servicioagregador.models.dtos.ContribuyenteDto;
import lombok.Data;

import java.time.LocalDateTime;

@Data
public class SolicitudEliminacionOutputDto {
  private Long id;
  private Long hechoId;
  private ContribuyenteDto contribuyente;
  private String motivo;
  private LocalDateTime fechaAlta;
}
