package ar.edu.utn.frba.dds.fuentedinamica.models.dtos.output;

import ar.edu.utn.frba.dds.fuentedinamica.models.dtos.input.HechoInputDTO;
import lombok.Data;

import java.time.LocalDateTime;
import java.util.List;

@Data
public class SolicitudDeModificacionOutputDTO {
  private Long id;
  private Long idHechoARevisar;
  private HechoOutputDTO hechoModificado;
  private LocalDateTime fechaDeAlta;
  private List<String> estadosRevision;
  private List<HechoOutputDTO> historialModificados;
}
