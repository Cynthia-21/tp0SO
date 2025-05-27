package ar.edu.utn.frba.dds.fuentedinamica.models.dtos.input;

import java.time.LocalDateTime;
import java.util.List;

public class SolicitudDeModificacionInputDTO {
  private String  solicitante;
  private Long idHechoARevisar;
  private HechoInputDTO hechoModificado;
  private LocalDateTime fechaDeAlta;
}
