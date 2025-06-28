package ar.edu.utn.frba.dds.servicioagregador.models.dtos.input;

import ar.edu.utn.frba.dds.servicioagregador.models.dtos.ContribuyenteDto;
import lombok.Data;

@Data
public class SolicitudEliminacionInputDto {
  private Long hechoId;
  private ContribuyenteDto contribuyente;
  private String motivo;
}
